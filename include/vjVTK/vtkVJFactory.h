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


#ifndef __vtkVJFactory_h
#define __vtkVJFactory_h

#include "vtkObjectFactory.h"

class VTK_RENDERING_EXPORT vtkVJFactory : public vtkObjectFactory
{
public: 
// Methods from vtkObject
  vtkTypeRevisionMacro(vtkVJFactory,vtkObjectFactory);
  static vtkVJFactory *New();
  void PrintSelf(ostream& os, vtkIndent indent);
  virtual const char* GetVTKSourceVersion();
  virtual const char* GetDescription();
protected:
  vtkVJFactory();
  ~vtkVJFactory() { }
private:
  vtkVJFactory(const vtkVJFactory&);  // Not implemented.
  void operator=(const vtkVJFactory&);  // Not implemented.
};

extern "C" VTK_RENDERING_EXPORT vtkObjectFactory* vtkLoad();
#endif
