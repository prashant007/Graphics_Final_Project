#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <cmath>
#include <string>
#include <stack> 
#include <random>
#include <iostream>


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
#include "Setup.h"


using namespace std;

#define PI 3.1415926535


extern float Time;
extern const GLfloat Colors[8][3];
extern bool ShowPoint;

extern bool Tree1, Tree1M, Tree1N, Tree2, Tree3,
            Tree4, Tree5, Tree6, Tree7, Tree3D;


struct Rule
{
	char l;
	string r;
};

struct State
{
	float x, y, z, angle, t, l;
};

struct Color
{
	float r=0., g=1., b=0.;
};


string derive(string s, int rlen, Rule R[],int n)
{
	int flag = 0;
	string snew("");
	if (n == 0)
	{
		return s;
	}
	else
	{
		for (int i = 0; i < s.length(); i++)
		{   
			flag = 0;
			for (int j = 0; j <= rlen; j++)
			{
				if (s[i] == R[j].l)
				{
					snew += R[j].r;
					flag = 1;
					break;
				}
			}
			if (flag == 0)
			{
				snew += s[i];
			}
		}
		return (derive(snew, rlen, R, n - 1));
	}  
}


void drawLine(float len)
{
	glBegin(GL_LINES);
	glVertex3f(0., 0., 0.);
	glVertex3f(len, 0, 0.);
	glEnd();
	glTranslatef(len,0., 0.);
	//glVertex3f(0., 0., len);
	//glEnd();
	//glTranslatef(0.,0.,len);
}

void leaf() {
	glPushAttrib(GL_LIGHTING_BIT);//saves current lighting stuff
		//glColor3f(0.50, 1.0, 0.0);
	GLfloat ambient[4] = { 0.50, 1.0, 0.0 };    // ambient reflection
	GLfloat specular[4] = { 0.55, 1.0, 0.0 };   // specular reflection
	GLfloat diffuse[4] = { 0.50, 0.9, 0.0 };   // diffuse reflection

	// set the ambient reflection for the object
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	// set the diffuse reflection for the object
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	// set the specular reflection for the object      
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	// set the size of the specular highlights
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20.0);

	
	//glutSolidCube(depth+1);
	glBegin(GL_TRIANGLES);
	
	glColor3f(0.98,.04,0.7);
	glVertex3f(0, 0, 0);
	glVertex3f(0.2, 0, 0.3);
	glVertex3f(0, 1, 0);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(0.88, .44, 0.7);
	glVertex3f(0, 0, 0);
	glVertex3f(-0.2, 0, -0.3);
	glVertex3f(0, 1, 0);
	glEnd();
	glPopAttrib();
}

struct State3D {
	float t, l;
};

void turtle3D(string s, float len, float delta, Color C)
{
	float x = -3, y = -3, z = 1, alpha = 90;
	float sx = -3, sy = -3, sz = 1, salpha = 90;
	bool cond1;
	char cond;
	float thickness = 8.5;
	State3D S;
	stack<State3D> St;
	
	//glBegin(GL_LINE_STRIP);
	for (int i = 0; i < s.length(); i++)
	{
		char current = s[i];
		cond = current == 'F';
		cond1 = current == 'A' || current == 'B' ||
			    current == 'C' || current == 'D';

		//glBegin(GL_LINE_STRIP);
		if (cond)
		{
			glColor3f(C.r, C.g, C.b);
			glLineWidth(thickness);
			drawLine(len);
		}
		else if (current == 'L')
		{
			leaf();
		}
		//else if (current == 'f') {
		//	x = x + len * mycos(alpha);
		//	y = y + len * mysin(alpha);
		//}
		else if (current == '+') glRotatef(delta, 0, 0, 1);
		else if (current == '-') glRotatef(-delta, 0, 0, 1);
		else if (current == '&') glRotatef(delta, 0, 1, 0);
		else if (current == '^') glRotatef(-delta, 0, 1, 0);
		else if (current == '<') glRotatef(delta, 1, 0, 0);
		else if (current == '/') glRotatef(-delta, 1, 0, 0);
		else if (current == '|') glRotatef(180, 0, 0, 1);
		else if (current == '[')
		{
			glPushMatrix();
			St.push({ thickness,len });
			thickness -= 2.;
			len -= 0.05;
		}
		else if (current == ']')
		{
			S = St.top();
			St.pop();
			thickness = S.t;
			len = S.l;
			glPopMatrix();
		}
			
	}
}


