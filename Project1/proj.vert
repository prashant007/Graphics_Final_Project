#version 330 compatibility


uniform float uTime;		// "Time", from Animate( )
uniform float uA, uB;
out vec2 vST;		// texture coords
out vec3 vert;

/*
out vec3 vN; // normal vector
out vec3 vL; // vector from point to light
out vec3 vE; // vector from point to eye
*/

const float PI = 	3.14159265;
const float expn = 2.71828 ;
const float AMP = 	0.2;		// amplitude
const float W = 	2.;		// frequency


void
main( )
{ 
    
	vST = gl_MultiTexCoord0.st;
	vert = gl_Vertex.xyz;
	if (uA > 0)
	{
    vert.x = vert.x * 0.7 * cos(vert.x*uA);
	vert.y = vert.y * 0.7 * cos(vert.y*uA);
	vert.z = vert.z * 0.7 * cos(vert.z*uA);
	} 
	gl_Position = gl_ModelViewProjectionMatrix * vec4( vert, 1. );
}