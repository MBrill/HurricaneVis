#include "vtkVolumeDataReader.h"

vtkStandardNewMacro(vtkVolumeDataReader);

////////////////////////////////////////////////////////////////////////////////////////////
vtkVolumeDataReader::vtkVolumeDataReader()
{
}

////////////////////////////////////////////////////////////////////////////////////////////
vtkVolumeDataReader::~vtkVolumeDataReader()
{
}

////////////////////////////////////////////////////////////////////////////////////////////
void vtkVolumeDataReader::ReadVolumeData()
{
	// read all timesteps
	// timestep ist set to the first index, that is the first timestep. This is necessary since
	// some data sets start with timestep 0 (Lili) and some do with timestep 1 (Isabel).
	// The data is stored in the VolumeData array beginning with the index in FIRST_INDEX.
	// That means if the timesteps start at 1, the VolumeData array has no valid date in
	// VolumeData[0].

	for(int timestep = FIRST_INDEX; timestep < FIRST_INDEX + TIMESTEPS; timestep++)
	{
		// compose complete filename
		char timestepAsString[5];
		char filename[100];
		sprintf(timestepAsString, "%d", timestep);
		strcpy(filename, DATA_DIR);
		strcat(filename, FILENAME);
		strcat(filename, timestepAsString);
		strcat(filename, ENDING);

		// create data reader and set properties
		vtkImageReader2 *reader = vtkImageReader2::New();
		reader->SetFileDimensionality(3);
		reader->SetDataOrigin(0.0, 0.0, 0.0);
		reader->SetDataSpacing(1.0, 1.0, 1.0);
		reader->SetNumberOfScalarComponents(NUMBER_OF_SCALAR_COMPONENTS);
		reader->SetDataExtent(0, SIZEX-1, 0, SIZEY-1, 0, SIZEZ-1);

		// differentiate between little and big endian data
		if(LTL_ENDIAN)
			reader->SetDataByteOrderToLittleEndian();
		else
			reader->SetDataByteOrderToBigEndian();

		// differentiate between 1-byte values (unsigned char) and 4-byte values (floats)
		if(DATA_TYPE == 1)
			reader->SetDataScalarTypeToUnsignedChar();
		if(DATA_TYPE == 4)
			reader->SetDataScalarTypeToFloat();

		reader->SetFileName(filename);
		reader->Update();

		// set min and max value for the current timestep
		// if min and max values are not set in the config file use the reader's scalar
		// range otherwise use the values set in the config file
		if(MIN_VALUE == 0.0 && MAX_VALUE == 0.0)
		{
			double *scalarRange = reader->GetOutput()->GetScalarRange();
			this->MinValue[timestep] = scalarRange[0];
			this->MaxValue[timestep] = scalarRange[1];
		}
		else
		{
			this->MinValue[timestep] = MIN_VALUE;
			this->MaxValue[timestep] = MAX_VALUE;
		}

		// The data has to be converted to 8-bit [0, 255]. If it is already in 1-byte 
		// format it can be saved directly
		if(DATA_TYPE != 1)
		{
			double diff = this->MaxValue[timestep] - this->MinValue[timestep];
			double slope = 255.0/diff;
			double inter = -slope * this->MinValue[timestep];
			double shift = inter / slope;

			vtkImageShiftScale *shifter = vtkImageShiftScale::New();
			shifter->SetOutputScalarTypeToUnsignedChar();
			shifter->SetInputConnection(reader->GetOutputPort());
			shifter->ReleaseDataFlagOff();
			shifter->SetShift(shift);
			shifter->SetScale(slope);
			shifter->Update();

			// save data
			this->VolumeData[timestep] = shifter->GetOutput();
		}
		else
		{
			// save data directly
			this->VolumeData[timestep] = reader->GetOutput();
		}
	}

	// update overall min and max value
	MIN_VALUE = this->MinValue[FIRST_INDEX];
	MAX_VALUE = this->MaxValue[FIRST_INDEX];
	for(int timestep = FIRST_INDEX; timestep < FIRST_INDEX + TIMESTEPS; timestep++)
	{
		if(this->MinValue[timestep] < MIN_VALUE)
			MIN_VALUE = this->MinValue[timestep];
		if(this->MaxValue[timestep] > MAX_VALUE)
			MAX_VALUE = this->MaxValue[timestep];
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
vtkImageData *vtkVolumeDataReader::GetOutput(int timestep)
{
	return this->VolumeData[timestep];
}