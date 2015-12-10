/*=========================================================================

  Program:   ParaView
  Module:    vtkPVEnsembleDataReaderInformation.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkPVEnsembleDataReaderInformation - Information obeject to
// collect file information from vtkEnsembleDataReader.
// .SECTION Description
// Gather information about data files from vtkEnsembleDataReader.

#ifndef __vtkPVEnsembleDataReaderInformation_h
#define __vtkPVEnsembleDataReaderInformation_h

#include "vtkPVClientServerCoreDefaultModule.h" //needed for exports
#include "vtkPVInformation.h"

class VTKPVCLIENTSERVERCOREDEFAULT_EXPORT vtkPVEnsembleDataReaderInformation
  : public vtkPVInformation
{
public:
  static vtkPVEnsembleDataReaderInformation* New();
  vtkTypeMacro(vtkPVEnsembleDataReaderInformation, vtkPVInformation);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Transfer information about a single object into this object.
  virtual void CopyFromObject(vtkObject*);

  //BTX
  // Description:
  // Manage a serialized version of the information.
  virtual void CopyToStream(vtkClientServerStream*);
  virtual void CopyFromStream(const vtkClientServerStream*);
  //ETX

  // Description:
  // Get number of files contained in the ensemble.
  virtual unsigned int GetFileCount();

  // Description:
  // Get the file path for the input row index.
  virtual vtkStdString GetFilePath(const unsigned int);

protected:
  vtkPVEnsembleDataReaderInformation();
  ~vtkPVEnsembleDataReaderInformation();

private:
  vtkPVEnsembleDataReaderInformation(const vtkPVEnsembleDataReaderInformation&); // Not implemented.
  void operator=(const vtkPVEnsembleDataReaderInformation&); // Not implemented.

  class vtkInternal;
  vtkInternal *Internal;
};

#endif
