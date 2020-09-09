#include "vtkEventHandler.h"

vtkStandardNewMacro(vtkEventHandler);

////////////////////////////////////////////////////////////////////////////////////////////
vtkEventHandler::vtkEventHandler()
{
	this->Mapper				= NULL;
	this->VolumeReader			= NULL;
	this->TFReader				= NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////
vtkEventHandler::~vtkEventHandler()
{
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkEventHandler::SetVolumeMapper(vtkGPUVolumeMapper *mapper)
{
	this->Mapper = mapper;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkEventHandler::SetVolumeReader(vtkVolumeDataReader *volumeReader)
{
	this->VolumeReader = volumeReader;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkEventHandler::SetTransferFunctionReader(vtkTransferFunctionReader *tfReader)
{
	this->TFReader = tfReader;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkEventHandler::PrintUserManual()
{
	cout << "********************************************" << endl;
	cout << "* left   mouse + drag      = rotate volume *" << endl;
	cout << "* middle mouse + drag      = pan volume    *" << endl;
	cout << "* right  mouse + drag up   = zoom in       *" << endl;
	cout << "* right  mouse + drag down = zoom out      *" << endl;
	cout << "*                                          *" << endl;
	cout << "********************************************" << endl;
	cout << "* l = load transfer function from file     *" << endl;
	cout << "* w = write transfer function to file      *" << endl;
	cout << "* x = toggle bounding box                  *" << endl;
	cout << "*                                          *" << endl;
	cout << "* Page Up    = last timestep               *" << endl;
	cout << "* Up Arrow   = next timestep               *" << endl;
	cout << "* Down Arrow = previous timestep           *" << endl;
	cout << "* Page Down  = first timestep              *" << endl;
	cout << "*                                          *" << endl;
	cout << "* b/B = remove/add slices on bottom        *" << endl;
	cout << "* t/T = remove/add slices on top           *" << endl;
	cout << "* s/S = dec/inc scale value                *" << endl;
	cout << "*                                          *" << endl;
	cout << "* Rendering method:                        *" << endl;
	cout << "* 1 = object aligned slices                *" << endl;
	cout << "* 2 = view aligned slices                  *" << endl;
	cout << "*                                          *" << endl;
	cout << "* Esc/q = quit application                 *" << endl;
	cout << "********************************************" << endl << endl;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkEventHandler::OnLeftButtonDown()
{
	// get mouse coordinates
	int *x = new int;
	int *y = new int;
	this->Interactor->GetEventPosition(*x, *y);

	// forward event to TFReader
	if(!this->TFReader->HandleButtonEvent(0,0,*x,*y))
		this->vtkInteractorStyleJoystickCamera::OnLeftButtonDown();
	else
		this->Interactor->Render();
}

////////////////////////////////////////////////////////////////////////////////////////////

void vtkEventHandler::OnLeftButtonUp()
{
	// get mouse coordinates
	int *x = new int;
	int *y = new int;
	this->Interactor->GetEventPosition(*x, *y);

	// forward event to TFReader
	if(!this->TFReader->HandleButtonEvent(0,1,*x,*y))
		this->vtkInteractorStyleJoystickCamera::OnLeftButtonUp();
	else
		this->Interactor->Render();

	delete x;
	delete y;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkEventHandler::OnMouseMove()
{
	// get mouse coordinates
	int *x = new int;
	int *y = new int;
	this->Interactor->GetEventPosition(*x, *y);

	// forward event to TFReader
	if(*x < 350 && *y < 300)
	{
		this->TFReader->HandleMoveEvent(*x,*y);
		this->Interactor->Render();
	}

	delete x;
	delete y;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkEventHandler::OnRightButtonDown()
{
	this->vtkInteractorStyleJoystickCamera::OnRightButtonDown();
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkEventHandler::OnRightButtonUp()
{
	this->vtkInteractorStyleJoystickCamera::OnRightButtonUp();
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkEventHandler::OnMiddleButtonDown()
{
	this->vtkInteractorStyleJoystickCamera::OnMiddleButtonDown();
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkEventHandler::OnMiddleButtonUp()
{
	this->vtkInteractorStyleJoystickCamera::OnMiddleButtonUp();
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkEventHandler::OnKeyDown()
{
	// get pressed key
	char key = this->Interactor->GetKeyCode();

	// handle normal key events
	switch(key)
	{
		case 'q':
			exit(EXIT_SUCCESS);
		case 'l':
		{
			this->TFReader->ReadTransferFunction();
			this->Mapper->SetTransferFunctionTextureLoaded(false);
			break;
		}
		case 'w':
		{
			this->TFReader->WriteTransferFunction();
			break;
		}
		case 'x':
		{
			if(this->Mapper->IsDrawBoundingBoxEnabled())
				this->Mapper->SetDrawBoundingBoxEnabled(false);
			else
				this->Mapper->SetDrawBoundingBoxEnabled(true);
			break;
		}
		case 'b':
		{
			this->Mapper->RemoveBottomSlices();
			break;
		}
		case 'B':
		{
			this->Mapper->AddBottomSlices();
			break;
		}
		case 't':
		{
			this->Mapper->RemoveTopSlices();
			break;
		}
		case 'T':
		{
			this->Mapper->AddTopSlices();
			break;
		}
		case 's':
		{
			this->Mapper->DecScaleValue();
			break;
		}
		case 'S':
		{
			this->Mapper->IncScaleValue();
			break;
		}
		
		case '1':
		{
			if(vtkGPUVolumeTextureMapper3D* texMapper = dynamic_cast<vtkGPUVolumeTextureMapper3D*>(this->Mapper))
			{
				texMapper->SetViewAlignedSlices(false);
				vtkShaderHandler *shaderHandler = vtkShaderHandler::GetInstance();
				shaderHandler->SetVertexShaderSource(VERTEX_SHADER_3D_TEXTURE_MAPPING_OBJECT_ALIGNED);
				shaderHandler->SetInstalled(false);
			}
			break;
		}
		case '2':
		{
			if(vtkGPUVolumeTextureMapper3D* texMapper = dynamic_cast<vtkGPUVolumeTextureMapper3D*>(this->Mapper))
			{
				texMapper->SetViewAlignedSlices(true);
				vtkShaderHandler *shaderHandler = vtkShaderHandler::GetInstance();
				shaderHandler->SetVertexShaderSource(VERTEX_SHADER_3D_TEXTURE_MAPPING_VIEW_ALIGNED);
				shaderHandler->SetInstalled(false);
			}
			break;
		}
	}

	// handle special key events
	const char *spKey = this->Interactor->GetKeySym();

	if(spKey)
	{
		if(strcmp(spKey, "Escape") == 0)
			exit(EXIT_SUCCESS);

		if(strcmp(spKey, "Up") == 0)
		{
			this->Mapper->NextTimestep();
		}

		if(strcmp(spKey, "Down") == 0)
		{
			this->Mapper->PreviousTimestep();
		}

		// Page Up
		if(strcmp(spKey, "Prior") == 0)
		{
			this->Mapper->LastTimestep();
		}

		// Page Down
		if(strcmp(spKey, "Next") == 0)
		{
			this->Mapper->FirstTimestep();
		}
	}

	// re-render the entire scene
	this->Interactor->Render();
}