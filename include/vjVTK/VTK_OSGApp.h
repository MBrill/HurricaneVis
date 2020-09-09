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

#ifndef _VRJ_OSG_VTK_APP_H_
#define _VRJ_OSG_VTK_APP_H_

#include <vrj/vrjConfig.h>

#include <vrj/Draw/OSG/OsgApp.h>
//#include <vrj/Draw/OGL/GlApp.h>
//#include <vrj/Draw/OGL/GlContextData.h>

//#include <vrj/Display/CameraProjection.h>
#include <vrj/Display/TrackedSurfaceProjection.h>

#include <vtkMath.h>

#include <VTKApp_mixin.h>
#include <vjRenderer.h>

namespace vrj
{


/** \class VTK_OSGApp VTK_OSGApp.h
 *
 * Encapulates an Visualization ToolKit (VTK) application.  This defines 
 * the base class from which VTK-based application classes should be 
 * derived.  It makes use of the OpenGL Draw Manager.
 *
 * @see vrj::GlApp
 */
class VTK_OSGApp : public OsgApp, public VTKApp_mixin
{

 public:
   VTK_OSGApp(Kernel* kern) : OsgApp(kern), VTKApp_mixin()
   {
   }

   VTK_OSGApp() : OsgApp(), VTKApp_mixin()
   {
   }

   virtual ~VTK_OSGApp()
   {
   }

   /**
    * Initializes the scene.
    * Called by init() to initialize the scene.
    */
//   virtual void initScene() = 0;

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

protected:
};

/**
 * We first make sure that the 
 * vjRenderer knows that it is a new 
 * frame and clears it's command buffer
 * for the frame. (don't want to 
 * run the commands every frame)
 * 
 */
inline void VTK_OSGApp::preFrame()
{
   OsgApp::preFrame();
//      std::cerr << "FRAME\n\n\n\n\n\n\n\n" << std::endl;
   VTKApp_mixin::preFrame(); 
}


inline void VTK_OSGApp::contextInit()
{
   OsgApp::contextInit();
   VTKApp_mixin::contextInit();

   //This line in very important for playing well with the
   //SG.  This keeps VTK from clearing the Window.
   // The assumption is that VTK Render comes after the 
   // SG render and that the SG render will take care of
   // all the clearing.
   mVTKContextData->mVTKWindow->SetErase(false );


   //The next section is from the OSG App.  This needs to be
   // evaluated as to whether it should be here at all. 
   // i.e. this is a HACK

   //Setup OpenGL light
   //This should actualy be done in the simulator code
   GLfloat light0_ambient[] = { 0.1f,  0.1f,  0.1f,  1.0f};
   GLfloat light0_diffuse[] = { 0.8f,  0.8f,  0.8f,  1.0f};
   GLfloat light0_specular[] = { 1.0f,  1.0f,  1.0f,  1.0f};
   GLfloat light0_position[] = {0.0f, 0.75f, 0.75f, 0.0f};

   GLfloat mat_ambient[] = { 0.7f, 0.7f,  0.7f,  1.0f};
   GLfloat mat_diffuse[] = { 1.0f,  0.5f,  0.8f,  1.0f};
   GLfloat mat_specular[] = { 1.0f,  1.0f,  1.0f,  1.0f};
   GLfloat mat_shininess[] = { 50.0f};
   //GLfloat mat_emission[] = { 1.0f,  1.0f,  1.0f,  1.0f};
   GLfloat no_mat[] = { 0.0f,  0.0f,  0.0f,  1.0f};

   glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR,  light0_specular);
   glLightfv(GL_LIGHT0, GL_POSITION,  light0_position);

   glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );
   glMaterialfv( GL_FRONT,  GL_DIFFUSE, mat_diffuse );
   glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
   glMaterialfv( GL_FRONT,  GL_SHININESS, mat_shininess );
   glMaterialfv( GL_FRONT,  GL_EMISSION, no_mat);

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_COLOR_MATERIAL);
   glShadeModel(GL_SMOOTH);
}

inline void VTK_OSGApp::contextClose()
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
inline void VTK_OSGApp::contextPreDraw()
{
   OsgApp::contextPreDraw();
   VTKApp_mixin::contextPreDraw();   
}

/* This function is a modified version of the OSGApp
 * this should all be evaluated for whether it is 
 * necessary. Probably most of the GL pieces are not
 * as I believe there are due to OSG not playing nicely
 * hopefully VTK is better....
 */
inline void VTK_OSGApp::draw()
{
   OsgApp::draw();
//   std::cerr << "VTK_OSGApp::draw()" << std::endl;
   VTKApp_mixin::draw();
}

}


#endif /* _VRJ_OSG_APP_H_ */
