/*=========================================================================

  Program:   ParaView
  Module:    $RCSfile$

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkSMSessionClient
// .SECTION Description
// vtkSMSessionClient is a remote-session that connects to a remote server.
// vtkSMSessionClient supports both connecting a pvserver as well as connecting
// a pvdataserver/pvrenderserver.

#ifndef __vtkSMSessionClient_h
#define __vtkSMSessionClient_h

#include "vtkSMSession.h"
#include "vtkSMMessage.h"

class vtkMultiProcessController;

class VTK_EXPORT vtkSMSessionClient : public vtkSMSession
{
public:
  static vtkSMSessionClient* New();
  vtkTypeMacro(vtkSMSessionClient, vtkSMSession);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Connects a remote server. URL can be of the following format:
  // cs://<pvserver-host>:<pvserver-port>
  // cdsrs://<pvdataserver-host>:<pvdataserver-port>/<pvrenderserver-host>:<pvrenderserver-port>
  // In both cases the port is optional. When not provided default
  // pvserver/pvdataserver port // is 11111, while default pvrenderserver port
  // is 22221.
  // For reverse connect i.e. the client waits for the server to connect back,
  // simply add "rc" to the protocol e.g.
  // csrc://<pvserver-host>:<pvserver-port>
  // cdsrsrc://<pvdataserver-host>:<pvdataserver-port>/<pvrenderserver-host>:<pvrenderserver-port>
  // In this case, the hostname is irrelevant and is ignored.
  virtual bool Connect(const char* url);

  // Description:
  // Returns true is this session is active/alive/valid.
  virtual bool GetIsAlive();

  // Description:
  // Push the state.
  virtual void PushState(vtkSMMessage* msg);

  // Description:
  // When Connect() is waiting for a server to connect back to the client (in
  // reverse connect mode), then it periodically fires ProgressEvent.
  // Application can add observer to this signal and set this flag to true, if
  // it wants to abort the wait for the server.
  vtkSetMacro(AbortConnect, bool);

  // Description:
  // Gracefully exits the session.
  void CloseSession();

//BTX
  enum {
    PUSH=1,
    INVOKE=2,
    PULL=3,
    CLIENT_SERVER_MESSAGE_RMI=55625,
    CLOSE_SESSION=55626
    };

protected:
  vtkSMSessionClient();
  ~vtkSMSessionClient();

  void SetRenderServerController(vtkMultiProcessController*);
  void SetDataServerController(vtkMultiProcessController*);

  // Both maybe the same when connected to pvserver.
  vtkMultiProcessController* RenderServerController;
  vtkMultiProcessController* DataServerController;

  bool AbortConnect;
private:
  vtkSMSessionClient(const vtkSMSessionClient&); // Not implemented
  void operator=(const vtkSMSessionClient&); // Not implemented
//ETX
};

#endif
