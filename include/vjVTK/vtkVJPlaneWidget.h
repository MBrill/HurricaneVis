/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: nil c-basic-offset: 3 -*- */
// vim:cindent:ts=3:sw=3:et:tw=80:sta:
/***************************vjVTK copyright begin *****************************
 * 
 * The vjVTK library is a derivitive work, based on two 
 * seperate projects, VR Juggler and the Visualization Toolkit.
 * Those portions of this package which are taken from the 
 * original works remains the works of their original authors.
 * For the original copyrights please refer to vrj-copyright.c.txt
 * and VTK_Copyright.txt respectively. vjVTK is distributed
 * to the extent possible under the terms of the LGPL licensing
 *
 * copyright (c) 2005,2006 Kristopher J. Blom
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 *
 ***************************vjVTK copyright end ******************************/
/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkVJPlaneWidget.h,v $
  Language:  C++
  Date:      $Date: 2006/10/22 19:45:45 $
  Version:   $Revision: 1.2 $

  Copyright (c) 1993-2002 Ken Martin, Will Schroeder, Bill Lorensen 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkPlaneWidget - 3D widget for manipulating a finite plane
// .SECTION Description
// This 3D widget defines a finite (bounded) plane that can be interactively
// placed in a scene. The plane has four handles (at its corner vertices), a
// normal vector, and the plane itself. The handles are used to resize the
// plane; the normal vector to rotate it, and the plane can be picked and
// translated. A nice feature of the object is that the vtkPlaneWidget, like
// any 3D widget, will work with the current interactor style. That is, if
// vtkPlaneWidget does not handle an event, then all other registered
// observers (including the interactor style) have an opportunity to process
// the event. Otherwise, the vtkPlaneWidget will terminate the processing of
// the event that it handles.
//
// To use this object, just invoke SetInteractor() with the argument of the
// method a vtkRenderWindowInteractor.  You may also wish to invoke
// "PlaceWidget()" to initially position the widget. If the "i" key (for
// "interactor") is pressed, the vtkPlaneWidget will appear. (See superclass
// documentation for information about changing this behavior.) By grabbing
// the one of the four handles (use the left mouse button), the plane can be
// resized.  By grabbing the plane itself, the entire plane can be
// arbitrarily translated. If you select the normal vector, the plane can be
// arbitrarily rotated. Selecting any part of the widget with the middle
// mouse button enables translation of the plane along its normal. (Once
// selected using middle mouse, moving the mouse in the direction of the
// normal translates the plane in the direction of the normal; moving in the
// direction opposite the normal translates the plane in the direction
// opposite the normal.) Scaling (about the center of the plane) is achieved
// by using the right mouse button. By moving the mouse "up" the render
// window the plane will be made bigger; by moving "down" the render window
// the widget will be made smaller. Events that occur outside of the widget
// (i.e., no part of the widget is picked) are propagated to any other
// registered obsevers (such as the interaction style).  Turn off the widget
// by pressing the "i" key again (or invoke the Off() method).
//
// The vtkPlaneWidget has several methods that can be used in conjunction
// with other VTK objects. The Set/GetResolution() methods control the number
// of subdivisions of the plane; the GetPolyData() method can be used to get
// the polygonal representation and can be used for things like seeding
// stream lines. GetPlane() can be used to update a vtkPlane implicit
// function. Typical usage of the widget is to make use of the
// StartInteractionEvent, InteractionEvent, and EndInteractionEvent
// events. The InteractionEvent is called on mouse motion; the other two
// events are called on button down and button up (either left or right
// button).
//
// Some additional features of this class include the ability to control the
// properties of the widget. You can set the properties of the selected and
// unselected representations of the plane. For example, you can set the
// property for the handles and plane. In addition there are methods to
// constrain the plane so that it is perpendicular to the x-y-z axes.

// .SECTION Caveats
// Note that handles and plane can be picked even when they are "behind" other
// actors.  This is an intended feature and not a bug.

// .SECTION See Also
// vtk3DWidget vtkBoxWidget vtkLineWidget vtkSphereWidget
// vtkImplicitPlaneWidget

#ifndef __vtkPlaneWidget_h
#define __vtkPlaneWidget_h

#include "vtkPolyDataSourceWidget.h"

class vtkActor;
class vtkCellPicker;
class vtkConeSource;
class vtkLineSource;
class vtkPlaneSource;
class vtkPoints;
class vtkPolyData;
class vtkPolyDataMapper;
class vtkProp;
class vtkProperty;
class vtkSphereSource;
class vtkTransform;
class vtkPlane;

class vtkVJPicker;
#include <vjRenderer.h>

