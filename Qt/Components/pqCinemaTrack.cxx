/*=========================================================================

   Program: ParaView
   Module:    pqSGExportStateWizard.cxx

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

#include "pqCinemaTrack.h"

#include <vtkNew.h>
#include <vtkSmartPointer.h>
#include <vtksys/SystemTools.hxx>

#include <vtkSMProxy.h>
#include <vtkSMProperty.h>
#include <vtkSMDoubleRangeDomain.h>
#include <pqPipelineFilter.h>
#include <pqScalarValueListPropertyWidget.h>

#include "ui_pqCinemaTrack.h"

//widget and state for a cinema property track

//-----------------------------------------------------------------------------
pqCinemaTrack::pqCinemaTrack(
  QWidget *parentObject, Qt::WindowFlags parentFlags,
  pqPipelineFilter* filter):
  QWidget(parentObject, parentFlags),
  Track(new Ui::CinemaTrack())
{
  this->valsWidget = NULL;
  this->Track->setupUi(this);

  vtkSMProxy *prox = filter->getProxy();
  //"ContourValues" because slice and isocontour happen to use same name
  vtkSMProperty *prop = prox->GetProperty("ContourValues");
  if (prop)
    {
    vtkSMDoubleRangeDomain *dom = vtkSMDoubleRangeDomain::SafeDownCast(prop->GetDomain("bounds"));
    if (!dom)
      {
      dom = vtkSMDoubleRangeDomain::SafeDownCast(prop->GetDomain("scalar_range"));
      }
    if (dom)
      {
      this->Track->label->setText(filter->getSMName().toLower());
      this->Track->radioButton->setEnabled(true);
      this->Track->radioButton->setChecked(false);
      QObject::connect(this->Track->radioButton, SIGNAL(toggled(bool)),
                       this, SLOT(toggleTrack(bool)));

      pqScalarValueListPropertyWidget *vals = new pqScalarValueListPropertyWidget(prop, prox, this);
      vals->setFixedHeight(200);
      this->valsWidget = vals;

      vals->setRangeDomain(dom);
      vals->setEnabled(false);
      QVBoxLayout *valsLayout = new QVBoxLayout(this->Track->scrollAreaWidgetContents_2);
      valsLayout->addWidget(vals);
      valsLayout->insertSpacing(-1, 100);
      }
    }
};

//-----------------------------------------------------------------------------
pqCinemaTrack::~pqCinemaTrack()
{
}

//-----------------------------------------------------------------------------
void pqCinemaTrack::toggleTrack(bool checked)
{
  this->valsWidget->setEnabled(checked);
}

//-----------------------------------------------------------------------------
bool pqCinemaTrack::explore() const
{
  return (this->Track->radioButton->isChecked() && 
          this->valsWidget &&
          !this->valsWidget->scalars().isEmpty());
}

//-----------------------------------------------------------------------------
QVariantList pqCinemaTrack::scalars() const
{
  return this->valsWidget->scalars();
}

//-----------------------------------------------------------------------------
QString pqCinemaTrack::filterName() const
{
  return this->Track->label->text();
}
