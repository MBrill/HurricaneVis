#include "vtkTrapezoid.h"

vtkStandardNewMacro(vtkTrapezoid);

////////////////////////////////////////////////////////////////////////////////////////////
vtkTrapezoid::vtkTrapezoid() 
{
	this->Start	 = 0;
	this->Mid1	 = 0;
	this->Mid2	 = 0;
	this->End	 = 0;
	this->Height = 0;
	for(int i = 0; i < 4; i++)
	{
		Points[i].set(0.0, 0.0);
	}
	Color.set(1.0, 1.0, 1.0, 0.0);
}

////////////////////////////////////////////////////////////////////////////////////////////
vtkTrapezoid::~vtkTrapezoid() 
{
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkTrapezoid::SetTrapezoid(int start, int mid1, int mid2,  int end,  int height)
{
	this->Start  = start;
	this->Mid1   = mid1;
	this->Mid2   = mid2;
	this->End    = end;
	this->Height = height;

	this->Points[0].set(start, 0);
	this->Points[1].set(mid1, height);
	this->Points[2].set(mid2, height);
	this->Points[3].set(end, 0);

}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkTrapezoid::Render() 
{
	// draw filling of trapezoid
	glLineWidth(1.0);
	glColor4f(this->Color[0], this->Color[1], this->Color[2], 0.5);
	glBegin(GL_QUADS);
		glVertex2i(this->Start, 256);
		glVertex2i(this->Mid1,  256 - this->Height);
		glVertex2i(this->Mid2,  256 - this->Height);
		glVertex2i(this->End,   256);
	glEnd();

	// draw line of trapezoid
	glColor4f(this->Color[0], this->Color[1], this->Color[2], 1.0);
	glBegin(GL_LINE_STRIP);
		glVertex2i(this->Start, 256);
		glVertex2i(this->Mid1,  256 - this->Height);
		glVertex2i(this->Mid2,  256 - this->Height);
		glVertex2i(this->End,   256);
	glEnd();

	// draw points of trapezoid
	glPointSize(8.0);
	glBegin(GL_POINTS);
		glVertex2i(this->Start, 256);
		glVertex2i(this->Mid1,  256 - this->Height);
		glVertex2i(this->Mid2,  256 - this->Height);
		glVertex2i(this->End,   256);
	glEnd();
	
}

////////////////////////////////////////////////////////////////////////////////////////////
bool vtkTrapezoid::HandleButtonEvent(int button, int state, int x, int y)
{	
	switch(state)
	{
		case GLUT_DOWN:
		{
			int dummy = 0;
			this->MouseDown = CPosition(x,y);
			this->DragMode = ClassifyPoint(this->MouseDown, dummy);
			this->DragValid = true;
			break;
		}
		return (this->DragMode != NO_INTERACTION);

		case GLUT_UP:
		{
			this->DragMode  = NO_INTERACTION;
			this->DragValid = false;
			break;
		}
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////
bool vtkTrapezoid::HandleMoveEvent(int x, int y)
{
	CPosition point(x,y);

	bool retval = false;
	int sel = this->SelectedPoint;
	
	InteractionState oldState = this->State;
	this->State = this->ClassifyPoint(point, this->SelectedPoint);
	
	if((oldState != this->State) || (sel != this->SelectedPoint))
	{
		retval = true;	
	}	
	
	if(this->DragValid)
	{
		int moveX  = int(point[0] - this->MouseDown[0]);
		int moveY  = -int(point[1] - this->MouseDown[1]);
		
		switch(this->DragMode)
		{
			case MOVE:
			{
				if((this->End + moveX) > TF_TABLE_SIZE - 1) moveX = TF_TABLE_SIZE - 1 - this->End;
				if((this->Start + moveX) < 0) moveX = -this->Start;
				if((this->Mid2 + moveX) > TF_TABLE_SIZE - 1) moveX = TF_TABLE_SIZE - 1 - this->Mid2;
				if((this->Mid1 + moveX) < 0) moveX = -this->Mid1;
				this->Start += moveX;
				this->Mid1  += moveX;
				this->End   += moveX;
				this->Mid2  += moveX;
				retval = true; 
				break;
			}

			case MOVE_0:
			{
				this->Start += moveX;
				if(this->Start > this->Mid1) this->Start = this->Mid1;
				if(this->Start < 0) this->Start = 0;
				retval = true; 
				break;
			}

			case MOVE_1:
			{
				this->Height -= moveY;
				if(this->Height > 255) this->Height = 255;
				if(this->Height < 0) this->Height = 0;
				this->Mid1 += moveX;
				if(this->Mid1 > this->Mid2) this->Mid1 = this->Mid2;
				if(this->Mid1 < this->Start) this->Mid1 = this->Start;
				retval = true; 
				break;
			}
		
			case MOVE_2:
			{
				this->Height -= moveY;
				if(this->Height > 255) this->Height = 255;
				if(this->Height < 0) this->Height = 0;
				//asymmetric movement
				this->Mid2 += moveX;
				if(this->Mid2 > this->End) this->Mid2 = this->End;
				if(this->Mid2 < this->Mid1) this->Mid2 = this->Mid1;
				retval = true; 
				break;
			}
		
			case MOVE_3:
			{
				this->End += moveX;
				if(this->End > TF_TABLE_SIZE - 1) this->End = TF_TABLE_SIZE - 1;
				if(this->End < this->Mid2) this->End = this->Mid2;
				retval = true; 
				break;
			}

			case MOVE_0_AND_1:
			{
				if((this->Mid1 + moveX) > this->Mid2) moveX = this->Mid2 - this->Mid1;
				if((Start + moveX) < 0) moveX = -this->Start;
				this->Start += moveX;
				this->Mid1 += moveX;
				retval = true;
				break;
			}
			
			case MOVE_2_AND_3:
			{
				if((this->End + moveX) > TF_TABLE_SIZE - 1) moveX = TF_TABLE_SIZE - 1 - this->End;
				if((this->Mid2 + moveX) < this->Mid1) moveX = this->Mid1 - this->Mid2;
				this->End += moveX;
				this->Mid2 += moveX;
				retval = true; 
				break;
			}

			case MOVE_1_AND_2:
			{
				this->Height -= moveY;
				if(this->Height > 255) this->Height = 255;
				if(this->Height < 0) this->Height = 0;
				
				int tmp = this->Mid2 + moveX;
				if(tmp > this->End) moveX = this->End - this->Mid2;
				tmp = this->Mid1 + moveX;
				if(tmp < this->Start) moveX = this->Start - this->Mid1;

				this->Mid2 += moveX;			
				this->Mid1 += moveX;
				retval = true;
				break;
			}
				
			default:
				break;
		}
	}
	else
	{
		DragValid = false;
	}

	this->Points[0].set(this->Start, 0);
	this->Points[1].set(this->Mid1, this->Height);
	this->Points[2].set(this->Mid2, this->Height);
	this->Points[3].set(this->End, 0);
	this->MouseDown = point;
	return retval;
}


////////////////////////////////////////////////////////////////////////////////////////////
int vtkTrapezoid::GetStart()
{
	return this->Start;
}

////////////////////////////////////////////////////////////////////////////////////////////
int vtkTrapezoid::GetMid1()
{
	return this->Mid1;
}

////////////////////////////////////////////////////////////////////////////////////////////
int vtkTrapezoid::GetMid2()
{
	return this->Mid2;
}

////////////////////////////////////////////////////////////////////////////////////////////
int vtkTrapezoid::GetEnd()
{
	return this->End;
}

////////////////////////////////////////////////////////////////////////////////////////////
int vtkTrapezoid::GetHeight()
{
	return this->Height;
}

////////////////////////////////////////////////////////////////////////////////////////////
const CVector &vtkTrapezoid::GetColor()
{
	return this->Color;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkTrapezoid::SetColor(CVector color)
{
	this->Color = color;
}

////////////////////////////////////////////////////////////////////////////////////////////
int vtkTrapezoid::GetValue(int pos)
{
	if((pos > this->End) || (pos < this->Start)) return 0;
	
	if(pos < this->Mid1)
	{
		double alpha = double(pos - this->Start) / double(this->Mid1 - this->Start);
		return int(alpha * double(this->Height));
	}

	if(pos > this->Mid2)
	{
		double alpha = double(pos - this->End) / double(this->Mid2 - this->End);
		return int(alpha * double(this->Height));
	}

	return this->Height;

}

////////////////////////////////////////////////////////////////////////////////////////////
vtkTrapezoid::InteractionState vtkTrapezoid::ClassifyPoint(CPosition point, int &selectedPoint)
{	
	int i;
	CPosition dist[4];
	selectedPoint = -1;
	
	// is the point close to one of the selection points?
	for(i = 0; i < 4; i++)
	{
		dist[i] = this->Points[i] - point;
		if(dist[i][0] < 0) dist[i][0] = - dist[i][0];
		if(dist[i][1] < 0) dist[i][1] = - dist[i][1];
		
		if((dist[i][0] < OFFSET) && (dist[i][1] < OFFSET))
		{
			selectedPoint = i;
			return InteractionState(int(1<< i));
		}
	}	

	// have we selected the left edge? CASE: edge is almost vertical
	if((point[1] < this->Points[1][1]) && (dist[0][0] < OFFSET) && (dist[1][0] < OFFSET))
	{
		return MOVE_0_AND_1;
	}
	
	// have we selected the left edge? CASE: edge is not almost vertical
	if(this->Points[1][0] - this->Points[0][0] > 1.0)
	{
		double alpha = (this->Points[1][1] - point[1]) / (this->Points[1][1] - Points[0][1]);
		double x3 = Points[1][0] + alpha * (this->Points[0][0] - this->Points[1][0]);
		
		x3 -= point[0];
		if(x3 < 0.0) x3 = -x3;
		if(x3 < OFFSET)
		{
			return MOVE_0_AND_1;
		} 
	}
	
	// have we selected the right edge? CASE: edge is almost vertical
	if((point[1] < this->Points[1][1]) && (dist[2][0] < OFFSET) && (dist[3][0] < OFFSET))
	{
		return MOVE_2_AND_3;
	}
	
	// have we selected the right edge? CASE: edge is not almost vertical
	if(this->Points[3][0] - this->Points[2][0] > 1.0)
	{
		double alpha = (this->Points[3][1] - point[1]) / (this->Points[3][1] - this->Points[2][1]);
		double x3 = this->Points[3][0] + alpha * (this->Points[2][0] - this->Points[3][0]);
		x3 -= point[0];
		if (x3 < 0.0) x3 = -x3;
		if (x3 < OFFSET)
		{
			return MOVE_2_AND_3;
		} 
	}
	
	// have we selected the top edge?
	if((dist[1][1] < 3) && (point[0] > this->Points[1][0]) && (point[0] < this->Points[2][0]))
	{
		return MOVE_1_AND_2;
	} 
	
	// have we selected whole region?
	if((point[1] < this->Points[1][1]) && (point[0] > this->Points[1][0]) && (point[0] < this->Points[2][0]))
	{
		return MOVE;
	}
	if((point[0] > this->Points[2][0]) && (point[0] < this->Points[3][0]))
	{
		double alpha = (this->Points[3][0] - point[0]) / (this->Points[3][0] - this->Points[2][0]);
		if (point[1] < alpha * this->Points[2][1])
			return MOVE;
	}
	if((point[0] > this->Points[0][0]) && (point[0] < this->Points[1][0]))
	{
		double alpha = (this->Points[0][0] - point[0]) / (this->Points[0][0] - this->Points[1][0]);
		if (point[1] < (alpha) * this->Points[1][1])
			return MOVE;
	}

	// seems we haven't selected anything!
	return NO_INTERACTION;
}