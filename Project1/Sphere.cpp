#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>



struct point {
	float x, y, z;		// coordinates
	float nx, ny, nz;	// surface normal
	float s, t;		// texture coords
};

int		NumLngs, NumLats;
struct point* Pts;

struct point*
	PtsPointer(int lat, int lng)
{
	if (lat < 0)	lat += (NumLats - 1);
	if (lng < 0)	lng += (NumLngs - 1);
	if (lat > NumLats - 1)	lat -= (NumLats - 1);
	if (lng > NumLngs - 1)	lng -= (NumLngs - 1);
	return &Pts[NumLngs * lat + lng];
}



void
DrawPoint(struct point* p)
{
	glNormal3f(p->nx, p->ny, p->nz);
	glTexCoord2f(p->s, p->t);
	glVertex3f(p->x, p->y, p->z);
}

#define M_PI 3.14159265358979323846

void
MjbSphere(float radius, int slices, int stacks)
{
	struct point top, bot;		// top, bottom points
	struct point* p;

	// set the globals:

	NumLngs = slices;
	NumLats = stacks;

	if (NumLngs < 3)
		NumLngs = 3;

	if (NumLats < 3)
		NumLats = 3;

	
	// allocate the point data structure:

	Pts = new struct point[NumLngs * NumLats];


	// fill the Pts structure:

	for (int ilat = 0; ilat < NumLats; ilat++)
	{
		float lat = -M_PI / 2. + M_PI * (float)ilat / (float)(NumLats - 1);
		float xz = cos(lat);
		float y = sin(lat);
		for (int ilng = 0; ilng < NumLngs; ilng++)
		{
			float lng = -M_PI + 2. * M_PI * (float)ilng / (float)(NumLngs - 1);
			float x = xz * cos(lng);
			float z = -xz * sin(lng);
			float theta, phi, valt,valp,v1,v2;
			p = PtsPointer(ilat, ilng);
			p->x = radius * x;
			p->y = radius * y;
			p->z = radius * z;
			p->nx = x;
			p->ny = y;
			p->nz = z;
			p->s = (lng + M_PI) / (2. * M_PI);
			p->t = (lat + M_PI / 2.) / M_PI;
			
		}
	}

	top.x = 0.;		top.y = radius;	 top.z = 0.;
	top.nx = 0.;	top.ny = 1.;     top.nz = 0.;
	top.s = 0.;		top.t = 1.;

	bot.x = 0.;		bot.y = -radius;	bot.z = 0.;
	bot.nx = 0.;	bot.ny = -1.;		bot.nz = 0.;
	bot.s = 0.;		bot.t = 0.;


	// connect the north pole to the latitude NumLats-2:

	glBegin(GL_QUADS);
	for (int ilng = 0; ilng < NumLngs - 1; ilng++)
	{
		p = PtsPointer(NumLats - 1, ilng);
		DrawPoint(p);

		p = PtsPointer(NumLats - 2, ilng);
		DrawPoint(p);

		p = PtsPointer(NumLats - 2, ilng + 1);
		DrawPoint(p);

		p = PtsPointer(NumLats - 1, ilng + 1);
		DrawPoint(p);
	}
	glEnd();

	// connect the south pole to the latitude 1:

	glBegin(GL_QUADS);
	for (int ilng = 0; ilng < NumLngs - 1; ilng++)
	{
		p = PtsPointer(0, ilng);
		DrawPoint(p);

		p = PtsPointer(0, ilng + 1);
		DrawPoint(p);

		p = PtsPointer(1, ilng + 1);
		DrawPoint(p);

		p = PtsPointer(1, ilng);
		DrawPoint(p);
	}
	glEnd();


	// connect the other 4-sided polygons:

	glBegin(GL_QUADS);
	for (int ilat = 2; ilat < NumLats - 1; ilat++)
	{
		for (int ilng = 0; ilng < NumLngs - 1; ilng++)
		{
			p = PtsPointer(ilat - 1, ilng);
			DrawPoint(p);

			p = PtsPointer(ilat - 1, ilng + 1);
			DrawPoint(p);

			p = PtsPointer(ilat, ilng + 1);
			DrawPoint(p);

			p = PtsPointer(ilat, ilng);
			DrawPoint(p);
		}
	}
	glEnd();

	delete[] Pts;
	Pts = NULL;
}

