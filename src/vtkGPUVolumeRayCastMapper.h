/**
 * \file   vtkGPUVolumeRayCastMapper.h
 * \author Ralph Brecheisen
 * \class  vtkGPUVolumeRayCastMapper
 * \brief  Processes volume rendering by shooting rays through the volume.
 *
 * The vtkGPUVolumeRayCastMapper inherits from the abstract class vtkGPUVolumeMapper 
 * which implements all basic functionality for volume rendering. All derived classes have
 * to implement the pure virtual function "DrawQuads" to draw the volume.
 *
 * This class is taken from the Raycasting project of Ralph Brecheisen. It was slightly
 * modified though to fit into the design of this project.
 */
#ifndef __vtkGPUVolumeRayCastMapper_h
#define __vtkGPUVolumeRayCastMapper_h

// C++ includes
#include <iostream>
using namespace std;

// user-own includes
#include "vtkGPUVolumeMapper.h"
#include "vtkShaderHandler.h"
#include "CMatrix.h"

class vtkGPUVolumeRayCastMapper : public vtkGPUVolumeMapper
{
public:
	static vtkGPUVolumeRayCastMapper *New();

	/**
	 * Sets the zero-opacity threshold. This threshold is used to determine when to stop the ray
	 * traversal. If the accumulated opacity along the ray approaches 1.0 any objects further along
	 * the ray will be occluded. In that case, further raytracing does not yield new information
	 * so it can be stopped. The zero-opacity threshold is a dynamic shader uniform
	 * and can be updated for each rendering pass.
	 */
	void  SetZeroOpacityThreshold(float threshold);

	/** Gets the zero-opacity threshold */
	float GetZeroOpacityThreshold();

	/**
	 * Sets the shading threshold. If shading is enabled you may want to increase the opacity
	 * threshold at which to apply shading. For (almost) fully transparent tissues shading will
	 * have very little visual effect so this computation may be skipped. Whether this really
	 * improves the performance is not clear since computing shading is not so difficult for a
	 * good graphics card. The shading opacity threshold is a dynamic shader uniform
	 * and can be updated for each rendering pass.
	 * \sa SetShadingEnabled()
	 */
	void SetShadingOpacityThreshold(float threshold);

	/** Gets the shading threshold */
	float GetShadingOpacityThreshold();

	/**
	 * Sets shading enabled or disabled. If this is set to false, no lighting will be applied and
	 * the transfer function color and opacity will be directly displayed. For MIP this is disabled
	 * by default. If it is enabled, it will be ignored. The shading mode is a dynamic shader uniform
	 * and can be updated for each rendering pass.
	 * \sa SetShadingOpacityThreshold()
	 */
	void SetShadingEnabled(bool enabled);

	/** Gets the shading enabled mode */
	bool IsShadingEnabled();

	/**
	 * Sets the camera position. This value is needed to pass it to the vertex shader for computing
	 * the viewing direction. Since the camera is defined in the vtkRenderWindow object this function
	 * is needed to give it to the volume raycast function. The camera position is a dynamic shader
	 * uniform that can be updated for each rendering pass.
	 */
	void SetCameraPosition(double position[3]);

	/** Gets the camera position */
	void GetCameraPosition(double position[3]);

	/** 
	 * Sets the material diffuse color. Set by default to black (0.0, 0.0, 0.0) since color is not determined
	 * by the object but by a combination of lighting and transfer function. 
	 */
	void SetDiffuseColor(float color[3]);

	/** 
	 * Sets the material diffuse color. Set by default to black (0.0, 0.0, 0.0) since color is not determined
	 * by the object but by a combination of lighting and transfer function. 
	 */
	void SetAmbientColor(float color[3]);

	/** 
	 * Sets the material diffuse color. Set by default to black (0.0, 0.0, 0.0) since color is not determined
	 * by the object but by a combination of lighting and transfer function. 
	 */
	void SetSpecularColor(float color[3]);

