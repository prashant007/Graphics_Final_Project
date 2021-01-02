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
#include "Sphere.h"
#include "Setup.h"
#include "glslprogram.h" 

#define PI 3.1415926535

int      ActiveButton;        // current button that is down
GLuint   AxesList;            // list to hold the axes
int      AxesOn;              // != 0 means to draw the axes
int      DebugOn;          // != 0 means to print debugging info
int      DepthCueOn;          // != 0 means to use intensity depth cueing
int      DepthBufferOn;       // != 0 means to use the z-buffer
int      DepthFightingOn;     // != 0 means to force the creation of z-fighting
GLuint   BoxList;          // object display list
int      MainWindow;          // window id for main graphics window
float    Scale;               // scaling factor
int      ShadowsOn;           // != 0 means to turn shadows on
int      WhichColor;          // index into Colors[ ]
int      WhichProjection;     // ORTHO or PERSP
int      Xmouse, Ymouse;         // mouse values
float    Xrot, Yrot;          // rotation angles in degrees
int      width, height;          // width and height of the textures
bool     Freeze;
bool     ShowPoint;
unsigned const char* Texture;
bool  Distort;    // global -- true means to distort the texture

bool Tree1 = false, Tree1M = false, Tree1N = false, 
     Tree2 = false, Tree3  = false, Tree4 = false,
     Tree5 = false, Tree6  = false, Tree7 = false,
     Tree3D = true;

// minimum allowable scale factor:
const float MINSCALE = { 0.05f };
const GLfloat AXES_WIDTH = { 5. };

bool FragmentShaderOn, VertexShaderOn;

const GLfloat Colors[][3] =
{
   { 1., 0., 0. },      // red
   { 1., 1., 0. },      // yellow
   { 0., 1., 0. },      // green
   { 0., 1., 1. },      // cyan
   { 0., 0., 1. },      // blue
   { 1., 0., 1. },      // magenta
   { 1., 1., 1. },      // white
   { 0., 0., 0. },      // black
};

// which projection:

enum Projections
{
    ORTHO,
    PERSP
};



// This is a sample OpenGL / GLUT program
//
// The objective is to draw a 3d object and change the color of the axes
//    with a glut menu
//
// The left mouse button does rotation
// The middle mouse button does scaling
// The user interface allows:
//    1. The axes to be turned on and off
//    2. The color of the axes to be changed
//    3. Debugging to be turned on and off
//    4. Depth cueing to be turned on and off
//    5. The projection to be changed
//    6. The transformations to be reset
//    7. The program to quit
//
// Author:        Joe Graphics

// NOTE: There are a lot of good reasons to use const variables instead
// of #define's.  However, Visual C++ does not allow a const variable
// to be used as an array size or as the case in a switch( ) statement.  So in
// the following, all constants are const variables except those which need to
// be array sizes or cases in switch( ) statements.  Those are #defines.

// title of these windows:

const char* WINDOWTITLE = { "Prashant Kumar - Assignment 3" };
const char* GLUITITLE = { "User Interface Window" };

// what the glui package defines as true and false:

const int GLUITRUE = { true };
const int GLUIFALSE = { false };

// the escape key:

#define ESCAPE    0x1b

// initial window size:

const int INIT_WINDOW_SIZE = { 1400 };

// size of the 3d box:

const float BOXSIZE = { 2.f };

// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };



// scroll wheel button values:

const int SCROLL_WHEEL_UP = { 3 };
const int SCROLL_WHEEL_DOWN = { 4 };

// equivalent mouse movement when we click a the scroll wheel:

const float SCROLL_WHEEL_CLICK_FACTOR = { 5. };

// active mouse buttons (or them together):

const int LEFT = { 4 };
const int MIDDLE = { 2 };
const int RIGHT = { 1 };




// which button:

enum ButtonVals
{
    RESET,
    QUIT
};

// window background color (rgba):

const GLfloat BACKCOLOR[] = { 0., 0., 0., 1. };

enum Colors
{
    RED,
    YELLOW,
    GREEN,
    CYAN,
    BLUE,
    MAGENTA,
    WHITE,
    BLACK
};

// the color numbers:
// this order must match the radio button order


