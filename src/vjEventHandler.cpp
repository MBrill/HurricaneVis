#include "vjEventHandler.h"

////////////////////////////////////////////////////////////////////////////////////////////
vjEventHandler::vjEventHandler()
{
	// initialize Wand
	this->Wand.init("VJWand");
	this->WandButton0.init("VJButton0");
	this->WandButton1.init("VJButton1");
	this->WandButton2.init("VJButton2");
	this->WandButton3.init("VJButton3");
	this->WandButton4.init("VJButton4");
	this->WandButton5.init("VJButton5");
	this->WandJoystickX.init("VJAnalog0");
	this->WandJoystickY.init("VJAnalog1");
}

////////////////////////////////////////////////////////////////////////////////////////////
vjEventHandler::~vjEventHandler()
{
}

////////////////////////////////////////////////////////////////////////////////////////////
void vjEventHandler::SetVolumeMapper(vtkGPUVolumeMapper *mapper)
{
	this->Mapper = mapper;
}

////////////////////////////////////////////////////////////////////////////////////////////
void vjEventHandler::HandleUserEvents()
{
	Matrix44f position = this->Wand->getData();
	const float *m = position.getData();

	// exit application
	switch(this->WandButton5->getData())
	{
		case Digital::ON:
		{
			cout << "exit" << endl;
			Kernel *kernel = Kernel::instance();
			kernel->stop();
			break;
		}
	}

	// zoom
	switch(this->WandButton4->getData())
	{
		case Digital::ON:
		{
			float z = *(m+14);
			glTranslatef(0.0, 0.0, z / 10.0);
			break;
		}
	}

	// rotation
	switch(this->WandButton0->getData())
	{
		case Digital::ON:
		{
			float x = WandJoystickX->getData();
			float y = WandJoystickY->getData();

			x -= 0.5;
			y -= 0.5;

			glRotatef(10.0, x*4.0, y*4.0, 0.0);

			/*for(int i = 0; i < 16; i++)			
				cout << *(m+i);
			cout << endl;
			glMultMatrixf(m);*/

			break;
		}
	}

	// translation
	switch(this->WandButton1->getData())
	{
		case Digital::ON:
		{
			float x = WandJoystickX->getData();
			float y = WandJoystickY->getData();

			x -= 0.5;
			y -= 0.5;

			glTranslatef(x, y, 0.0);

			break;
		}
	}


	// switch between timesteps
	switch(this->WandButton2->getData())
	{
		case Digital::ON:
		{
			this->Mapper->PreviousTimestep();
			break;
		}
	}
	switch(this->WandButton3->getData())
	{
		case Digital::ON:
		{
			this->Mapper->NextTimestep();
			break;
		}
	}
}