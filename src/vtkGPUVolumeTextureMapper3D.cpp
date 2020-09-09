#include "vtkGPUVolumeTextureMapper3D.h"

vtkStandardNewMacro(vtkGPUVolumeTextureMapper3D);

////////////////////////////////////////////////////////////////////////////////////////////
vtkGPUVolumeTextureMapper3D::vtkGPUVolumeTextureMapper3D()
{
	// set the initial shader programs
	this->ViewAlignedSlices			= false;
	vtkShaderHandler *shaderHandler = vtkShaderHandler::GetInstance();

	shaderHandler->SetVertexShaderSource(VERTEX_SHADER_3D_TEXTURE_MAPPING_OBJECT_ALIGNED);
	shaderHandler->SetFragmentShaderSource(FRAGMENT_SHADER_3D_TEXTURE_MAPPING);
}

////////////////////////////////////////////////////////////////////////////////////////////
vtkGPUVolumeTextureMapper3D::~vtkGPUVolumeTextureMapper3D()
{
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeTextureMapper3D::DrawVolume(vtkRenderer *ren)
{
	// forward variables to shader program
	vtkShaderHandler *shaderHandler = vtkShaderHandler::GetInstance();
	shaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("orientation", (int) ORIENTATION));
	shaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("offsetBottom", (float) this->OffsetBottom));
	shaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("offsetTop",    (float) this->OffsetTop));
	shaderHandler->Update();

	// draw the volume
	if(this->ViewAlignedSlices)
		DrawViewAlignedSlices(ren);
	else
		DrawObjectAlignedSlices(ren);
}
	
