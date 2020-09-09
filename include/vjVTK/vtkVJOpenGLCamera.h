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
  Module:    $RCSfile: vtkVJOpenGLCamera.h,v $
  Language:  C++
  Date:      $Date: 2006/10/22 19:45:45 $
  Version:   $Revision: 1.6 $

  Copyright (c) 1993-2002 Ken Martin, Will Schroeder, Bill Lorensen 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkOpenGLCamera - OpenGL camera
// .SECTION Description
// vtkOpenGLCamera is a concrete implementation of the abstract class
// vtkCamera.  vtkOpenGLCamera interfaces to the OpenGL rendering library.

#ifndef __vtkVJOpenGLCamera_h
#define __vtkVJOpenGLCamera_h

#include "vtkCamera.h"
#include <vrj/Display/Projection.h>

class vtkOpenGLRenderer;

class VTK_RENDERING_EXPORT vtkVJOpenGLCamera : public vtkCamera
{
public:
  static vtkVJOpenGLCamera *New();
  vtkTypeRevisionMacro(vtkVJOpenGLCamera,vtkCamera);
  virtual void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Implement base class method.
  void Render(vtkRenderer *ren);

  void UpdateViewport(vtkRenderer *ren);

  /** 
   * This function is really a hack.  Basically it is due to the fact
   * That Juggler created the windows and we have to accept the possibility
   * of tracked surfaces (changing off-axis projection) which VTK doesn't
   * natively work with.  Introduced originally to enable VTK's sphere 
   * bounding box culling method.
   * 
   * @param projection The vrj:Projection object for this camera's face.
   *     Needed to get the projection matrix (frustum)
   */
  void SetVJProjection(vrj::Projection* projection);

  virtual vtkMatrix4x4 *GetPerspectiveTransformMatrix(double aspect,
                                              double nearz, 
                                              double farz);

  virtual vtkMatrix4x4 *GetViewTransformMatrix();

  /** 
   * This function is overridden in order to allow the 
   * off-axis projection which we may end up with Juggler
   * Basically we just use Juggler's information to fill
   * in methods native to VTK.
   * 
   * @param aspect window's aspect ratio
   * @param planes the parameters of the viewing volume
   */
//  virtual void GetFrustumPlanes(float aspect, float planes[24]);

protected:  
  vtkVJOpenGLCamera();
  ~vtkVJOpenGLCamera() {};
private:
  vtkVJOpenGLCamera(const vtkVJOpenGLCamera&);  // Not implemented.
  void operator=(const vtkVJOpenGLCamera&);  // Not implemented.

//  vrj::Projection* mVJProjection;
//  double mVJViewMat[16];
  vtkMatrix4x4* mUserTransformMatrix;
  float mVJFrustum[6];
};

#endif