#define VTK_PLANE_OFF 0
#define VTK_PLANE_OUTLINE 1
#define VTK_PLANE_WIREFRAME 2
#define VTK_PLANE_SURFACE 3

class VTK_HYBRID_EXPORT vtkVJPlaneWidget : public vtkPolyDataSourceWidget
{
public:
  // Description:
  // Instantiate the object.
  static vtkVJPlaneWidget *New();

  vtkTypeRevisionMacro(vtkVJPlaneWidget,vtkPolyDataSourceWidget);
  void PrintSelf(ostream& os, vtkIndent indent);
						
  void SetVJRenderer(vrj::vjRenderer* renderer){mVJRenderer = renderer;}

  // Description:
  // Methods that satisfy the superclass' API.
  virtual void SetEnabled(int);
  virtual void PlaceWidget(double bounds[6]);
  void PlaceWidget()
    {this->Superclass::PlaceWidget();}

  // Description:
  // Set/Get the resolution (number of subdivisions) of the plane.
  void SetResolution(int r);
  int GetResolution();

  // Description:
  // Set/Get the origin of the plane.
  void SetOrigin(double x, double y, double z);
  void SetOrigin(double x[3]);
  double* GetOrigin();
  void GetOrigin(double xyz[3]);

  // Description:
  // Set/Get the position of the point defining the first axis of the plane.
  void SetPoint1(double x, double y, double z);
  void SetPoint1(double x[3]);
  double* GetPoint1();
  void GetPoint1(double xyz[3]);
  
  // Description:
  // Set/Get the position of the point defining the second axis of the plane.
  void SetPoint2(double x, double y, double z);
  void SetPoint2(double x[3]);
  double* GetPoint2();
  void GetPoint2(double xyz[3]);

  // Description:
  // Get the center of the plane.
  void SetCenter(double x, double y, double z);
  void SetCenter(double x[3]);
  double* GetCenter();
  void GetCenter(double xyz[3]);

  // Description:
  // Get the normal to the plane.
  void SetNormal(double x, double y, double z);
  void SetNormal(double x[3]);
  double* GetNormal();
  void GetNormal(double xyz[3]);
  
  // Description:
  // Control how the plane appears when GetPolyData() is invoked.
  // If the mode is "outline", then just the outline of the plane
  // is shown. If the mode is "wireframe" then the plane is drawn
  // with the outline plus the interior mesh (corresponding to the
  // resolution specified). If the mode is "surface" then the plane
  // is drawn as a surface.
  vtkSetClampMacro(Representation,int,VTK_PLANE_OFF,VTK_PLANE_SURFACE);
  vtkGetMacro(Representation,int);
  void SetRepresentationToOff()
    {this->SetRepresentation(VTK_PLANE_OFF);}
  void SetRepresentationToOutline()
    {this->SetRepresentation(VTK_PLANE_OUTLINE);}
  void SetRepresentationToWireframe()
    {this->SetRepresentation(VTK_PLANE_WIREFRAME);}
  void SetRepresentationToSurface()
    {this->SetRepresentation(VTK_PLANE_SURFACE);}

  // Description:
  // Force the plane widget to be aligned with one of the x-y-z axes.
  // Remember that when the state changes, a ModifiedEvent is invoked.
  // This can be used to snap the plane to the axes if it is orginally
  // not aligned.
  vtkSetMacro(NormalToXAxis,int);
  vtkGetMacro(NormalToXAxis,int);
  vtkBooleanMacro(NormalToXAxis,int);
  vtkSetMacro(NormalToYAxis,int);
  vtkGetMacro(NormalToYAxis,int);
  vtkBooleanMacro(NormalToYAxis,int);
  vtkSetMacro(NormalToZAxis,int);
  vtkGetMacro(NormalToZAxis,int);
  vtkBooleanMacro(NormalToZAxis,int);

  // Description:
  // Grab the polydata (including points) that defines the plane.  The
  // polydata consists of (res+1)*(res+1) points, and res*res quadrilateral
  // polygons, where res is the resolution of the plane. These point values
  // are guaranteed to be up-to-date when either the InteractionEvent or
  // EndInteraction events are invoked. The user provides the vtkPolyData and
  // the points and polyplane are added to it.
  void GetPolyData(vtkPolyData *pd);

  // Description:
  // Get the planes describing the implicit function defined by the plane
  // widget. The user must provide the instance of the class vtkPlane. Note
  // that vtkPlane is a subclass of vtkImplicitFunction, meaning that it can
  // be used by a variety of filters to perform clipping, cutting, and
  // selection of data.
  void GetPlane(vtkPlane *plane);

