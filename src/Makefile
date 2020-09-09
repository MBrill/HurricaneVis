CC = g++

VTK_DIR = /usr/local/vtk

VTK_INC_DIR = -I${VTK_DIR}/include/vtk-5.0

VTK_LIB_DIR = -L${VTK_DIR}/lib -lvtkCommon -lvtkFiltering -lvtkGraphics -lvtkImaging -lvtkRendering -lvtksys -lvtkIO -lvtkftgl -lvtkfreetype -lvtkVolumeRendering -lvtkWidgets
                                                                                
VRJ_INC_DIR = -I/usr/local/vrjuggler/include/vrjuggler-2.2  \
			-I/usr/local/vrjuggler/include/gadgeteer-1.2 -I/usr/local/vrjuggler/include/jccl-1.2 \
			-I/usr/local/vrjuggler/include/vpr-2.0 -I/usr/local/vrjuggler/boost/include/boost-1_34_1 \
			-I/usr/local/vrjuggler/cppdom/include/cppdom-0.7.10 \
			-I/usr/local/vrjuggler/gmtl/include/gmtl-0.5.4 -I/usr/local/vrjuggler/include/sonix-1.2
                                                                                
VRJ_LIB_DIR = -L$(VJ_BASE_DIR)/lib -L/usr/X11R6/lib -L/usr/local/vrjuggler/boost/lib \
			-L/usr/local/vrjuggler/cppdom/lib -lvrj-2_2 -Wl,-Bdynamic -lSM -lICE -lX11 -lm -lgadget-1_2 -ljccl-1_2 -lvrj_ogl-2_2 \
			-lvpr-2_0 -pthread -ldl -lboost_signals-gcc33-mt-1_34_1 -lboost_filesystem-gcc33-mt-1_34_1 -lcppdom-0_7_10 -lsonix-1_2

VJVTK_INC_DIR = -I/gri/general/michaelb/hurricane_vis_current_version/vjVTK

VJVTK_LIB_DIR = -L/gri/general/michaelb/hurricane_vis_current_version/vjVTK -lvjVTK

ADD_INC_DIR = -I/gri/general/michaelb/hurricane_vis_current_version/include

ADD_LIB_DIR = -L/gri/general/michaelb/hurricane_vis_current_version/lib -lGLEW

CFLAGS = -g -DNO_NUMERIC_LIMITS 

INCLUDES = -I. ${VTK_INC_DIR} ${VRJ_INC_DIR} ${VJVTK_INC_DIR} ${ADD_INC_DIR}

LIBS = -lglut ${VTK_LIB_DIR} ${VRJ_LIB_DIR} ${VJVTK_LIB_DIR} ${ADD_LIB_DIR}

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

OBJS = CMatrix.o CPosition.o CVector.o main.o vjApplication.o vjEventHandler.o vtkApplication.o \
vtkConfigFileReader.o vtkEventHandler.o vtkGPUVolumeMapper.o vtkGPUVolumeRayCastMapper.o  \
vtkGPUVolumeTextureMapper3D.o vtkShaderHandler.o vtkShaderSourceReader.o vtkShaderUniform.o \
vtkTransferFunctionReader.o vtkTrapezoid.o vtkVolumeDataReader.o

hurricaneVIS: $(OBJS)
	$(CC) -g -o ../bin/hurricaneVIS $(LIBS) $(OBJS)

clean:
	rm -f hurricaneVIS
	rm -f $(OBJS)