void turtle2D(string s, float len, float delta,Color C)
{   
	float x = 0, y = 0, z = 0, alpha = 90;
	float thickness = 5.3;
	bool cond1;
	State S;
	stack<State> St;
	
	for (int i = 0; i < s.length(); i++)
	{
		char current = s[i];
		cond1 = current == 'L' || current == 'R' ||
			    current == 'l' || current == 'r'  ;
		
		glLineWidth(thickness);
		if (current == 'F' || cond1)
		{   
			glBegin(GL_LINE_STRIP);
			
			glVertex3f(x, y, z);
			x = x + len * mycos(alpha);
			y = y + len * mysin(alpha);
			//z = z + len * mycos(1.1*alpha);
			glVertex3f(x, y, z);
			glEnd();
		}
		else if (cond1)
		{

		}
		else if (current == 'f') {
			x = x + len * mycos(alpha);
			y = y + len * mysin(alpha);
			z = 0;
		}
		else if (current == '+') {
			alpha = alpha + delta;
		}
		else if (current == '-') {
			alpha = alpha - delta;
		}
		else if (current == '[') {
			glColor3f(C.r, C.g, C.b);
			St.push({x,y,z,alpha,thickness,len});
			thickness = thickness - 0.45;
			//len -= 0.1 ;
		}
		else if (current == ']') {
			S = St.top();
			St.pop();
			x = S.x; y = S.y; z = S.z; alpha = S.angle; 
			thickness = S.t; len = S.l;
			glColor3f(0, 1, 0);
		}
	}
	
}


string rmvSpace(string s)
{   
	string snew = "";
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] != ' ')
			snew += s[i];
	}
	return snew;
		
}

void setupAndDraw(string s, int nrules, Rule R[], int n, float d, 
	              float delta,Color C)
{
	string fs;
	for (int i = 0; i < nrules; i++)
	{
		R[i].r = rmvSpace(R[i].r);
	}
	fs = derive(rmvSpace(s),nrules,R, n);
	turtle2D(fs, d, delta, C);
}

Color HilbertCubeColor = { 0.7,0.2,0.6 };
Color TreeColor = { 0.2,0.8,0.1 };

void setupAndDraw3D(string s, int nrules, Rule R[], int n, float d, float delta)
{
	string fs;
	for (int i = 0; i < nrules; i++)
	{
		R[i].r = rmvSpace(R[i].r);
	}
	fs = derive(rmvSpace(s), nrules, R, n);
	turtle3D(fs, d, delta, TreeColor);
}


Color DefColor;

// Page 8, Figure 1.6
void kochIsland()
{
	Rule R[1];
	R[0] = { 'F', "F - F + F + F F - F - F + F" };
	setupAndDraw("F - F - F - F",1, R, 4, 0.25, 90,DefColor);
}

// Page 10, Figure 1.9(a)
void kochIsland2()
{   
	Rule R[1];
	R[0] = { 'F', "F F - F - F - F - F - F + F" };
	setupAndDraw("F - F - F - F",1, R, 5,0.1,90,DefColor);
}

//page 12, Figure 1.11 a
void fassCurves1()
{
	Rule R[2];
	R[0] = {'L', "L+R++R-L--LL-R+" };
	R[1] = {'R', "-L+RR++R+L--L-R" };
	setupAndDraw("-L", 2, R, 4, 0.8, 60, DefColor);
}

void leafL()
{
	Rule R[2];
	R[0] = { 'L',"{−F X + X − F X −|− F X + X + F X}" };
	R[1] = { 'X',"F X" };
	setupAndDraw3D("L", 2, R, 8, 0.2, 20);
}


void tree1(int times, float len, float angle)
{
	Rule R[1];
	R[0] = { 'F', "FF+[+F-F-F]-[-F+F+F]" };
	setupAndDraw("F", 1, R, times, len, angle, {0.9,0.8,0.1});
}


void tree2(int times, float len,float angle)
{
	Rule R[2];
	R[0] = { 'X', "F[+X]F[-X]+X" };
	R[1] = { 'F', "FF" };
	setupAndDraw("X", 2, R, times,len, angle, { 0.8,0.2,0.8 });
}

void tree3(int times, float len, float angle)
{
	Rule R[2];
	R[0] = { 'X', "F[+X][-X]FX" };
	R[1] = { 'F', "FF" };
	setupAndDraw("X", 2, R, times, len, angle, { 0.8,0.2,0.8});
}

void tree4(int times, float len, float angle)
{
	Rule R[2];
	R[0] = { 'X', "F-[[X]+X]+F[+FX]-X" };
	R[1] = { 'F', "FF" };
	setupAndDraw("X", 2, R, times, len, angle, { 0.9,0.2,0.2 });
}

void tree5(int times, float len, float angle)
{
	Rule R[1];
	R[0] = { 'F', "F[+F]F[-F][F]" };
	setupAndDraw("F", 1, R, times, len, angle, {0.8,0.2,0.});
}

void tree6(int times, float len, float angle)
{
	Rule R[2];
	R[0] = { 'Y', "YFX[+Y][-Y]" };
	R[1] = { 'X', "X[-FFF][+FFF]FX" };
	setupAndDraw("Y", 2, R, times, len, angle, { 0.4,0.7,0.8 });
}