// ================================================================================================
// This is the lighting part

float White[] = { 1.,1.,1.,1. };
// utility to create an array from 3 separate values:
float*
Array3(float a, float b, float c)
{
	static float array[4];
	array[0] = a;
	array[1] = b;
	array[2] = c;
	array[3] = 1.;
	return array;
}
// utility to create an array from a multiplier and an array:
float*
MulArray3(float factor, float array0[3])
{
	static float array[4];
	array[0] = factor * array0[0];
	array[1] = factor * array0[1];
	array[2] = factor * array0[2];
	array[3] = 1.;
	return array;
}

//
//void
//SetMaterial(float r, float g, float b, float shininess)
//{
//	glMaterialfv(GL_BACK, GL_EMISSION, Array3(0., 0., 0.));
//	glMaterialfv(GL_BACK, GL_AMBIENT, MulArray3(.1f, White));
//	glMaterialfv(GL_BACK, GL_DIFFUSE, MulArray3(1., White));
//	glMaterialfv(GL_BACK, GL_SPECULAR, Array3(0., 0., 0.));
//	glMaterialf(GL_BACK, GL_SHININESS, 1);
//
//	glMaterialfv(GL_FRONT, GL_EMISSION, Array3(0., 0., 0.));
//	glMaterialfv(GL_FRONT, GL_AMBIENT, Array3(r, g, b));
//	glMaterialfv(GL_FRONT, GL_DIFFUSE, Array3(r, g, b));
//	glMaterialfv(GL_FRONT, GL_SPECULAR, MulArray3(.8f, White));
//	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
//}



void
SetMaterial(float r, float g, float b, float shininess)
{
	glMaterialfv(GL_BACK, GL_EMISSION, Array3(0., 0., 0.));
	glMaterialfv(GL_BACK, GL_AMBIENT, MulArray3(1., White));
	glMaterialfv(GL_BACK, GL_DIFFUSE, MulArray3(1., White));
	glMaterialfv(GL_BACK, GL_SPECULAR, Array3(0., 0., 0.));
	glMaterialf(GL_BACK, GL_SHININESS, 1);

	glMaterialfv(GL_FRONT, GL_EMISSION, Array3(0., 0., 0.));
	glMaterialfv(GL_FRONT, GL_AMBIENT, Array3(r, g, b));
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Array3(r, g, b));
	glMaterialfv(GL_FRONT, GL_SPECULAR, MulArray3(.8f, White));
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}



void
SetBlob(float r, float g, float b)
{
	glMaterialfv(GL_BACK, GL_EMISSION, Array3(r, g, b));
	glMaterialfv(GL_BACK, GL_AMBIENT, Array3(r, g, b));
	glMaterialfv(GL_BACK, GL_DIFFUSE, Array3(r, g, b));
	glMaterialfv(GL_BACK, GL_SPECULAR, Array3(r, g, b));
	glMaterialf(GL_BACK, GL_SHININESS, 1);

	glMaterialfv(GL_FRONT, GL_EMISSION, Array3(r, g, b));
	glMaterialfv(GL_FRONT, GL_AMBIENT, Array3(r, g, b));
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Array3(r, g, b));
	glMaterialfv(GL_FRONT, GL_SPECULAR, Array3(r, g, b));
	glMaterialf(GL_FRONT, GL_SHININESS, 1);
}



void
SetPointLight(int ilight, float x, float y, float z, float r, float g, float b)
{
	glLightfv(ilight, GL_POSITION, Array3(x, y, z));
	glLightfv(ilight, GL_AMBIENT, Array3(0., 0., 0.));
	glLightfv(ilight, GL_DIFFUSE, Array3(r, g, b));
	glLightfv(ilight, GL_SPECULAR, Array3(r, g, b));

	glLightf(ilight, GL_CONSTANT_ATTENUATION, 1.);
	glLightf(ilight, GL_LINEAR_ATTENUATION, 0.);
	glLightf(ilight, GL_QUADRATIC_ATTENUATION, 0.);
	glEnable(ilight);
}

