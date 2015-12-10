/*=========================================================================

  Program:   ParaView
  Module:    vtkSMInputFileNameDomain.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkSMInputFileNameDomain - a string domain that can be set automatically
// with the source file name
// .SECTION Description
// vtkSMInputFileNameDomain does not really restrict the values of the property
// that contains it. All string values are valid. Rather, it is used to
// annotate a pipeline with the source name. This domain
// works with only vtkSMStringVectorProperty.
// .SECTION See Also
// vtkSMDomain vtkSMStringVectorProperty

#ifndef __vtkSMInputFileNameDomain_h
#define __vtkSMInputFileNameDomain_h

#include "vtkPVServerManagerCoreModule.h" //needed for exports
#include "vtkSMStringListDomain.h"

class VTKPVSERVERMANAGERCORE_EXPORT vtkSMInputFileNameDomain : public vtkSMStringListDomain
{
public:
  static vtkSMInputFileNameDomain* New();
  vtkTypeMacro(vtkSMInputFileNameDomain, vtkSMStringListDomain);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Update self checking the "unchecked" values of all required
  // properties. Overwritten by sub-classes.
  virtual void Update(vtkSMProperty*);

  vtkGetMacro(FileName, std::string);

  // Description:
  // This method is over-written to automatically update the 
  // default filename at run time. The default filename 
  // depends on the filename of the source.
  // Returns 1 if the domain updated the property.
  virtual int SetDefaultValues(vtkSMProperty*, bool use_unchecked_values);

protected:
  vtkSMInputFileNameDomain();
  ~vtkSMInputFileNameDomain();

private:
  vtkSMInputFileNameDomain(const vtkSMInputFileNameDomain&); // Not implemented
  void operator=(const vtkSMInputFileNameDomain&); // Not implemented
  
  std::string FileName;
};

#endif
