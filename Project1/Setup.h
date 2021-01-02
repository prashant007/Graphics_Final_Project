#pragma once

extern int      ActiveButton;        // current button that is down
extern GLuint   AxesList;            // list to hold the axes
extern int      AxesOn;              // != 0 means to draw the axes
extern int      DebugOn;          // != 0 means to print debugging info
extern int      DepthCueOn;          // != 0 means to use intensity depth cueing
extern int      DepthBufferOn;       // != 0 means to use the z-buffer
extern int      DepthFightingOn;     // != 0 means to force the creation of z-fighting
extern GLuint   BoxList;          // object display list
extern int      MainWindow;          // window id for main graphics window
extern float Scale;               // scaling factor
extern int      ShadowsOn;           // != 0 means to turn shadows on
extern int      WhichColor;          // index into Colors[ ]
extern int      WhichProjection;     // ORTHO or PERSP
extern int      Xmouse, Ymouse;         // mouse values
extern float    Xrot, Yrot;          // rotation angles in degrees
extern int     width, height;          // width and height of the textures
extern unsigned const char* Texture;
extern bool ShowPoint;
extern bool  Distort;    // global -- true means to distort the texture

extern const float MINSCALE;
extern const float AXES_WIDTH;
extern const GLfloat Colors[8][3];

extern bool  VertexShaderOn, FragmentShaderOn;

extern const GLfloat FOGCOLOR[4];
extern const GLenum  FOGMODE;
extern const GLfloat FOGDENSITY;
extern const GLfloat FOGSTART;
extern const GLfloat FOGEND ;

// function prototypes:
void  Animate();
void MyInitGraphics();
void  MyDisplay();
void  Display();
void  DoAxesMenu(int);
void  makeTextureSphere();
void  DoColorMenu(int);
void  DoDepthBufferMenu(int);
//void  DoDepthFightingMenu(int);
void  DoDepthMenu(int);
void  DoDebugMenu(int);
void  DoMainMenu(int);
void  DoProjectMenu(int);
//void  DoShadowMenu();
void  DoRasterString(float, float, float, const char*);
float ElapsedSeconds();
void  InitGraphics();
void  InitLists();
void  InitMenus();
void  Keyboard(unsigned char, int, int);
void  MouseButton(int, int, int, int);
void  MouseMotion(int, int);
void  Reset();
void  Resize(int, int);
void  Visibility(int);
float mysin(float);
float mycos(float);
