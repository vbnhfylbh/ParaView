/*=========================================================================

   Program: ParaView
   Module:    pqCinemaTrack.cxx

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.2.

   See License_v1.2.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

========================================================================*/
#include "pqCPExportStateWizard.h"

#include <vtkCamera.h>
#include <vtkSMProxyManager.h>
#include <vtkSMRenderViewProxy.h>
#include <vtkSMSessionProxyManager.h>
#include <vtkSMSourceProxy.h>
#include <vtkSMViewProxy.h>
#include <vtkPVXMLElement.h>

#include <pqApplicationCore.h>
#include <pqCinemaTrack.h>
#include <pqFileDialog.h>
#include <pqImageOutputInfo.h>
#include <pqPipelineSource.h>
#include <pqPythonDialog.h>
#include <pqPythonManager.h>
#include <pqRenderView.h>
#include <pqServerManagerModel.h>
#include <pqView.h>

#include <QMessageBox>

namespace
{
  static const char* cp_python_export_code =
    "from paraview import cpexport\n"
    "cpexport.DumpCoProcessingScript(export_rendering=%1,\n"
    "   simulation_input_map={%2},\n"
    "   screenshot_info={%3},\n"
    "   rescale_data_range=%4,\n"
    "   enable_live_viz=%5,\n"
    "   live_viz_frequency=%6,\n"
    "   cinema_tracks=%7,\n"
    "   filename='%8')\n";
}

//-----------------------------------------------------------------------------
pqCPExportStateWizard::pqCPExportStateWizard(
  QWidget *parentObject, Qt::WindowFlags parentFlags)
  : Superclass(parentObject, parentFlags)
{
}


//-----------------------------------------------------------------------------
pqCPExportStateWizard::~pqCPExportStateWizard()
{
}

//-----------------------------------------------------------------------------
void pqCPExportStateWizard::customize()
{
  this->Internals->timeCompartmentSize->hide();
  this->Internals->label_2->hide();
}

