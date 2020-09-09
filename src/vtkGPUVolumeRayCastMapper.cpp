#include "vtkGPUVolumeRayCastMapper.h"

vtkStandardNewMacro(vtkGPUVolumeRayCastMapper);

////////////////////////////////////////////////////////////////////////////////////////////
vtkGPUVolumeRayCastMapper::vtkGPUVolumeRayCastMapper()
{
	// set the initial shader programs
	this->ShaderHandler = vtkShaderHandler::GetInstance();	
	this->ShaderHandler->SetVertexShaderSource  (VERTEX_SHADER_RAY_CASTING);
	this->ShaderHandler->SetFragmentShaderSource(FRAGMENT_SHADER_RAY_CASTING);
	
	this->SetShadingEnabled          (true);
	this->SetZeroOpacityThreshold    (0.99);
	this->SetShadingOpacityThreshold (0.0);
	this->SetDiffuseCoefficient      (0.7);
	this->SetAmbientCoefficient      (0.1);
	this->SetSpecularCoefficient     (0.5);
	this->SetSpecularPower           (70.0);
	this->SetStepSize                (0.1);

    this->CameraPosition[0] = 0.0; this->CameraPosition[1] = 0.0; this->CameraPosition[2] = 0.0; this->CameraPosition[3] = 0.0;
	this->SetCameraPosition(this->CameraPosition);

	this->DiffuseColor[0] = 0.0; this->DiffuseColor[1] = 0.0; this->DiffuseColor[2] = 0.0;
	this->SetDiffuseColor(this->DiffuseColor);

	this->AmbientColor[0] = 0.0; this->AmbientColor[1] = 0.0; this->AmbientColor[2] = 0.0;
	this->SetAmbientColor(this->AmbientColor);

	this->SpecularColor[0] = 0.0; this->SpecularColor[1] = 0.0; this->SpecularColor[2] = 0.0;
	this->SetSpecularColor(this->SpecularColor);
}

