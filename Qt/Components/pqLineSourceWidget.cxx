/*=========================================================================

   Program:   ParaQ
   Module:    pqLineSourceWidget.cxx

   Copyright (c) 2005-2008 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaQ is a free software; you can redistribute it and/or modify it
   under the terms of the ParaQ license version 1.2. 

   See License_v1.2.txt for the full ParaQ license.
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

=========================================================================*/

#include "pqApplicationCore.h"
#include "pqLineSourceWidget.h"
#include "pqPropertyLinks.h"
#include "pqSMSignalAdaptors.h"

#include "ui_pqLineSourceControls.h"

#include <vtkSMProxyProperty.h>
#include <vtkSMSourceProxy.h>
#include <vtkSMNewWidgetRepresentationProxy.h>

#include "../../unicode_text.h"

/////////////////////////////////////////////////////////////////////////
// pqLineSourceWidget::pqImplementation

class pqLineSourceWidget::pqImplementation
{
public:
  pqImplementation()
  {
    this->Links.setUseUncheckedProperties(false);
    this->Links.setAutoUpdateVTKObjects(true);
  }
  
  ~pqImplementation()
  {
    this->Links.removeAllPropertyLinks();
  }

  /// Contains the Qt widgets
  QWidget ControlsContainer;
  /// Stores the Qt widgets
  Ui::pqLineSourceControls UI;
  
  /// Maps Qt widgets to the 3D widget
  pqPropertyLinks Links;
};

/////////////////////////////////////////////////////////////////////////
// pqLineSourceWidget

pqLineSourceWidget::pqLineSourceWidget(vtkSMProxy* o, vtkSMProxy* pxy, QWidget* p) :
  Superclass(o, pxy, p, "LineSourceWidgetRepresentation"),
  Implementation(new pqImplementation())
{
  this->Implementation->UI.setupUi(
    &this->Implementation->ControlsContainer);

  this->layout()->addWidget(&this->Implementation->ControlsContainer);
  QLabel* label = new QLabel(UNICODE_TEXT("<b>\xD0\x9F\xD1\x80\xD0\xB8\xD0\xBC\xD0\xB5\xD1\x87\xD0\xB0\xD0\xBD\xD0\xB8\xD0\xB5\x3A\x20\xD0\xBF\xD0\xB5\xD1\x80\xD0\xB5\xD0\xB4\xD0\xB2\xD0\xB8\xD0\xB3\xD0\xB0\xD0\xB9\xD1\x82\xD0\xB5\x20\xD1\x83\xD0\xBA\xD0\xB0\xD0\xB7\xD0\xB0\xD1\x82\xD0\xB5\xD0\xBB\xD1\x8C\x20\xD0\xBC\xD1\x8B\xD1\x88\xD0\xB8\x20\xD0\xB8\x20\xD0\xB8\xD1\x81\xD0\xBF\xD0\xBE\xD0\xBB\xD1\x8C\xD0\xB7\xD1\x83\xD0\xB9\xD1\x82\xD0\xB5\x20\xD0\xBA\xD0\xBB\xD0\xB0\xD0\xB2\xD0\xB8\xD1\x88\xD1\x83\x20\x27\x50\x27\x20\xD0\xB4\xD0\xBB\xD1\x8F\x20\xD0\xB8\xD0\xB7\xD0\xBC\xD0\xB5\xD0\xBD\xD0\xB5\xD0\xBD\xD0\xB8\xD1\x8F\x20\xD0\xBF\xD0\xBE\xD0\xBB\xD0\xBE\xD0\xB6\xD0\xB5\xD0\xBD\xD0\xB8\xD1\x8F\x20\xD1\x82\xD0\xBE\xD1\x87\xD0\xBA\xD0\xB8</b>"), this);
  label->setWordWrap(1);
  this->layout()->addWidget(label);

  QObject::connect(&this->Implementation->Links, SIGNAL(qtWidgetChanged()),
    this, SLOT(setModified()));
}

//-----------------------------------------------------------------------------
pqLineSourceWidget::~pqLineSourceWidget()
{
  delete this->Implementation;
}

//-----------------------------------------------------------------------------
void pqLineSourceWidget::setControlledProperties(vtkSMProperty* point1, 
  vtkSMProperty* point2, vtkSMProperty* resolution)
{
  this->Superclass::setControlledProperties(point1, point2);
  this->setControlledProperty("Resolution", resolution);
}

//-----------------------------------------------------------------------------
void pqLineSourceWidget::setControlledProperty(const char* function,
  vtkSMProperty* _property)
{
  if (strcmp(function, "Resolution") == 0)
    {
    this->Implementation->Links.addPropertyLink(
      this->Implementation->UI.resolution, "value", 
      SIGNAL(valueChanged(int)),
      this->getWidgetProxy(), 
      this->getWidgetProxy()->GetProperty("Resolution"));
    }
  this->Superclass::setControlledProperty(function, _property);
}

