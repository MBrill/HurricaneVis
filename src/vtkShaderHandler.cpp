#include "vtkShaderHandler.h"

vtkShaderHandler *vtkShaderHandler::instance = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
vtkShaderHandler *vtkShaderHandler::GetInstance()
{
    if(instance == NULL)
	{
        instance = new vtkShaderHandler();
	}
	
	return instance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
vtkShaderHandler::vtkShaderHandler()
{
	this->VertexShaderSource    = NULL;
	this->FragmentShaderSource  = NULL;
	this->ShaderFunctionSources = NULL;
	this->StaticShaderUniforms  = NULL;
	this->DynamicShaderUniforms = NULL;
	this->ShaderProgram         = -1;
	this->Installed             = false;
	this->SetShaderInfoEnabled (true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
vtkShaderHandler::~vtkShaderHandler()
{
	if(this->VertexShaderSource    != NULL) this->VertexShaderSource->Delete();
	if(this->FragmentShaderSource  != NULL) this->FragmentShaderSource->Delete();
	if(this->ShaderFunctionSources != NULL) this->ShaderFunctionSources->Delete();
	if(this->StaticShaderUniforms  != NULL) this->StaticShaderUniforms->Delete();
	if(this->DynamicShaderUniforms != NULL) this->DynamicShaderUniforms->Delete();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkShaderHandler::SetVertexShaderSource(const char *fname)
{
	if(this->VertexShaderSource != NULL)
		this->VertexShaderSource->Delete();

	vtkShaderSourceReader *reader = vtkShaderSourceReader::New();
	vtkCharArray *src = reader->LoadFromFile(fname);
	this->VertexShaderSource = src;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
vtkCharArray *vtkShaderHandler::GetVertexShaderSource()
{
	return this->VertexShaderSource;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkShaderHandler::SetFragmentShaderSource(const char *fname)
{
	if(this->FragmentShaderSource != NULL)
		this->FragmentShaderSource->Delete();

	vtkShaderSourceReader *reader = vtkShaderSourceReader::New();
	vtkCharArray *src = reader->LoadFromFile(fname);
	this->FragmentShaderSource = src;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
vtkCharArray *vtkShaderHandler::GetFragmentShaderSource()
{
	return this->FragmentShaderSource;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkShaderHandler::AddShaderFunctionSource(const char *fname)
{
	if(this->ShaderFunctionSources == NULL)
		this->ShaderFunctionSources = vtkCollection::New();

	vtkShaderSourceReader *reader = vtkShaderSourceReader::New();
	vtkCharArray *src = reader->LoadFromFile(fname);
	this->ShaderFunctionSources->AddItem(src);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkShaderHandler::SetStaticShaderUniform(vtkShaderUniform *uniform)
{
	if(this->StaticShaderUniforms == NULL)
		this->StaticShaderUniforms = vtkCollection::New();

	vtkShaderUniform *tmp = (vtkShaderUniform *) this->GetStaticShaderUniform(uniform->GetName());
	if(tmp == NULL)
	{
		this->StaticShaderUniforms->AddItem(uniform);
	}
	else
	{
		this->StaticShaderUniforms->RemoveItem(tmp);
		this->StaticShaderUniforms->AddItem(uniform);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
vtkShaderUniform *vtkShaderHandler::GetStaticShaderUniform(const char *name)
{
	if(this->StaticShaderUniforms == NULL)
		return NULL;

	this->StaticShaderUniforms->InitTraversal();

	vtkShaderUniform *tmp   = (vtkShaderUniform *) this->StaticShaderUniforms->GetNextItemAsObject();
	vtkShaderUniform *found = NULL;

	while(tmp != NULL)
	{
		if(strcmp(name, tmp->GetName()) == 0)
		{
			found = tmp;
			break;
		}

		tmp = (vtkShaderUniform *) this->StaticShaderUniforms->GetNextItemAsObject();
	}

	return found;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkShaderHandler::SetDynamicShaderUniform(vtkShaderUniform *uniform)
{
	if(this->DynamicShaderUniforms == NULL)
		this->DynamicShaderUniforms = vtkCollection::New();

	vtkShaderUniform *tmp = (vtkShaderUniform *) this->GetDynamicShaderUniform(uniform->GetName());
	if(tmp == NULL)
	{
		this->DynamicShaderUniforms->AddItem(uniform);
	}
	else
	{
		this->DynamicShaderUniforms->RemoveItem(tmp);
		this->DynamicShaderUniforms->AddItem(uniform);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
vtkShaderUniform *vtkShaderHandler::GetDynamicShaderUniform(const char *name)
{
	if(this->DynamicShaderUniforms == NULL)
		return NULL;

	this->DynamicShaderUniforms->InitTraversal();

	vtkShaderUniform *tmp   = (vtkShaderUniform *) this->DynamicShaderUniforms->GetNextItemAsObject();
	vtkShaderUniform *found = NULL;

	while(tmp != NULL)
	{
		if(strcmp(name, tmp->GetName()) == 0)
		{
			found = tmp;
			break;
		}

		tmp = (vtkShaderUniform *) this->DynamicShaderUniforms->GetNextItemAsObject();
	}

	return found;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkShaderHandler::Update()
{
	this->InstallDynamicShaderUniforms();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkShaderHandler::Install()
{
	this->InstallShaders();
	this->InstallStaticShaderUniforms();
	this->InstallDynamicShaderUniforms();

	this->Installed = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool vtkShaderHandler::IsInstalled()
{
	return this->Installed;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkShaderHandler::InstallShaders()
{
	if(this->VertexShaderSource == NULL || this->FragmentShaderSource == NULL)
	{
		vtkErrorMacro(<< "no shader sources available");
		return;
	}

	int vertex_shader   = glCreateShader(GL_VERTEX_SHADER);
	int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	const char *vsrc    = this->VertexShaderSource->GetPointer(0);
	const char *fsrc    = this->FragmentShaderSource->GetPointer(0);
	
	glShaderSource(vertex_shader,   1, &vsrc, NULL);
	glShaderSource(fragment_shader, 1, &fsrc, NULL);
	glCompileShader(vertex_shader);   this->PrintShaderInfo(vertex_shader);
	glCompileShader(fragment_shader); this->PrintShaderInfo(fragment_shader);
	
	this->ShaderProgram = glCreateProgram();
	glAttachShader(this->ShaderProgram, vertex_shader);
	glAttachShader(this->ShaderProgram, fragment_shader);

	if(this->ShaderFunctionSources != NULL)
	{
		this->ShaderFunctionSources->InitTraversal();
		vtkCharArray *source = (vtkCharArray *) this->ShaderFunctionSources->GetNextItemAsObject();
		while(source != NULL)
		{
			unsigned int srcid = glCreateShader(GL_FRAGMENT_SHADER);
			const char *src    = source->GetPointer(0);
			glShaderSource(srcid, 1, &src, NULL);
			glCompileShader(srcid); this->PrintShaderInfo(srcid);
			glAttachShader(this->ShaderProgram, srcid);
			source = (vtkCharArray *) this->ShaderFunctionSources->GetNextItemAsObject();
		}
	}

	glLinkProgram(this->ShaderProgram); this->PrintShaderInfo(this->ShaderProgram);
	glUseProgram(this->ShaderProgram);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkShaderHandler::InstallStaticShaderUniforms()
{
	if(this->StaticShaderUniforms == NULL)
		return;

	this->StaticShaderUniforms->InitTraversal();
	vtkShaderUniform *tmp = (vtkShaderUniform *) this->StaticShaderUniforms->GetNextItemAsObject();

	while(tmp != NULL)
	{
		tmp->Install(this->ShaderProgram);
		tmp = (vtkShaderUniform *) this->StaticShaderUniforms->GetNextItemAsObject();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkShaderHandler::InstallDynamicShaderUniforms()
{

	if(this->DynamicShaderUniforms == NULL)
		return;

	this->DynamicShaderUniforms->InitTraversal();
	vtkShaderUniform *tmp = (vtkShaderUniform *) this->DynamicShaderUniforms->GetNextItemAsObject();

	while(tmp != NULL)
	{
		tmp->Install(this->ShaderProgram);
		tmp = (vtkShaderUniform *) this->DynamicShaderUniforms->GetNextItemAsObject();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkShaderHandler::EnableShaderProgram()
{
	glUseProgram(this->ShaderProgram);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkShaderHandler::DisableShaderProgram()
{
	glUseProgram(NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkShaderHandler::SetShaderInfoEnabled(bool enabled)
{
	this->ShaderInfoEnabled = enabled;
}

////////////////////////////////////////////////////////////////////////////////////////////
bool vtkShaderHandler::IsShaderInfoEnabled()
{
	return this->ShaderInfoEnabled;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkShaderHandler::PrintShaderInfo(unsigned int obj)
{
	if(!IsShaderInfoEnabled())
		return;

	int len = 0, written = 0;
	char *log;
	if(glIsProgram(obj)) 
	{ 
		glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &len); 
		if(len == 0) 
			return; 
		log = (char *) malloc(len); 
		glGetProgramInfoLog(obj, len, &written, log); 
	}
	else 
	{ 
		glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &len); 
		if(len == 0) 
			return; 
		log = (char *) malloc(len); 
		glGetShaderInfoLog(obj, len, &written, log); 
	}
	if(log && strlen(log) > 1) 
	{ 
		printf("object %d: %s\n", obj, log);
		free(log); 
	}
}