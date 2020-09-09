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


#ifndef __vtkVJInteractor_h
#define __vtkVJInteractor_h


#include <vtkRenderWindowInteractor.h>

#include <gmtl/Vec.h>
#include <gmtl/Point.h>
#include <gmtl/Matrix.h>
#include <gmtl/Containment.h>
#include <gmtl/Generate.h>

#include <gadget/Type/DigitalInterface.h>
#include <gadget/Type/PositionInterface.h>

/********
 *
 * Notes:  I have set the InteractorStyle to NULL in here...
 * there shouldn't really be an interactor as that is a concept
 * in VTK involving standard screen input and movement and should
 * be take over by the Juggler app.  At this point my testing 
 * indicates this isn't a major problem (no segfaults yet) but
 * that doesn't mean it wont' happen... so if segfaults are happening
 * it is a good place to start. (check a core dump though...)
 *
 */ 



class VTK_RENDERING_EXPORT vtkVJInteractor : public vtkRenderWindowInteractor
{
public:
  static vtkVJInteractor *New();
  vtkTypeRevisionMacro(vtkVJInteractor,vtkRenderWindowInteractor);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Have to override the Render or it will try to do something not
  // allowed.
  virtual void Render(){;}

  void SetSelectorInformation(const gmtl::Point4f& point, const gmtl::Vec3f& ray);
  void SetSelectorInformation(gadget::PositionInterface&);

  void GetSelectorInformation(gmtl::Point4f& point, gmtl::Vec3f& ray)const;
  
  const gmtl::Point4f& GetSelectorPosition()const;
  const gmtl::Vec3f& GetSelectorRay()const;

  /** 
   * 
   * 
   * @param button_number corresponds to which of the three mouse buttons that the interactors use, 0,1,2 for Left,Middle,Right
   */
  void SetButtonInformation(const int button_number, gadget::DigitalInterface&);
 

  // Description:
  // Create default picker. Used to create one when none is specified.
  // Default is an instance of vtkVJPicker.
  //virtual 
  vtkAbstractPropPicker *CreateDefaultPicker();

protected:
  vtkVJInteractor();
  ~vtkVJInteractor();

private:
  vtkVJInteractor(const vtkVJInteractor&);  // Not implemented.
  void operator=(const vtkVJInteractor&);  // Not implemented.

  gmtl::Point4f mSelectorPosition;
  gmtl::Vec3f   mSelectorRay;


};

#endif
