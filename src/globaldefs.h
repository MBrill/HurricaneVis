/**
 * \file   globaldefs.h
 * \author Michael Berberich
 * \brief  global definitions and variables
 *
 * The variables in this header file are needed in most classes of this application.
 * Their values are constant during the entire runtime of the program and are loaded
 * by a vtkConfigFileReader instance only ONCE in the very beginning of the program 
 * before any other action is processed. Therefore these variables are defined as 
 * global variables in order to avoid a complex distribution to all classes.
 */
#ifndef __globaldefs_h
#define __globaldefs_h

/** 
 * global variables
 */
extern char   DATA_DIR[256];
extern char   FILENAME[256];
extern int    FIRST_INDEX;
extern int    TIMESTEPS;
extern char   ENDING[8];
extern bool   LTL_ENDIAN;
extern int    DATA_TYPE;
extern int    SIZEX;
extern int    SIZEY;
extern int    SIZEZ;
extern double MIN_VALUE;
extern double MAX_VALUE;
extern double LATITUDE_LEFT;
extern double LATITUDE_RIGHT;
extern double LONGITUDE_BOTTOM;
extern double LONGITUDE_TOP;
extern double LATITUDE_DISPLACEMENT;
extern double LONGITUDE_DISPLACEMENT;
extern int	  ORIENTATION;
extern char   TF_NAME[256];
extern char   DESCRIPTION[256];
extern char   UNITS[256];

/** 
 * global definitions
 */
#define VRJUGGLER_APP 0
#define MAX_TIMESTEPS 100
#define TF_TABLE_SIZE 256
#define NUM_SLICES_OBJECT_ALIGNED 100
#define SLICE_INC_VIEW_ALIGNED 0.1
#define DEFAULT_HEIGHT 4.0
#define NUMBER_OF_SCALAR_COMPONENTS 1
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 600
#define STATE_LINE_FNAME                                "../data/statelines/ALLstatelines.vtk"
#define VERTEX_SHADER_3D_TEXTURE_MAPPING_OBJECT_ALIGNED "../shaders/3D_texture_mapping/3DTexMapObjectAligned.vert"
#define VERTEX_SHADER_3D_TEXTURE_MAPPING_VIEW_ALIGNED   "../shaders/3D_texture_mapping/3DTexMapViewAligned.vert"
#define FRAGMENT_SHADER_3D_TEXTURE_MAPPING              "../shaders/3D_texture_mapping/3DTexMap.frag"
#define VERTEX_SHADER_RAY_CASTING                       "../shaders/ray_casting/ray_casting.vert"
#define FRAGMENT_SHADER_RAY_CASTING                     "../shaders/ray_casting/ray_casting.frag"

#endif