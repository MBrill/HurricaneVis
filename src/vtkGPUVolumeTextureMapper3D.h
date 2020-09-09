/**
 * \file   vtkGPUVolumeTextureMapper3D.h
 * \author Michael Berberich
 * \class  vtkGPUVolumeTextureMapper3D
 * \brief  Processes volume rendering by drawing slices of the volume texture.
 *
 * vtkGPUVolumeTextureMapper3D inherits from the abstract class vtkGPUVolumeMapper 
 * which implements all basic functionality for volume rendering. All derived classes have
 * to implement the pure virtual function "DrawVolume" to draw the volume.
 */
#ifndef __vtkGPUVolumeTextureMapper3D_h
#define __vtkGPUVolumeTextureMapper3D_h

// C++ includes
#include <iostream>
using namespace std;

// VTK includes
#include <vtkRenderer.h>

// user-own includes
#include "vtkGPUVolumeMapper.h"
#include "CVector.h"
#include "CMatrix.h"

class vtkGPUVolumeTextureMapper3D : public vtkGPUVolumeMapper
{
public:
	static vtkGPUVolumeTextureMapper3D *New();

	/** Enables/disables view aligned slices. */
	void SetViewAlignedSlices(bool enabled);

protected:
	vtkGPUVolumeTextureMapper3D();
	~vtkGPUVolumeTextureMapper3D();

	/**
	 * \brief Draws the volume data.
	 *
	 * Draws the slices of the volume data. The slices are rendered with attached texture 
	 * coordinates for the volume texture. The volume can either be rendered with object
	 * aligned slices or view aligned slices depending on the value of the boolean variable
	 * ViewAlignedSlices.
	 */
	virtual void DrawVolume(vtkRenderer *ren);

private:
	bool	ViewAlignedSlices;

	/** Draws the volume with object aligned slices. */
	void DrawObjectAlignedSlices(vtkRenderer *ren);

	/** Draws the volume with view aligned slices. */
	void DrawViewAlignedSlices(vtkRenderer *ren);

	/** Draws a single slice. This method is only used in DrawObjectAlignedSlices. */
	void DrawSingleSlice(int slice);
};

#endif