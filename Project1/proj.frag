#version 330 compatibility

//uniform sampler uTexUnit;	
in vec2 vST;		// texture coords
in vec3 vert;
uniform float uC;
uniform sampler2D uTexUnit;


void
main( )
{
	vec3 myColor = texture (uTexUnit,vST).rgb;
	float x , y, z;
	if (uC > 0)
	{
		x = abs(vST.s * cos(vert.x*4*uC)) ;
		y = abs(vST.s * cos(vert.y*4*uC)) ;
		z = sqrt(x) + sqrt(y) ;
		if ( z >= 1)
		{
		  myColor.r = 0.8 + abs(cos(vert.x*uC))/5 ;
		  myColor.g = 0.1 + abs(cos(vert.y*uC))/3 ;
		  myColor.b = 0.1 + abs(cos(vert.z*uC))/2 ;
		}
		else 
		{
		 myColor = texture (uTexUnit,vST).rgb;
		}
	}
	gl_FragColor = vec4( myColor,  1 );
	
}