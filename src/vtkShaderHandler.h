/**
 * \file   vtkShaderHandler.h
 * \author Michael Berberich
 * \class  vtkShaderHandler
 * \brief  Handles shader program and variable installation on the graphic card.
 *
 * vtkShaderHandler handles the installation of shaders programs on the graphics card. 
 * It installs the shader source code for both vertex and fragment shader programs
 * as well as uniform variables that have to be passed to the graphic card.
 * The vtkShaderHandler object is used in many different classes in this application
 * to register uniform variables. To avoid the problem of forwarding the created object
 * to all of these classes it is implemented according to the Singleton pattern.
 * Thus it can be accessed from everywhere without priveous distribution.
 *
 * Parts of this code are taken from the Raycasting project of Ralph Brecheisen since
 * the way of installing shaders on the GPU is always the same.
 */
#ifndef __vtkShaderHandler_h
#define __vtkShaderHandler_h

// C++ includes
#include <iostream>
using namespace std;

// VTK includes
#include <vtkObject.h>
#include <vtkCollection.h>

// user-own includes
#include "vtkShaderUniform.h"
#include "vtkShaderSourceReader.h"

class vtkShaderHandler : vtkObject
{
public:
	static vtkShaderHandler* GetInstance();

	/**
	 * \brief Sets the vertex shader source.
	 *
	 * Sets the vertex shader source. By default, this shader contains code for
	 * transforming and interpolating vertices to pixel coordinates.
	 */
	void SetVertexShaderSource(const char *fname);

	/**
	 * \brief Sets the fragment shader source.
	 *
	 * Sets the main fragment shader source. By default, this shader contains the
	 * per fragment processing, e.g. assigning a color to the current fragment.
	 */
	void SetFragmentShaderSource(const char *fname);

	/** Gets the vertex shader source */
	vtkCharArray *GetVertexShaderSource();

	/** Gets the fragment shader source */
	vtkCharArray *GetFragmentShaderSource();

	/**
	 * \brief Adds additional fragment shader source.
	 *
	 * This function allows you to add additional fragment shaders that contain any 
	 * functions required by a specific volume rendering algorithm. All function shaders
	 * are collected in a list and installed together with the main vertex and fragment shaders
	 * upon calling Install().
	 * \sa Install()
	 */
	void AddShaderFunctionSource(const char *fname);

	/**
	 * \brief Sets a static shader uniform.
	 *
	 * Sets or adds a static shader uniform. A static shader uniform is a variable that is passed
	 * to the GPU only once (upon startup of the application). Setting it using this function will
	 * either add it to the collection, if it does not already exist, or update it, if it does
	 * exist.
	 * \sa vtkShaderUniform
	 */
	void SetStaticShaderUniform (vtkShaderUniform *uniform);

	/**
	 * \brief Sets a dynamic shader uniform.
	 *
	 * Sets or adds a dynamic shader uniform. A dynamic shader uniform is a variable that is passed
	 * to the GPU for every rendering. Setting it using this function will
	 * either add it to the collection, if it does not already exist, or update it, if it does
	 * exist.
	 * \sa vtkShaderUniform
	 */
	void SetDynamicShaderUniform(vtkShaderUniform *uniform);

	/** Gets the static shader uniform of the given name */
	vtkShaderUniform *GetStaticShaderUniform (const char *name);

	/** Gets the dynamic shader uniform of the given name */
	vtkShaderUniform *GetDynamicShaderUniform(const char *name);

	/**
	 * \brief Updates the dynamic shader uniforms.
	 *
	 * Re-installs the dynamic shader uniforms. This function is called for each rendering pass allowing
	 * the application to change settings at run-time. Typical shader uniforms that are updated frequently
	 * are the camera and light positions.
	 */
	void Update();

	/**
	 * \brief Installs the shader programs as well as static and dynamic shader uniforms on the GPU.
	 *
	 * This installs the shaders on the GPU. First a check is performed to verify whether
	 * any shader source files have been set. At least one vertex shader and one fragment shader source
	 * file should be set. For additional functions to be used by the fragment shader, additional shader
	 * source files may be specified. These are optional however.
	 * If all the required and optional shader source files are known, they are compiled and attached
	 * to the main shader program. After that, the shader uniform variables (variables that need to be
	 * passed from the main application to the GPU) are installed.
	 * \sa SetStaticShaderUniform(), SetDynamicShaderUniform()
	 */
	void Install();

	/** Indicates whether the shaders are already installed or not */
	bool IsInstalled();

	inline void SetInstalled(bool installed){this->Installed = installed;}

	/** Enables the shader program */
	void EnableShaderProgram();

	/** Disables the shader program */
	void DisableShaderProgram();

	/** Sets the compile-time logging enabled/disabled. */
	void SetShaderInfoEnabled(bool enabled);

	/** Indicates whether compile-time logging is enabled */
	bool IsShaderInfoEnabled();

protected:	
	/**
	 * \brief Installs the shaders on the GPU.
	 *
	 * Installs the shaders and attaches them to the main shader program. In principle, there should be 1
	 * vertex shader, 1 fragment shader containing the main fragment shading and optionally 0 or more fragment
	 * shaders containing additional functions.
	 */
	void InstallShaders();

	/**
	 * \brief Installs the static shader uniforms on the GPU.
	 *
	 * Installs the static shader uniforms. These are variables that are passed from the main application to
	 * the shader program only once (at startup).
	 */
	void InstallStaticShaderUniforms();

	/**
	 * \brief Installs the dynamic shader uniforms on the GPU.
	 *
	 * Installs the dynamic shader uniforms. These are variables that are passed from the main application to
	 * the shader program for each rendering pass.
	 */
	void InstallDynamicShaderUniforms();

	/** Prints compile info when installing the shaders. */
	void PrintShaderInfo(unsigned int obj);

private:
	vtkCollection *ShaderFunctionSources;
	vtkCollection *StaticShaderUniforms;
	vtkCollection *DynamicShaderUniforms;
	vtkCharArray  *VertexShaderSource;
	vtkCharArray  *FragmentShaderSource;
	bool           Installed;
	bool           ShaderInfoEnabled;
	int            ShaderProgram;

	static vtkShaderHandler *instance;

	vtkShaderHandler();										// hidden constructor
	~vtkShaderHandler();									// hidden destructor
	vtkShaderHandler(vtkShaderHandler const&);				// hidden copy constructor
	vtkShaderHandler& operator=(vtkShaderHandler const&);	// hidden assign operator
};

#endif


