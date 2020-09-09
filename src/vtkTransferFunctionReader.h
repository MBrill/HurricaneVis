/**
 * \file   vtkTransferFunctionReader.h
 * \author Michael Berberich
 * \class  vtkTransferFunctionReader
 * \brief  Reads transfer function data from file and saves the data in an 8-bit color table
 *
 * vtkTransferFunctionReader reads transfer function data from file and constructs a 8-bit 
 * color table from this data. This color table can be adapted during run-time by the user
 * through the interactive transfer function displayed in the rendering window. The volume 
 * mapper gets notified of the updated color table and reloads the transfer function texture
 * in the next rendering cycle. Re-rendering the window interactor will then display the 
 * result.
 *
 * Parts of this class are taken from the example for the "Real-Time Volume Graphics" book.
 * The example can be downloaded at http://www.real-time-volume-graphics.org/
 * [accessed January 2009].
 */

#ifndef __vtkTransferFunctionReader_h
#define __vtkTransferFunctionReader_h

// C++ includes
#include <iostream>
#include <string.h>
using namespace std;

// OpenGL includes
#include <GL/glew.h>
#include <GL/glut.h>

// VTK includes
#include <vtkObject.h>
#include <vtkObjectFactory.h>

// user-own includes
#include "globaldefs.h"
#include "vtkGPUVolumeMapper.h"
#include "vtkTrapezoid.h"
#include "CVector.h"

// forward declaration
class vtkGPUVolumeMapper;

// define constants
#define MAX_NUM_TRAPEZOIDS	 6
#define OFFSET_X			 0
#define OFFSET_Y			 0
#define MARGIN_LEFT			40
#define MARGIN_TOP			20
#define MARGIN_RIGHT		50
#define MARGIN_BOTTOM		30
#define MAX_LINE_LENGTH		64

class vtkTransferFunctionReader : public vtkObject
{
public:
	static vtkTransferFunctionReader *New();

	/**
	 * \brief Reads the transfer function from file.
	 *
	 * Any existing points in the transfer function will be removed. Then the file containing 
	 * the transfer function data is opened. Each line in the file contains 9 values. The 
	 * first 4 values define a vtkTrapezoid (start, middle1, middle2 and end points). The 5th 
	 * value represents the height of the vtkTrapezoid. The last 4 values define a RGBA color. 
	 * It has to be considered that the first 5 values for the vtkTrapezoid have to be in the 
	 * range [0-255] and the last 4 values defining the color between 0.0 and 1.0.
	 * The Trapezoids are loaded and saved and the color table is updated.
	 */
	void ReadTransferFunction();

	/** Writes the transfer function to file */
	void WriteTransferFunction();

	/**
	 * \brief Sets the related volume mapper.
	 *
	 * Sets the related volume mapper which is necessary for notifying the mapper when the 
	 * transfer function has changed.
	 */
	void SetMapper(vtkGPUVolumeMapper *mapper);

	/** Gets the color table */
	unsigned char* GetColorTable();

	/** 
	 * Handles the button event if the mouse is located in the transfer function area and
	 * returns true. Returns false if the mouse is not in the transfer function area.
	 */
	bool HandleButtonEvent(int button, int state, int x, int y);

	/**
	 * Handles the mouse move event if the mouse is located in the transfer function area
	 * and returns true. Returns false if the mouse is not in the transfer function area.
	 */
	bool HandleMoveEvent(int x, int y);

	/** Renders the transfer function area including the Trapezoids */
	void Render();

protected:
	vtkTransferFunctionReader();
	~vtkTransferFunctionReader();

private:
	unsigned char       ColorTable[TF_TABLE_SIZE * 4];
	vtkTrapezoid        Trapezoids[MAX_NUM_TRAPEZOIDS];
	int                 NumTrapezoids;
	vtkGPUVolumeMapper *Mapper;

	/** Updates the color table. */
	void UpdateColorTable();
};

#endif