/**
 * \file   vtkShaderUniform.h
 * \author Ralph Brecheisen
 * \class  vtkShaderUniform
 * \brief  Wrapper for OpenGL shader uniforms.
 *
 * vtkShaderUniform is a wrapper class for dealing with OpenGL shader uniform variables. These
 * variables are used to pass application data to the GPU of the graphics card. For each type
 * of shader uniform a different constructor of vtkShaderUniform can be used. Calling 'Install'
 * will pass the value of the shader uniform to the GPU.
 *
 * This class is taken from the Raycasting project of Ralph Brecheisen. It was slightly
 * modified though to fit into the design of this project.
 */
#ifndef __vtkShaderUniform_h
#define __vtkShaderUniform_h

// OpenGL includes
#include <GL/glew.h>

// VTK includes
#include <vtkObject.h>

// define constants
#define TYPE_UNKNOWN -1
#define TYPE_1B 0
#define TYPE_1I 1
#define TYPE_2I 2
#define TYPE_3I 3
#define TYPE_4I 4
#define TYPE_1F 11
#define TYPE_2F 12
#define TYPE_3F 13
#define TYPE_4F 14

class vtkShaderUniform : public vtkObject
{
public:
	vtkShaderUniform();
	~vtkShaderUniform();

	/** Instantiates bool uniform */
	vtkShaderUniform(char *name, bool v);

	/** Instantiates int uniform */
	vtkShaderUniform(char *name, int v);

	/** Instantiates float uniform */
	vtkShaderUniform(char *name, float v);

	/** Instantiates ivec2 uniform */
	vtkShaderUniform(char *name, int v1, int v2);

	/** Instantiates vec2 uniform */
	vtkShaderUniform(char *name, float v1, float v2);

	/** Instantiates ivec3 uniform */
	vtkShaderUniform(char *name, int v1, int v2, int v3);

	/** Instantiates vec3 uniform */
	vtkShaderUniform(char *name, float v1, float v2, float v3);

	/** Instantiates ivec4 uniform */
	vtkShaderUniform(char *name, int v1, int v2, int v3, int v4);

	/** Instantiates vec4 uniform */
	vtkShaderUniform(char *name, float v1, float v2, float v3, float v4);

	/** Sets the name of the uniform variable */
	void SetName(char *name);

	/** Gets the name of the uniform variable */
	char *GetName();

	/** Sets the object ID of the corresponding shader program */
	void SetShaderProgram(int progid);

	/** Gets the object ID of the corresponding shader program */
	int GetShaderProgram();

	/**	Installs the shader uniform. */
	void Install();

	/** Installs the shader uniform for shader program with the given ID. */
	void Install(int prog);

private:

	char *Name;

	int Type;

	bool b;
	int i;
	float f;
	int i1, i2, i3, i4;
	float f1, f2, f3, f4;

	int ShaderProgram;
};

#endif
