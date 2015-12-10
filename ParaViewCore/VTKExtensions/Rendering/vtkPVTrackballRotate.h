/*=========================================================================

  Program:   ParaView
  Module:    vtkPVTrackballRotate.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkPVTrackballRotate - Rotates camera with xy mouse movement.
// .SECTION Description
// vtkPVTrackballRotate allows the user to interactively
// manipulate the camera, the viewpoint of the scene.

#ifndef __vtkPVTrackballRotate_h
#define __vtkPVTrackballRotate_h

#include "vtkCameraManipulator.h"
#include "vtkPVVTKExtensionsRenderingModule.h" // needed for export macro

class VTKPVVTKEXTENSIONSRENDERING_EXPORT vtkPVTrackballRotate : public vtkCameraManipulator
{
public:
  static vtkPVTrackballRotate *New();
  vtkTypeMacro(vtkPVTrackballRotate, vtkCameraManipulator);
  void PrintSelf(ostream& os, vtkIndent indent);
  
  // Description:
  // Event bindings controlling the effects of pressing mouse buttons
  // or moving the mouse.
  virtual void OnMouseMove(int x, int y, vtkRenderer *ren,
                           vtkRenderWindowInteractor *rwi);
  virtual void OnButtonDown(int x, int y, vtkRenderer *ren,
                            vtkRenderWindowInteractor *rwi);
  virtual void OnButtonUp(int x, int y, vtkRenderer *ren,
                          vtkRenderWindowInteractor *rwi);

  // Description:
  // These methods are called on all registered manipulators, not just the
  // active one. Hence, these should just be used to record state and not
  // perform any interactions.
  // Overridden to capture if the x,y,z key is pressed.
  virtual void OnKeyUp(vtkRenderWindowInteractor* iren);
  virtual void OnKeyDown(vtkRenderWindowInteractor* iren);

  // Description:
  // For setting the center of rotation.
  vtkSetVector3Macro(Center, double);
  vtkGetVector3Macro(Center, double);

  // Description:
  // Returns the currently pressed key code.
  vtkGetMacro(KeyCode, char);
protected:
  vtkPVTrackballRotate();
  ~vtkPVTrackballRotate();

  double Center[3];
  double DisplayCenter[2];

  char KeyCode;

  vtkPVTrackballRotate(const vtkPVTrackballRotate&); // Not implemented
  void operator=(const vtkPVTrackballRotate&); // Not implemented
};

#endif