////////////////////////////////////////////////////////////////////////////////////////////
vtkGPUVolumeRayCastMapper::~vtkGPUVolumeRayCastMapper()
{
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeRayCastMapper::DrawVolume(vtkRenderer *ren)
{
	/*
	int *dataDimensions = this->GetInput()->GetDimensions();
	double *dataSpacing = this->GetInput()->GetSpacing();
	double sx = dataDimensions[0] * dataSpacing[0];
	double sy = dataDimensions[1] * dataSpacing[1];
	double sz = dataDimensions[2] * dataSpacing[2];

	this->SetCameraPosition(ren->GetActiveCamera()->GetPosition());

	// pass camera position to shader
	double renCamPos[3];
	ren->GetActiveCamera()->GetPosition(renCamPos);
	cout << endl << "renCamPos: " << renCamPos[0] << "  " << renCamPos[1] << "  " << renCamPos[2] << endl;

	float pMatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX,pMatrix);
	CMatrix matModelView(pMatrix[0],pMatrix[4],pMatrix[ 8],pMatrix[12],
						 pMatrix[1],pMatrix[5],pMatrix[ 9],pMatrix[13],
						 pMatrix[2],pMatrix[6],pMatrix[10],pMatrix[14],
						 pMatrix[3],pMatrix[7],pMatrix[11],pMatrix[15]);
	matModelView.invert();
	CVector camPosition = matModelView * CVector(0.0,0.0,0.0,1.0);
	camPosition[3] = 1.0;

	cout << "camPosNew: " << camPosition[0] << " " << camPosition[1] << " " << camPosition[2] << endl;
	this->ShaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("CameraPosition", (float) camPosition[0], (float) camPosition[1], (float) camPosition[2]));
	*/

	// get the current camera position
	double vec[3];
	ren->GetActiveCamera()->GetPosition(vec);
	this->ShaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("CameraPosition", (float) vec[0], (float) vec[1], (float) vec[2], (float) 1.0));
	this->ShaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("StepSize", (float) 0.5));
	this->ShaderHandler->Update();

	//cout << vec[0] << vec[1] << vec[2] << endl;

	//glDisable(GL_LIGHTING);
	//glEnable(GL_BLEND);

	// enable back face culling
	glEnable( GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	// draw cube
	glBegin(GL_QUADS);
		// front face
		glNormal3f(0.0, 0.0, -1.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.01, 0.01, 0.0, 1.0);  glVertex3f(LATITUDE_LEFT,  LONGITUDE_BOTTOM, 3.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.01, 0.99, 0.0, 1.0);  glVertex3f(LATITUDE_LEFT,  LONGITUDE_TOP,    3.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.99, 0.99, 0.0, 1.0);  glVertex3f(LATITUDE_RIGHT, LONGITUDE_TOP,    3.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.99, 0.01, 0.0, 1.0);  glVertex3f(LATITUDE_RIGHT, LONGITUDE_BOTTOM, 3.0);
	glEnd();
	glBegin(GL_QUADS);
		// back face
		glNormal3f(0.0, 0.0, 1.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.99, 0.01, 0.99, 1.0);  glVertex3f(LATITUDE_RIGHT, LONGITUDE_BOTTOM, 0.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.99, 0.99, 0.99, 1.0);  glVertex3f(LATITUDE_RIGHT, LONGITUDE_TOP,    0.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.01, 0.99, 0.99, 1.0);  glVertex3f(LATITUDE_LEFT,  LONGITUDE_TOP,    0.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.01, 0.01, 0.99, 1.0);  glVertex3f(LATITUDE_LEFT,  LONGITUDE_BOTTOM, 0.0);		
	glEnd();
	glBegin(GL_QUADS);
		// left face
		glNormal3f(1.0, 0.0, 0.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.01, 0.01, 0.99, 1.0); glVertex3f(LATITUDE_LEFT, LONGITUDE_BOTTOM, 0.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.01, 0.99, 0.99, 1.0); glVertex3f(LATITUDE_LEFT, LONGITUDE_TOP,    0.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.01, 0.99, 0.01, 1.0); glVertex3f(LATITUDE_LEFT, LONGITUDE_TOP,    3.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.01, 0.01, 0.01, 1.0); glVertex3f(LATITUDE_LEFT, LONGITUDE_BOTTOM, 3.0);
	glEnd();
	glBegin(GL_QUADS);
		// right face
		glNormal3f(-1.0, 0.0, 0.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.99, 0.01, 0.01, 1.0); glVertex3f(LATITUDE_RIGHT, LONGITUDE_BOTTOM, 3.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.99, 0.99, 0.01, 1.0); glVertex3f(LATITUDE_RIGHT, LONGITUDE_TOP,    3.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.99, 0.99, 0.99, 1.0); glVertex3f(LATITUDE_RIGHT, LONGITUDE_TOP,    0.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.99, 0.01, 0.99, 1.0); glVertex3f(LATITUDE_RIGHT, LONGITUDE_BOTTOM, 0.0);
	glEnd();
	glBegin(GL_QUADS);
		// top face
		glNormal3f(0.0, -1.0, 0.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.01, 0.99, 0.01, 1.0); glVertex3f(LATITUDE_LEFT,  LONGITUDE_TOP, 3.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.01, 0.99, 0.99, 1.0); glVertex3f(LATITUDE_LEFT,  LONGITUDE_TOP, 0.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.99, 0.99, 0.99, 1.0); glVertex3f(LATITUDE_RIGHT, LONGITUDE_TOP, 0.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.99, 0.99, 0.01, 1.0); glVertex3f(LATITUDE_RIGHT, LONGITUDE_TOP, 3.0);
	glEnd();
	glBegin(GL_QUADS);
		// bottom face
		glNormal3f(0.0, -1.0, 0.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.99, 0.01, 0.99, 1.0); glVertex3f(LATITUDE_RIGHT, LONGITUDE_BOTTOM, 0.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.01, 0.01, 0.99, 1.0); glVertex3f(LATITUDE_LEFT,  LONGITUDE_BOTTOM, 0.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.01, 0.01, 0.01, 1.0); glVertex3f(LATITUDE_LEFT,  LONGITUDE_BOTTOM, 3.0);
		glMultiTexCoord4f(GL_TEXTURE0, 0.99, 0.01, 0.01, 1.0); glVertex3f(LATITUDE_RIGHT, LONGITUDE_BOTTOM, 3.0);
	glEnd();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeRayCastMapper::SetZeroOpacityThreshold(float threshold)
{
	this->ZeroOpacityThreshold = threshold;

	if(this->ZeroOpacityThreshold < 0.0) this->ZeroOpacityThreshold = 0.0;
	if(this->ZeroOpacityThreshold > 1.0) this->ZeroOpacityThreshold = 1.0;

	this->ShaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("ZeroOpacityThreshold", this->ZeroOpacityThreshold));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
float vtkGPUVolumeRayCastMapper::GetZeroOpacityThreshold()
{
	return this->ZeroOpacityThreshold;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeRayCastMapper::SetShadingOpacityThreshold(float threshold)
{
	this->ShadingOpacityThreshold = threshold;

	if(this->ShadingOpacityThreshold < 0.0) this->ShadingOpacityThreshold = 0.0;
	if(this->ShadingOpacityThreshold > 1.0) this->ShadingOpacityThreshold = 1.0;

	this->ShaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("ShadingOpacityThreshold", this->ShadingOpacityThreshold));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
float vtkGPUVolumeRayCastMapper::GetShadingOpacityThreshold()
{
	return this->ShadingOpacityThreshold;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeRayCastMapper::SetShadingEnabled(bool enabled)
{
	this->ShadingEnabled = enabled;
	this->ShaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("ShadingEnabled", this->ShadingEnabled ? 1 : 0));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool vtkGPUVolumeRayCastMapper::IsShadingEnabled()
{
	return this->ShadingEnabled;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeRayCastMapper::SetCameraPosition(double position[3])
{
	this->CameraPosition[0] = position[0];
	this->CameraPosition[1] = position[1];
	this->CameraPosition[2] = position[2];
	this->CameraPosition[3] = 1.0;

	this->ShaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("CameraPosition",
																		(float) this->CameraPosition[0],
																		(float) this->CameraPosition[1],
																		(float) this->CameraPosition[2],
																		(float) this->CameraPosition[3]));

	cout << endl << "CamPos: " << position[0] << "  " << position[1] << "  " << position[2] << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeRayCastMapper::GetCameraPosition(double position[3])
{
	position[0] = this->CameraPosition[0];
	position[1] = this->CameraPosition[1];
	position[2] = this->CameraPosition[2];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeRayCastMapper::SetDiffuseColor(float color[3])
{
	this->DiffuseColor[0] = color[0];
	this->DiffuseColor[1] = color[1];
	this->DiffuseColor[2] = color[2];

	this->ShaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("DiffuseColor",
																	   this->DiffuseColor[0],
																	   this->DiffuseColor[1],
																	   this->DiffuseColor[2],
																	   0.0));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeRayCastMapper::SetAmbientColor(float color[3])
{
	this->AmbientColor[0] = color[0];
	this->AmbientColor[1] = color[1];
	this->AmbientColor[2] = color[2];

	this->ShaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("AmbientColor",
																	   this->AmbientColor[0],
																	   this->AmbientColor[1],
																	   this->AmbientColor[2],
																	   0.0));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeRayCastMapper::SetSpecularColor(float color[3])
{
	this->SpecularColor[0] = color[0];
	this->SpecularColor[1] = color[1];
	this->SpecularColor[2] = color[2];

	this->ShaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("SpecularColor",
																	   this->SpecularColor[0],
																	   this->SpecularColor[1],
																	   this->SpecularColor[2],
																	   0.0));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeRayCastMapper::SetDiffuseCoefficient(float coefficient)
{
	this->DiffuseCoefficient = coefficient;
	this->ShaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("DiffuseCoefficient", this->DiffuseCoefficient));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeRayCastMapper::SetAmbientCoefficient(float coefficient)
{
	this->AmbientCoefficient = coefficient;
	this->ShaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("AmbientCoefficient", this->AmbientCoefficient));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeRayCastMapper::SetSpecularCoefficient(float coefficient)
{
	this->SpecularCoefficient = coefficient;
	this->ShaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("SpecularCoefficient", this->SpecularCoefficient));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeRayCastMapper::SetSpecularPower(float power)
{
	this->SpecularPower = power;
	this->ShaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("SpecularPower", this->SpecularPower));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeRayCastMapper::GetDiffuseColor(float color[3])
{
	color[0] = this->DiffuseColor[0];
	color[1] = this->DiffuseColor[1];
	color[2] = this->DiffuseColor[2];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeRayCastMapper::GetAmbientColor(float color[3])
{
	color[0] = this->AmbientColor[0];
	color[1] = this->AmbientColor[1];
	color[2] = this->AmbientColor[2];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeRayCastMapper::GetSpecularColor(float color[3])
{
	color[0] = this->SpecularColor[0];
	color[1] = this->SpecularColor[1];
	color[2] = this->SpecularColor[2];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
float vtkGPUVolumeRayCastMapper::GetDiffuseCoefficient()
{
	return this->DiffuseCoefficient;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
float vtkGPUVolumeRayCastMapper::GetAmbientCoefficient()
{
	return this->AmbientCoefficient;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
float vtkGPUVolumeRayCastMapper::GetSpecularCoefficient()
{
	return this->SpecularCoefficient;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
float vtkGPUVolumeRayCastMapper::GetSpecularPower()
{
	return this->SpecularPower;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeRayCastMapper::SetNumberOfLights(int number)
{
	this->NumberOfLights = number;
	this->ShaderHandler->SetStaticShaderUniform(new vtkShaderUniform("NumberOfLights", this->NumberOfLights));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int vtkGPUVolumeRayCastMapper::GetNumberOfLights()
{
	return this->NumberOfLights;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeRayCastMapper::SetStepSize(float stepsize)
{
	this->StepSize = stepsize;
	this->ShaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("StepSize", stepsize));
	this->OpacityCorrectionFactor = stepsize;
	this->ShaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("OpacityCorrectionFactor", this->OpacityCorrectionFactor));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
float vtkGPUVolumeRayCastMapper::GetStepSize()
{
	return this->StepSize;
}