void tree7(int times, float len, float angle)
{
	Rule R[5];
	R[0] = { 'S', "[+++G][---G]TS" };
	R[1] = { 'G', "+H[-G]L" };
	R[2] = { 'H', "-G[+H]L" };
	R[3] = { 'T', "TL" };
	R[4] = { 'L', "[-FFF][+FFF]F" };

	setupAndDraw("SLFFF", 5, R, times, len, angle, { 0.2,0.7,0.7 });
}




void forest()
{   
	float ZS[7],XS[7], ycord = -43;
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist1(0.0, 10.0);
	std::uniform_real_distribution<double> dist2(-60.0, -10.0);

	float t = -60.;
	for (int i = 0 ; i < 7; ++i)
	{   
		t += 25;
		XS[i] = t + dist1(mt);
		ZS[i] = dist2(mt);
	}
		
	glColor3f(0.6, 0.7, 0);

	glPushMatrix();
	glTranslatef(XS[0], ycord, ZS[0]);
	tree1(4,1.2, 28.);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(XS[6], ycord, ZS[6]);
	tree2(9, 0.11, 20);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(XS[3], ycord, ZS[3]);
	tree3(8, 0.24, 25.7);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(XS[4], ycord, -60);
	tree5(5, 1.4, 20);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(XS[5], ycord, -90.);
	//tree4(8, 0.19, 22.7);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(XS[2], ycord, ZS[2]);
	tree6(6, 0.59, 25.7);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(XS[1], ycord, ZS[1]);
	tree7(9, 1.79, 18);
	glPopMatrix();

	//glutPostRedisplay();
}

void hilbert3D()
{
	Rule R[4];
	glColor3f(cos(2*PI*Time)/2, sin(2*PI*Time)/2,0.7);
	//glLineWidth(3);
	R[0] = { 'A',"B-F+CFC+F-D&F^D-F+&&CFC+F+B//"};
	R[1] = { 'B',"A&F^CFB^F^D^^-F-D^|F^B|FC^F^A//" };
	R[2] = { 'C',"|D^|F^B-F+C^F^A&&FA&F^C+F+B^F^D//" };
	R[3] = { 'D',"|CFB-F+B|FA&F^A&&FB-F+B|FC//" };
	setupAndDraw3D("A", 4, R, 3, 0.8, 90);
}

void tree3D()
{
	Rule R[4];
	R[0] = { 'A',"[&FL!A]/////'[&FL!A]///////'[&FL!A]" };
	R[1] = { 'F',"S/////F" };
	R[2] = { 'S',"FL" };
	R[3] = { 'L',"['''^^{-f+f+f-|-f+f+f}]" };
	setupAndDraw3D("A", 4, R, 7, 1.4, 22.5);
}



//int main() {
//	setupAndDraw();
//
//	return 0;
//}

void MyDisplay()
{

	glCallList(BoxList);

	glEnable(GL_NORMALIZE);
	float White[] = { 1.,1.,1.,1. };

	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, MulArray3(1.0f, White));
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);


	if (Tree3D)
	{
		glRotatef(90, 0., 1., 0.);
		glTranslatef(-10, -8, -5.);
		tree3D();
	}
	glTranslatef(0, -38, -20);
	glColor3f(0., 1., 0.);
	if (Tree1)  tree4(6, 0.29, 22.7);
	if (Tree1N) tree4(5, 0.49, 22.7);
	if (Tree1M) tree4(7, 0.19, 22.7);
	if (Tree2)  tree2(7, 0.14, 20);
	if (Tree3)  tree3(7, 0.28, 25.7);
	if (Tree4)  tree1(4, 1.4, 20.);
	if (Tree5)  tree5(5, 0.9, 20);
	if (Tree6) 	tree6(6, 0.49, 25.7);
	if (Tree7) 	tree7(9, 1.79, 18);
}


void
InitLists()
{
	glutSetWindow(MainWindow);

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	BoxList = glGenLists(1);
		glNewList(BoxList, GL_COMPILE);
		//glRotatef(90, 0., 0., 1.);
		//glTranslatef(-25., -30.,-10.);
		//tree3D();
		//hilbert3D();
		//someFun();
		//forest();
		

		//glTranslatef(0.,0, -7);
		//glColor3f(0.9,0.4,0.2);
		//kochIsland();
		//kochIsland2();
		//fassCurves1();
	glEndList();

	// create the axes:
	//AxesList = glGenLists(1);
	//glNewList(AxesList, GL_COMPILE);
	//glLineWidth(AXES_WIDTH);
	//Axes(5.);
	//glLineWidth(3.);
	//glEndList();
}


// main program:

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