const char* ColorNames[] =
{
   "Red",
   "Yellow",
   "Green",
   "Cyan",
   "Blue",
   "Magenta",
   "White",
   "Black"
};

// the color definitions:
// this order must match the menu order



// fog parameters:

const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum  FOGMODE = { GL_LINEAR };
const GLfloat FOGDENSITY = { 0.30f };
const GLfloat FOGSTART = { 1.5 };
const GLfloat FOGEND = { 4. };


// what options should we compile-in?
// in general, you don't need to worry about these
// i compile these in to show class examples of things going wrong


//#define DEMO_DEPTH_BUFFER

// should we turn the shadows on?

//#define ENABLE_SHADOWS



// non-constant global variables:


// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutMainLoop( ) do it

#define MS_IN_THE_ANIMATION_CYCLE   5000
float Time;
void
Animate()
{
    // put animation stuff in here -- change some global variables
    // for Display( ) to find:

    int ms = glutGet(GLUT_ELAPSED_TIME);   // milliseconds
    ms %= MS_IN_THE_ANIMATION_CYCLE;
    Time = (float)ms / (float)MS_IN_THE_ANIMATION_CYCLE;
    // force a call to Display( ) next time it is convenient:

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}


// draw the complete scene:




void
DoAxesMenu(int id)
{
    AxesOn = id;

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}


void
makeTextureSphere()
{

    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    MjbSphere(2, 140, 140);
}

void
DoColorMenu(int id)
{
    WhichColor = id - RED;

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}


void
DoDebugMenu(int id)
{
    DebugOn = id;

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}




// main menu callback:

void
DoMainMenu(int id)
{
    switch (id)
    {
    case RESET:
        Reset();
        break;

    case QUIT:
        // gracefully close out the graphics:
        // gracefully close the graphics window:
        // gracefully exit the program:
        glutSetWindow(MainWindow);
        glFinish();
        glutDestroyWindow(MainWindow);
        exit(0);
        break;

    default:
        fprintf(stderr, "Don't know what to do with Main Menu ID %d\n", id);
    }

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}


void
DoProjectMenu(int id)
{
    WhichProjection = id;

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}


void
DoShadowsMenu(int id)
{
    ShadowsOn = id;

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}


// use glut to display a string of characters using a raster font:

