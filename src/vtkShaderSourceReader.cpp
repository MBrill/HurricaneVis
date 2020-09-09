#include "vtkShaderSourceReader.h"

vtkStandardNewMacro(vtkShaderSourceReader);

////////////////////////////////////////////////////////////////////////////////////////////
vtkShaderSourceReader::vtkShaderSourceReader()
{
}

////////////////////////////////////////////////////////////////////////////////////////////
vtkShaderSourceReader::~vtkShaderSourceReader()
{
}

////////////////////////////////////////////////////////////////////////////////////////////
vtkCharArray *vtkShaderSourceReader::LoadFromFile(const char *fname)
{
	int count = 0;
	FILE *f = fopen(fname, "rt");
	if(f == NULL)
		vtkErrorMacro(<< "Could not open shader source " << fname);

	fseek(f, 0, SEEK_END);
	count = ftell(f);
	rewind(f);

	char *content = (char *) malloc((count+1)*sizeof(char));
	count = (int) fread(content, sizeof(char), count, f);
	content[count] = '\0';
	fclose(f);

	vtkCharArray *source = vtkCharArray::New();
	source->SetArray(content, strlen(content), 1);
	return source;
}
