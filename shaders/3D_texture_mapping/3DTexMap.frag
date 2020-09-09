uniform sampler3D Voxels;
uniform sampler1D TransferFunctionLookupTable;

uniform int orientation;
uniform float offsetBottom;
uniform float offsetTop;

vec4  sample_coord;
vec4  sample_value;
vec4  final_color;

void main()
{
	// Get current position in texture
	sample_coord = gl_TexCoord[0];
	
	// check if current fragment is within the bounds of the volume to draw
	if((orientation == 0 && (sample_coord.z > offsetBottom && sample_coord.z < 1.0 - offsetTop)) || (orientation == 1 && (sample_coord.z > offsetTop && sample_coord.z < 1.0 - offsetBottom)))
	{
		// Data access to scalar value in 3D volume texture
		sample_value = texture3D(Voxels, sample_coord.xyz);

		// Apply transfer function
		final_color = texture1D(TransferFunctionLookupTable, sample_value.r);
			
		// Assign final color
		gl_FragColor = final_color;
	}
	else
	{
		// Assign full transpareny if fragment is out of bounds
		gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
	}
}