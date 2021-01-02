//#include <stdio.h>
//#include <stdlib.h>
//#include <ctype.h>
//#include <cmath>
//
//
//#define _USE_MATH_DEFINES
//#include <math.h>
//
//#ifdef WIN32
//#include <windows.h>
//#pragma warning(disable:4996)
//#endif
//
//#include <GL/glew.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <GL/glut.h>
//#include "Setup.h"
//#include "Sphere.h"
//
//#define NUMCURVES 27
//#define CURVEPOINTS (NUMCURVES + 3)
//#define INTRPOINTS 30
//
//
//extern float Time;
//extern const GLfloat Colors[8][3]; 
//extern bool ShowPoint;
//
//float drawNormalFlag = 0;
//
//
//struct Point
//{
//	float x0, y0, z0; // initial coordinates
//};
//struct Curve
//{
//	float r, g, b;
//	Point* points;
//	int count;
//};
//Curve Curves[NUMCURVES]; // if you are creating a pattern of curves
//
//float CatmullRomOnX(float p0, float p1, float p2, float p3, float t)
//{
//	float res;
//	res = 0.5 * (2. * p1 + t * (p2 - p0) + t * t * (2. * p0 - 5. * p1 + 4. * p2 - p3)
//		+ t * t * t * (-1. * p0 + 3. * p1 - 3. * p2 + p3));
//	return res;
//}
//
//Point CatmullRomOnPoints(Point P[4],float t)
//{
//	Point Q;
//	Q.x0 = CatmullRomOnX(P[0].x0,P[1].x0,P[2].x0,P[3].x0,t);
//	Q.y0 = CatmullRomOnX(P[0].y0,P[1].y0,P[2].y0,P[3].y0,t);
//	Q.z0 = CatmullRomOnX(P[0].z0,P[1].z0,P[2].z0,P[3].z0,t);
//	return Q;
//}
//
//
//void catmullRom(Point P[],Point genPoints[])
//{
//	Point Q;
//	for (int i = 0; i < INTRPOINTS; i++)
//	{
//		genPoints[i] = CatmullRomOnPoints(P, (float)i / (INTRPOINTS - 1));
//	}
//}
//
//
//void catmullRomList(Point P[],Point Q[])
//{
//	Point FourP[4];
//	Point GP[INTRPOINTS];
//
//	for (int i=0; i < NUMCURVES ; i++)
//	{
//		for (int j = 0; j < 4; j++)
//		{
//			FourP[j] = P[i+j] ;
//		}
//		
//		catmullRom(FourP,GP);
//		
//		for (int k = 0; k < INTRPOINTS; k++)
//		{
//			Q[i* INTRPOINTS + k] = GP[k];
//		}
//	}
//
//}
//
//
//Point* Pond(float r)
//{
//	float theta = (float)6.28 / (NUMCURVES-1);
//	static Point P[CURVEPOINTS];
//	float z;
//	float t = Time * pow(sin(Time * 6.28),10);
//	for (int i = 1; i < (CURVEPOINTS-1); i++)
//	{
//		if (i % 2 == 0)
//		{
//			r = r + 5.5;
//			z = 4.9 * t ;
//		}
//		else
//		{
//			r = r - 5.5;
//			z = -4.9 * t;
//		}
//		P[i] = { r * cos(i * theta),r*sin(i * theta),z};
//	}
//
//	P[0] = { 0.,0.,0. };
//	P[CURVEPOINTS-1] = { 0.,0.,0. };
//	drawNormalFlag = 1;
//	return P;
//}
//
//
//
//
//void drawCurve(Point* f)
//{
//	glPushMatrix();
//		int i, j;
//		Point P[CURVEPOINTS], Q[INTRPOINTS * NUMCURVES], * q, * p;
//		p = f;
//
//
//		for (i = 0; i < CURVEPOINTS; i++)
//		{
//			P[i] = *(p + i);
//		}
//		if (ShowPoint)
//		{
//			glPointSize(5.0);
//			glColor3f(1, 0, 0);
//			glBegin(GL_POINTS);
//		}
//		for (int i = 0; i < CURVEPOINTS; i++)
//		{
//			glVertex3f(P[i].x0, P[i].y0, P[i].z0);
//		}
//		glEnd();
//
//		catmullRomList(P, Q);
//		
//
//		glBegin(GL_LINE_STRIP);
//
//		for (i = 0; i < INTRPOINTS * NUMCURVES; i++)
//		{
//			glLineWidth(26.);
//			glColor3f(0,(pow(3,Time)*sin(i))/3, 1);
//			glVertex3f(Q[i].x0, Q[i].y0, Q[i].z0);
//		}
//
//		glEnd();
//	glPopMatrix();
//}
//
//
//void MyDisplay()
//{
//
//	//glCallList(BoxList);
//
//	glEnable(GL_NORMALIZE);
//	float White[] = { 1.,1.,1.,1. };
//
//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, MulArray3(1.0f, White));
//	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
//	
//	for (int i = 0; i < 15; i++)
//	{
//		glRotatef(10, 0., 0., 0.);
//		drawCurve(Pond(9 + i * 0.8));
//	}
//
//}
//
//
//void
//InitLists()
//{
//	glutSetWindow(MainWindow);
//	BoxList = glGenLists(1);
//	//glNewList(BoxList, GL_COMPILE);
//	//glPushMatrix();
//	//glEnable(GL_NORMALIZE);
//	//glEnable(GL_LIGHTING);
//	//float White[] = { 1.,1.,1.,1. };
//	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, MulArray3(1.0f, White));
//	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
//
//
//	//glDisable(GL_LIGHTING);
//	//glPopMatrix();
//	//glEndList();
//
//	glShadeModel(GL_SMOOTH);
//	glClearColor(0.0, 0.0, 0.0, 0.0);
//
//	// create the axes:
//	AxesList = glGenLists(1);
//	glNewList(AxesList, GL_COMPILE);
//	glLineWidth(AXES_WIDTH);
//	Axes(5.);
//	glLineWidth(3.);
//	glEndList();
//}
//
//
//// main program:
//
//int
//main(int argc, char* argv[])
//{
//	// turn on the glut package:
//	// (do this before checking argc and argv since it might
//	// pull some command line arguments out)
//
//	glutInit(&argc, argv);
//
//	// setup all the graphics stuff:
//
//	InitGraphics();
//
//	// create the display structures that will not change:
//
//	InitLists();
//
//	// init all the global variables used by Display( ):
//	// this will also post a redisplay
//
//	Reset();
//
//	// setup all the user interface stuff:
//
//	InitMenus();
//
//	// draw the scene once and wait for some interaction:
//	// (this will never return)
//
//	glutSetWindow(MainWindow);
//	glutMainLoop();
//
//	// glutMainLoop( ) never returns
//	// this line is here to make the compiler happy:
//
//	return 0;
//}
//
//
//
//
//
//
