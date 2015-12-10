/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkSMEnsembleReaderProxy.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkSMEnsembleDataReaderProxy
// .SECTION Description
// Examines file paths found in ensemble data files (.pve) and creates readers
// that can read those files. Sets the correct reader for each file on the
// corresponding VTK object.
#ifndef __vtkSMEnsembleDataReaderProxy_h
#define __vtkSMEnsembleDataReaderProxy_h

#include "vtkPVServerManagerDefaultModule.h" // for export
#include "vtkSMSourceProxy.h"

class VTKPVSERVERMANAGERDEFAULT_EXPORT vtkSMEnsembleDataReaderProxy : public vtkSMSourceProxy
{
public:
  vtkTypeMacro(vtkSMEnsembleDataReaderProxy, vtkSMSourceProxy);
  void PrintSelf(ostream &os, vtkIndent indent);
  static vtkSMEnsembleDataReaderProxy *New();

  virtual void UpdateVTKObjects();

protected:
  vtkSMEnsembleDataReaderProxy();
  virtual ~vtkSMEnsembleDataReaderProxy();

  virtual void SetPropertyModifiedFlag(const char *name, int flag);

  bool FileNamePotentiallyModified;
private:
  bool FetchFileNames();

  vtkSMEnsembleDataReaderProxy(const vtkSMEnsembleDataReaderProxy&); // Not implemented.
  void operator=(const vtkSMEnsembleDataReaderProxy&); // Not implemented.
};

#endif
