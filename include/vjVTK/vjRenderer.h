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
/*
 * @file vjRenderer.h
 * @author Kristopher Blom <blom@informatik.uni-hamburg.de>
 * @date Sun Aug  7 13:00:46 2005
 * 
 * 
 * @brief The vjRenderer class is a Proxy for
 * the vtkRenderer type.  However, it
 * also does not fit the classic design pattern.
 * It has many of the functions of the vtkRenderer,
 * namely AddActor, AddProp, AddLight, etc.
 * however, it's true function is to act as a buffer
 * for context saftey as the frame function makes
 * normal usage of the AddProp etc, functions
 * 
 * For VTK users, this class can be used as the 
 * "vtkRenderer" for your Juggler application. 
 *
 */

#ifndef _VJ_RENDERER_H
#define _VJ_RENDERER_H

#include <vector>

#include <vjRendererCommand.h>

class vtkProp;
class vtkActor;
class vtkLight;

//#include <VTKApp.h>
//using namespace vrj;

namespace vrj{

class VTKApp_mixin;

class VTK_RENDERING_EXPORT vjRenderer{

  public:
   
   // Description:
   // Add/Remove different types of props to the renderer.
   // These methods are all synonyms to AddProp and RemoveProp.
   // They are here for convenience and backwards compatibility.
  void AddProp(vtkProp *p);// {this-_add_prop(p);
  void AddActor(vtkProp *p);// {this->AddProp(p);};
  void AddVolume(vtkProp *p);// {this->AddProp(p);};
  void RemoveProp(vtkProp *p);
  void RemoveActor(vtkProp *p);// {this->Actors->RemoveItem(p);this->RemoveProp(p);};
  void RemoveVolume(vtkProp *p);// {this->Volumes->RemoveItem(p);this->RemoveProp(p);};

  bool HasProp(vtkProp* p){return (p && mProps->IsItemPresent(p));}


  // Description:
  // Add a light to the list of lights.
  void AddLight(vtkLight *);

  // Description:
  // Remove a light from the list of lights.
  void RemoveLight(vtkLight *);

  // Description:
  // Return the collection of lights.
  vtkLightCollection *GetLights(){return mLights;}
/*  
  // Description:
  // Create and add a light to renderer.
  void CreateLight(void);
  
  // Description:
  // Create a new Light sutible for use with this type of Renderer.
  // For example, a vtkMesaRenderer should create a vtkMesaLight 
  // in this function.   The default is to just call vtkLight::New.
  virtual vtkLight *MakeLight();

  // Description:
  // Turn on/off two-sided lighting of surfaces. If two-sided lighting is
  // off, then only the side of the surface facing the light(s) will be lit,
  // and the other side dark. If two-sided lighting on, both sides of the 
  // surface will be lit.
  vtkGetMacro(TwoSidedLighting,int);
  vtkSetMacro(TwoSidedLighting,int);
  vtkBooleanMacro(TwoSidedLighting,int);

  // Description:
  // Turn on/off the automatic repositioning of lights as the camera moves.
  // If LightFollowCamera is on, lights that are designated as Headlights
  // or CameraLights will be adjusted to move with this renderer's camera.
  // If LightFollowCamera is off, the lights will not be adjusted.  
  //
  // (Note: In previous versions of vtk, this light-tracking
  // functionality was part of the interactors, not the renderer. For
  // backwards compatibility, the older, more limited interactor
  // behavior is enabled by default. To disable this mode, turn the
  // interactor's LightFollowCamera flag OFF, and leave the renderer's
  // LightFollowCamera flag ON.)
  vtkSetMacro(LightFollowCamera,int);
  vtkGetMacro(LightFollowCamera,int);
  vtkBooleanMacro(LightFollowCamera,int);

  // Description:
  // Ask the lights in the scene that are not in world space
  // (for instance, Headlights or CameraLights that are attached to the 
  // camera) to update their geometry to match the active camera.
  virtual int UpdateLightsGeometryToFollowCamera(void);
*/

  // Description:
  // Return the collection of props
  vtkPropCollection* GetViewProps(){return mProps;}

  // Description:
  // Return the collection of volumes.
  vtkVolumeCollection *GetVolumes();

  // Description:
  // Return any actors in this renderer.
  vtkActorCollection *GetActors();


  void PrintSelf()
  {
     std::cerr << "vjRenderer:\n  have " << mProps->GetNumberOfItems() << " items" << std::endl;
     std::cerr << "  and " << mCommandList.size() << " commands to fulfill" << std::endl;
  }

  protected:
  friend class VTKApp_mixin;
   vjRenderer();
   ~vjRenderer();

   // This function is to tell the proxy 
   // that the new frame as started.  Therfore
   // our command list is cleared, so 
   // that we don't run commands every frame...
   void frame(){}; //std::cerr << "in vjRenderer::frame()" << std::endl; mCommandList.clear();}

   // this function is to be called when 
   // initializing a new Renderer, at this
   // point singly when creating a new context
   void initRenderer(vtkRenderer*);


   // this function is where the command list
   // is executed on the actual vtkRenderer for
   // the display.  
   void updateRenderer(vtkRenderer*);

  private:
   // Following are a number of local functions
   // which are used to make vjRenderer appear
   // to be a "normal" vtkRenderer to the user
   // Most of the code within these functions is
   // simply an adaption of the original code
   // either from vtkRenderer or it's parent
   // vtkViewport
   void _add_prop(vtkProp*);
   void _remove_prop(vtkProp*);

   std::vector<RendererCommand*> mCommandList;
   vtkPropCollection *mProps;
   vtkActorCollection *mActors;
   vtkVolumeCollection *mVolumes;
   vtkLightCollection *mLights;

};

} // namespace vrj;

#endif
