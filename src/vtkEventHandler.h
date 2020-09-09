/**
 * \file   vtkEventHandler.h
 * \author Michael Berberich
 * \class  vtkEventHandler
 * \brief  Handles user events for the VTK application.
 *
 * vtkEventHandler implements functionality for user interaction at runtime. The most 
 * important interactions are moving the camera around the volume to allow different views 
 * and to change the transfer function during runtime.
 */
#ifndef __vtkEventHandler_h
#define __vtkEventHandler_h

// C++ includes
#include <iostream>
#include <stdlib.h>
using namespace std;

// VTK includes
#include <vtkObjectFactory.h>
#include <vtkInteractorStyleJoystickCamera.h>
#include <vtkRenderWindowInteractor.h>

// user-own includes
#include "globaldefs.h"
#include "vtkGPUVolumeMapper.h"
#include "vtkGPUVolumeTextureMapper3D.h"
#include "vtkTransferFunctionReader.h"
#include "vtkVolumeDataReader.h"
#include "vtkShaderHandler.h"

class vtkEventHandler : public vtkInteractorStyleJoystickCamera
{
public:
	static vtkEventHandler *New();

	/** Sets the current mapper */
	void SetVolumeMapper(vtkGPUVolumeMapper *mapper);

	/** Sets the current volume data reader */
	void SetVolumeReader(vtkVolumeDataReader *volumeReader);

	/** Sets the current volume property */
	void SetTransferFunctionReader(vtkTransferFunctionReader *tfReader);

	/** Prints the user manual into the command prompt */
	void PrintUserManual();

	/** Handles the left mouse button down event */
	void OnLeftButtonDown();

	/** Handles the left mouse button up event */
	void OnLeftButtonUp();

	/** Handles all mouse move events */
	void OnMouseMove();

	/** Handles the right mouse button down event */
	void OnRightButtonDown();

	/** Handles the right mouse button up event */
	void OnRightButtonUp();

	/** Handles the middle mouse button down event */
	void OnMiddleButtonDown();

	/** Handles the middle mouse button up event */
	void OnMiddleButtonUp();

	/** Handles all keyboard event */
	void OnKeyDown();

protected:
	vtkEventHandler();
	~vtkEventHandler();

private:
	vtkGPUVolumeMapper			*Mapper;
	vtkVolumeDataReader			*VolumeReader;
	vtkTransferFunctionReader	*TFReader;
};

#endif

