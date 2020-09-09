#include "vtkTransferFunctionReader.h"

vtkStandardNewMacro(vtkTransferFunctionReader);

////////////////////////////////////////////////////////////////////////////////////////////
vtkTransferFunctionReader::vtkTransferFunctionReader()
{
	this->NumTrapezoids = 0;
	this->Mapper = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////
vtkTransferFunctionReader::~vtkTransferFunctionReader()
{
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkTransferFunctionReader::ReadTransferFunction()
{
	// compose filename
	char filename[256];
	strcpy(filename, DATA_DIR);
	strcat(filename, TF_NAME);

	// open the transfer function file
	FILE *f = fopen(filename, "rt");
	if(f == NULL)
	{
		vtkErrorMacro(<< "Could not open transfer function file " << filename);
		return;
	}

	// remove all existing Trapezoids by setting the number to 0
	this->NumTrapezoids = 0;

	// read the file
	char line[MAX_LINE_LENGTH];
	fgets(line, MAX_LINE_LENGTH, f);
	while(!feof(f))
	{
		if(line[0] != '#')
		{
			float vals[9];

			int i = 0;
			char *token = strtok(line, " ");
			
			while(token != NULL || i < 9)
			{
				vals[i++] = atof(token);
				token = strtok(NULL, " ");
			}

			// create a vtkTrapezoid for each line
			this->Trapezoids[this->NumTrapezoids].SetTrapezoid(vals[0], vals[1], vals[2], vals[3], vals[4]);
			this->Trapezoids[this->NumTrapezoids].SetColor(CVector(vals[5], vals[6], vals[7], vals[8]));
			this->NumTrapezoids++;
		}

		fgets(line, MAX_LINE_LENGTH, f);
	}

	fclose(f);

	// update the color table
	this->UpdateColorTable();
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkTransferFunctionReader::WriteTransferFunction()
{
	// compose complete filename
	char filename[256];
	strcpy(filename, DATA_DIR);
	strcat(filename, TF_NAME);

	// writing the transfer function to file
	ofstream file(filename);
	if(file.is_open())
	{
		for(int i = 0; i < this->NumTrapezoids; i++)
		{
			file << this->Trapezoids[i].GetStart() << " ";
			file << this->Trapezoids[i].GetMid1() << " ";
			file << this->Trapezoids[i].GetMid2() << " ";
			file << this->Trapezoids[i].GetEnd() << " ";
			file << this->Trapezoids[i].GetHeight() << " ";

			CVector color = this->Trapezoids[i].GetColor();
			file << color[0] << " ";
			file << color[1] << " ";
			file << color[2] << " ";
			file << color[3] << endl;
		}
		file.close();
	}
	else
		vtkErrorMacro(<< "Could not open transfer function file " << filename);
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkTransferFunctionReader::SetMapper(vtkGPUVolumeMapper *mapper)
{
	this->Mapper = mapper;
}

////////////////////////////////////////////////////////////////////////////////////////////
unsigned char* vtkTransferFunctionReader::GetColorTable()
{
	return this->ColorTable;
}

////////////////////////////////////////////////////////////////////////////////////////////
bool vtkTransferFunctionReader::HandleButtonEvent(int button, int state, int x, int y)
{
	if((x < OFFSET_X + MARGIN_LEFT + MARGIN_RIGHT + TF_TABLE_SIZE) && (y < OFFSET_Y + MARGIN_TOP + MARGIN_BOTTOM + 256))
	{
		for(int i = 0; i < this->NumTrapezoids; i++)
		{
			this->Trapezoids[i].HandleButtonEvent(button, state, x - OFFSET_X - MARGIN_LEFT, y - OFFSET_Y - MARGIN_BOTTOM);
		}
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////
bool vtkTransferFunctionReader::HandleMoveEvent(int x, int y)
{
	int oldStart, oldEnd, start, end;
	for(int i = 0; i < this->NumTrapezoids; i++)
	{
		oldStart = this->Trapezoids[i].GetStart();
		oldEnd = this->Trapezoids[i].GetEnd();
		if(this->Trapezoids[i].HandleMoveEvent(x - OFFSET_X - MARGIN_LEFT, y - OFFSET_Y - MARGIN_BOTTOM))
		{
			start = this->Trapezoids[i].GetStart();
			end = this->Trapezoids[i].GetEnd();
			if(start > oldStart) start = oldStart;
			if(end < oldEnd) end = oldEnd;
			this->UpdateColorTable();
			break;
		}
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkTransferFunctionReader::Render() 
{
	// setup for planar 2D rendering
	glViewport(OFFSET_X, OFFSET_Y, MARGIN_LEFT + MARGIN_RIGHT + 330, MARGIN_TOP + MARGIN_BOTTOM + 256);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0, float(MARGIN_LEFT + MARGIN_RIGHT + 330), float(MARGIN_TOP + MARGIN_BOTTOM + 256), 0.0, 0.0, 2.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
		// enable blending
		glEnable(GL_BLEND);
		glAlphaFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		// disable depth test
		glDisable(GL_DEPTH_TEST);

		// draw transfer function area
		glBegin(GL_QUADS);
			glColor4f(1.0, 1.0, 1.0, 0.7);
			glVertex2i(0, 0);
			glVertex2i(MARGIN_LEFT + MARGIN_RIGHT + 330, 0);
			glVertex2i(MARGIN_LEFT + MARGIN_RIGHT + 330, MARGIN_TOP + MARGIN_BOTTOM + 256);
			glVertex2i(0, MARGIN_TOP + MARGIN_BOTTOM + 256);
		glEnd();

		// draw Trapezoids
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
			glTranslatef(MARGIN_LEFT, MARGIN_TOP, 0.0);

			for(int i = 0; i < NumTrapezoids; ++i)
				this->Trapezoids[i].Render();


			// draw grid
			glLineWidth(1.0);
			glBegin(GL_LINES);
				glColor4f(0.0, 0.0, 0.0, 1.0);

				// vertical lines
				glVertex2i(0, 0);
				glVertex2i(0, 260);
				glVertex2i(64, 0);
				glVertex2i(64, 260);
				glVertex2i(128, 0);
				glVertex2i(128, 260);
				glVertex2i(192, 0);
				glVertex2i(192, 260);
				glVertex2i(256, 0);
				glVertex2i(256, 260);

				// horizontal lines
				glVertex2i(0, 256);
				glVertex2i(256, 256);
			glEnd();

		// draw legend
		char valueAsString[100];
		char * ptr;

		// x-axis min value
		glRasterPos2f(-1, 270);
		sprintf(valueAsString, "%.5f", MIN_VALUE);
		for(ptr = valueAsString; *ptr != '\0'; ptr++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *ptr);

		// x-axis 1/4 value
		glRasterPos2f(64, 280);
		sprintf(valueAsString, "%.5f", ((float) MIN_VALUE + (((float) (MIN_VALUE + MAX_VALUE)) / 2.0)) / 2.0);
		for(ptr = valueAsString; *ptr != '\0'; ptr++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *ptr);

		// x-axis 1/2 value
		glRasterPos2f(128, 270);
		sprintf(valueAsString, "%.5f", ((float) (MIN_VALUE + MAX_VALUE)) / 2.0);
		for(ptr = valueAsString; *ptr != '\0'; ptr++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *ptr);

		// x-axis 3/4 value
		glRasterPos2f(192, 280);
		sprintf(valueAsString, "%.5f", ((float) MAX_VALUE + (((float) (MIN_VALUE + MAX_VALUE)) / 2.0)) / 2.0);
		for(ptr = valueAsString; *ptr != '\0'; ptr++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *ptr);

		// x-axis max value
		glRasterPos2f(256, 270);
		sprintf(valueAsString, "%.5f", MAX_VALUE);
		for(ptr = valueAsString; *ptr != '\0'; ptr++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *ptr);

		// x-axis label
		glRasterPos2f(270, 240);
		for(ptr = DESCRIPTION; *ptr != '\0'; ptr++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *ptr);

		// x-axis units
		glRasterPos2f(270, 250);
		for(ptr = UNITS; *ptr != '\0'; ptr++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *ptr);

		// y-axis label
		glRasterPos2f(-MARGIN_LEFT, -5);
		for(ptr = "Opacity"; *ptr != '\0'; ptr++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *ptr);

		// y-axis max value
		glRasterPos2f(-18, 10);
		for(ptr = "1.0"; *ptr != '\0'; ptr++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *ptr);

		// y-axis min value
		glRasterPos2f(-18, 256);
		for(ptr = "0.0"; *ptr != '\0'; ptr++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *ptr);

		// draw timestep information
		glRasterPos2f(270, -5);
		for(ptr = "Timestep:"; *ptr != '\0'; ptr++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *ptr);

		glRasterPos2f(320, -5);
		sprintf(valueAsString, "%i", this->Mapper->GetTimestep());
		for(ptr = valueAsString; *ptr != '\0'; ptr++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *ptr);

		// reset OpenGL stuff
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkTransferFunctionReader::UpdateColorTable()
{
	int start = 0;
	int end = TF_TABLE_SIZE;
		
	double alphaMax, alphaSum, red, green, blue, value, height, factor;
	CVector color;
	int index = start * 4;
	
	// update the color table
	for(int step = start; step <= end; step++)
	{		
		alphaMax = 0.0;
		alphaSum = 0.0;
		red      = 0.0;
		green    = 0.0;
		blue     = 0.0;
		
		// traverse all Trapezoids
		for(int i = 0; i < NumTrapezoids; i++)
		{
			// get the height of the vtkTrapezoid and the value at the current x value
			// and scale both to [0.0 1.0]
			height = Trapezoids[i].GetHeight()/255.0;
			value  = Trapezoids[i].GetValue(step)/255.0;

			// only if there is a vtkTrapezoid at this x value the colors are modified
			if(value > 0.0)
			{
				factor = value/height;

				// keep the highest alpha value at this x value
				if(value > alphaMax)
				{
					alphaMax = value;
				}
				alphaSum += value;
				
				color = this->Trapezoids[i].GetColor();
				red   += value * factor * color[0];
				green += value * factor * color[1];
				blue  += value * factor * color[2];
			}
		}

		red   /= alphaSum;
		green /= alphaSum;
		blue  /= alphaSum;

		ColorTable[index++] = (unsigned char) (red      * 255.0);
		ColorTable[index++] = (unsigned char) (green    * 255.0);
		ColorTable[index++] = (unsigned char) (blue     * 255.0);
		ColorTable[index++] = (unsigned char) (alphaMax * 255.0);
	}

	// notify the mapper that the transfer function texture has to be reloaded
	if(this->Mapper != NULL)
		this->Mapper->SetTransferFunctionTextureLoaded(false);
}