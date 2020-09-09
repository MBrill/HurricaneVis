/**
 * \file   main.cpp
 * \author Michael Berberich
 *
 * Main program for hurricane visualization.
 * OpenGL version 2.0 or higher is required to run this application since GLSL shaders are 
 * used for the rendering.
 * The first parameter must state the relative path and name of the data config file.
 * The parameters 2 and up must specify the VRJuggler config files in the case that the
 * application is started as a VRJuggler application.
 */

// C++ includes
#include <iostream>
#include <stdlib.h>
using namespace std;

// OpenGL includes
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>

// user-own includes
#include "vtkConfigFileReader.h"
#include "vtkApplication.h"
#include "vjApplication.h"

int main(int argc, char* argv[])
{
	// init glut and glew
	// a default window is created but not displayed to check OpenGL version
	GLint window;
	glutInit(&argc, argv);
	window = glutCreateWindow("OpenGL version test");
	glewInit();

	// check OpenGL version
	int glMajor, glMinor;
	const char *version = (const char*) glGetString(GL_VERSION);
	if((version == NULL) || (sscanf(version, "%d.%d", &glMajor, &glMinor ) != 2))
    {
		glMajor = glMinor = 0;
		cerr << "Invalid GL_VERSION format!" << endl;
    }

	// OpenGL 2.0 is required
	if(glMajor < 2)
	{
		cout << "OpenGL 2.0 is required for OpenGL SL ... exiting" << endl;
		exit(EXIT_SUCCESS);
	}

	// argument 1 has to state the data config file
	if(argc < 2)
	{
		cerr << "usage: " << argv[0] << " dataconfigfile" << endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		// read the config file
		vtkConfigFileReader *cfReader = vtkConfigFileReader::New();
		cfReader->ReadConfigFile(argv[1]);

		if(VRJUGGLER_APP)
		{
			// create a VRJuggler application
			vjApplication *application = new vjApplication(argc, argv);
		}
		else
		{
			// create a VTK application
			vtkApplication *application = new vtkApplication();
		}
	}

	return 0;
}