void
DoRasterString(float x, float y, float z, const char* s)
{
    glRasterPos3f((GLfloat)x, (GLfloat)y, (GLfloat)z);

    char c;        // one character to print
    for (; (c = *s) != '\0'; s++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
}




// return the number of seconds since the start of the program:

float
ElapsedSeconds()
{
    // get # of milliseconds since the start of the program:

    int ms = glutGet(GLUT_ELAPSED_TIME);

    // convert it to seconds:

    return (float)ms / 1000.f;
}


// initialize the glui window:

void
InitMenus()
{
    glutSetWindow(MainWindow);

    int numColors = sizeof(Colors) / (3 * sizeof(int));
    int colormenu = glutCreateMenu(DoColorMenu);
    for (int i = 0; i < numColors; i++)
    {
        glutAddMenuEntry(ColorNames[i], i);
    }

    int axesmenu = glutCreateMenu(DoAxesMenu);
    glutAddMenuEntry("Off", 0);
    glutAddMenuEntry("On", 1);

    int debugmenu = glutCreateMenu(DoDebugMenu);
    glutAddMenuEntry("Off", 0);
    glutAddMenuEntry("On", 1);

    int projmenu = glutCreateMenu(DoProjectMenu);
    glutAddMenuEntry("Orthographic", ORTHO);
    glutAddMenuEntry("Perspective", PERSP);

    int shadowsmenu = glutCreateMenu(DoShadowsMenu);
    glutAddMenuEntry("Off", 0);
    glutAddMenuEntry("On", 1);

    int depthcuemenu = glutCreateMenu(DoDepthMenu);
    glutAddMenuEntry("Off", 0);
    glutAddMenuEntry("On", 1);

    int depthbuffermenu = glutCreateMenu(DoDepthBufferMenu);
    glutAddMenuEntry("Off", 0);
    glutAddMenuEntry("On", 1);

    int mainmenu = glutCreateMenu(DoMainMenu);
    glutAddSubMenu("Axes", axesmenu);
    glutAddSubMenu("Colors", colormenu);

#ifdef DEMO_DEPTH_BUFFER
    glutAddSubMenu("Depth Buffer", depthbuffermenu);
#endif

#ifdef DEMO_Z_FIGHTING
    glutAddSubMenu("Depth Fighting", depthfightingmenu);
#endif

    glutAddSubMenu("Depth Cue", depthcuemenu);
    glutAddSubMenu("Projection", projmenu);


#ifdef ENABLE_SHADOWS
    glutAddSubMenu("Shadows", shadowsmenu);
#endif

    glutAddMenuEntry("Reset", RESET);
    glutAddSubMenu("Debug", debugmenu);
    glutAddMenuEntry("Quit", QUIT);

    // attach the pop-up menu to the right mouse button:

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void
DoDepthBufferMenu(int id)
{
    DepthBufferOn = id;

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}

void
DoDepthMenu(int id)
{
    DepthCueOn = id;

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}


// initialize the glut and OpenGL libraries:
// also setup display lists and callback functions

void
InitGraphics()
{
    // request the display modes:
    // ask for red-green-blue-alpha color, double-buffering, and z-buffering:

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    // set the initial window configuration:

    glutInitWindowPosition(0, 0);
    glutInitWindowSize(INIT_WINDOW_SIZE, INIT_WINDOW_SIZE);

    // open the window and set its title:

    MainWindow = glutCreateWindow(WINDOWTITLE);
    glutSetWindowTitle(WINDOWTITLE);

    // set the framebuffer clear values:

    glClearColor(BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3]);

    // setup the callback functions:
    // DisplayFunc -- redraw the window
    // ReshapeFunc -- handle the user resizing the window
    // KeyboardFunc -- handle a keyboard input
    // MouseFunc -- handle the mouse button going down or up
    // MotionFunc -- handle the mouse moving with a button down
    // PassiveMotionFunc -- handle the mouse moving with a button up
    // VisibilityFunc -- handle a change in window visibility
    // EntryFunc   -- handle the cursor entering or leaving the window
    // SpecialFunc -- handle special keys on the keyboard
    // SpaceballMotionFunc -- handle spaceball translation
    // SpaceballRotateFunc -- handle spaceball rotation
    // SpaceballButtonFunc -- handle spaceball button hits
    // ButtonBoxFunc -- handle button box hits
    // DialsFunc -- handle dial rotations
    // TabletMotionFunc -- handle digitizing tablet motion
    // TabletButtonFunc -- handle digitizing tablet button hits
    // MenuStateFunc -- declare when a pop-up menu is in use
    // TimerFunc -- trigger something to happen a certain time from now
    // IdleFunc -- what to do when nothing else is going on

    glutSetWindow(MainWindow);
    glutDisplayFunc(Display);
    glutReshapeFunc(Resize);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(MouseButton);
    glutMotionFunc(MouseMotion);
    glutPassiveMotionFunc(NULL);
    glutVisibilityFunc(Visibility);
    glutEntryFunc(NULL);
    glutSpecialFunc(NULL);
    glutSpaceballMotionFunc(NULL);
    glutSpaceballRotateFunc(NULL);
    glutSpaceballButtonFunc(NULL);
    glutButtonBoxFunc(NULL);
    glutDialsFunc(NULL);
    glutTabletMotionFunc(NULL);
    glutTabletButtonFunc(NULL);
    glutMenuStateFunc(NULL);
    glutTimerFunc(-1, NULL, 0);
    glutIdleFunc(Animate);

    // init glew (a window must be open to do this):

#ifdef WIN32
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        fprintf(stderr, "glewInit Error\n");
    }
    else
        fprintf(stderr, "GLEW initialized OK\n");
    fprintf(stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

    MyInitGraphics();

}

// the keyboard callback:

void
Keyboard(unsigned char c, int x, int y)
{
    if (DebugOn != 0)
        fprintf(stderr, "Keyboard: '%c' (0x%0x)\n", c, c);

    switch (c)
    {
    case 'o':
    case 'O':
        WhichProjection = ORTHO;
        break;

    case 'p':
    case 'P':
        WhichProjection = PERSP;
        break;

    case 'S':
    case 's':
        ShowPoint = !ShowPoint;
        break;

    case 'f':
        Freeze = !Freeze;
        if (Freeze)
            glutIdleFunc(NULL);
        else
            glutIdleFunc(Animate);
        break;
    
    case 'F':
        FragmentShaderOn = !FragmentShaderOn;
        break;
    case 'V':
        VertexShaderOn = !VertexShaderOn;
        break;
    case 'b':
        FragmentShaderOn = true;
        VertexShaderOn = true;
        break;


    case 'q':
    case 'Q':
    case ESCAPE:
        DoMainMenu(QUIT); // will not return here
        break;            // happy compiler

    default:
        fprintf(stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c);
    }

    // force a call to Display( ):

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}


// called when the mouse button transitions down or up:

void
MouseButton(int button, int state, int x, int y)
{
    int b = 0;        // LEFT, MIDDLE, or RIGHT

    if (DebugOn != 0)
        fprintf(stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y);

    // get the proper button bit mask:

    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        b = LEFT;      break;

    case GLUT_MIDDLE_BUTTON:
        b = MIDDLE;    break;

    case GLUT_RIGHT_BUTTON:
        b = RIGHT;     break;

    case SCROLL_WHEEL_UP:
        Scale += SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
        // keep object from turning inside-out or disappearing:
        if (Scale < MINSCALE)
            Scale = MINSCALE;
        break;

    case SCROLL_WHEEL_DOWN:
        Scale -= SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
        // keep object from turning inside-out or disappearing:
        if (Scale < MINSCALE)
            Scale = MINSCALE;
        break;

    default:
        b = 0;
        fprintf(stderr, "Unknown mouse button: %d\n", button);
    }

    // button down sets the bit, up clears the bit:

    if (state == GLUT_DOWN)
    {
        Xmouse = x;
        Ymouse = y;
        ActiveButton |= b;      // set the proper bit
    }
    else
    {
        ActiveButton &= ~b;     // clear the proper bit
    }

    glutSetWindow(MainWindow);
    glutPostRedisplay();

}


