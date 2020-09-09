/**
 * \file   vjEventHandler.h
 * \author Michael Berberich
 * \class  vjEventHandler
 * \brief  Handles user events for the VRJuggler application.
 * \todo For using the application in a virtual environment this class has to be extended
 * with user interaction functionality. Right now only spare functionality is available
 * to demonstrate the usage.
 *
 * vjEventHandler implements functionality for user interaction at runtime. The most 
 * important interactions are moving the camera around the volume to allow different views 
 * and to change the transfer function during runtime.
 */
#ifndef __vjEventHandler_h
#define __vjEventHandler_h

// C++ includes
#include <iostream>
#include <stdlib.h>
using namespace std;

#ifdef _WIN32
#include <windows.h>
#endif

// OpenGL includes
#include <GL/glew.h>
#include <GL/glut.h>

// user-own includes
#include "vtkGPUVolumeMapper.h"

// VRJuggler includes
#include <vrj/Kernel/Kernel.h>
#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>
#include <gadget/Type/AnalogInterface.h>
#include <gmtl/gmtl.h>
using namespace vrj;
using namespace gadget;
using namespace gmtl;

class vjEventHandler
{
public:
	vjEventHandler();
	~vjEventHandler();

	/** Handles user event */
	void HandleUserEvents();

	/** Sets the current mapper */
	void SetVolumeMapper(vtkGPUVolumeMapper *mapper);

private:
	PositionInterface Wand;
	DigitalInterface  WandButton0;
	DigitalInterface  WandButton1;
	DigitalInterface  WandButton2;
	DigitalInterface  WandButton3;
	DigitalInterface  WandButton4;
	DigitalInterface  WandButton5;
	AnalogInterface   WandJoystickX;
	AnalogInterface   WandJoystickY;

	vtkGPUVolumeMapper *Mapper;
};

#endif