////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeTextureMapper3D::DrawObjectAlignedSlices(vtkRenderer *ren)
{
	// The object aligned slices have to be drawn either from bottom to top or from top to
	// bottom depending on the camera's z-position. This is necessary to get the correct alpha
	// blending (front-to-back or back-to-front compositing).

	// get the current camera position
	float modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

	if(modelview[10] > 0)
	{
		// draw slices from bottom to top
		for(int slice = 0; slice < NUM_SLICES_OBJECT_ALIGNED; slice++)
		{
			this->DrawSingleSlice(slice);
		}
	}
	else
	{
		// draw slices from top to bottom
		for(int slice = NUM_SLICES_OBJECT_ALIGNED; slice > 0; slice--)
		{
			this->DrawSingleSlice(slice);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeTextureMapper3D::DrawSingleSlice(int slice)
{
	// compute the slice's z-coordinate (sz)
	double sz = ((this->Height / (NUM_SLICES_OBJECT_ALIGNED - 1)) * slice);

	if(ORIENTATION == 0)
	{
		// Isabel orientation
		// compute the texture's z-coordinate (tz)
		double tz = 0.01 + (0.98 / (NUM_SLICES_OBJECT_ALIGNED - 1)) * slice;

		glBegin(GL_QUADS);
			glMultiTexCoord4f(GL_TEXTURE0, 0.99, 0.99, tz, 1.0);  glVertex3f(LATITUDE_LEFT,  LONGITUDE_BOTTOM, sz);
			glMultiTexCoord4f(GL_TEXTURE0, 0.99, 0.01, tz, 1.0);  glVertex3f(LATITUDE_RIGHT, LONGITUDE_BOTTOM, sz);
			glMultiTexCoord4f(GL_TEXTURE0, 0.01, 0.01, tz, 1.0);  glVertex3f(LATITUDE_RIGHT, LONGITUDE_TOP,    sz);
			glMultiTexCoord4f(GL_TEXTURE0, 0.01, 0.99, tz, 1.0);  glVertex3f(LATITUDE_LEFT,  LONGITUDE_TOP,    sz);
		glEnd();
	}
	else
	{
		// Lili orientation
		// compute the texture's z-coordinate (tz)
		double tz = 0.01 + (0.98 / (NUM_SLICES_OBJECT_ALIGNED - 1)) * (NUM_SLICES_OBJECT_ALIGNED - 1 - slice);

		glBegin(GL_QUADS);
			glMultiTexCoord4f(GL_TEXTURE0, 0.01, 0.99, tz, 1.0);  glVertex3f(LATITUDE_LEFT,  LONGITUDE_BOTTOM, sz);
			glMultiTexCoord4f(GL_TEXTURE0, 0.99, 0.99, tz, 1.0);  glVertex3f(LATITUDE_RIGHT, LONGITUDE_BOTTOM, sz);
			glMultiTexCoord4f(GL_TEXTURE0, 0.99, 0.01, tz, 1.0);  glVertex3f(LATITUDE_RIGHT, LONGITUDE_TOP,    sz);
			glMultiTexCoord4f(GL_TEXTURE0, 0.01, 0.01, tz, 1.0);  glVertex3f(LATITUDE_LEFT,  LONGITUDE_TOP,    sz);
		glEnd();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeTextureMapper3D::DrawViewAlignedSlices(vtkRenderer *ren)
{
	// define the bounding box
	CVector vertices[8];
	vertices[0] = CVector(LATITUDE_LEFT,  LONGITUDE_BOTTOM, 0.0,          1.0);
	vertices[1] = CVector(LATITUDE_RIGHT, LONGITUDE_BOTTOM, 0.0,          1.0);
	vertices[2] = CVector(LATITUDE_LEFT,  LONGITUDE_TOP,    0.0,          1.0);
	vertices[3] = CVector(LATITUDE_RIGHT, LONGITUDE_TOP,    0.0,          1.0);
															
	vertices[4] = CVector(LATITUDE_LEFT,  LONGITUDE_BOTTOM, this->Height, 1.0);
	vertices[5] = CVector(LATITUDE_RIGHT, LONGITUDE_BOTTOM, this->Height, 1.0);
	vertices[6] = CVector(LATITUDE_LEFT,  LONGITUDE_TOP,    this->Height, 1.0);
	vertices[7] = CVector(LATITUDE_RIGHT, LONGITUDE_TOP,    this->Height, 1.0);

	// compute the viewing vector
	float pMatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX,pMatrix);
	CMatrix matModelView(pMatrix[0],pMatrix[4],pMatrix[ 8],pMatrix[12],
						 pMatrix[1],pMatrix[5],pMatrix[ 9],pMatrix[13],
						 pMatrix[2],pMatrix[6],pMatrix[10],pMatrix[14],
						 pMatrix[3],pMatrix[7],pMatrix[11],pMatrix[15]);
	matModelView.invert();
	CVector viewVec(-pMatrix[2], -pMatrix[6], -pMatrix[10], 0.0);

	// determine the front index
	double maxDist = viewVec * vertices[0];
	int frontIdx = 0;
	for(int i = 1; i < 8; i++)
	{
		double dist = viewVec * vertices[i];
		if(dist > maxDist)
		{
			maxDist = dist;
			frontIdx = i;
		}
	}
	
	// This two dimensional array contains magic numbers that are necessary for the
	// algorithm used. The algorithm is explained in the book Real-Time Volume Graphics,
	// pages 73 to 79. I got the numbers by e-mail from one of the authors, Christof
	// Rezk-Salama. If anyone ever figures out how these numbers work please send an
	// e-mail to mberberich@gmx.net and explain it to me :-)
	const int nSequence[8][8] ={{7,3,5,6,1,2,4,0},
								{6,2,4,7,0,3,5,1},
								{5,1,4,7,0,3,6,2},
								{4,0,5,6,1,2,7,3},
								{3,1,2,7,0,5,6,4},
								{2,0,3,6,1,4,7,5},
								{1,0,3,5,2,4,7,6},
								{0,1,2,4,3,5,6,7},
								};

	// compute the number of slices to draw
	double dStartDist = viewVec * vertices[nSequence[frontIdx][0]];
	double dEndDist   = viewVec * vertices[frontIdx];
	double dPlaneIncr = SLICE_INC_VIEW_ALIGNED;
	double dS  = ceil((dStartDist)/dPlaneIncr);
	dStartDist = dS * dPlaneIncr;
	int nNumSlices = int((dEndDist-dStartDist)/dPlaneIncr)+1; 
	
	// forward variables to shader program
	vtkShaderHandler *shaderHandler = vtkShaderHandler::GetInstance();
	shaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("viewVec", (float) viewVec[0], (float) viewVec[1], (float) viewVec[2]));
	shaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("LATITUDE_LEFT",    (float) LATITUDE_LEFT));
	shaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("LATITUDE_RIGHT",   (float) LATITUDE_RIGHT));
	shaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("LONGITUDE_BOTTOM", (float) LONGITUDE_BOTTOM));
	shaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("LONGITUDE_TOP",    (float) LONGITUDE_TOP));
	shaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("sizex", (float) SIZEX));
	shaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("sizey", (float) SIZEY));
	shaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("sizez", (float) this->Height));
	shaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("frontIdx", frontIdx));
	shaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("dPlaneStart", (float) dStartDist));
	shaderHandler->SetDynamicShaderUniform(new vtkShaderUniform("dPlaneIncr", (float) dPlaneIncr));
	shaderHandler->Update();

	// draw the volume
	for(double i = nNumSlices; i > 0; i--)
	{
		glBegin(GL_POLYGON);
			glVertex2i(0, i);
			glVertex2i(1, i);
			glVertex2i(2, i);
			glVertex2i(3, i);
			glVertex2i(4, i);
			glVertex2i(5, i);
		glEnd();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkGPUVolumeTextureMapper3D::SetViewAlignedSlices(bool enabled)
{
	this->ViewAlignedSlices = enabled;
}