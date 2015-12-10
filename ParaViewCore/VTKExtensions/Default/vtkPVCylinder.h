/*=========================================================================

  Program:   ParaView
  Module:    vtkPVCylinder

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkPVCylinder - extends vtkCylinder to add ParaView specific API.
// .SECTION Description
// vtkPVCylinder extends vtkCylinder to add ParaView specific API.

#ifndef __vtkPVCylinder_h
#define __vtkPVCylinder_h

#include "vtkPVVTKExtensionsDefaultModule.h" //needed for exports
#include "vtkCylinder.h"

class VTKPVVTKEXTENSIONSDEFAULT_EXPORT vtkPVCylinder : public vtkCylinder
{
public:
  static vtkPVCylinder* New();
  vtkTypeMacro(vtkPVCylinder, vtkCylinder)
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Get/Set the vector defining the direction of the cylinder.
  void SetAxis(double x, double y, double z);
  void SetAxis(const double axis[3]);
  vtkGetVector3Macro(Axis, double)

  // Reimplemented to update transform on change:
  virtual void SetCenter(double x, double y, double z);
  virtual void SetCenter(double xyz[3]);

protected:
  vtkPVCylinder();
  ~vtkPVCylinder();

  void UpdateTransform();

  double Axis[3];

private:
  vtkPVCylinder(const vtkPVCylinder&); // Not implemented
  void operator=(const vtkPVCylinder&); // Not implemented
};

inline void vtkPVCylinder::SetAxis(double x, double y, double z)
{
  double axis[3] = {x, y, z};
  this->SetAxis(axis);
}

#endif
