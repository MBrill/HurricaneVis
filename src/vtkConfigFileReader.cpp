#include "vtkConfigFileReader.h"

vtkStandardNewMacro(vtkConfigFileReader);

////////////////////////////////////////////////////////////////////////////////////////////

// definition of global variables; see globaldefs.h
char   DATA_DIR[256];
char   FILENAME[256];
int    FIRST_INDEX;
int    TIMESTEPS;
char   ENDING[8];
bool   LTL_ENDIAN;
int    DATA_TYPE;
int    SIZEX;
int    SIZEY;
int    SIZEZ;
double MIN_VALUE;
double MAX_VALUE;
double LATITUDE_LEFT;
double LATITUDE_RIGHT;
double LONGITUDE_BOTTOM;
double LONGITUDE_TOP;
int    ORIENTATION;
char   TF_NAME[256];
char   DESCRIPTION[256];
char   UNITS[256];
double LATITUDE_DISPLACEMENT;
double LONGITUDE_DISPLACEMENT;

///////////////////////////////////////////////////////////////////////////////////////////////////
vtkConfigFileReader::vtkConfigFileReader()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
vtkConfigFileReader::~vtkConfigFileReader()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkConfigFileReader::ReadConfigFile(const char *filename)
{
	char line[256];
	char *ptr;
	ifstream configFile(filename);
  
	// open the file
	if(!configFile.is_open())
	{
		cerr << "Could not open config file " << filename;
	}
	else
	{
		// read the data
		configFile.getline(line, 256);
		ptr = strstr(line, "=") + 2;
		strcpy(DATA_DIR, ptr);

		configFile.getline(line, 256);
		ptr = strstr(line, "=") + 2;
		strcpy(FILENAME, ptr);

		configFile.getline(line, 256);
		ptr = strstr(line, "=") + 2;
		FIRST_INDEX = atoi(ptr);

		configFile.getline(line, 256);
		ptr = strstr(line, "=") + 2;
		TIMESTEPS = atoi(ptr);

		configFile.getline(line, 256);
		ptr = strstr(line, "=") + 2;
		strcpy(ENDING, ptr);

		configFile.getline(line, 256);
		ptr = strstr(line, "=") + 2;
		LTL_ENDIAN = atoi(ptr);

		configFile.getline(line, 256);
		ptr = strstr(line, "=") + 2;
		DATA_TYPE = atoi(ptr);		

		configFile.getline(line, 256);
		ptr = strstr(line, "=") + 2;
		SIZEX = atoi(ptr);

		configFile.getline(line, 256);
		ptr = strstr(line, "=") + 2;
		SIZEY = atoi(ptr);

		configFile.getline(line, 256);
		ptr = strstr(line, "=") + 2;
		SIZEZ = atoi(ptr);

		configFile.getline(line, 256);
		ptr = strstr(line, "=") + 2;
		MIN_VALUE = atof(ptr);

		configFile.getline(line, 256);
		ptr = strstr(line, "=") + 2;
		MAX_VALUE = atof(ptr);

		configFile.getline(line, 256);
		ptr = strstr(line, "=") + 2;
		LATITUDE_LEFT = atof(ptr);

		configFile.getline(line, 256);
		ptr = strstr(line, "=") + 2;
		LATITUDE_RIGHT = atof(ptr);

		configFile.getline(line, 256);
		ptr = strstr(line, "=") + 2;
		LONGITUDE_BOTTOM = atof(ptr);

		configFile.getline(line, 256);
		ptr = strstr(line, "=") + 2;
		LONGITUDE_TOP = atof(ptr);

		configFile.getline(line, 256);
		ptr = strstr(line, "=") + 2;
		ORIENTATION = atoi(ptr);

		configFile.getline(line, 256);
		ptr = strstr(line, "=") + 2;
		strcpy(TF_NAME, ptr);

		configFile.getline(line, 256);
		ptr = strstr(line, "=") + 2;
		strcpy(DESCRIPTION, ptr);

		configFile.getline(line, 256);
		ptr = strstr(line, "=") + 2;
		strcpy(UNITS, ptr);

		// close the file
		configFile.close();

		// compute the latitude and longitude displacement to set the center of the hurricane
		// to the origin of the world coordinate system
		LATITUDE_DISPLACEMENT = -1.0 * LATITUDE_RIGHT + (LATITUDE_RIGHT - LATITUDE_LEFT) / 2.0;
		LONGITUDE_DISPLACEMENT = (LONGITUDE_BOTTOM + (LONGITUDE_TOP - LONGITUDE_BOTTOM) / 2.0) * -1.0;
	}
}