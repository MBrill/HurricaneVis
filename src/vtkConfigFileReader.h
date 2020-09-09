/**
 * \file   vtkConfigFileReader.h
 * \author Michael Berberich
 * \class  vtkConfigFileReader
 * \brief  Reads the config file that contains information about the hurricane data.
 *
 * vtkConfigFileReader reads the config file that contains information about the
 * hurricane data to load. Therefore the application has not to be recompiled for
 * different hurricane data sets, only the config file has to be adapted. The config
 * file can be any text file and has to be the parameter 1 when starting the
 * application. One can start the application on Windows e.g. like this:
 *
 * C:\> HurricaneVis.exe config_Isabel_QVAPOR.txt
 *
 * It is important that the config file follows exactly the required pattern since
 * the application will fail otherwise. Here is an example of how the config file is
 * supposed to look like whereas the comments after the values are not part of the
 * config file.
 * 
 * DATA_DIR         = C:/DATA/Isabel/		// data directory
 * FILENAME         = QVAPORf				// volume data filename
 * FIRST_INDEX      = 1						// first index of volume data
 * TIMESTEPS        = 48					// number of timesteps
 * ENDING           = .bin					// file ending
 * LTL_ENDIAN       = 0						// 0 == big endian, 1 == little endian
 * DATA_TYPE        = 4						// data type; 1 == unsigned char (1 Byte), 4 == float (4 Byte)
 * SIZEX            = 500					// size x
 * SIZEY            = 500					// size y
 * SIZEZ            = 100					// size z
 * MIN_VALUE        = 0						// minimum value in the data set
 * MAX_VALUE        = 0.02368				// maximum value in the data set
 * LATITUDE_LEFT    = -83.0					// latitude left
 * LATITUDE_RIGHT   = -62.0					// latitude right
 * LONGITUDE_BOTTOM = 23.7					// longitude bottom
 * LONGITUDE_TOP    = 41.7					// longitude top
 * ORIENTATION      = 0						// orientation, 0 == Isabel, 1 == Lili
 * TF_NAME          = QVAPORf.tf			// transfer function filename
 * DESCRIPTION      = water vapor			// variable description
 * UNITS            = [kg/kg]				// units of variable
 *
 * Because these parameters are needed in almost every class of this application they
 * are saved as global variables which are defined in globaldefs.h.
 */

#ifndef __vtkConfigFileReader_h
#define __vtkConfigFileReader_h

// C++ includes
#include <fstream>
#include <iostream>
#include <stdlib.h>
using namespace std;

// VTK includes
#include <vtkObject.h>
#include <vtkObjectFactory.h>

// user-own includes
#include "globaldefs.h"

class vtkConfigFileReader : public vtkObject
{
public:
	static vtkConfigFileReader *New();

	/** Reads the config file. */
	void ReadConfigFile(const char *filename);

protected:
	vtkConfigFileReader();
	~vtkConfigFileReader();
};

#endif