/**
 * \file   vtkApplication.h
 * \author Michael Berberich
 * \class  vtkApplication
 * \brief  Implements a hurricane visualization application using a VTK renderer.
 *
 * vtkApplication implements a hurricane visualization application using a VTK renderer.
 */
#ifndef __vtkApplication_h
#define __vtkApplication_h

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

class vtkApplication
{
public:
	vtkApplication();
	~vtkApplication();
};

#endif