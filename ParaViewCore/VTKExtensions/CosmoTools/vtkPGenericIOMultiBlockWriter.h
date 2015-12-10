/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPGenericIOMultiBlockWriter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkPGenericIOMultiBlockWriter 
// .SECTION Description

#ifndef __vtkPGenericIOMultiBlockWriter_h
#define __vtkPGenericIOMultiBlockWriter_h

#include "vtkWriter.h" // superclass
#include "vtkPVVTKExtensionsCosmoToolsModule.h" // for export macro

class vtkMultiProcessController;

class VTKPVVTKEXTENSIONSCOSMOTOOLS_EXPORT vtkPGenericIOMultiBlockWriter : public vtkWriter
{
public:
  static vtkPGenericIOMultiBlockWriter* New();
  vtkTypeMacro(vtkPGenericIOMultiBlockWriter, vtkWriter);
  void PrintSelf(ostream& os, vtkIndent indent);

  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);


//BTX
protected:
  vtkPGenericIOMultiBlockWriter();
  ~vtkPGenericIOMultiBlockWriter();

  virtual int FillInputPortInformation(int port, vtkInformation *info);
  virtual void WriteData();

private:
  class vtkInternals;
  vtkInternals* Internals;
  char* FileName;
  vtkMultiProcessController* Controller;
  vtkPGenericIOMultiBlockWriter(const vtkPGenericIOMultiBlockWriter&); // Not implemented.
  void operator=(const vtkPGenericIOMultiBlockWriter&); // Not implemented.
//ETX
};

#endif