	/** 
	 * Sets the material diffuse coefficient. Determines how much of the diffuse color, which
	 * is a combination of material and light diffuse colors, is reflected by the object. The
	 * coefficient value should sum up to 1 together with ambient and specular coefficients.
	 */
	void SetDiffuseCoefficient(float coefficient);

	/** 
	 * Sets the material ambient coefficient. Determines how much of the ambient color, which
	 * is a combination of material and light ambient colors, is reflected by the object. Ambient
	 * color determines the color of the object in places hidden from the light source. Only
	 * diffuse shading would make these places black. The coefficient value should sum up to 1
	 * together with diffuse and specular coefficients.
	 */
	void SetAmbientCoefficient(float coefficient);

	/** 
	 * Sets the material specular coefficient. Determines how much of the specular color, which
	 * is a combination of material and light specular colors, is reflected by the object. The
	 * coefficient value should sum up to 1 together with diffuse and ambient coefficients.
	 */
	void SetSpecularCoefficient(float coefficient);

	/** 
	 * Sets the material specular strength. This value determines the area of the highlight. For
	 * high values the highlight will be very focussed and concentrated in a small spot. For low 
	 * values it will be more blurred and spread out across the surface. Default value is 10.0.
	 */
	void SetSpecularPower(float power);

	/** Gets diffuse color */
	void GetDiffuseColor(float color[3]);

	/** Gets ambient color */
	void GetAmbientColor(float color[3]);

	/** Gets specular color */
	void GetSpecularColor(float color[3]);

	/** Gets diffuse coefficient */
	float GetDiffuseCoefficient();

	/** Gets ambient coefficient */
	float GetAmbientCoefficient();

	/** Gets specular coefficient */
	float GetSpecularCoefficient();

	/** Gets specular power */
	float GetSpecularPower();

	/** Sets the number of lights */
	void SetNumberOfLights(int number);

	/** Gets the number of lights */
	int GetNumberOfLights();

	/**
	 * Sets the stepsize to be used for the raycasting. It should be specified relative to the
	 * actual volume dimensions. so, for a 256-cubed volume a sensible stepsize would be 1.0 which
	 * is about 1 step per voxel.
	 * Before being passed to the fragment shader the stepsize is automatically downscaled for a
	 * unit cube by dividing it by the maximum dimension of the volume (for a perfect cube it can
	 * be either the x-, y- or z-dimension).
	 * Furthermore, changing the stepsize will also modify the opacity correction factor. This is 
	 * needed for composite raycasting when the stepsize changes. Decreasing the stepsize means more 
	 * steps for sampling the volume resulting in a higher opacity end value when accumulating 
	 * opacities at each step. Tissues that should have been partially transparent may now
	 * become fully opaque. Increasing the stepsize may render tissues too transparent.
	 * To compensate for this effect, the opacity values from the transfer function should be adapted 
	 * together with the stepsize.
	 */
	void SetStepSize(float stepsize);

	/** 
	 * Gets the stepsize relative to actual volume dimensions.
	 * \sa SetStepSize() for internal downscaling of this parameter.
	 */
	float GetStepSize();

protected:
	vtkGPUVolumeRayCastMapper();
	~vtkGPUVolumeRayCastMapper();

	/**
	 * Renders the cube containing the volume data. First the camera position is passed to the
	 * raycast function. Then the cube is rendered with attached texture coordinates for the
	 * volume texture.
	 */
	virtual void DrawVolume(vtkRenderer *ren);

private:
	vtkShaderHandler *ShaderHandler;

	float	ZeroOpacityThreshold;
	float	ShadingOpacityThreshold;
	bool	ShadingEnabled;
	float	OpacityCorrectionFactor;
	double	CameraPosition[4];
	float	DiffuseColor  [3];
	float	AmbientColor  [3];
	float	SpecularColor [3];
	float	DiffuseCoefficient;
	float	AmbientCoefficient;
	float	SpecularCoefficient;
	float	SpecularPower;
	float	StepSize;
	int		NumberOfLights;

};

#endif

