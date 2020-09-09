/**
 * \file   vtkShaderSourceReader.h
 * \author Michael Berberich
 * \class  vtkShaderSourceReader
 * \brief  Reads shader source text from file and returns it as character array.
 *
 * vtkShaderSourceReader reads shader source text from file and returns it as a
 * vtkCharArray object.
 */
#ifndef __vtkShaderSourceReader_h
#define __vtkShaderSourceReader_h

// VTK includes
#include <vtkObject.h>
#include <vtkCharArray.h>
#include <vtkObjectFactory.h>

class vtkShaderSourceReader : public vtkObject
{
public:
	static vtkShaderSourceReader *New();

	/**
	 * \brief Loads the shader source text from file.
	 *
	 * The source text is loaded as a single character array. This array is then
	 * wrapped inside a vtkCharArray object and returned.
	 */
	vtkCharArray *LoadFromFile(const char *fname);

protected:
	vtkShaderSourceReader();
	~vtkShaderSourceReader();
};

#endif

