/*=========================================================================

  Program:   ParaView
  Module:    vtkImageFileSeriesReader.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkImageFileSeriesReader - adds support for optionally reading image
// stacks.
// .SECTION Description
// vtkImageFileSeriesReader is designed for vtkImageReader2 and subclasses. This
// adds API to optionally treat the file series as an image stack rather than an
// temporal dataset.
// When ReadAsImageStack is true, we simply by-pass the superclass and instead
// pass all filenames to the internal reader and then let it handle the pipeline
// requests.

#ifndef __vtkImageFileSeriesReader_h
#define __vtkImageFileSeriesReader_h

#include "vtkFileSeriesReader.h"

class VTKPVVTKEXTENSIONSDEFAULT_EXPORT vtkImageFileSeriesReader : public vtkFileSeriesReader
{
public:
  static vtkImageFileSeriesReader* New();
  vtkTypeMacro(vtkImageFileSeriesReader, vtkFileSeriesReader);
  void PrintSelf(ostream& os, vtkIndent indent);

  vtkSetMacro(ReadAsImageStack, bool);
  vtkGetMacro(ReadAsImageStack, bool);
  vtkBooleanMacro(ReadAsImageStack, bool);

  // Description:
  // Overridden to directly call the internal reader after passing it the
  // correct filenames when ReadAsImageStack is true.
  virtual int ProcessRequest(vtkInformation*, vtkInformationVector**, vtkInformationVector*);


//BTX
protected:
  vtkImageFileSeriesReader();
  ~vtkImageFileSeriesReader();

  void UpdateFileNames();

  bool ReadAsImageStack;
private:
  vtkImageFileSeriesReader(const vtkImageFileSeriesReader&); // Not implemented
  void operator=(const vtkImageFileSeriesReader&); // Not implemented
//ETX
};

#endif
