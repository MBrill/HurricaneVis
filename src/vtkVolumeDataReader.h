/**
 * \file   vtkVolumeDataReader.h
 * \author Michael Berberich
 * \class  vtkVolumeDataReader
 * \brief  Reads volume data from file and saves the data as vtkImageData objects.
 *
 * vtkVolumeDataReader reads volume data from file and constructs vtkImageData objects out of 
 * this data. These objects can then be used to update a vtkVolumeMapper object. Re-rendering 
 * the window interactor will then display the result.
 * The reader can read several data sets, usually several timesteps of hurricane data. Common 
 * event handling can let the volume mapper reload the next data set, that is the next 
 * timestep of the hurricane.
 */

#ifndef __vtkVolumeDataReader_h
#define __vtkVolumeDataReader_h

// C++ includes
#include <stdio.h>
#include <string.h>

// VTK includes
#include <vtkObject.h>
#include <vtkObjectFactory.h>
#include <vtkImageReader2.h>
#include <vtkImageData.h>
#include <vtkImageShiftScale.h>

// user-own includes
#include "globaldefs.h"

class vtkVolumeDataReader : public vtkObject
{
public:
	static vtkVolumeDataReader *New();

	/**
	 * \brief Reads the volume data.
	 *
	 * The scalar values are read from one or multiple brick-of-float files and vtkImageData 
	 * objects are created for each timestep. If the scalar values are not 8-bit values a 
	 * shift/scale is applied to the input to produce 8-bit values.
	 */
	void ReadVolumeData();

	/** Gets the volume data containing the scalars for the given timestep. */
	vtkImageData *GetOutput(int timestep);

protected:
	vtkVolumeDataReader();
	~vtkVolumeDataReader();

private:
	vtkImageData *VolumeData[MAX_TIMESTEPS];
	double MinValue[MAX_TIMESTEPS];
	double MaxValue[MAX_TIMESTEPS];
};

#endif

