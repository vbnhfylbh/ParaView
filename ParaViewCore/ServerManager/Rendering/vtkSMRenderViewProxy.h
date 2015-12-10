/*=========================================================================

  Program:   ParaView
  Module:    vtkSMRenderViewProxy.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkSMRenderViewProxy - implementation for View that includes
// render window and renderers.
// .SECTION Description
// vtkSMRenderViewProxy is a 3D view consisting for a render window and two
// renderers: 1 for 3D geometry and 1 for overlayed 2D geometry.

#ifndef __vtkSMRenderViewProxy_h
#define __vtkSMRenderViewProxy_h

#include "vtkPVServerManagerRenderingModule.h" //needed for exports
#include "vtkSMViewProxy.h"
#include "vtkNew.h" // needed for vtkInteractorObserver.
class vtkCamera;
class vtkCollection;
class vtkIntArray;
class vtkRenderer;
class vtkRenderWindow;
class vtkRenderWindowInteractor;
class vtkSMDataDeliveryManager;
class vtkSMViewProxyInteractorHelper;

class VTKPVSERVERMANAGERRENDERING_EXPORT vtkSMRenderViewProxy : public vtkSMViewProxy
{
public:
  static vtkSMRenderViewProxy* New();
  vtkTypeMacro(vtkSMRenderViewProxy, vtkSMViewProxy);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Makes a new selection source proxy.
  bool SelectSurfaceCells(const int region[4],
    vtkCollection* selectedRepresentations,
    vtkCollection* selectionSources,
    bool multiple_selections=false);
  bool SelectSurfacePoints(const int region[4],
    vtkCollection* selectedRepresentations,
    vtkCollection* selectionSources,
    bool multiple_selections=false);
  bool SelectFrustumCells(const int region[4],
    vtkCollection* selectedRepresentations,
    vtkCollection* selectionSources,
    bool multiple_selections=false);
  bool SelectFrustumPoints(const int region[4],
    vtkCollection* selectedRepresentations,
    vtkCollection* selectionSources,
    bool multiple_selections=false);
  bool SelectPolygonPoints(vtkIntArray* polygon,
    vtkCollection* selectedRepresentations,
    vtkCollection* selectionSources,
    bool multiple_selections=false);
  bool SelectPolygonCells(vtkIntArray* polygon,
    vtkCollection* selectedRepresentations,
    vtkCollection* selectionSources,
    bool multiple_selections=false);

  // Description:
  // Returns the range for visible elements in the current view.
  bool ComputeVisibleScalarRange(const int region[4],
    int fieldAssociation, const char* scalarName,
    int component, double range[]);
  bool ComputeVisibleScalarRange(
    int fieldAssociation, const char* scalarName,
    int component, double range[]);

  // Description:
  // Convenience method to pick a location. Internally uses SelectSurfaceCells
  // to locate the picked object. In future, we can make this faster.
  vtkSMRepresentationProxy* Pick(int x, int y);

  // Description:
  // Convenience method to pick a block in a multi-block data set. Will return
  // the selected representation. Furthermore, if it is a multi-block data set
  // the flat index of the selected block will be returned in flatIndex.
  vtkSMRepresentationProxy* PickBlock(int x, int y, unsigned int &flatIndex);

  // Description:
  // Given a location is display coordinates (pixels), tries to compute and
  // return the world location on a surface, if possible. Returns true if the
  // conversion was successful, else returns false.
  // If Snap on mesh point is true, it will return a point from the mesh only
  bool ConvertDisplayToPointOnSurface(
    const int display_position[2], double world_position[3], bool snapOnMeshPoint = false);

  // Description:
  // Checks if color depth is sufficient to support selection.
  // If not, will return 0 and any calls to SelectVisibleCells will
  // quietly return an empty selection.
  virtual bool IsSelectionAvailable();

  // Description:
  // For backwards compatibility in Python scripts.
  void ResetCamera();
  void ResetCamera(double bounds[6]);
  void ResetCamera(
    double xmin, double xmax,
    double ymin, double ymax,
    double zmin, double zmax);

  // Description:
  // Convenience method for zooming to a representation.
  void ZoomTo(vtkSMProxy* representation);

  // Description:
  // Similar to IsSelectionAvailable(), however, on failure returns the
  // error message otherwise 0.
  virtual const char* IsSelectVisibleCellsAvailable();
  virtual const char* IsSelectVisiblePointsAvailable();

  // Description:
  // A client process need to set the interactor to enable interactivity. Use
  // this method to set the interactor and initialize it as needed by the
  // RenderView. This include changing the interactor style as well as
  // overriding VTK rendering to use the Proxy/ViewProxy API instead.
  virtual void SetupInteractor(vtkRenderWindowInteractor* iren);

  // Description:
  // Returns the interactor.
  virtual vtkRenderWindowInteractor* GetInteractor();

  // Description:
  // Returns the client-side renderer (composited or 3D).
  vtkRenderer* GetRenderer();

  // Description:
  // Returns the client-side camera object.
  vtkCamera* GetActiveCamera();

  // Description:
  // This method calls UpdateInformation on the Camera Proxy
  // and sets the Camera properties according to the info
  // properties.
  // This approach is a bit lame. We need to ensure that camera properties are
  // always/automatically synchronized. Camera properties cannot be treated same
  // way as other properties.
  void SynchronizeCameraProperties();

  // Description:
  // Returns true if the most recent render indeed employed low-res rendering.
  virtual bool LastRenderWasInteractive();

  // Description:
  // Returns the Z-buffer value at the given location in this view.
  double GetZBufferValue(int x, int y);

  // Description:
  // Called vtkPVView::Update on the server-side. Overridden to update the state
  // of NeedsUpdateLOD flag.
  virtual void Update();

  // Description:
  // We override that method to handle LOD and non-LOD NeedsUpdate in transparent manner.
  virtual bool GetNeedsUpdate();

  // Description:
  // Called to render a streaming pass. Returns true if the view "streamed" some
  // geometry.
  bool StreamingUpdate(bool render_if_needed);

  // Description:
  // Overridden to check through the various representations that this view can
  // create.
  virtual const char* GetRepresentationType(
    vtkSMSourceProxy* producer, int outputPort);

  // Description:
  // Returns the render window used by this view.
  virtual vtkRenderWindow* GetRenderWindow();

  // Description:
  // Provides access to the vtkSMViewProxyInteractorHelper object that handles
  // the interaction/view sync. We provide access to it for applications to
  // monitor timer events etc.
  vtkSMViewProxyInteractorHelper* GetInteractorHelper();

//BTX
protected:
  vtkSMRenderViewProxy();
  ~vtkSMRenderViewProxy();


  // Description:
  // Calls UpdateLOD() on the vtkPVRenderView.
  void UpdateLOD();

  // Description:
  // Overridden to ensure that we clean up the selection cache on the server
  // side.
  virtual void MarkDirty(vtkSMProxy* modifiedProxy);

  // Description:
  // Subclasses should override this method to do the actual image capture.
  virtual vtkImageData* CaptureWindowInternal(int magnification);
  virtual void CaptureWindowInternalRender();

  bool SelectFrustumInternal(const int region[4],
    vtkCollection* selectedRepresentations,
    vtkCollection* selectionSources,
    bool multiple_selections,
    int fieldAssociation);
  bool SelectPolygonInternal(vtkIntArray* polygon,
    vtkCollection* selectedRepresentations,
    vtkCollection* selectionSources,
    bool multiple_selections,
    const char* method);

  virtual vtkTypeUInt32 PreRender(bool interactive);
  virtual void PostRender(bool interactive);

  // Description:
  // Fetches the LastSelection from the data-server and then converts it to a
  // selection source proxy and returns that.
  bool FetchLastSelection(bool multiple_selections,
    vtkCollection* selectedRepresentations,
    vtkCollection* selectionSources);

  // Description:
  // Called at the end of CreateVTKObjects().
  virtual void CreateVTKObjects();

  // Description:
  // Returns true if the proxy is in interaction mode that corresponds to making
  // a selection i.e. vtkPVRenderView::INTERACTION_MODE_POLYGON or
  // vtkPVRenderView::INTERACTION_MODE_SELECTION.
  bool IsInSelectionMode();

  bool IsSelectionCached;
  void ClearSelectionCache(bool force = false);

  // Internal fields for the observer mechanism that is used to invalidate
  // the cache of selection when the current user became master
  unsigned long NewMasterObserverId;
  void NewMasterCallback(vtkObject* src, unsigned long event, void* data);

  vtkSMDataDeliveryManager* DeliveryManager;
  bool NeedsUpdateLOD;

private:
  vtkSMRenderViewProxy(const vtkSMRenderViewProxy&); // Not implemented
  void operator=(const vtkSMRenderViewProxy&); // Not implemented

  vtkNew<vtkSMViewProxyInteractorHelper> InteractorHelper;
//ETX
};

#endif
