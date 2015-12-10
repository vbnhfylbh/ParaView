/*=========================================================================

  Program:   ParaView
  Module:    vtkSMViewProxyInteractorHelper.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkSMViewProxyInteractorHelper - helper class that make it easier to
// hook vtkRenderWindowInteractor and vtkSMViewProxy.
// .SECTION Description
// vtkSMViewProxyInteractorHelper is a helper class designed to make it easier
// to hook up vtkRenderWindowInteractor to call methods on a vtkSMViewProxy (or
// subclass). It's primarily designed to work with vtkSMRenderViewProxy (and
// subclasses), but it should work with other types of views too.
//
// To use this helper, the view typically creates a instance for itself as register
// itself (using vtkSMViewProxyInteractorHelper::SetViewProxy) and then calls
// vtkSMViewProxyInteractorHelper::SetupInteractor(). This method will initialize
// the interactor (potentially changing some ivars on the interactor to avoid
// automatic rendering, using vtkRenderWindowInteractor::EnableRenderOff(), etc.)
// and setup event observer to monitor interaction.
//
// vtkSMViewProxyInteractorHelper only using vtkSMViewProxy::StillRender() and
// vtkSMViewProxy::InteractiveRender() APIs directly. However several properties can
// be optionally present on the view proxy to dictate this class' behaviour. These
// are as follows:
//
// \li \c NonInteractiveRenderDelay :- when present provides time in seconds to
// delay the StillRender() call after user interaction has ended i.e.
// vtkRenderWindowInteractor fires the vtkCommand::EndInteractionEvent. If
// missing, or less than 0.01, the view will immediately render.
//
// \li \c EnableRenderOnInteraction :- when present provides a flag whether the interactor
// should trigger the render calls (either StillRender or InteractiveRender) as
// a consequence of interaction. If missing, we treat EnableRender as ON.
#ifndef __vtkSMViewProxyInteractorHelper_h
#define __vtkSMViewProxyInteractorHelper_h

#include "vtkPVServerManagerRenderingModule.h" //needed for exports
#include "vtkWeakPointer.h" //needed for vtkWeakPointer
#include "vtkObject.h"

class vtkCommand;
class vtkRenderWindowInteractor;
class vtkSMViewProxy;

class VTKPVSERVERMANAGERRENDERING_EXPORT vtkSMViewProxyInteractorHelper : public vtkObject
{
public:
  static vtkSMViewProxyInteractorHelper* New();
  vtkTypeMacro(vtkSMViewProxyInteractorHelper, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set the view proxy.
  // This is a weak reference i.e. the view proxy's
  // reference count will be unchanged by this call.
  void SetViewProxy(vtkSMViewProxy* proxy);
  vtkSMViewProxy* GetViewProxy();

  // Description:
  // Set the interactor to "help" the view with.
  // This is a weak reference i.e. the interactor's
  // reference count will be unchanged by this call.
  void SetupInteractor(vtkRenderWindowInteractor* iren);
  vtkRenderWindowInteractor* GetInteractor();
  void CleanupInteractor() { this->SetupInteractor(NULL); }

//BTX
protected:
  vtkSMViewProxyInteractorHelper();
  ~vtkSMViewProxyInteractorHelper();

  // Description:
  // Handle event.
  void Execute(vtkObject* caller, unsigned long event, void* calldata);
  void Render();
  void CleanupTimer();

  vtkCommand* Observer;
  vtkWeakPointer<vtkSMViewProxy> ViewProxy;
  vtkWeakPointer<vtkRenderWindowInteractor> Interactor;
  int DelayedRenderTimerId;
  bool Interacting;
  bool Interacted;

private:
  vtkSMViewProxyInteractorHelper(const vtkSMViewProxyInteractorHelper&); // Not implemented
  void operator=(const vtkSMViewProxyInteractorHelper&); // Not implemented
//ETX
};

#endif
