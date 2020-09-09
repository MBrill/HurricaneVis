/**
 * \file   vjApplication.h
 * \author Michael Berberich
 * \class  vjApplication
 * \brief  Implements a hurricane visualization application using VRJuggler.
 *
 * vjApplication implements a hurricane visualization application using VRJuggler.
 */
#ifndef __vjApplication_h
#define __vjApplication_h

// VTK includes for rendering
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkVolume.h>
#include <vtkCamera.h>

// VTK includes for state line visualization
#include <vtkPolyDataReader.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

// user-own includes
#include "globaldefs.h"
#include "vtkVolumeDataReader.h"
#include "vtkTransferFunctionReader.h"
#include "vtkGPUVolumeTextureMapper3D.h"
#include "vtkGPUVolumeRayCastMapper.h"
#include "vtkEventHandler.h"
#include "vjEventHandler.h"

// C++ includes
#include <iostream>
#include <stdlib.h>
using namespace std;

// VRJuggler and vjVTK includes
#include <VTKApp.h>
#include <vrj/Kernel/Kernel.h>
#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>
#include <gadget/Type/AnalogInterface.h>
#include <gmtl/gmtl.h>
using namespace vrj;
using namespace gadget;
using namespace gmtl;


class vjApplication : public VTKApp
{
public:
	vjApplication(int argc, char* argv[]);
	~vjApplication();

	/** Initializes the application. */
    void init();

	/** Draws the scene. */
	void draw();

	/** Initializes the scene */
	virtual void initScene();

private:
	vjEventHandler *EventHandler;
	vtkGPUVolumeMapper *Mapper;
};

#endif