  // Description:
  // Satisfies superclass API.  This returns a pointer to the underlying
  // PolyData.  Make changes to this before calling the initial PlaceWidget()
  // to have the initial placement follow suit.  Or, make changes after the
  // widget has been initialised and call UpdatePlacement() to realise.
  vtkPolyDataAlgorithm* GetPolyDataAlgorithm();

  // Description:
  // Satisfies superclass API.  This will change the state of the widget to
  // match changes that have been made to the underlying PolyDataSource
  void UpdatePlacement(void);

  // Description:
  // Get the handle properties (the little balls are the handles). The 
  // properties of the handles when selected and normal can be 
  // manipulated.
  vtkGetObjectMacro(HandleProperty,vtkProperty);
  vtkGetObjectMacro(SelectedHandleProperty,vtkProperty);
  
  // Description:
  // Get the plane properties. The properties of the plane when selected 
  // and unselected can be manipulated.
  virtual void SetPlaneProperty(vtkProperty*);
  vtkGetObjectMacro(PlaneProperty,vtkProperty);
  vtkGetObjectMacro(SelectedPlaneProperty,vtkProperty);
  
protected:
  vtkVJPlaneWidget();
  ~vtkVJPlaneWidget();

//BTX - manage the state of the widget
  int State;
  enum WidgetState
  {
    Start=0,
    Moving,
    Scaling,
    Pushing,
    Rotating,
    Outside
  };
//ETX
    
  //handles the events
  static void ProcessEvents(vtkObject* object, 
                            unsigned long event,
                            void* clientdata, 
                            void* calldata);

  // ProcessEvents() dispatches to these methods.
  void OnLeftButtonDown();
  void OnLeftButtonUp();
  void OnMiddleButtonDown();
  void OnMiddleButtonUp();
  void OnRightButtonDown();
  void OnRightButtonUp();
  void OnMouseMove();

  // controlling ivars
  int NormalToXAxis;
  int NormalToYAxis;
  int NormalToZAxis;
  int Representation;
  void SelectRepresentation();

  // the plane
  vtkActor          *PlaneActor;
  vtkPolyDataMapper *PlaneMapper;
  vtkPlaneSource    *PlaneSource;
  vtkPolyData       *PlaneOutline;
  void HighlightPlane(int highlight);

  // glyphs representing hot spots (e.g., handles)
  vtkActor          **Handle;
  vtkPolyDataMapper **HandleMapper;
  vtkSphereSource   **HandleGeometry;
  void PositionHandles();
  void HandlesOn(double length);
  void HandlesOff();
  int HighlightHandle(vtkProp *prop); //returns cell id
  virtual void SizeHandles();
  
  // the normal cone
  vtkActor          *ConeActor;
  vtkPolyDataMapper *ConeMapper;
  vtkConeSource     *ConeSource;
  void HighlightNormal(int highlight);

  // the normal line
  vtkActor          *LineActor;
  vtkPolyDataMapper *LineMapper;
  vtkLineSource     *LineSource;

  // the normal cone
  vtkActor          *ConeActor2;
  vtkPolyDataMapper *ConeMapper2;
  vtkConeSource     *ConeSource2;

  // the normal line
  vtkActor          *LineActor2;
  vtkPolyDataMapper *LineMapper2;
  vtkLineSource     *LineSource2;

  // Do the picking
  vtkVJPicker *HandlePicker;
  vtkVJPicker *PlanePicker;
/*   vtkCellPicker *HandlePicker; */
/*   vtkCellPicker *PlanePicker; */
  vtkActor *CurrentHandle;
  
  // Methods to manipulate the hexahedron.
  void MoveOrigin(double *p1, double *p2);
  void MovePoint1(double *p1, double *p2);
  void MovePoint2(double *p1, double *p2);
  void MovePoint3(double *p1, double *p2);
  void Rotate(int X, int Y, double *p1, double *p2, double *vpn);
  void Scale(double *p1, double *p2, int X, int Y);
  void Translate(double *p1, double *p2);
  void Push(double *p1, double *p2);
  
  // Plane normal, normalized
  double Normal[3];

  // Transform the hexahedral points (used for rotations)
  vtkTransform *Transform;
  
  // Properties used to control the appearance of selected objects and
  // the manipulator in general.
  vtkProperty *HandleProperty;
  vtkProperty *SelectedHandleProperty;
  vtkProperty *PlaneProperty;
  vtkProperty *SelectedPlaneProperty;
  void CreateDefaultProperties();
  
  void GeneratePlane();

  vrj::vjRenderer* mVJRenderer;
  
private:
  vtkVJPlaneWidget(const vtkVJPlaneWidget&);  //Not implemented
  void operator=(const vtkVJPlaneWidget&);  //Not implemented
};

#endif
