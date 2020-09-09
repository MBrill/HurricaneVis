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

/**
 * This class is an extension and derivation of the vtkPointPicker 
 * to use in Juggler.  The difference are twofold.  The first is that
 * we don't perform selection based on screen position, but instead
 * typically from a 3D tracked pointing device.  As such a new
 * pick routine which changes the ray creation slightly is created.
 * basically it is simplier than the window position method.
 *   
 * The second difference, is an implementation detail.  VTK uses
 5B
 * requires a renderer to get the list of props to test.  Bad hack
 * essentially.  
 *
 *
 */

#ifndef __vtkVJPointPicker_h
#define __vtkVJPointPicker_h


#include <vtkPointPicker.h>

#include "vjRenderer.h"

class vtkProp3DCollection;

#ifndef vtkFloatingPointType

#define vtkFloatingPointType vtkFloatingPointType

typedef float vtkFloatingPointType;

#endif

#include <gmtl/Vec.h>
#include <gmtl/Point.h>

class VTK_RENDERING_EXPORT vtkVJPointPicker : public vtkPointPicker
{
public:
  static vtkVJPointPicker *New();
  vtkTypeRevisionMacro(vtkVJPointPicker,vtkPointPicker);
  void PrintSelf(ostream& os, vtkIndent indent);

  
  // Description:
  // Perform pick operation with initial point and ray direction
  // given. Return non-zero if something was successfully 
  // picked.
  // ray must be scaled to the length of the pick ray you desire
  // the usual case will be the depth to the far clipping plane
  // as that includes all visible geometry.  
  virtual int Pick(const gmtl::Point4f& point, gmtl::Vec3f ray, 
                   vrj::vjRenderer *renderer);  

   // override the Pick function... which is no longer allowed
   // mostly we have a context problem....
   // If you need to use this function, it is possible to use the 
   // standard vtkPicks.  To sucessfully perform this though it must
   // be called where the juggler context is available, basically 
   // in the draw...which means you will have a one frame delay.
   int Pick(float selectionPt[3], vtkRenderer *ren) 
      {vtkWarningMacro(<< "vtkVJPicker: Picking with screen coordinates is not supported by the vtkVJPicker!"); return 0;}; 
          
protected:
  vtkVJPointPicker();
  ~vtkVJPointPicker();

  
private:
  vtkVJPointPicker(const vtkVJPointPicker&);  // Not implemented.
  void operator=(const vtkVJPointPicker&);  // Not implemented.
};


#endif


