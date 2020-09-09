#include "vjApplication.h"

////////////////////////////////////////////////////////////////////////////////////////////
vjApplication::vjApplication(int argc, char* argv[]) : VTKApp()
{
	// get the kernel
	Kernel *kernel = Kernel::instance();

	// load config files
	// argument 1 specifies the data config file, arguments 2 and up are VRJuggler config files
	if(argc < 3)
	{
		// if no config files were specified load the standard simulator config files
		kernel->loadConfigFile("sim.base.jconf");
		kernel->loadConfigFile("sim.analog.mixin.jconf");
		kernel->loadConfigFile("sim.Wand.mixin.jconf");
	}
	else
	{
		// load any config files specified on the command line
	   for(int i = 2; i < argc; i++)
	   {
		  kernel->loadConfigFile(argv[i]);
	   }
	}

	// start the kernel
	kernel->start();

	// set the application which the kernel will run now
	kernel->setApplication(this);
	kernel->waitForKernelStop();
}

////////////////////////////////////////////////////////////////////////////////////////////
vjApplication::~vjApplication()
{
};

////////////////////////////////////////////////////////////////////////////////////////////
void vjApplication::init()
{
	// read volume data
	vtkVolumeDataReader *volumeReader = vtkVolumeDataReader::New();
	volumeReader->ReadVolumeData();

	// read transfer function
	vtkTransferFunctionReader *tfReader = vtkTransferFunctionReader::New();
	tfReader->ReadTransferFunction();

	// create volume texture mapper
	this->Mapper = vtkGPUVolumeTextureMapper3D::New();
	//this->Mapper = vtkGPUVolumeRayCastMapper::New();
	this->Mapper->SetInput(volumeReader->GetOutput(FIRST_INDEX));
	this->Mapper->SetVolumeDataReader(volumeReader);
	this->Mapper->SetTransferFunctionReader(tfReader);
	tfReader->SetMapper(this->Mapper);

	// create volume
	vtkVolume *volume = vtkVolume::New();
	volume->SetMapper(this->Mapper);

	// read state lines
	vtkPolyDataReader *slReader = vtkPolyDataReader::New();
	slReader->SetFileName(STATE_LINE_FNAME);

	// transform state lines to camera focus
	vtkTransform *slTransform = vtkTransform::New();
	slTransform->Translate(LATITUDE_DISPLACEMENT, LONGITUDE_DISPLACEMENT, 0.0);
	
	vtkTransformPolyDataFilter *slFilter = vtkTransformPolyDataFilter::New();
	slFilter->SetInput(slReader->GetOutput());
	slFilter->SetTransform(slTransform);

	// create state line mapper
	vtkPolyDataMapper *slMapper = vtkPolyDataMapper::New();
	slMapper->SetInput(slFilter->GetOutput());

	// create actor
	vtkActor *slActor = vtkActor::New();
	slActor->SetMapper(slMapper);

	// add actors to renderer
	this->getRenderer()->AddVolume(volume);
	this->getRenderer()->AddActor(slActor);

	// create event handler
	this->EventHandler = new vjEventHandler();
	this->EventHandler->SetVolumeMapper(this->Mapper);
}

////////////////////////////////////////////////////////////////////////////////////////////
void vjApplication::draw()
{
	this->EventHandler->HandleUserEvents();
	this->Mapper->SetVolumeTextureLoaded(false);
	this->Mapper->SetTransferFunctionTextureLoaded(false);
	VTKApp::draw();
}

////////////////////////////////////////////////////////////////////////////////////////////
void vjApplication::initScene()
{
}