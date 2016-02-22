/*=========================================================================

   Program: ParaView
   Module:    pqVCRToolbar.cxx

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
#include "pqVCRToolbar.h"
#include "ui_pqVCRToolbar.h"

#include "pqActiveObjects.h"
#include "pqPVApplicationCore.h"
#include "pqVCRController.h"
#include "pqUndoStack.h"
#include "pqAnimationManager.h"

#ifndef UNICODE_TEXT
#include <typeinfo>
#include <QApplication>
#define UNICODE_TEXT(text) QApplication::translate(typeid(*this).name(), QString(text).toStdString().c_str(), 0, QApplication::UnicodeUTF8)
#endif

class pqVCRToolbar::pqInternals : public Ui::pqVCRToolbar
{
};

//-----------------------------------------------------------------------------
void pqVCRToolbar::constructor()
{
  this->UI = new pqInternals();
  Ui::pqVCRToolbar &ui = *this->UI;
  ui.setupUi(this);

  pqVCRController* controller = new pqVCRController(this);
  this->Controller = controller;
  QObject::connect(pqPVApplicationCore::instance()->animationManager(),
    SIGNAL(activeSceneChanged(pqAnimationScene*)),
    controller, SLOT(setAnimationScene(pqAnimationScene*)));

  // Ideally pqVCRController needs to be deprecated in lieu of a more
  // action-reaction friendly implementation. But for now, I am simply reusing
  // the old code.
  QObject::connect(ui.actionVCRPlay, SIGNAL(triggered()),
    controller, SLOT(onPlay()));
  QObject::connect(ui.actionVCRFirstFrame, SIGNAL(triggered()),
    controller, SLOT(onFirstFrame()));
  QObject::connect(ui.actionVCRPreviousFrame, SIGNAL(triggered()),
    controller, SLOT(onPreviousFrame()));
  QObject::connect(ui.actionVCRNextFrame, SIGNAL(triggered()),
   controller, SLOT(onNextFrame()));
  QObject::connect(ui.actionVCRLastFrame, SIGNAL(triggered()),
    controller, SLOT(onLastFrame()));
  QObject::connect(ui.actionVCRLoop, SIGNAL(toggled(bool)),
    controller, SLOT(onLoop(bool)));

  QObject::connect(controller, SIGNAL(enabled(bool)),
    ui.actionVCRPlay, SLOT(setEnabled(bool)));
  QObject::connect(controller, SIGNAL(enabled(bool)),
    ui.actionVCRFirstFrame, SLOT(setEnabled(bool)));
  QObject::connect(controller, SIGNAL(enabled(bool)),
    ui.actionVCRPreviousFrame, SLOT(setEnabled(bool)));
  QObject::connect(controller, SIGNAL(enabled(bool)),
    ui.actionVCRNextFrame, SLOT(setEnabled(bool)));
  QObject::connect(controller, SIGNAL(enabled(bool)),
    ui.actionVCRLastFrame, SLOT(setEnabled(bool)));
  QObject::connect(controller, SIGNAL(enabled(bool)),
    ui.actionVCRLoop, SLOT(setEnabled(bool)));
  QObject::connect(controller, SIGNAL(timeRanges(double, double)),
    this, SLOT(setTimeRanges(double, double)));
  QObject::connect(controller, SIGNAL(loop(bool)),
    ui.actionVCRLoop, SLOT(setChecked(bool)));
  QObject::connect(controller, SIGNAL(playing(bool)),
    this, SLOT(onPlaying(bool)));
}

//-----------------------------------------------------------------------------
pqVCRToolbar::~pqVCRToolbar()
{
  delete this->UI;
  this->UI = 0;
}

//-----------------------------------------------------------------------------
void pqVCRToolbar::setTimeRanges(double start, double end)
{
  this->UI->actionVCRFirstFrame->setToolTip(
    UNICODE_TEXT("\xD0\x9F\xD0\xB5\xD1\x80\xD0\xB2\xD1\x8B\xD0\xB9\x20\xD0\xBA\xD0\xB0\xD0\xB4\xD1\x80 (%1)").arg(start, 0, 'g'));
  this->UI->actionVCRLastFrame->setToolTip(
    UNICODE_TEXT("\xD0\x9F\xD0\xBE\xD1\x81\xD0\xBB\xD0\xB5\xD0\xB4\xD0\xBD\xD0\xB8\xD0\xB9\x20\xD0\xBA\xD0\xB0\xD0\xB4\xD1\x80 (%1)").arg(end, 0, 'g'));
}

//-----------------------------------------------------------------------------
void pqVCRToolbar::onPlaying(bool playing)
{
  if(playing)
    {
    disconnect(this->UI->actionVCRPlay, SIGNAL(triggered()),
      this->Controller, SLOT(onPlay()));
    connect(this->UI->actionVCRPlay, SIGNAL(triggered()),
      this->Controller, SLOT(onPause()));
    this->UI->actionVCRPlay->setIcon(
      QIcon(":/pqWidgets/Icons/pqVcrPause24.png"));
    this->UI->actionVCRPlay->setText("Pa&use");
    }
  else
    {
    connect(this->UI->actionVCRPlay, SIGNAL(triggered()),
      this->Controller, SLOT(onPlay()));
    disconnect(this->UI->actionVCRPlay, SIGNAL(triggered()),
      this->Controller, SLOT(onPause()));
    this->UI->actionVCRPlay->setIcon(
      QIcon(":/pqWidgets/Icons/pqVcrPlay24.png"));
    this->UI->actionVCRPlay->setText(UNICODE_TEXT("\xD0\x9F\xD1\x80\xD0\xBE\xD0\xB8\xD0\xB3\xD1\x80\xD0\xB0\xD1\x82\xD1\x8C"));
    }

  // this becomes a behavior.
  // this->Implementation->Core->setSelectiveEnabledState(!playing);
}