void
SetSpotLight(int ilight, float x, float y, float z, float xdir, float ydir, float zdir, float r, float g, float b)
{
	glLightfv(ilight, GL_POSITION, Array3(x, y, z));
	glLightfv(ilight, GL_SPOT_DIRECTION, Array3(xdir, ydir, zdir));
	glLightf(ilight, GL_SPOT_EXPONENT, 0.);
	glLightf(ilight, GL_SPOT_CUTOFF, 10.);

	glLightfv(ilight, GL_AMBIENT, Array3(0., 0., 0.));
	glLightfv(ilight, GL_DIFFUSE, Array3(r, g, b));
	glLightfv(ilight, GL_SPECULAR, Array3(r, g, b));

	glLightf(ilight, GL_CONSTANT_ATTENUATION, 1.);
	glLightf(ilight, GL_LINEAR_ATTENUATION, 0.);
	glLightf(ilight, GL_QUADRATIC_ATTENUATION, 0.);
	glEnable(ilight);
}

// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//    glColor3fv( rgb );

void
HsvRgb(float hsv[3], float rgb[3])
{
	// guarantee valid input:

	float h = hsv[0] / 60.f;
	while (h >= 6.)   h -= 6.;
	while (h < 0.)    h += 6.;

	float s = hsv[1];
	if (s < 0.)
		s = 0.;
	if (s > 1.)
		s = 1.;

	float v = hsv[2];
	if (v < 0.)
		v = 0.;
	if (v > 1.)
		v = 1.;

	// if sat==0, then is a gray:

	if (s == 0.0)
	{
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}

	// get an rgb from the hue itself:

	float i = (float)floor(h);
	float f = h - i;
	float p = v * (1.f - s);
	float q = v * (1.f - s * f);
	float t = v * (1.f - (s * (1.f - f)));

	float r = 0., g = 0., b = 0.;       // red, green, blue
	switch ((int)i)
	{
	case 0:
		r = v;   g = t;   b = p;
		break;

	case 1:
		r = q;   g = v;   b = p;
		break;

	case 2:
		r = p;   g = v;   b = t;
		break;

	case 3:
		r = p;   g = q;   b = v;
		break;

	case 4:
		r = t;   g = p;   b = v;
		break;

	case 5:
		r = v;   g = p;   b = q;
		break;
	}


	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}


///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////


// the stroke characters 'X' 'Y' 'Z' :

static float xx[] = {
	  0.f, 1.f, 0.f, 1.f
};

static float xy[] = {
	  -.5f, .5f, .5f, -.5f
};

static int xorder[] = {
	  1, 2, -3, 4
};

static float yx[] = {
	  0.f, 0.f, -.5f, .5f
};

static float yy[] = {
	  0.f, .6f, 1.f, 1.f
};

static int yorder[] = {
	  1, 2, 3, -2, 4
};

static float zx[] = {
	  1.f, 0.f, 1.f, 0.f, .25f, .75f
};

static float zy[] = {
	  .5f, .5f, -.5f, -.5f, 0.f, 0.f
};

static int zorder[] = {
	  1, 2, 3, 4, -5, 6
};

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

// Draw a set of 3D axes:
// (length is the axis length in world coordinates)

void
Axes(float length)
{
	glBegin(GL_LINE_STRIP);
	glVertex3f(length, 0., 0.);
	glVertex3f(0., 0., 0.);
	glVertex3f(0., length, 0.);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(0., 0., 0.);
	glVertex3f(0., 0., length);
	glEnd();

	float fact = LENFRAC * length;
	float base = BASEFRAC * length;

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 4; i++)
	{
		int j = xorder[i];
		if (j < 0)
		{

			glEnd();
			glBegin(GL_LINE_STRIP);
			j = -j;
		}
		j--;
		glVertex3f(base + fact * xx[j], fact * xy[j], 0.0);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 5; i++)
	{
		int j = yorder[i];
		if (j < 0)
		{

			glEnd();
			glBegin(GL_LINE_STRIP);
			j = -j;
		}
		j--;
		glVertex3f(fact * yx[j], base + fact * yy[j], 0.0);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 6; i++)
	{
		int j = zorder[i];
		if (j < 0)
		{

			glEnd();
			glBegin(GL_LINE_STRIP);
			j = -j;
		}
		j--;
		glVertex3f(0.0, fact * zy[j], base + fact * zx[j]);
	}
	glEnd();

}
