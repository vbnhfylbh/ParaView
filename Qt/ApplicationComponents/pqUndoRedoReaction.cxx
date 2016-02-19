/*=========================================================================

   Program: ParaView
   Module:    pqUndoRedoReaction.cxx

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
#include "pqUndoRedoReaction.h"

#include "pqApplicationCore.h"
#include "pqUndoStack.h"

#ifndef UNICODE_TEXT
#include <typeinfo>
#include <QApplication>
#define UNICODE_TEXT(text) QApplication::translate(typeid(*this).name(), QString(text).toStdString().c_str(), 0, QApplication::UnicodeUTF8)
#endif

//-----------------------------------------------------------------------------
pqUndoRedoReaction::pqUndoRedoReaction(QAction* parentObject, bool _undo)
  : Superclass(parentObject)
{
  this->Undo = _undo;
  this->enable(false);

  pqUndoStack* stack = pqApplicationCore::instance()->getUndoStack();
  if (!stack)
    {
    QObject::connect(pqApplicationCore::instance(),
      SIGNAL(undoStackChanged(pqUndoStack*)),
      this, SLOT(setUndoStack(pqUndoStack*)));
    }
  else
    {
    this->setUndoStack(stack);
    }
}

//-----------------------------------------------------------------------------
void pqUndoRedoReaction::setUndoStack(pqUndoStack* stack)
{
  if (this->Undo)
    {
    QObject::connect(stack, SIGNAL(canUndoChanged(bool)),
      this, SLOT(enable(bool)));
    QObject::connect(stack, SIGNAL(undoLabelChanged(const QString&)),
      this, SLOT(setLabel(const QString&)));
    }
  else
    {
    QObject::connect(stack, SIGNAL(canRedoChanged(bool)),
      this, SLOT(enable(bool)));
    QObject::connect(stack, SIGNAL(redoLabelChanged(const QString&)),
      this, SLOT(setLabel(const QString&)));
    }
}

//-----------------------------------------------------------------------------
void pqUndoRedoReaction::undo()
{
  pqUndoStack* stack = pqApplicationCore::instance()->getUndoStack();
  if (!stack)
    {
    qCritical("No application wide undo stack.");
    return;
    }
  stack->undo();
}

//-----------------------------------------------------------------------------
void pqUndoRedoReaction::redo()
{
  pqUndoStack* stack = pqApplicationCore::instance()->getUndoStack();
  if (!stack)
    {
    qCritical("No application wide undo stack.");
    return;
    }
  stack->redo();
}
//-----------------------------------------------------------------------------
void pqUndoRedoReaction::clear()
{
  pqUndoStack* stack = pqApplicationCore::instance()->getUndoStack();
  if (!stack)
    {
    qCritical("No application wide undo stack.");
    return;
    }
  stack->clear();
}

//-----------------------------------------------------------------------------
void pqUndoRedoReaction::enable(bool can_undo)
{
  this->parentAction()->setEnabled(can_undo);
}

//-----------------------------------------------------------------------------
void pqUndoRedoReaction::setLabel(const QString& label)
{
  if (this->Undo)
    {
    this->parentAction()->setText(UNICODE_TEXT(
      label.isEmpty() ? QString("\xD0\x9D\xD0\xB5\xD0\xBB\xD1\x8C\xD0\xB7\xD1\x8F\x20\xD0\xBE\xD1\x82\xD0\xBC\xD0\xB5\xD0\xBD\xD0\xB8\xD1\x82\xD1\x8C")
                      : QString("&\xD0\x9E\xD1\x82\xD0\xBC\xD0\xB5\xD0\xBD\xD0\xB8\xD1\x82\xD1\x8C %1").arg(label)));
    this->parentAction()->setStatusTip(UNICODE_TEXT(
      label.isEmpty() ? QString("\xD0\x9D\xD0\xB5\xD0\xBB\xD1\x8C\xD0\xB7\xD1\x8F\x20\xD0\xBE\xD1\x82\xD0\xBC\xD0\xB5\xD0\xBD\xD0\xB8\xD1\x82\xD1\x8C")
                      : QString("\xD0\x9E\xD1\x82\xD0\xBC\xD0\xB5\xD0\xBD\xD0\xB8\xD1\x82\xD1\x8C %1").arg(label)));
    }
  else
    {
    this->parentAction()->setText(UNICODE_TEXT(
      label.isEmpty() ? QString("\xD0\x9D\xD0\xB5\xD0\xBB\xD1\x8C\xD0\xB7\xD1\x8F\x20\xD0\xBF\xD0\xBE\xD0\xB2\xD1\x82\xD0\xBE\xD1\x80\xD0\xB8\xD1\x82\xD1\x8C")
                      : QString("&\xD0\x9F\xD0\xBE\xD0\xB2\xD1\x82\xD0\xBE\xD1\x80\xD0\xB8\xD1\x82\xD1\x8C %1").arg(label)));
    this->parentAction()->setStatusTip(UNICODE_TEXT(
      label.isEmpty() ? QString("\xD0\x9D\xD0\xB5\xD0\xBB\xD1\x8C\xD0\xB7\xD1\x8F\x20\xD0\xBF\xD0\xBE\xD0\xB2\xD1\x82\xD0\xBE\xD1\x80\xD0\xB8\xD1\x82\xD1\x8C")
                      : QString("\xD0\x9F\xD0\xBE\xD0\xB2\xD1\x82\xD0\xBE\xD1\x80\xD0\xB8\xD1\x82\xD1\x8C %1").arg(label)));
    }
}