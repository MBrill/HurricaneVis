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
  Module:    $RCSfile: vtkVJRenderWindow.h,v $
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
// .NAME vtkVJOpenGLRenderWindow - OpenGL rendering window
// .SECTION Description
// vtkVJOpenGLRenderWindow is a concrete implementation of the abstract class
// vtkRenderWindow. vtkOpenGLRenderer interfaces to the OpenGL graphics
// library. Application programmers should normally use vtkRenderWindow
// instead of the OpenGL specific version.

//  vtkVJRenderWindow is a class which introduces a function and type that is
// required for VR Juggler usage.  This class exists here so that we can have
// a parent class that isn't dependent on OS specifics while still having
// VR Juggler specific components introduced, i.e. for polymorphism

#ifndef __vtkVJRenderWindow_h
#define __vtkVJRenderWindow_h

#include <vtkOpenGLRenderWindow.h>

#include <vrj/Draw/OGL/GlUserData.h>
#include <vrj/Draw/OGL/GlWindow.h>


class VTK_RENDERING_EXPORT vtkVJRenderWindow : public vtkOpenGLRenderWindow
{
public:
//  static vtkVJRenderWindow *New();
  vtkTypeRevisionMacro(vtkVJRenderWindow,vtkOpenGLRenderWindow);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set the GLUserData from Juggler for the Window that
  // Juggler created for us.
  virtual void SetGLUserData(vrj::GlUserData*);

  // Description:
  // "Deinitialize" the rendering window.  This will shutdown all system-specific
  // resources.  After having called this, it should be possible to destroy
  // a window that was used for a SetWindowId() call without any ill effects.
//  virtual void Finalize(void);

   //  virtual void testthingy(void);

protected:
  vtkVJRenderWindow();
  ~vtkVJRenderWindow();

private:
  vtkVJRenderWindow(const vtkVJRenderWindow&);  // Not implemented.
  void operator=(const vtkVJRenderWindow&);  // Not implemented.

  vrj::GlWindow* mJugglerWindow;

};



#endif
