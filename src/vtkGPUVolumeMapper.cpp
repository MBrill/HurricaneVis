#include "vtkGPUVolumeMapper.h"

////////////////////////////////////////////////////////////////////////////////////////////
vtkGPUVolumeMapper::vtkGPUVolumeMapper()
{
	this->GlewInitialized               = false;
	this->VolumeTextureLoaded           = false;
	this->TransferFunctionTextureLoaded = false;
	this->DrawBoundingBoxEnabled		= false;
	this->FrameCountEnabled             = false;
	this->FrameCount                    = NULL;
	this->Timestep						= FIRST_INDEX;
	this->VolumeTextureID				= NULL;
	this->TransferFunctionTextureID		= NULL;
	this->OffsetBottom					= 0.0;
	this->OffsetTop						= 0.0;
	this->ScaleValue					= 1.0;
	this->Height						= DEFAULT_HEIGHT;
}

////////////////////////////////////////////////////////////////////////////////////////////
vtkGPUVolumeMapper::~vtkGPUVolumeMapper()
{
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeMapper::Render(vtkRenderer *ren, vtkVolume *vol)
{
	// initialize glew
	if(!this->GlewInitialized)
	{
		glewInit();
		this->GlewInitialized = true;
	}

	// load volume texture
	if(!this->VolumeTextureLoaded)
		this->LoadVolumeTexture();

	// load transfer function texture
	if(!this->TransferFunctionTextureLoaded)
		this->LoadTransferFunctionTexture();

	// install shaders
	vtkShaderHandler *shaderHandler = vtkShaderHandler::GetInstance();
	if(!shaderHandler->IsInstalled())
		shaderHandler->Install();

	// OpenGL stuff
	// lighting
	glDisable(GL_LIGHTING);
	// blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// compute the height
	this->Height = DEFAULT_HEIGHT * this->ScaleValue;

	// transformation to the center of the hurricane
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

		glTranslatef(LATITUDE_DISPLACEMENT, LONGITUDE_DISPLACEMENT, 0.0);
	
		// Make sure that the shader program is enabled, then draw the volume.
		// Disable the shader program afterwards so that further drawings
		// are done with fixed-functionality.
		shaderHandler->EnableShaderProgram();
		this->DrawVolume(ren);
		shaderHandler->DisableShaderProgram();

		// draw the bounding box
		if(this->DrawBoundingBoxEnabled)
			this->DrawBoundingBox();

	glPopMatrix();

	// draw the transfer function area for vtkApplication
	// right now the drawing of the transfer function area is disables for vjApplication
	// since user interaction has not been implemented yet
	if(!VRJUGGLER_APP)
		this->TFReader->Render();

	// count frame
	if(this->FrameCountEnabled)
		this->FrameCount++;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeMapper::DrawBoundingBox()
{
	glLineWidth(3.0);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBegin(GL_LINE_STRIP);
		glVertex3f(LATITUDE_LEFT,  LONGITUDE_BOTTOM, 0.0);
		glVertex3f(LATITUDE_LEFT,  LONGITUDE_TOP,	 0.0);
		glVertex3f(LATITUDE_RIGHT, LONGITUDE_TOP,	 0.0);
		glVertex3f(LATITUDE_RIGHT, LONGITUDE_BOTTOM, 0.0);
		glVertex3f(LATITUDE_LEFT,  LONGITUDE_BOTTOM, 0.0);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex3f(LATITUDE_LEFT,  LONGITUDE_BOTTOM, this->Height);
		glVertex3f(LATITUDE_LEFT,  LONGITUDE_TOP,	 this->Height);
		glVertex3f(LATITUDE_RIGHT, LONGITUDE_TOP,	 this->Height);
		glVertex3f(LATITUDE_RIGHT, LONGITUDE_BOTTOM, this->Height);
		glVertex3f(LATITUDE_LEFT,  LONGITUDE_BOTTOM, this->Height);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex3f(LATITUDE_LEFT, LONGITUDE_BOTTOM, this->Height);
		glVertex3f(LATITUDE_LEFT, LONGITUDE_BOTTOM, 0.0);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex3f(LATITUDE_LEFT, LONGITUDE_TOP, this->Height);
		glVertex3f(LATITUDE_LEFT, LONGITUDE_TOP, 0.0);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex3f(LATITUDE_RIGHT, LONGITUDE_BOTTOM, this->Height);
		glVertex3f(LATITUDE_RIGHT, LONGITUDE_BOTTOM, 0.0);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex3f(LATITUDE_RIGHT, LONGITUDE_TOP, this->Height);
		glVertex3f(LATITUDE_RIGHT, LONGITUDE_TOP, 0.0);
	glEnd();
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeMapper::LoadVolumeTexture()
{
	int pixel_format = GL_LUMINANCE;
	int pixel_datatype = GL_UNSIGNED_BYTE;
	unsigned char* data = (unsigned char*) this->GetInput()->GetPointData()->GetScalars()->GetVoidPointer(0);

	// delete the previous texture to avoid memory trashing
	if(this->VolumeTextureID != NULL)
		glDeleteTextures(1, &this->VolumeTextureID);

	// load volume texture into texture memory
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &this->VolumeTextureID);
	glBindTexture(GL_TEXTURE_3D, this->VolumeTextureID);
	glTexImage3D(GL_TEXTURE_3D, 0, NUMBER_OF_SCALAR_COMPONENTS, SIZEX, SIZEY, SIZEZ, 0, pixel_format, pixel_datatype, data);
	glTexEnvi(      GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,   GL_REPLACE);
	glTexParameterf(GL_TEXTURE_3D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_3D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_3D,  GL_TEXTURE_WRAP_S,     GL_CLAMP);
	glTexParameterf(GL_TEXTURE_3D,  GL_TEXTURE_WRAP_T,     GL_CLAMP);
	glTexParameterf(GL_TEXTURE_3D,  GL_TEXTURE_WRAP_R,     GL_CLAMP);
	
	// install static shader uniform to provide the texture to shader programs
	vtkShaderHandler::GetInstance()->SetStaticShaderUniform(new vtkShaderUniform("Voxels", 0));
	this->VolumeTextureLoaded = true;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeMapper::LoadTransferFunctionTexture()
{
	// delete the previous texture to avoid memory trashing
	if(this->TransferFunctionTextureID != NULL)
		glDeleteTextures(1, &this->TransferFunctionTextureID);

	// load transfer function texture into texture memory
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &this->TransferFunctionTextureID);
	glBindTexture(GL_TEXTURE_1D, this->TransferFunctionTextureID);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, TF_TABLE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->TFReader->GetColorTable());
	glTexEnvi(      GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,   GL_REPLACE);
	glTexParameterf(GL_TEXTURE_1D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D,  GL_TEXTURE_WRAP_S,     GL_CLAMP);

	// install static shader uniform to provide the texture to shader programs
	vtkShaderHandler::GetInstance()->SetStaticShaderUniform(new vtkShaderUniform("TransferFunctionLookupTable", 1));
	this->TransferFunctionTextureLoaded = true;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeMapper::SetVolumeDataReader(vtkVolumeDataReader *volumeReader)
{
	this->VolumeReader = volumeReader;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeMapper::SetTransferFunctionReader(vtkTransferFunctionReader *tfReader)
{
	this->TFReader = tfReader;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeMapper::SetVolumeTextureLoaded(bool loaded)
{
	this->VolumeTextureLoaded = loaded;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeMapper::SetTransferFunctionTextureLoaded(bool loaded)
{
	this->TransferFunctionTextureLoaded = loaded;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeMapper::SetDrawBoundingBoxEnabled(bool enabled)
{
	this->DrawBoundingBoxEnabled = enabled;
}

////////////////////////////////////////////////////////////////////////////////////////////
bool vtkGPUVolumeMapper::IsDrawBoundingBoxEnabled()
{
	return this->DrawBoundingBoxEnabled;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeMapper::SetFrameCountEnabled(bool enabled)
{
	this->FrameCountEnabled = enabled;
}

////////////////////////////////////////////////////////////////////////////////////////////
bool vtkGPUVolumeMapper::IsFrameCountEnabled()
{
	return this->FrameCountEnabled;
}

////////////////////////////////////////////////////////////////////////////////////////////
int vtkGPUVolumeMapper::GetFrameCount()
{
	return this->FrameCount;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeMapper::ResetFrameCount()
{
	this->FrameCount = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeMapper::RemoveBottomSlices()
{
	if(this->OffsetBottom < 1.0)
		this->OffsetBottom += 0.05;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeMapper::AddBottomSlices()
{
	if(this->OffsetBottom > 0.0)
		this->OffsetBottom -= 0.05;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeMapper::RemoveTopSlices()
{
	if(this->OffsetTop < 1.0)
		this->OffsetTop += 0.05;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeMapper::AddTopSlices()
{
	if(this->OffsetTop > 0.0)
		this->OffsetTop -= 0.05;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeMapper::IncScaleValue()
{
	this->ScaleValue += 0.1;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeMapper::DecScaleValue()
{
	if(this->ScaleValue > 0.1)
		this->ScaleValue -= 0.1;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeMapper::LastTimestep()
{
	this->Timestep = FIRST_INDEX + TIMESTEPS - 1;

	this->SetInput(this->VolumeReader->GetOutput(this->Timestep));
	this->VolumeTextureLoaded = false;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeMapper::NextTimestep()
{
	if(this->Timestep < FIRST_INDEX + TIMESTEPS - 1)
	{
		this->Timestep++;
		this->SetInput(this->VolumeReader->GetOutput(this->Timestep));
		this->VolumeTextureLoaded = false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeMapper::PreviousTimestep()
{
	if(this->Timestep > FIRST_INDEX)
	{
		this->Timestep--;
		this->SetInput(this->VolumeReader->GetOutput(this->Timestep));
		this->VolumeTextureLoaded = false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeMapper::FirstTimestep()
{
	this->Timestep = FIRST_INDEX;

	this->SetInput(this->VolumeReader->GetOutput(this->Timestep));
	this->VolumeTextureLoaded = false;
}

////////////////////////////////////////////////////////////////////////////////////////////
int vtkGPUVolumeMapper::GetTimestep()
{
	return this->Timestep ;
}