// called when the mouse moves while a button is down:

void
MouseMotion(int x, int y)
{
    if (DebugOn != 0)
        fprintf(stderr, "MouseMotion: %d, %d\n", x, y);

    //if (WhichHeliMode == OUTSIDE)
    //{
    int dx = x - Xmouse;    // change in mouse coords
    int dy = y - Ymouse;

    if ((ActiveButton & LEFT) != 0)
    {
        Xrot += (ANGFACT * dy);
        Yrot += (ANGFACT * dx);
    }


    if ((ActiveButton & MIDDLE) != 0)
    {
        Scale += SCLFACT * (float)(dx - dy);

        // keep object from turning inside-out or disappearing:

        if (Scale < MINSCALE)
            Scale = MINSCALE;
    }

    Xmouse = x;       // new current position
    Ymouse = y;
    //}

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}


// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene

void
Reset()
{
    ActiveButton = 0;
    AxesOn = 1;
    DebugOn = 0;
    DepthCueOn = 0;
    Scale = 1.0;
    ShadowsOn = 0;
    WhichColor = WHITE;
    WhichProjection = PERSP;
    Freeze = false;
    Xrot = Yrot = 0.;
    FragmentShaderOn = false;
    VertexShaderOn = false;
}


// called when user resizes the window:

void
Resize(int width, int height)
{
    if (DebugOn != 0)
        fprintf(stderr, "ReSize: %d, %d\n", width, height);

    // don't really need to do anything since window size is
    // checked each time in Display( ):

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}


// handle a change to the window's visibility:

void
Visibility(int state)
{
    if (DebugOn != 0)
        fprintf(stderr, "Visibility: %d\n", state);

    if (state == GLUT_VISIBLE)
    {
        glutSetWindow(MainWindow);
        glutPostRedisplay();
    }
    else
    {
        // could optimize by keeping track of the fact
        // that the window is not visible and avoid
        // animating or redrawing it ...
    }
}

// draw the complete scene:

