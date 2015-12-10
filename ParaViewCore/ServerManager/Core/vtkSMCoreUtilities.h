/*=========================================================================

  Program:   ParaView
  Module:    vtkSMCoreUtilities.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkSMCoreUtilities - collection of utilities.
// .SECTION Description
// vtkSMCoreUtilities provides miscellaneous utility functions.

#ifndef __vtkSMCoreUtilities_h
#define __vtkSMCoreUtilities_h

#include "vtkPVServerManagerCoreModule.h" //needed for exports
#include "vtkObject.h"
#include "vtkStdString.h" // needed for vtkStdString.

class vtkSMProxy;

class VTKPVSERVERMANAGERCORE_EXPORT vtkSMCoreUtilities : public vtkObject
{
public:
  static vtkSMCoreUtilities* New();
  vtkTypeMacro(vtkSMCoreUtilities, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Given a proxy (or proxy prototype), returns the name of the property that
  // ParaView application will be use as the default FileName property.
  // Returns the name of the property or NULL when no such property is found.
  static const char* GetFileNameProperty(vtkSMProxy*);

  // Description:
  // Sanitize a label/name to be remove spaces, delimiters etc.
  static vtkStdString SanitizeName(const char*);

  // Description:
  // Given a range, converts it to be a valid range to switch to log space. If
  // the range is changed, returns true, otherwise returns false.
  static bool AdjustRangeForLog(double range[2]);
  static bool AdjustRangeForLog(double &rmin, double &rmax)
    {
    double range[2] = { rmin, rmax };
    bool retVal = vtkSMCoreUtilities::AdjustRangeForLog(range);
    rmin = range[0]; rmax = range[1];
    return retVal;
    }

  // Description:
  // Given a range, adjusts it so that it is a valid range i.e. range[0] <
  // range[1]. This will always perturb the range[1] by a factor of the value itself.
  // This assumes range[1] < range[0] to indicate an invalid range and returns
  // false without changing them. If the range is changed, returns true,
  // otherwise false.
  static bool AdjustRange(double range[2]);
  static bool AdjustRange(double &rmin, double &rmax)
    {
    double range[2] = { rmin, rmax };
    bool retVal = vtkSMCoreUtilities::AdjustRange(range);
    rmin = range[0]; rmax = range[1];
    return retVal;
    }

//BTX
protected:
  vtkSMCoreUtilities();
  ~vtkSMCoreUtilities();

private:
  vtkSMCoreUtilities(const vtkSMCoreUtilities&); // Not implemented
  void operator=(const vtkSMCoreUtilities&); // Not implemented
//ETX
};

#endif
