/**
 * \file   vtkGPUVolumeMapper.h
 * \author Michael Berberich
 * \class  vtkGPUVolumeMapper
 * \brief  Manages the visualization process by loading the volume data and the transfer
 * function and by rendering the data.
 *
 * vtkGPUVolumeMapper class is an abstract class that implements all basic functionality 
 * necessary for processing volume rendering on the GPU. It has basically the same 
 * funcionality as vtkVolumeMapper but the rendering is done on the GPU instead of the CPU.
 * vtkGPUVolumeMapper is responsible for both loading the volume data and the transfer 
 * function as texture maps into texture memory. Furthermore it implements the rendering 
 * process.
 * The actual drawing method that draws the volume is "DrawVolume". It is a pure virtual method
 * and has to be implemented in derived classes since the technique for drawing the volume is 
 * different in every volume rendering approach.
 */
#ifndef __vtkGPUVolumeMapper_h
#define __vtkGPUVolumeMapper_h

// C++ includes
#include <stdlib.h>
#include <math.h>

// OpenGL includes
#include <GL/glew.h>
#include <GL/glut.h>

// VTK includes
#include <vtkVolumeMapper.h>
#include <vtkRenderer.h>
#include <vtkVolume.h>
#include <vtkCamera.h>
#include <vtkImageData.h>
#include <vtkPointData.h>

// user-own includes
#include "globaldefs.h"
#include "vtkShaderHandler.h"
#include "vtkVolumeDataReader.h"
#include "vtkTransferFunctionReader.h"

// forward declaration
class vtkVolumeDataReader;
class vtkTransferFunctionReader;

class vtkGPUVolumeMapper : public vtkVolumeMapper
{
public:
	/**
	 * \brief Renders the display.
	 *
	 * Renders the display. If not already loaded, it loads the volume into a 3D
	 * texture. If not already loaded, it loads the transfer function table into
	 * a 1D texture. If not already done, it installs the shaders on the GPU.
	 */
	void Render(vtkRenderer *ren, vtkVolume *vol);

	/** Sets the volume data reader */
	void SetVolumeDataReader(vtkVolumeDataReader *volumeReader);

	/** Sets the transfer function reader */
	void SetTransferFunctionReader(vtkTransferFunctionReader *tfReader);

	/**
	 * Sets volume texture loaded state. If set to false, the mapper will re-load the volume
	 * texture upon the next rendering pass.
	 */
	void SetVolumeTextureLoaded(bool loaded);

	/** 
	 * Sets transfer function loaded state. If set to false, the mapper will re-load the transfer
	 * function texture upon the next rendering pass.
	 */
	void SetTransferFunctionTextureLoaded(bool loaded);

	/** Enables/disables drawing the bounding box */
	void SetDrawBoundingBoxEnabled(bool enabled);

	/** Indicates whether drawing the bounding box is enabled */
	bool IsDrawBoundingBoxEnabled();

	/** Enables/disables frame counting */
	void SetFrameCountEnabled(bool enabled);

	/** Indicates whether frame counting is enabled */
	bool IsFrameCountEnabled();

	/** Gets the frame count */
	int GetFrameCount();

	/** Resets the frame count */
	void ResetFrameCount();

	/** Removes slices from the bottom of the volume */
	void RemoveBottomSlices();

	/** Adds slices to the bottom of the volume */
	void AddBottomSlices();

	/** Removes slices from the top of the volume. */
	void RemoveTopSlices();

	/** Adds slices to the top of the volume. */
	void AddTopSlices();

	/** Increments the scale value with that the final image is drawn. */
	void IncScaleValue();

	/** Decrements the scale value with that the final image is drawn. */
	void DecScaleValue();

	/** Switches to the last timestep */
	void LastTimestep();

	/** Switches to the next timestep */
	void NextTimestep();

	/** Switches to the previous timestep */
	void PreviousTimestep();

	/** Switches to the first timestep */
	void FirstTimestep();

	/** Gets the current timestep */
	int GetTimestep();

protected:
	vtkGPUVolumeMapper();
	~vtkGPUVolumeMapper();

	/**
	 * \brief Draws the volume data.
	 *
	 * Draws the volume data. This function is declared as a pure virtual function and thus 
	 * has to be implemented in all derived classes since the drawing is done differently 
	 * in each volume rendering approach.
	 */
	virtual void DrawVolume(vtkRenderer *ren) = 0;

	/**
	 * \brief Loads the volume texture.
	 *
	 * Loads the volume texture into texture memory.
	 */
	virtual void LoadVolumeTexture();

	/**
	 * \brief Loads the transfer function texture.
	 *
	 * Loads the transfer function texture into texture memory.
	 */
	virtual void LoadTransferFunctionTexture();

	double  ScaleValue;
	double  Height;
	double  OffsetBottom;
	double  OffsetTop;

private:
	vtkVolumeDataReader		  *VolumeReader;
	vtkTransferFunctionReader *TFReader;

	bool	GlewInitialized;
	bool	VolumeTextureLoaded;
	bool	TransferFunctionTextureLoaded;
	bool	DrawBoundingBoxEnabled;
	bool	FrameCountEnabled;
	int		FrameCount;
	int		Timestep;

	unsigned int VolumeTextureID;
	unsigned int TransferFunctionTextureID;

	/** Draws the bounding box */
	void DrawBoundingBox();
};

#endif


