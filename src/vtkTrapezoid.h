/**
 * \file   vtkTrapezoid.h
 * \author Christof Rezk-Salama
 * \class  vtkTrapezoid
 * \brief  Represents a trapezoid in the tranfer function area.
 *
 * vtkTrapezoid respresents a trapezoid in the tranfer function area. Each trapezoid
 * respresents a specific color. By moving the points of the trapezoid the user can
 * modify the transfer function.
 *
 * This class is taken from the example for the "Real-Time Volume Graphics" book.
 * The example can be downloaded at http://www.real-time-volume-graphics.org/
 * [accessed January 2009]. It was slightly modified to fit into the design 
 * of this project.
 */

#ifndef __vtkTrapezoid_h
#define __vtkTrapezoid_h

// C++ includes
#include <stdlib.h>

// OpenGL includes
#include <GL/glew.h>
#include <GL/glut.h>

// VTK includes
#include <vtkObject.h>
#include <vtkObjectFactory.h>

// user-own includes
#include "globaldefs.h"
#include "CPosition.h"
#include "CVector.h"

// define constants
#define OFFSET 5

class vtkTrapezoid : public vtkObject
{
public:
	static vtkTrapezoid *New();

	/** Sets the trapezoid. */
	void SetTrapezoid(int Start, int Mid1, int Mid2,  int End,  int Height);

	/** Renders the trapezoid. */
	void Render();

	/** Handles a button event. */
	bool  HandleButtonEvent(int button, int state, int x, int y);

	/** Handles a mouse move event. */
	bool  HandleMoveEvent(int x, int y);

	/** Gets the start value. */
	int GetStart();

	/** Gets the mid1 value. */
	int GetMid1();

	/** Gets the mid2 value. */
	int GetMid2();

	/** Gets the end value. */
	int GetEnd();

	/** Gets the height. */
	int GetHeight();

	/** Gets the color. */
	const CVector &GetColor();

	/** Sets the color. */
	void SetColor(CVector color);

	/** Gets the value on the given position. */
	int GetValue(int pos);

	vtkTrapezoid();
	~vtkTrapezoid();

private:
	int Start;
	int Mid1;
	int Mid2;
	int End;
	int Height;
	CVector Color;

	/** create type for interaction state
	 *  0 == no interaction
	 *  1 == start point
	 *  2 == mid1 point
	 *  4 == mid2 point
	 *  8 == end point
	 * 16 == lines
	 */ 
	typedef enum
	{
		NO_INTERACTION         = 0,                   
		MOVE_0                 = 1,
		MOVE_1                 = 2,
		MOVE_0_AND_1           = 3,  
		MOVE_2                 = 4,
		MOVE_1_AND_2           = 6,
		MOVE_3                 = 8, 
		MOVE_2_AND_3           = 12,
		MOVE                   = 15		  
	} InteractionState;

	/** Classifies the selected point */
	InteractionState ClassifyPoint(CPosition pos, int &selectedPoint);

	CPosition        Points[4];
	InteractionState State;
	InteractionState DragMode;
	bool             DragValid;
	bool             MouseIn;
	CPosition        MouseDown;
	int              SelectedPoint;
};

#endif