void
Display()
{
    if (DebugOn != 0)
    {
        fprintf(stderr, "Display\n");
    }


    // set which window we want to do the graphics into:

    glutSetWindow(MainWindow);


    // erase the background:

    glDrawBuffer(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
#ifdef DEMO_DEPTH_BUFFER
    if (DepthBufferOn == 0)
        glDisable(GL_DEPTH_TEST);
#endif


    // specify shading to be flat:

    glShadeModel(GL_SMOOTH);


    // set the viewport to a square centered in the window:

    GLsizei vx = glutGet(GLUT_WINDOW_WIDTH);
    GLsizei vy = glutGet(GLUT_WINDOW_HEIGHT);
    GLsizei v = vx < vy ? vx : vy;			// minimum dimension
    GLint xl = (vx - v) / 2;
    GLint yb = (vy - v) / 2;
    glViewport(xl, yb, v, v);
    //glViewport(0, 0, vx, vy);



    // set the viewing volume:
    // remember that the Z clipping  values are actually
    // given as DISTANCES IN FRONT OF THE EYE
    // USE gluOrtho2D( ) IF YOU ARE DOING 2D !

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (WhichProjection == ORTHO)
        glOrtho(-3., 3., -3., 3., 0.1, 1000.);
    else
        gluPerspective(90., 1., 0.1, 1000.);


    // place the objects into the scene:

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    // set the eye position, look-at position, and up-vector:

    gluLookAt(0., 0., 3., 0., 0., 0., 0., 1., 0.);


    // rotate the scene:

    glRotatef((GLfloat)Yrot, 0., 1., 0.);
    glRotatef((GLfloat)Xrot, 1., 0., 0.);


    // uniformly scale the scene:

    if (Scale < MINSCALE)
        Scale = MINSCALE;
    glScalef((GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale);


    // set the fog parameters:

    if (DepthCueOn != 0)
    {
        glFogi(GL_FOG_MODE, FOGMODE);
        glFogfv(GL_FOG_COLOR, FOGCOLOR);
        glFogf(GL_FOG_DENSITY, FOGDENSITY);
        glFogf(GL_FOG_START, FOGSTART);
        glFogf(GL_FOG_END, FOGEND);
        glEnable(GL_FOG);
    }
    else
    {
        glDisable(GL_FOG);
    }

    glColor3f(1, 1, 0);
    if (AxesOn != 0)
    {
        glColor3fv(&Colors[WhichColor][0]);
        glCallList(AxesList);
    }


    // since we are using glScalef( ), be sure normals get unitized:

    // draw the current object:
    MyDisplay();

#ifdef DEMO_Z_FIGHTING
    if (DepthFightingOn != 0)
    {
        glPushMatrix();
        glRotatef(90., 0., 1., 0.);
        glCallList(BoxList);
        glPopMatrix();
    }
#endif

    // swap the double-buffered framebuffers:

    glutSwapBuffers();


    // be sure the graphics buffer has been sent:
    // note: be sure to use glFlush( ) here, not glFinish( ) !

    glFlush();
}



float mysin(float theta)
{
    if (theta == 30 || theta == 150)
    {
        return 0.5;
    }
    else if (theta == 45 || theta == 135)
    {
        return 0.707;
    }
    else if (theta == 60 || theta == 120)
    {
        return 0.866;
    }
    else if (theta == 0 || theta == 180 || theta == 360)
    {
        return 0.;
    }
    else if (theta == 90)
    {
        return 1;

    }
    else if (theta == 210 || theta == 330)
    {
        return -0.5;
    }
    else if (theta == 225 || theta == 315)
    {
        return -0.707;
    }
    else if (theta == 240 || theta == 300)
    {
        return -0.866;
    }
    else if (theta == 270)
    {
        return -1.;
    }
    else
    {
        return (sin(theta * PI / 180.));
    }
}


float mycos(float theta)
{
    if (theta == 0 || theta == 360)
    {
        return 1.;
    }
    else if (theta == 30 || theta == 330)
    {
        return 0.866;
    }
    else if (theta == 45 || theta == 315)
    {
        return 0.707;
    }
    else if (theta == 60 || theta == 300)
    {
        return 0.5;
    }
    else if (theta == 90 || theta == 270)
    {
        return 0.;
    }
    else if (theta == 150 || theta == 210)
    {
        return -0.866;
    }
    else if (theta == 135 || theta == 225)
    {
        return -0.707;
    }
    else if (theta == 120 || theta == 240)
    {
        return -0.5;
    }
    else
    {
        return (cos(theta * PI / 180.));
    }
}

