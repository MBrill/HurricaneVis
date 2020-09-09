uniform sampler3D Voxels;
uniform sampler1D TransferFunctionLookupTable;
uniform vec3  CameraPosition;
uniform float StepSize;

vec3 volExtentMin = vec3(-10.0, -9.0, 0.0);
vec3 volExtentMax = vec3( 10.0,  9.0, 3.0);

void main()
{
	vec4 value;
	float scalar;
	
	// Initialize accumulated color and opacity
	vec4 dst = vec4(0.0,0.0,0.0,0.0);
	
	// Determine volume entry point
	vec3 texPos = gl_TexCoord[0].xyz;
	vec3 volPos = gl_TexCoord[0].xyz;
		
	// Compute position in volume
	volPos.x = volPos.x - 10.0; volPos.x = volPos.x * 10.0;
	volPos.y = volPos.y -  9.0; volPos.y = volPos.y *  9.0;
	volPos.z = volPos.z *  3.0;
	
	// Compute ray direction
	vec3 direction = volPos - CameraPosition;
	direction = normalize(direction);
	
	
	// Loop for ray traversal
	for(int i = 0; i < 200; i++)
	{
		// Data access to scalar value in 3D volume texture
		value = texture3D(Voxels, texPos);
		scalar = value.r;
		
		// Apply transfer function
		vec4 src = texture1D(TransferFunctionLookupTable, scalar);
		
		// Fron-to-back compositing
		dst = (1.0 - dst.a) * src + dst;
		//dst.rgb = (1.0 - dst.a) * src.rgb * src.a + dst.rgb;
		//dst.a   = (1.0 - dst.a) * src.a   + dst.a;
		
		// Advance ray position along ray direction
		volPos = volPos + (direction * StepSize);
		texPos = texPos + (direction * StepSize);
		
		// Ray Termination
		vec3 temp1 = sign(volPos - volExtentMin);
		vec3 temp2 = sign(volExtentMax - volPos);
		float inside = dot(temp1, temp2);
		if(inside < 3.0)
			break;
	}
	
	gl_FragColor = dst;
}