//-----------------------------------------------------------------------------
bool pqCPExportStateWizard::getCommandString(QString& command)
{
  QString export_rendering = this->Internals->outputRendering->isChecked() ?
    "True" : "False";
  QString rendering_info; // a map from the render view name to render output params
  if (this->Internals->outputRendering->isChecked() == 0 &&
      this->Internals->liveViz->isChecked() == 0)
    {
    // check to make sure that there is a writer hooked up since we aren't
    // exporting an image
    vtkSMSessionProxyManager* proxyManager =
        vtkSMProxyManager::GetProxyManager()->GetActiveSessionProxyManager();
    pqServerManagerModel* smModel =
      pqApplicationCore::instance()->getServerManagerModel();
    bool haveSomeWriters = false;
    QStringList filtersWithoutConsumers;
    for(unsigned int i=0;i<proxyManager->GetNumberOfProxies("sources");i++)
      {
      if(vtkSMSourceProxy* proxy = vtkSMSourceProxy::SafeDownCast(
           proxyManager->GetProxy("sources", proxyManager->GetProxyName("sources", i))))
        {
        vtkPVXMLElement* writerProxyHint = proxy->GetHints();
        if(writerProxyHint && writerProxyHint->FindNestedElementByName("WriterProxy"))
          {
          haveSomeWriters = true;
          }
        else
          {
          pqPipelineSource* input = smModel->findItem<pqPipelineSource*>(proxy);
          if(input && input->getNumberOfConsumers() == 0)
            {
            filtersWithoutConsumers << proxyManager->GetProxyName("sources", i);
            }
          }
        }
      }
    if(!haveSomeWriters)
      {
      QMessageBox messageBox;
      QString message(tr("No output specified. Generated script should be modified to output information."));
      messageBox.setText(message);
      messageBox.exec();
      }
    else if(filtersWithoutConsumers.size() != 0)
      {
      QMessageBox messageBox;
      QString message(tr("The following filters have no consumers and will not be saved:\n"));
      for(QStringList::const_iterator iter=filtersWithoutConsumers.constBegin();
          iter!=filtersWithoutConsumers.constEnd();iter++)
        {
        message.append("  ");
        message.append(iter->toLocal8Bit().constData());
        message.append("\n");
        }
      messageBox.setText(message);
      messageBox.exec();
      }
    }
  else if(this->Internals->outputRendering->isChecked())
    { // we are creating images so add information to the view proxies
    vtkSMSessionProxyManager* proxyManager =
        vtkSMProxyManager::GetProxyManager()->GetActiveSessionProxyManager();
    for(int i=0;i<this->Internals->viewsContainer->count();i++)
      {
      pqImageOutputInfo* viewInfo = dynamic_cast<pqImageOutputInfo*>(
        this->Internals->viewsContainer->widget(i));
      pqView* view = viewInfo->getView();
      QSize viewSize = view->getSize();
      vtkSMViewProxy* viewProxy = view->getViewProxy();
      vtkSMRenderViewProxy* rvp = vtkSMRenderViewProxy::SafeDownCast(viewProxy);
      pqRenderView* rview = dynamic_cast<pqRenderView*>(view);
      //cinema camera parameters
      QString cinemaCam = "{}";
      QString camType = viewInfo->getCameraType();
      if (rvp && (camType != "None"))
        {
        cinemaCam = QString("{\"camera\":\"");
        cinemaCam += camType;
        cinemaCam += "\"";
        if (camType != "Static")
          {
          cinemaCam += ", ";

          cinemaCam += "\"phi\":[";
          int j;
          for (j = -180; j < 180; j+= (360/viewInfo->getPhi()))
            {
            cinemaCam += QString::number(j) + ",";
            }
          cinemaCam.chop(1);
          cinemaCam += "],";

          cinemaCam += "\"theta\":[";
          for (j = -180; j < 180; j+= (360/viewInfo->getTheta()))
            {
            cinemaCam += QString::number(j) + ",";
            }
          cinemaCam.chop(1);
          cinemaCam += "], ";

          vtkCamera *cam = rvp->GetActiveCamera();
          double eye[3];
          double at[3];
          double up[3];
          cam->GetPosition(eye);
          rview->getCenterOfRotation(at);
          cam->GetViewUp(up);
          cinemaCam += "\"initial\":{ ";
          cinemaCam += "\"eye\": [" +
            QString::number(eye[0]) + "," + QString::number(eye[1]) + "," + QString::number(eye[2]) + "], ";
          cinemaCam += "\"at\": [" +
            QString::number(at[0]) + "," + QString::number(at[1]) + "," + QString::number(at[2]) + "], ";
          cinemaCam += "\"up\": [" +
            QString::number(up[0]) + "," + QString::number(up[1]) + "," + QString::number(up[2]) + "] ";
          cinemaCam += "} ";
          }
        cinemaCam += "}";
        }

      QString info = QString(" '%1' : ['%2', %3, '%4', '%5', '%6', '%7', '%8'],").
        arg(proxyManager->GetProxyName("views", viewProxy)).
        arg(viewInfo->getImageFileName()).arg(viewInfo->getWriteFrequency()).
        arg(static_cast<int>(viewInfo->fitToScreen())).
        arg(viewInfo->getMagnification()).
        arg(viewSize.width()).
        arg(viewSize.height()).
        arg(cinemaCam);
      rendering_info+= info;
      }
    // remove the last comma -- assume that there's at least one view
    rendering_info.chop(1);
    }

  QString cinema_tracks = "{ "; //trailing space matters
  if(this->Internals->outputCinema->isChecked())
    {
    for(int i=0;i<this->Internals->cinemaContainer->count();i++)
      {
      pqCinemaTrack *p = dynamic_cast<pqCinemaTrack*>(this->Internals->cinemaContainer->widget(i));
      if (!p->explore())
        {
        continue;
        }
      QString name = p->filterName();
      QString values = "[";
      QVariantList vals = p->scalars();
      for (int j = 0; j < vals.count(); j++)
        {
        values += QString::number(vals.value(j).toDouble());
        values += ",";
        }
      values.chop(1);
      values += "]";
      QString info = QString(" '%1' : %2,").arg(name).arg(values);
      cinema_tracks+= info;
      }

    cinema_tracks.chop(1);
    }
  cinema_tracks+="}";

  QString filters ="ParaView Python State Files (*.py);;All files (*)";

  pqFileDialog file_dialog (NULL, this,
    tr("Save Server State:"), QString(), filters);
  file_dialog.setObjectName("ExportCoprocessingStateFileDialog");
  file_dialog.setFileMode(pqFileDialog::AnyFile);
  if (!file_dialog.exec())
    {
    return false;
    }

  QString filename = file_dialog.getSelectedFiles()[0];
#ifdef _WIN32
  // Convert to forward slashes. The issue is that the path is interpreted as a
  // Python string when passed to the interpreter, so a path such as "C:\tests"
  // is read as "C:<TAB>ests" which isn't what we want. Since Windows is
  // flexible anyways, just use Unix separators.
  filename.replace('\\', '/');
#endif

  // the map from the simulation inputs in the paraview gui
  // to the adaptor's named inputs (usually 'input')
  QString sim_inputs_map;
  for (int cc=0; cc < this->Internals->nameWidget->rowCount(); cc++)
    {
    QTableWidgetItem* item0 = this->Internals->nameWidget->item(cc, 0);
    QTableWidgetItem* item1 = this->Internals->nameWidget->item(cc, 1);
    sim_inputs_map +=
      QString(" '%1' : '%2',").arg(item0->text()).arg(item1->text());
    }
  // remove last ","
  sim_inputs_map.chop(1);

  QString rescale_data_range = (this->Internals->rescaleDataRange->isChecked() == true ?
                                "True" : "False");

  QString live_visualization = (this->Internals->liveViz->isChecked() == true ?
                                "True" : "False");

  command = cp_python_export_code;
  // may be set by the user in the future
  const int live_visualization_frequency = 1;
  command = command.arg(export_rendering)
                   .arg(sim_inputs_map)
                   .arg(rendering_info)
                   .arg(rescale_data_range)
                   .arg(live_visualization)
                   .arg(live_visualization_frequency)
                   .arg(cinema_tracks)
                   .arg(filename);

  return true;
}
