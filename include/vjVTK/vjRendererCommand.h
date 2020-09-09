/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: nil c-basic-offset: 3 -*- */
// vim:cindent:ts=3:sw=3:et:tw=80:sta:
//* -*- Mode: C++; tab-width: 3; indent-tabs-mode: nil c-basic-offset: 3 -*- */
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
  * @file   vjRendererCommand.h
  * @author Kristopher Blom <blom@informatik.uni-hamburg.de>
  * @date   Sun Aug  7 13:01:52 2005
  * 
  * @brief  The RendererCommand class is a command design 
  * pattern used for keeping track of commands issued
  * to the Renderer and to be further propegated.
  * 
  * 
  */

#ifndef _VJ_RENDERER_COMMAND_H
#define _VJ_RENDERER_COMMAND_H

class vtkProp;
class vtkLight;
#include <vtkRenderer.h>

namespace vrj
{

class vjRenderer;


/**
 * RendererCommmand class
 *
 * This class is an abstract base class for the
 * command pattern for the Renderer Proxy
 * vjRenderer and distribution of the 
 * commands to the real vtkRenderer's
 * 
 */
class RendererCommand	       
{
  public:
   virtual void execute(vtkRenderer*)= 0;
   
  protected:
   RendererCommand(){}
   virtual ~RendererCommand(){}
};

/**
 * PropCommand class:
 * This class is the concrete command for
 * all vtkProp based commands of the 
 * Renderer. this includes Props, Actors,
 * and Volumes.
 * 
 */
class PropCommand: public RendererCommand
{
  public:
   void execute(vtkRenderer*);
   
  private:
   friend class vjRenderer;
   
   enum PCOMMAND{ADD_PROP, ADD_ACTOR, ADD_VOLUME, REMOVE_PROP, REMOVE_ACTOR, REMOVE_VOLUME};
   
   PropCommand(PCOMMAND c, vtkProp *p)
      :RendererCommand(), mCommand(c), mProp(p)
   {};//mProp->AddConsumer(this)};

   virtual ~PropCommand(){};//mProp->RemoveConsumer(this);}
      
   
   PCOMMAND mCommand;
   vtkProp* mProp;
};

inline void PropCommand::execute(vtkRenderer* renderer)
{
   switch(mCommand)
   {
      // Both ACTOR and VOLUME are wrappers for the AddViewProp
      // command in the renderer... so all handled the same
      case ADD_PROP:
      case ADD_ACTOR:
      case ADD_VOLUME:
	 renderer->AddViewProp(mProp);
	 break;

      // Actor and Volume do more than Prop... 
      // so we have to call them individually
      case REMOVE_PROP:
	 renderer->RemoveViewProp(mProp);
	 break;
      case REMOVE_ACTOR:
	 renderer->RemoveActor(mProp);
	 break;
      case REMOVE_VOLUME:
	 renderer->RemoveVolume(mProp);
	 break;      
   }
}


/**
 * PropCommand class:
 * This class is the concrete command for
 * all vtkProp based commands of the 
 * Renderer. this includes Props, Actors,
 * and Volumes.
 * 
 */
class LightCommand: public RendererCommand
{
  public:
   void execute(vtkRenderer*);
   
  private:
   friend class vjRenderer;
   
   enum LCOMMAND{ADD_LIGHT, REMOVE_LIGHT};
   
   LightCommand(LCOMMAND c, vtkLight *light)
      :RendererCommand(), mCommand(c), mLight(light)
   {};//mProp->AddConsumer(this)};

   virtual ~LightCommand(){};//mProp->RemoveConsumer(this);}
      
   
   LCOMMAND mCommand;
   vtkLight* mLight;
};

inline void LightCommand::execute(vtkRenderer* renderer)
{
   switch(mCommand)
   {
      case ADD_LIGHT:
         renderer->AddLight(mLight);
         break;
         
     case REMOVE_LIGHT:
         renderer->RemoveLight(mLight);
         break;
   }
}



} // namespace vrj;
#endif
