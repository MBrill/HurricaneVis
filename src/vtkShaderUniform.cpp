#include "vtkShaderUniform.h"

////////////////////////////////////////////////////////////////////////////////////////////
vtkShaderUniform::vtkShaderUniform()
{
}

////////////////////////////////////////////////////////////////////////////////////////////
vtkShaderUniform::~vtkShaderUniform()
{
}

////////////////////////////////////////////////////////////////////////////////////////////
vtkShaderUniform::vtkShaderUniform(char *name, bool v)
{
	this->Name = name;
	this->b = v;
	this->Type = TYPE_1B;
}

////////////////////////////////////////////////////////////////////////////////////////////
vtkShaderUniform::vtkShaderUniform(char *name, int v)
{
	this->Name = name;
	this->i = v;
	this->Type = TYPE_1I;
}

////////////////////////////////////////////////////////////////////////////////////////////
vtkShaderUniform::vtkShaderUniform(char *name, float v)
{
	this->Name = name;
	this->f = v;
	this->Type = TYPE_1F;
}

////////////////////////////////////////////////////////////////////////////////////////////
vtkShaderUniform::vtkShaderUniform(char *name, int v1, int v2) 
{
	this->Name = name;
	this->i1 = v1;
	this->i2 = v2;
	this->Type = TYPE_2I;
}

////////////////////////////////////////////////////////////////////////////////////////////
vtkShaderUniform::vtkShaderUniform(char *name, float v1, float v2)
{
	this->Name = name;
	this->f1 = v1;
	this->f2 = v2;
	this->Type = TYPE_2F;
}

////////////////////////////////////////////////////////////////////////////////////////////
vtkShaderUniform::vtkShaderUniform(char *name, int v1, int v2, int v3)
{
	this->Name = name;
	this->i1 = v1;
	this->i2 = v2;
	this->i3 = v3;
	this->Type = TYPE_3I;
}

////////////////////////////////////////////////////////////////////////////////////////////
vtkShaderUniform::vtkShaderUniform(char *name, float v1, float v2, float v3)
{
	this->Name = name;
	this->f1 = v1;
	this->f2 = v2;
	this->f3 = v3;
	this->Type = TYPE_3F;
}

////////////////////////////////////////////////////////////////////////////////////////////
vtkShaderUniform::vtkShaderUniform(char *name, int v1, int v2, int v3, int v4)
{
	this->Name = name;
	this->i1 = v1;
	this->i2 = v2;
	this->i3 = v3;
	this->i4 = v4;
	this->Type = TYPE_4I;
}

////////////////////////////////////////////////////////////////////////////////////////////
vtkShaderUniform::vtkShaderUniform(char *name, float v1, float v2, float v3, float v4)
{
	this->Name = name;
	this->f1 = v1;
	this->f2 = v2;
	this->f3 = v3;
	this->f4 = v4;
	this->Type = TYPE_4F;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkShaderUniform::SetName(char *name)
{
	this->Name = (char *) realloc(this->Name, sizeof(char) * ((int)strlen(name)+1));
	strcpy(this->Name, name);
}

////////////////////////////////////////////////////////////////////////////////////////////
char *vtkShaderUniform::GetName()
{
	return this->Name;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkShaderUniform::SetShaderProgram(int progid)
{
	this->ShaderProgram = progid;
}

////////////////////////////////////////////////////////////////////////////////////////////
int vtkShaderUniform::GetShaderProgram()
{
	return this->ShaderProgram;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkShaderUniform::Install()
{
	GLint uniformLocation = glGetUniformLocation(this->ShaderProgram, this->Name);

	switch(this->Type)
	{
	case TYPE_1B:
		glUniform1i(uniformLocation, (int) this->b);
		break;
	case TYPE_1I:
		glUniform1i(uniformLocation, this->i);
		break;
	case TYPE_2I:
		glUniform2i(uniformLocation, this->i1, this->i2);
		break;
	case TYPE_3I:
		glUniform3i(uniformLocation, this->i1, this->i2, this->i3);
		break;
	case TYPE_4I:
		glUniform4i(uniformLocation, this->i1, this->i2, this->i3, this->i4);
		break;
	case TYPE_1F:
		glUniform1f(uniformLocation, this->f);
		break;
	case TYPE_2F:
		glUniform2f(uniformLocation, this->f1, this->f2);
		break;
	case TYPE_3F:
		glUniform3f(uniformLocation, this->f1, this->f2, this->f3);
		break;
	case TYPE_4F:
		glUniform4f(uniformLocation, this->f1, this->f2, this->f3, this->f4);
		break;
	case TYPE_UNKNOWN: vtkErrorMacro(<< "Type never determined");
		break;
	default:
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkShaderUniform::Install(int prog)
{
	this->SetShaderProgram(prog);
	this->Install();
}