#include "vtkApplication.h"

////////////////////////////////////////////////////////////////////////////////////////////
vtkApplication::vtkApplication()
{
	// create window
	vtkRenderWindow *window = vtkRenderWindow::New();
	vtkRenderWindowInteractor *interactor = vtkRenderWindowInteractor::New();
	window->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	interactor->SetRenderWindow(window);

	// create renderer
	vtkRenderer *renderer = vtkRenderer::New();
	window->AddRenderer(renderer);

	// read volume data
	vtkVolumeDataReader *volumeReader = vtkVolumeDataReader::New();
	volumeReader->ReadVolumeData();

	// read transfer function
	vtkTransferFunctionReader *tfReader = vtkTransferFunctionReader::New();
	tfReader->ReadTransferFunction();

	// create volume texture mapper
	vtkGPUVolumeTextureMapper3D *mapper = vtkGPUVolumeTextureMapper3D::New();
	//vtkGPUVolumeRayCastMapper *mapper = vtkGPUVolumeRayCastMapper::New();
	mapper->SetInput(volumeReader->GetOutput(FIRST_INDEX));
	mapper->SetVolumeDataReader(volumeReader);
	mapper->SetTransferFunctionReader(tfReader);
	tfReader->SetMapper(mapper);

	// create volume
	vtkVolume *volume = vtkVolume::New();
	volume->SetMapper(mapper);

	// read state lines
	vtkPolyDataReader *slReader = vtkPolyDataReader::New();
	slReader->SetFileName(STATE_LINE_FNAME);

	// transform state lines to camera focus
	vtkTransform *sl2Transform = vtkTransform::New();
	sl2Transform->Translate(LATITUDE_DISPLACEMENT, LONGITUDE_DISPLACEMENT, 0.0);
	
	vtkTransformPolyDataFilter *slFilter = vtkTransformPolyDataFilter::New();
	slFilter->SetInput(slReader->GetOutput());
	slFilter->SetTransform(sl2Transform);

	// create state line mapper
	vtkPolyDataMapper *slMapper = vtkPolyDataMapper::New();
	slMapper->SetInput(slFilter->GetOutput());

	// create actor
	vtkActor *slActor = vtkActor::New();
	slActor->SetMapper(slMapper);

	// create event handler
	vtkEventHandler *style = vtkEventHandler::New();
	style->PrintUserManual();
	style->SetVolumeMapper(mapper);
	style->SetVolumeReader(volumeReader);
	style->SetTransferFunctionReader(tfReader);
	interactor->SetInteractorStyle(style);

	// create camera
	vtkCamera *camera = vtkCamera::New();
    camera->SetPosition(0, 0, 80);
    camera->SetFocalPoint(0, 0, 0);
    camera->ComputeViewPlaneNormal();

	// set renderer properties
	renderer->AddVolume(volume);
	renderer->AddActor(slActor);
	renderer->SetActiveCamera(camera);
	renderer->SetBackground(0.0, 0.0, 0.0);

	// start application
	interactor->Initialize();
	interactor->Start();
}

////////////////////////////////////////////////////////////////////////////////////////////
vtkApplication::~vtkApplication()
{
}
