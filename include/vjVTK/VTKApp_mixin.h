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

#ifndef _VRJ_VTK_APP_MIXIN_H_
#define _VRJ_VTK_APP_MIXIN_H_

#include <vrj/vrjConfig.h>

//#include <vrj/Display/CameraProjection.h>
#include <vrj/Display/TrackedSurfaceProjection.h>


#include <vtkVJFactory.h>
#include <vjRenderer.h>

#include <vtkVJRenderWindow.h>
#include <vtkVJOpenGLRenderer.h>
#include <vtkVJOpenGLCamera.h>

#include <vtkMath.h>

class vtkVJInteractor;

namespace vrj
{

/** \class VTKApp_mixin
 *
 * Encapulates an Visualization ToolKit (VTK) application.  This defines 
 * a mixin class from which VTK-based application classes should be 
 * derived.  This class takes care of all of the VTK portions which are
 * non-rendering method dependent (non-SG dependent)
 *
 * @see vrj::GlApp
 */
class VTKApp_mixin
{

  public:
   struct context_data
   {
	 vtkVJOpenGLCamera* mVTKCamera;
	 vtkVJRenderWindow* mVTKWindow;
	 vtkRenderer * mVTKRenderer;
   };

   VTKApp_mixin()
   {
      mVJRenderer = new vjRenderer();

      // this is here to ensure that it is called before contextInit and nowhere else seems to
      // ensure that.
      vtkVJFactory* vj_factory = vtkVJFactory::New();
//      vj_factory->DebugOn();
      vtkObjectFactory::RegisterFactory(vj_factory);
      vj_factory->Delete();

   }


   virtual ~VTKApp_mixin()
   {
//      std::cout << "Deleting my vjRenderer" << std::endl;
      delete mVJRenderer;
//      std::cout << "Deleted my vjRenderer" << std::endl;
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

   //Return the vjRenderer object
   vjRenderer* getRenderer(){return mVJRenderer;}

protected:
   vrj::GlContextData< context_data > mVTKContextData;

   vjRenderer* mVJRenderer;

};

/**
 * We first make sure that the 
 * vjRenderer knows that it is a new 
 * frame and clears it's command buffer
 * for the frame. (don't want to 
 * run the commands every frame)
 * 
 */
inline void VTKApp_mixin::preFrame()
{
//   GlApp::preFrame();
//      std::cerr << "FRAME\n\n\n\n\n\n\n\n" << std::endl;
   mVJRenderer->frame();
}


inline void VTKApp_mixin::contextInit()
{
//   unsigned int unique_context_id = GlDrawManager::instance()->getCurrentContext();
   context_data* c_data = &(*mVTKContextData); 
   
   // Add a Renderer for this context  --- this uses the Factory to create the correct one..
   c_data->mVTKRenderer = vtkRenderer::New();
   // debug output to see what class was created, i.e. is vtkVJFactory in place...
   //   std::cerr << c_data->mVTKRenderer->GetClassName() << std::endl;
   assert (mVTKContextData->mVTKRenderer);
   // mVTKContextData->mVTKRenderer->DebugOn();

   c_data->mVTKCamera = dynamic_cast<vtkVJOpenGLCamera*>(vtkCamera::New());
   
   assert (mVTKContextData->mVTKCamera);
   c_data->mVTKRenderer->SetActiveCamera(c_data->mVTKCamera);

   c_data->mVTKWindow = dynamic_cast<vtkVJRenderWindow*>(vtkRenderWindow::New());  
   assert (mVTKContextData->mVTKWindow);
   c_data->mVTKWindow->AddRenderer( c_data->mVTKRenderer );

   // Debug output for the vjRenderer
   // mVJRenderer->PrintSelf();
   
   // Add anyting the user had in the initScene call to the 
   // Renderer....
   mVJRenderer->initRenderer(mVTKContextData->mVTKRenderer);
}

inline void VTKApp_mixin::contextClose()
{
   //Clean up the context.  This means the VTK components.
   // The order is somehow important, so don't rearrange them.
   mVTKContextData->mVTKRenderer->SetActiveCamera(NULL);
   mVTKContextData->mVTKCamera->Delete();
   mVTKContextData->mVTKWindow->RemoveRenderer(mVTKContextData->mVTKRenderer );  
   mVTKContextData->mVTKRenderer->Delete();
   mVTKContextData->mVTKWindow->Delete();
   
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
inline void VTKApp_mixin::contextPreDraw()
{
   // If there is anything new to add/remove from the scene
   // do it now... :)
   mVJRenderer->updateRenderer(mVTKContextData->mVTKRenderer);
 
}

/* This function is a modified version of the OSGApp
 * this should all be evaluated for whether it is 
 * necessary. Probably most of the GL pieces are not
 * as I believe there are due to OSG not playing nicely
 * hopefully VTK is better....
 */
inline void VTKApp_mixin::draw()
{
//   std::cerr << "VTKApp_mixin::draw()" << std::endl;

   glClear(GL_DEPTH_BUFFER_BIT);

   glPushAttrib(GL_ALL_ATTRIB_BITS);
   glPushAttrib(GL_TRANSFORM_BIT);
   glPushAttrib(GL_VIEWPORT_BIT);

   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();

   glMatrixMode(GL_PROJECTION);
   glPushMatrix();

   glMatrixMode(GL_TEXTURE);
   glPushMatrix();

   GlDrawManager*    gl_manager;    /**< The openGL manager that we are rendering for. */
   gl_manager = GlDrawManager::instance();

/*
   // Set the up the viewport (since OSG clears it out)
   float vp_ox, vp_oy, vp_sx, vp_sy;   // The float vrj sizes of the view ports
   int w_ox, w_oy, w_width, w_height;  // Origin and size of the window
   gl_manager->currentUserData()->getViewport()->getOriginAndSize(vp_ox, vp_oy, vp_sx, vp_sy);
//   std::cout << "Viewport origin and size: " << vp_ox << ", " << vp_oy << ", " << vp_sx << ", " << vp_sy << std::endl;

   gl_manager->currentUserData()->getGlWindow()->getOriginSize(w_ox, w_oy, w_width, w_height);

//   glDisable(GL_LIGHTING);

//   std::cout << "Viewport: " << ll_x << ", " << ll_y << ", " << x_size << ", " << y_size << std::endl;
// std::cout << "Viewport: " << ll_x << ", " << ll_y << ", " << ll_x + x_size << ", " << ll_y + y_size << std::endl;
   mVTKContextData->mVTKRenderer->SetViewport(vp_ox, vp_oy, vp_ox + vp_sx, vp_oy + vp_sy);
*/

   /* Here we are creating the Faked Camera.  Basically it 
      Needs to report the Projection Juggler is using*/
   // Check to see if this is tracked surface... if not we don't 
   // need to update this.
//   if(dynamic_cast<vrj::TrackedSurfaceProjection*>(gl_manager->currentUserData()->getProjection())!= NULL)
//   {
      mVTKContextData->mVTKCamera->SetVJProjection(gl_manager->currentUserData()->getProjection());
//   }

   (*mVTKContextData).mVTKWindow->SetGLUserData(gl_manager->currentUserData());
   (*mVTKContextData).mVTKWindow->Render();


   glMatrixMode(GL_TEXTURE);
   glPopMatrix();

   glMatrixMode(GL_PROJECTION);
   glPopMatrix();

   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();

   glPopAttrib();
   glPopAttrib();
   glPopAttrib();
}

}


#endif /* _VRJ_VTK_APP_MIXIN_H_ */
