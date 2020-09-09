uniform int    frontIdx;
uniform float  LATITUDE_LEFT;
uniform float  LATITUDE_RIGHT;
uniform float  LONGITUDE_BOTTOM;
uniform float  LONGITUDE_TOP;
uniform float  sizex;
uniform float  sizey;
uniform float  sizez;
uniform vec3   viewVec;
uniform float  dPlaneStart;
uniform float  dPlaneIncr;
uniform int    orientation;

int nSequence[64] = int[64](0, 1, 4, 2, 3, 5, 6, 7,      
							1, 0, 3, 5, 4, 2, 7, 6, 
							2, 0, 6, 3, 1, 4, 7, 5, 
							3, 1, 2, 7, 5, 0, 6, 4, 
							4, 0, 5, 6, 2, 1, 7, 3, 
							5, 1, 7, 4, 0, 3, 6, 2, 
							6, 2, 4, 7, 3, 0, 5, 1, 
							7, 3, 6, 5, 1, 2, 4, 0);

int v1[24] = int[24](0, 1, 4, 4,   1, 0, 1, 4,   0, 2, 5, 5,   2, 0, 2, 5,   0, 3, 6, 6,   3, 0, 3, 6);
int v2[24] = int[24](1, 4, 7, 7,   5, 1, 4, 7,   2, 5, 7, 7,   6, 2, 5, 7,   3, 6, 7, 7,   4, 3, 6, 7);


vec3 vecVertices[8] = vec3[8]
(
	vec3(LATITUDE_LEFT,  LONGITUDE_BOTTOM, 0.0),
	vec3(LATITUDE_RIGHT, LONGITUDE_BOTTOM, 0.0),
	vec3(LATITUDE_LEFT,  LONGITUDE_TOP,    0.0),
	vec3(LATITUDE_RIGHT, LONGITUDE_TOP,    0.0),

	vec3(LATITUDE_LEFT,  LONGITUDE_BOTTOM, sizez),
	vec3(LATITUDE_RIGHT, LONGITUDE_BOTTOM, sizez),
	vec3(LATITUDE_LEFT,  LONGITUDE_TOP,    sizez),
	vec3(LATITUDE_RIGHT, LONGITUDE_TOP,    sizez)
);

void main()
{
	float dPlane = dPlaneStart + gl_Vertex.y * dPlaneIncr;
	vec3 Position = vec3(0.0, 0.0, 0.0);
	
	for(int e = 0; e < 4; e++)
	{
		// Compute indices i and j, that is the currently processed edge of the bounding box
		int vidx1 = nSequence[frontIdx * 8 + v1[int(gl_Vertex.x) * 4 + e] ];
		int vidx2 = nSequence[frontIdx * 8 + v2[int(gl_Vertex.x) * 4 + e] ];
		
		// Compute V1 and V2, the vertices of the current edge
		vec3 vecV1 = vecVertices[vidx1];
		vec3 vecV2 = vecVertices[vidx2];
		
		// Set start vector
		vec3 vecStart = vecV1;
		
		// Compute direction vector along edge
		vec3 vecDir   = vecV2 - vecV1;
		
		// Compute intersection point between edge and bounding box
		float denom  = dot(vecDir, viewVec);
		float lambda = (denom!=0.0) ? (dPlane - dot(vecStart, viewVec))/denom : -1.0;
		
		if(lambda >= 0.0 && lambda <= 1.0)
		{
			Position = vecStart + lambda * vecDir;
			break;
		}
	}
	
	gl_Position = gl_ModelViewProjectionMatrix * vec4(Position, 1.0);
	
	// Compute texture coordinate
	gl_TexCoord[0].x = (Position.x - LATITUDE_LEFT) / (LATITUDE_RIGHT - LATITUDE_LEFT);
	gl_TexCoord[0].y = (Position.y - LONGITUDE_BOTTOM) / (LONGITUDE_TOP - LONGITUDE_BOTTOM);
	gl_TexCoord[0].z = Position.z / (sizez);
	
	if(orientation == 0)
	{
		// Isabel orientation
		float tmp = gl_TexCoord[0].x;
		gl_TexCoord[0].x = 1.0 - gl_TexCoord[0].y;
		gl_TexCoord[0].y = 1.0 - tmp;
	}
	else
	{
		// Lili orientation
		gl_TexCoord[0].y = 1.0 - gl_TexCoord[0].y;
		gl_TexCoord[0].z = 1.0 - gl_TexCoord[0].z;
	}
	
	if(gl_TexCoord[0].x < 0.01)
		gl_TexCoord[0].x = 0.01;
	if(gl_TexCoord[0].x > 0.99)
		gl_TexCoord[0].x = 0.99;
		
	if(gl_TexCoord[0].y < 0.01)
		gl_TexCoord[0].y = 0.01;
	if(gl_TexCoord[0].y > 0.99)
		gl_TexCoord[0].y = 0.99;

	if(gl_TexCoord[0].z < 0.01)
		gl_TexCoord[0].z = 0.01;
	if(gl_TexCoord[0].z > 0.99)
		gl_TexCoord[0].z = 0.99;
}