#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <cmath>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "BMP.h"
#include "glslprogram.h" 
#include "Sphere.h"
#include "Setup.h"

using namespace std;

#define PI 3.1415926535


extern float Time;
extern const GLfloat Colors[8][3];
GLSLProgram* Pattern;
GLuint TexName;
extern bool  VertexShaderOn, FragmentShaderOn;

void 
MyDisplay()
{
		// draw the current object:
	Pattern->Use();
		glActiveTexture(GL_TEXTURE5); // use texture unit 5
		glBindTexture(GL_TEXTURE_2D, TexName);
		Pattern->SetUniformVariable((char*)"uTexUnit", 5); // tell your shader program you are using
		float A, B, C, D;
		A = -1 ; 
		if (VertexShaderOn)
		{
			A = Time; //fmod(Time * Time,10);

		}
		Pattern->SetUniformVariable((char*)"uA", A);
		MjbSphere(6, 400, 400);

		C = -1 ;
		if (FragmentShaderOn)
		{
			C = Time; 
		}
		Pattern->SetUniformVariable((char*)"uC", C);
	Pattern->Use(0);
}


void
setTexture()
{
	glGenTextures(1, &TexName);
	int nums, numt;
	Texture = BmpToTexture("worldtex.bmp", &nums, &numt);
	glBindTexture(GL_TEXTURE_2D, TexName);
	glEnable(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, nums, numt, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void MyInitGraphics()
{
	Pattern = new GLSLProgram();
	bool valid = Pattern->Create((char*)"proj.vert", (char*)"proj.frag");
	if (!valid)
	{
		fprintf(stderr, "Shader cannot be created!\n");
		//DoMainMenu(QUIT);
	}
	else
	{
		fprintf(stderr, "Shader created.\n");
	}
	Pattern->SetVerbose(false);

	setTexture();
}

void
InitLists()
{
	glutSetWindow(MainWindow);
	BoxList = glGenLists(1);
	glNewList(BoxList, GL_COMPILE);
	glColor3f(0.8, 0.5, 0.2);
	//glPushMatrix();
	//glEnable(GL_NORMALIZE);
	//glEnable(GL_LIGHTING);
	//float White[] = { 1.,1.,1.,1. };
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, MulArray3(1., White));
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//Texture = BmpToTexture("worldtex.bmp", &width, &height);
	//SetMaterial(1, 1, 1, 8);
	//glTranslatef(0, 2, -4.5);
	////MjbSphere(5, 200, 200);
	//glShadeModel(GL_SMOOTH);
	//makeTextureSphere();
	//glDisable(GL_TEXTURE_2D);
	//glPopMatrix();
	// create the axes:
	AxesList = glGenLists(1);
	glNewList(AxesList, GL_COMPILE);
	glLineWidth(AXES_WIDTH);
	Axes(5.);
	glLineWidth(3.);
	glEndList();
}

int
main(int argc, char* argv[])
{
	// turn on the glut package:
	// (do this before checking argc and argv since it might
	// pull some command line arguments out)

	glutInit(&argc, argv);



	// setup all the graphics stuff:

	InitGraphics();

	// create the display structures that will not change:

	InitLists();

	// init all the global variables used by Display( ):
	// this will also post a redisplay

	Reset();

	// setup all the user interface stuff:

	InitMenus();

	// draw the scene once and wait for some interaction:
	// (this will never return)

	glutSetWindow(MainWindow);
	glutMainLoop();

	// glutMainLoop( ) never returns
	// this line is here to make the compiler happy:

	return 0;
}




