/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkEnsembleDataReader.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkEnsembleDataReader - reader for ensemble data sets
// .SECTION Description
// vtkEnsembleDataReader reads a collection of data sources from a metadata
// file (of extension .pve).
// 'pve' a simply CSV file with the last column being the relative filename and
// other columns for each of the variables in the ensemble.
#ifndef __vtkEnsembleDataReader_h
#define __vtkEnsembleDataReader_h

#include "vtkPVVTKExtensionsDefaultModule.h" //needed for exports
#include "vtkDataObjectAlgorithm.h"

class VTKPVVTKEXTENSIONSDEFAULT_EXPORT vtkEnsembleDataReader : public vtkDataObjectAlgorithm
{
public:
  static vtkEnsembleDataReader *New();
  vtkTypeMacro(vtkEnsembleDataReader, vtkDataObjectAlgorithm);
  void PrintSelf(ostream &os, vtkIndent indent);

  // Description:
  // Set/Get the filename of the ensemble (.pve extension).
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);

  // Description:
  // Set/Get the current ensemble member to process.
  vtkSetMacro(CurrentMember, unsigned int);
  vtkGetMacro(CurrentMember, unsigned int);

  // Description:
  // Returns the number of ensemble members
  unsigned int GetNumberOfMembers() const;
  vtkGetVector2Macro(CurrentMemberRange, unsigned int);

  // Description:
  // Get the file path associated with the specified row of the meta data
  vtkStdString GetFilePath(unsigned int rowIndex) const;

  // Description:
  // Set the file reader for the specified row of data
  void SetReader(unsigned int rowIndex, vtkAlgorithm *reader);

  // Description:
  // Removes all readers set using SetReader().
  void ResetReaders();

  // Description:
  // Use this method to update the meta data, if needed. This will only read the
  // file again if cache is obsolete.
  bool UpdateMetaData();

protected:
  vtkEnsembleDataReader();
  ~vtkEnsembleDataReader();

  virtual int ProcessRequest(
    vtkInformation*, vtkInformationVector**, vtkInformationVector*);
  vtkAlgorithm *GetCurrentReader();

private:
  char *FileName;
  unsigned int CurrentMember;
  unsigned int CurrentMemberRange[2];

  class vtkInternal;
  vtkInternal *Internal;

  vtkEnsembleDataReader(const vtkEnsembleDataReader&); // Not implemented
  void operator=(const vtkEnsembleDataReader&); // Not implemented
};

#endif
