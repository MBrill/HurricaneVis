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

#ifndef _VRJ_OpenSG_VTK_APP_H_
#define _VRJ_OpenSG_VTK_APP_H_

#include <vrj/vrjConfig.h>

#include <vrj/Draw/OpenSG/OpenSGApp.h>
#include <vrj/Draw/OGL/GlContextData.h>

//#include <vrj/Display/CameraProjection.h>
#include <vrj/Display/TrackedSurfaceProjection.h>

#include <vtkMath.h>

#include <OpenSG/OSGLog.h>

#include <VTKApp_mixin.h>
#include <vjRenderer.h>

namespace vrj
{


/** \class VTK_OpenSGApp VTK_OpenSGApp.h
 *
 * Encapulates an Visualization ToolKit (VTK) application.  This defines 
 * the base class from which VTK-based application classes should be 
 * derived.  It makes use of the OpenSG Draw Manager.
 *
 * @see vrj::OpenSGApp
 */
class VTK_OpenSGApp : public vrj::OpenSGApp, public VTKApp_mixin
{
  public:
   VTK_OpenSGApp(Kernel* kern) : OpenSGApp(kern), VTKApp_mixin()
   {
   }
      
   VTK_OpenSGApp() : OpenSGApp(), VTKApp_mixin()
   {
   }

   virtual ~VTK_OpenSGApp()
   {
   }


   /**
    * Function to set up and render the scene using VTK.  Override this
    * method with great care.  All the logic to handle multi-pipe rendering
    * and other VR Juggler features happens here.
    *
    * @pre OpenGL state has correct transformation and buffer selected.
    * @post The current scene has been drawn.
    */
   virtual void draw();


   void contextInit();

   /**
    * Function that is called immediately before a context is closed.
    * Use the function to clean up any context-specific data structures.
    */
   virtual void contextClose();

   virtual void contextPreDraw();
   
   virtual void preFrame();


};

/**
 * We first make sure that the 
 * vjRenderer knows that it is a new 
 * frame and clears it's command buffer
 * for the frame. (don't want to 
 * run the commands every frame)
 * 
 */
inline void VTK_OpenSGApp::preFrame()
{
   OpenSGApp::preFrame();
   VTKApp_mixin::preFrame();
}


inline void VTK_OpenSGApp::contextInit()
{
   OpenSGApp::contextInit();
   VTKApp_mixin::contextInit();

   //This line in very important for playing well with the
   //SG.  This keeps VTK from clearing the Window.
   // The assumption is that VTK Render comes after the 
   // SG render and that the SG render will take care of
   // all the clearing.
   mVTKContextData->mVTKWindow->SetErase(false );

}

inline void VTK_OpenSGApp::contextClose()
{
   VTKApp_mixin::contextClose();     
}

/**
 * contextPreDraw() is called for each context
 * directly before the Draw
 * 
 * in this function we update the
 * vtkVJRenderer with any commands the
 * user has run on our Proxy vjRenderer
 * 
 */
inline void VTK_OpenSGApp::contextPreDraw()
{
   OpenSGApp::contextPreDraw();

   VTKApp_mixin::contextPreDraw();  
}

/* This function is a modified version of the OpenSGApp
 * this should all be evaluated for whether it is 
 * necessary. .
 */
inline void VTK_OpenSGApp::draw()
{
//   std::cerr << "VTK_OpenSGApp::draw()" << std::endl;

   OpenSGApp::draw();
   VTKApp_mixin::draw();
}

}//vrj::


#endif /* _VRJ_OpenSG_APP_H_ */
