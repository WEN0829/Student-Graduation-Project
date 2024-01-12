/*---------------------------------------------------------------------------------
 * This file contains a program, drawing & animating a wind-mill.
 * This program is used to teach LCS and scene graph concepts.
 *
 * by S. K. Ueng, Nov.  2006
 */
#include <stdio.h>
#include <math.h>

#include <GL/glut.h>

unsigned char circle[64][64][4];

 /*----Define normals of faces ----*/
float normal[][4] = { {0.0,  0.0,  -1.0},
                     {0.0,  -1.0, 0.0},
                     {1.0,  0.0,  0.0},
                     {0.0,  0.0,  1.0},
                     {0.0,  1.0,  0.0},
                     {-1.0, 0.0,  0.0} };
/*----Define light source properties -------*/
float lit_position[] = { 10.0, 14.0, 0.0, 0.0 };
float lit_direction[] = { -1.0, -1.0, 1.0, 0.0 };
float lit_diffuse[] = { 0.8, 0.4, 0.4, 1.0 };
float lit_specular[] = { 0.7, 0.7, 0.7, 1.0 };
float lit_cutoff = 60.0;
float lit_exponent = 8.0;

float lit1_position[] = { -0.5, 10.0, -2.0, 1.0 };
float lit1_diffuse[] = { 0.7, 0.7, 0.0, 1.0 };

float global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

/*----Some global variables for transformation------*/
float lit_angle = 0.0;

float mat_specular[] = { 0.9, 0.9, 0.9, 1.0 };

/*-----Define a unit box--------*/
/* Vertices of the box */
float  points[][3] = { {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5},
                      {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},
                      {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
                      {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5} };
/* face of box, each face composing of 4 vertices */
int    face[][4] = { {0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5},
                    {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3} };
/* indices of the box faces */
int    cube[6] = { 0, 1, 2, 3, 4, 5 };

/*-Declare GLU quadric objects, sphere, cylinder, and disk --*/
GLUquadricObj* sphere = NULL, * cylind = NULL, * disk;


/*-----Translation and rotations of eye coordinate system---*/
float   eyeDx = 0.0, eyeDy = 0.0, eyeDz = 0.0;
float   eyeAngx = 0.0, eyeAngy = 0.0, eyeAngz = 0.0;
double  Eye[3] = { 0.0, 0.0, 30.0 }, Focus[3] = { 0.0, 0.0, 0.0 },
Vup[3] = { 0.0, 1.0, 0.0 };

float   u[3][3] = { {1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0} };
float   eye[3];
float   cv, sv; /* cos(5.0) and sin(5.0) */

/*---Define fog color----*/
float fog_color[] = { 0.15, 0.20, 0.20, 0.50 };
float fog_color2[] = { 0.8, 0.8, 0.8, 0.50 };

/*-----Drawing stye
 0:4-windows, 1:x direction, 2:y direction, 3:z-dirtection, 4:perspective
 */
int    style = 0;
void draw_scene(void);
float norm2(float v[]);

#define  PI   3.141592653

#define Step  0.2

#define TSIZE 64 /* define texture dimension */
#define ON 1
#define OFF 0

/*---- the axes of billboard ----*/
float a[3], b[3];
/*-----Create image space for textures -----*/
unsigned char checkboard[TSIZE][TSIZE][4]; /* checkboard textures */
unsigned char cloud[TSIZE][TSIZE][4]; /* checkboard textures */
unsigned char brick[TSIZE][TSIZE][4];      /* brick wall textures */
unsigned char tow[TSIZE][TSIZE][4];      /* brick wall textures */
unsigned int textName[6];                  /* declare two texture maps*/
float ds = 0.0, dt = 0.0;
float textAng = 0.0;
// Texture mapping status
int status = ON;
float mtx[16]; /* the modelview matrix */
float mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
float cube_angle = 0.0;
float dens = 0.15;

int top_m;

/*-----Define window size----*/
int width = 800, height = 600;

//Global variables recording rotation angles of H-arm and blades.
float  arm_ang = 45.0, bld_ang = 10.0;
float  ver_x = 5.0, ver_y = 0.0, ver_z = 6.0; //define heli's coordi
float  ang_x = 0.0, ang_y = -90.0, ang_z = 0.0; //define heli's rotate_ang

void Create_Texture_Circle() {
    int y, x, i, temp;
    for (y = 0; y < 64; y++) {
        for (x = 0; x < 64; x++) {
            circle[y][x][0] = 100;
            circle[y][x][1] = 100;
            circle[y][x][2] = 100;
            circle[y][x][3] = 255;
        }
    }
    for (y = 16, x = 0; y < 48; y++) {
        if (y <= 20 && y >= 17)
            x = x + 2;
        else if (y == 22 || y == 25 || y == 31)
            x = x + 1;
        else if (y == 42 || y == 39 || y == 33)
            x = x - 1;
        else if (y <= 47 && y >= 44)
            x = x - 2;
        temp = x;
        for (x = 0; x < temp; x++) {
            circle[y][x][0] = 150;
            circle[y][x][1] = 150;
            circle[y][x][2] = 150;
            circle[y][x][3] = 255;
        }
        for (x = 64 - temp; x < 64; x++) {
            circle[y][x][0] = 150;
            circle[y][x][1] = 150;
            circle[y][x][2] = 150;
            circle[y][x][3] = 255;
        }
        x = temp;
    }

    for (x = 16, y = 0; x < 48; x++) {
        if (x <= 20 && x >= 17)
            y = y + 2;
        else if (x == 22 || x == 25 || x == 31)
            y = y + 1;
        else if (x == 42 || x == 39 || x == 33)
            y = y - 1;
        else if (x <= 47 && x >= 44)
            y = y - 2;
        temp = y;
        for (y = 0; y < temp; y++) {
            circle[y][x][0] = 200;
            circle[y][x][1] = 200;
            circle[y][x][2] = 200;
            circle[y][x][3] = 255;
        }
        for (y = 64 - temp; y < 64; y++) {
            circle[y][x][0] = 200;
            circle[y][x][1] = 200;
            circle[y][x][2] = 200;
            circle[y][x][3] = 255;
        }
        y = temp;
    }
    for (i = 0; i < 61; i++) {
        for (y = i; y < 4 + i; y++) {
            for (x = i; x < 4 + i; x++) {
                circle[y][x][0] = 255;
                circle[y][x][1] = 255;
                circle[y][x][2] = 255;
                circle[y][x][3] = 255;
            }
        }
    }
    for (i = 0; i < 61; i++) {
        for (y = i; y < 4 + i; y++) {
            for (x = 60 - i; x < 64 - i; x++) {
                circle[y][x][0] = 255;
                circle[y][x][1] = 255;
                circle[y][x][2] = 255;
                circle[y][x][3] = 255;
            }
        }
    }
}

/*----------------------------------------------------------
 * Procedure to make a checkboard texture.
 */
void make_check() {
    int i, j, c;

    for (i = 0; i < TSIZE; i++)
        for (j = 0; j < TSIZE; j++) {
            c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
            checkboard[i][j][0] = c;
            checkboard[i][j][1] = c;
            checkboard[i][j][2] = c;
            checkboard[i][j][3] = 255;
        }
}

/*----------------------------------------------------------
 * Procedure to make a checkboard texture.
 */
void make_check2() {
    int i, j, c;

    for (i = 0; i < TSIZE; i++)
        for (j = 0; j < TSIZE; j++) {
            if (j > i / 2 && j <= 64 - i / 2)
                c = 255;
            else
                c = 0;
            checkboard[i][j][0] = c / 8;
            checkboard[i][j][1] = c / 2;
            checkboard[i][j][2] = c / 4;
            if (c == 255)
                checkboard[i][j][3] = 255;
            else
                checkboard[i][j][3] = 0;
        }
    // Generate trunk
    for (i = 0; i < TSIZE / 3; i++) {
        for (j = 0; j < TSIZE / 2 - 4; j++)
            checkboard[i][j][3] = 0;
        for (j = TSIZE / 2 + 4; j < TSIZE; j++)
            checkboard[i][j][3] = 0;
    }
}

void make_cloud() {
    int i, j, c;

    for (i = 0; i < 32; i++)
        for (j = 0; j < TSIZE; j++) {
            if (j > i / 2 && j <= 64 - i / 2)
                c = 255;
            else
                c = 0;
            cloud[i][j][0] = 255;//c / 8;
            cloud[i][j][1] = 255;//c / 2;
            cloud[i][j][2] = 255;//c / 4;
            if (c == 255)
                cloud[i][j][3] = 255;
            else
                cloud[i][j][3] = 0;
        }
}

/*----------------------------------------------------------
 * Procedure to maKe a brick wall texture map
 */
void brick_pattern() {
    int y, x, i;

    for (y = 0; y < 12; y++) { /*----Create the first row of bricks----*/
        for (x = 0; x < 2; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 2; x < 30; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 30; x < 34; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 34; x < 62; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 62; x < 64; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }
    }

    for (y = 12; y < 16; y++) /*--Create the cement between two breick layers */
        for (x = 0; x < 64; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

    for (y = 16; y < 28; y++) { /*--Create the 2nd layer of bricks---*/
        for (x = 0; x < 14; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 14; x < 18; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 18; x < 46; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 46; x < 50; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 50; x < 64; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }
    }

    for (y = 28; y < 32; y++) /*---Add another layer of cement----*/
        for (x = 0; x < 64; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

    for (y = 32; y < 64; y++)
        for (x = 0; x < 64; x++)
            for (i = 0; i < 4; i++)
                brick[y][x][i] = brick[y - 32][x][i];
}

void draw_tow3() {
    int i, j, c;

    for (i = 0; i < TSIZE; i++)
        for (j = 0; j < TSIZE; j++) {
            if (j > i / 2 && j <= 64 - i / 2)
                c = 255;
            else
                c = 0;
            tow[i][j][0] = 255;//c / 8;
            tow[i][j][1] = 193;//c / 2;
            tow[i][j][2] = 37;//c / 4;
            if (c == 255)
                tow[i][j][3] = 255;
            else
                tow[i][j][3] = 0;
        }
}

/*-------------------------------------------------------
 * Procedure to compute the a[] and b[] axes of billboard
 * after eye parameters are specified.
 */
void compute_ab_axes(void) {
    float w0, w2;
    double len;

    /*----Get w0 and w2 from the modelview matrix mtx[] ---*/
    w0 = mtx[2];
    w2 = mtx[10];

    len = sqrt(w0 * w0 + w2 * w2);
    /*---- Define the a and b axes for billboards ----*/
    b[0] = 0.0;
    b[1] = 1.0;
    b[2] = 0.0;
    a[0] = w2 / len;
    a[1] = 0.0;
    a[2] = -w0 / len;
}

void setup_light() {

    glEnable(GL_LIGHT0);      /*Turn on light0 */
    glEnable(GL_LIGHT1);      /*Turn on light1 */
    /*-----Define light0 ---------*/
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, lit_cutoff);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, lit_exponent);

    /*-----Define light1 ---------*/
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lit_specular);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient); /*global ambient*/
}

/*----------------------------------------------------------
 * Procedure to initialize the working environment.
 */
void  myinit()
{
    glClearColor(fog_color[0], fog_color[1], fog_color[2], fog_color[3]);      /*set the background color BLACK */
                     /*Clear the Depth & Color Buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    /*---Create quadratic objects---*/
    if (sphere == NULL) {
        sphere = gluNewQuadric();
        gluQuadricDrawStyle(sphere, GLU_FILL);
        gluQuadricNormals(sphere, GLU_SMOOTH);
    }
    if (cylind == NULL) {
        cylind = gluNewQuadric();
        gluQuadricDrawStyle(cylind, GLU_FILL);
        gluQuadricNormals(cylind, GLU_SMOOTH);
    }
    if (disk == NULL) {
        disk = gluNewQuadric();
        gluQuadricDrawStyle(disk, GLU_FILL);
        gluQuadricNormals(disk, GLU_SMOOTH);
    }

    /*---- Compute cos(5.0) and sin(5.0) ----*/
    cv = cos(5.0 * PI / 180.0);
    sv = sin(5.0 * PI / 180.0);
    /*---- Copy eye position ---*/
    eye[0] = Eye[0];
    eye[1] = Eye[1];
    eye[2] = Eye[2];

    //glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);  /*Enable depth buffer for shading computing */
    glEnable(GL_NORMALIZE);   /*Enable mornalization  */

    glEnable(GL_LIGHTING);    /*Enable lighting effects */

    /*----Set up fog conditions ---*/
    glEnable(GL_FOG);                /*enable fog fade */
    glFogi(GL_FOG_MODE, GL_LINEAR);  /*fog factor=GL_LINEAR,GL_EXP,or GL_EXP2*/
    glFogf(GL_FOG_DENSITY, 0.15);    /*fog opacity(density)= 0.25*/
    glFogf(GL_FOG_START, 1.0);       /*Setup two ends for GL_LINEAR*/
    glFogf(GL_FOG_END, 36.0);
    glFogfv(GL_FOG_COLOR, fog_color);/*set the fog color */
    


    /*-----Define some global lighting status -----*/
    setup_light();
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); /* local viewer */

    /*-----Enable face culling -----*/
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glFlush();

    /*-----Create Texture -----*/
    make_check();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glGenTextures(4, textName);
    glBindTexture(GL_TEXTURE_2D, textName[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkboard);

    /*-----Create another texture ----*/
    brick_pattern();
    glBindTexture(GL_TEXTURE_2D, textName[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, brick);

    Create_Texture_Circle();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, textName[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, circle);

    make_check2();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, textName[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkboard);

    make_cloud();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, textName[4]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, cloud);

    draw_tow3();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, textName[5]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, tow);
}

/*--------------------------------------------------------
 * Procedure to draw a billboard, center=[x,z], width=w,
 * height = h;
 */
void draw_billboard(float x, float z, float w, float h) {
    float v0[3], v1[3], v2[3], v3[3];

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    /*----Compute the 4 vertices of the billboard ----*/
    v0[0] = x - (w / 2) * a[0];
    v0[1] = 0.0;
    v0[2] = z - (w / 2) * a[2];
    v1[0] = x + (w / 2) * a[0];
    v1[1] = 0.0;
    v1[2] = z + (w / 2) * a[2];
    v2[0] = x + (w / 2) * a[0];
    v2[1] = h;
    v2[2] = z + (w / 2) * a[2];
    v3[0] = x - (w / 2) * a[0];
    v3[1] = h;
    v3[2] = z - (w / 2) * a[2];

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(v0);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(v1);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(v2);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(v3);
    glEnd();
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}

/*-----------------------------------------------------
 * Draw a sphere
 */
void draw_cylinder() {
    static GLUquadricObj* cylind = NULL;

    if (cylind == NULL) {
        cylind = gluNewQuadric();
        gluQuadricDrawStyle(cylind, GLU_FILL);
        gluQuadricNormals(cylind, GLU_SMOOTH);
    }
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textName[1]);
    gluQuadricTexture(cylind, GL_TRUE);
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluCylinder(cylind, 1.0, 1.0, 8.0, 12, 3);
    glPopMatrix();
}


/*-------------------------------------------------------
 * Procedure to draw a polygon as a blade of the windmill
 */
void draw_blade()
{
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 4.0, 0.0);
    glVertex3f(1.0, 8.0, 0.0);
    glVertex3f(-1.0, 8.0, 0.0);
    glVertex3f(-1.0, 4.0, 0.0);
    glEnd();
}

/*--------------------------------------------------------
 * Procedure to draw a cube. The geometrical data of the cube
 * are defined above.
 */
void draw_cube()
{
    int    i;
    float range;

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    for (i = 0;i < 6;i++) {
        range = 1.0;
        glNormal3fv(normal[i]);
        glBegin(GL_POLYGON); /* Draw the face */
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(points[face[i][0]]);
        glTexCoord2f(0.0, range);
        glVertex3fv(points[face[i][1]]);
        glTexCoord2f(range, range);
        glVertex3fv(points[face[i][2]]);
        glTexCoord2f(range, 0.0);
        glVertex3fv(points[face[i][3]]);
        glEnd();
    }
}

/*---------------------------------------------------------
 * Procedure to draw the floor.
 */
void draw_floor()
{
    float flr_color[] = { 0.7, 0.0, 0.7, 1.0 };
    float flr_color2[] = { 0.5, 0.5, 0.5, 1.0 };
    float flr_color3[] = { 0.2, 0.7, 0.5, 1.0 };
    float flr_color4[] = { 0.6, 0.8, 0.2, 1.0 };
    float flr_color5[] = { 0.8, 0.8, 0.5, 1.0 };
    float waxed[] = { 0.11,0.06,0.11,1.0 };
    float waxed2[] = { 0.43,0.47,0.54,1.0 };
    float waxed3[] = { 0.33,0.33,0.52,1.0 };
    float white[] = { 0.95, 0.95, 0.95, 1.0 };

    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-2.0, -2.5, 3.0);
    glScalef(1.0, 1.5, 1.0);
    /*-------Draw the billboard ----*/
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, textName[5]);

    draw_billboard(5.0 * 4.0, 3.0 * 4.0, 5.0, 8.0);
    draw_billboard(6.0 * 4.0, 5.0 * 4.0, 5.0, 8.0);
    draw_billboard(3.0 * 4.0, 6.0 * 4.0, 5.0, 8.0);
    draw_billboard(2.0 * 4.0, 7.0 * 4.0, 5.0, 8.0);
    draw_billboard(7.0 * 4.0, 2.0 * 4.0, 5.0, 8.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 12.5, 0.0);
    glScalef(1.0, 0.3, 1.0);
    /*-------Draw the billboard ----*/
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, textName[4]);

    draw_billboard(5.0 * 4.0, 3.0 * 4.0, 5.0, 8.0);
    draw_billboard(6.0 * 4.0, 5.0 * 4.0, 5.0, 8.0);
    draw_billboard(3.0 * 4.0, 6.0 * 4.0, 5.0, 8.0);
    draw_billboard(2.0 * 4.0, 7.0 * 4.0, 5.0, 8.0);
    draw_billboard(7.0 * 4.0, 2.0 * 4.0, 5.0, 8.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, -2.5, 0.0);
    /*-------Draw the billboard ----*/
    glEnable(GL_TEXTURE_2D);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, textName[3]);

    draw_billboard(5.0 * 4.0, 3.0 * 4.0, 5.0, 8.0);
    draw_billboard(6.0 * 4.0, 5.0 * 4.0, 5.0, 8.0);
    draw_billboard(3.0 * 4.0, 6.0 * 4.0, 5.0, 8.0);
    draw_billboard(2.0 * 4.0, 7.0 * 4.0, 5.0, 8.0);
    draw_billboard(7.0 * 4.0, 2.0 * 4.0, 5.0, 8.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, -2.5, 0.0);
    //glRotatef(30.0, 0.0, 1.0, 0.0);
    glScalef(10000.0, 1.0, 10000.0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3fv(flr_color);
    draw_cube();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, textName[2]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glColor3f(0.5, 0.5, 0.5);
    glTranslatef(5.0, 1.5, 0.0);
    glRotatef(30.0, 0.0, 1.0, 0.0);
    glScalef(2.0, 6.0, 2.0);
    glColorMaterial(GL_FRONT, GL_SPECULAR, waxed3);
    glColor3fv(white);
    glMaterialf(GL_FRONT, GL_SHININESS, 9.8);
    //glColorMaterial(GL_FRONT, GL_DIFFUSE, waxed2);
    //glColorMaterial(GL_FRONT, GL_AMBIENT, waxed);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3fv(flr_color2);
    draw_cube();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glColor3f(0.2, 0.7, 0.5);
    glTranslatef(0.0, 3.5, 7.0);
    glRotatef(60.0, 0.0, 1.0, 0.0);
    glScalef(1.0, 10.0, 1.0);
    glColorMaterial(GL_FRONT, GL_SPECULAR, waxed3);
    glColor3fv(white);
    glMaterialf(GL_FRONT, GL_SHININESS, 9.8);
    //glColorMaterial(GL_FRONT, GL_DIFFUSE, waxed2);
    //glColorMaterial(GL_FRONT, GL_AMBIENT, waxed);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3fv(flr_color3);
    draw_cube();
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, textName[0]);
    glColor3f(0.6, 0.8, 0.2);
    glTranslatef(20.0, 0.5, 9.0);
    glRotatef(130.0, 0.0, 1.0, 0.0);
    glScalef(1.0, 4.0, 1.0);
    glColorMaterial(GL_FRONT, GL_SPECULAR, waxed3);
    glColor3fv(white);
    glMaterialf(GL_FRONT, GL_SHININESS, 9.8);
    //glColorMaterial(GL_FRONT, GL_DIFFUSE, waxed2);
    //glColorMaterial(GL_FRONT, GL_AMBIENT, waxed);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3fv(flr_color4);
    draw_cube();
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, textName[1]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glColor3f(0.8, 0.8, 0.5);
    glTranslatef(22.0, 2.0, 13.0);
    glRotatef(130.0, 0.0, 1.0, 0.0);
    glScalef(1.0, 7.0, 1.0);
    glColorMaterial(GL_FRONT, GL_SPECULAR, waxed3);
    glColor3fv(white);
    glMaterialf(GL_FRONT, GL_SHININESS, 9.8);
    //glColorMaterial(GL_FRONT, GL_DIFFUSE, waxed2);
    //glColorMaterial(GL_FRONT, GL_AMBIENT, waxed);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3fv(flr_color5);
    draw_cube();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glDisable(GL_COLOR_MATERIAL);
}

/*-------------------------------------------------------
 * Procedure to draw three axes and the orign
 */
void draw_axes()
{
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    /*----Draw a white sphere to represent the original-----*/
    glColor3f(0.9, 0.9, 0.9);

    gluSphere(sphere, 2.0,   /* radius=2.0 */
        12,            /* composing of 12 slices*/
        12);           /* composing of 8 stacks */

  /*----Draw three axes in colors, yellow, meginta, and cyan--*/
  /* Draw Z axis  */
    glColor3f(0.0, 0.95, 0.95);
    gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
        10.0,              /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */

/* Draw Y axis */
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);  /*Rotate about x by -90', z becomes y */
    glColor3f(0.95, 0.0, 0.95);
    gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
        10.0,             /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();

    /* Draw X axis */
    glColor3f(0.95, 0.95, 0.0);
    glPushMatrix();
    glRotatef(90.0, 0.0, 1.0, 0.0);  /*Rotate about y  by 90', x becomes z */
    gluCylinder(cylind, 0.5, 0.5,   /* radius of top and bottom circle */
        10.0,             /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();
    /*-- Restore the original modelview matrix --*/
    glPopMatrix();
    glDisable(GL_COLOR_MATERIAL);
}

/*-------------------------------------------------------
 * Make viewing matrix
 */
void make_view(int x)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    switch (x) {
    case 4:       /* Perspective */

      /* In this sample program, eye position and Xe, Ye, Ze are computed
         by ourselves. Therefore, use them directly; no trabsform is
         applied upon eye coordinate system
         */
        gluLookAt(eye[0], eye[1], eye[2],
            eye[0] - u[2][0], eye[1] - u[2][1], eye[2] - u[2][2],
            u[1][0], u[1][1], u[1][2]);
        glGetFloatv(GL_MODELVIEW_MATRIX, mtx);
        compute_ab_axes();
        break;

    case 1:       /* X direction parallel viewing */
        gluLookAt(30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, mtx);
        compute_ab_axes();
        break;
    case 2:       /* Y direction parallel viewing */
        gluLookAt(0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, mtx);
        compute_ab_axes();
        break;
    case 3:
        gluLookAt(0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, mtx);
        compute_ab_axes();
        break;
    }
}

/*------------------------------------------------------
 * Procedure to make projection matrix
 */
void make_projection(int x)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (x == 4) {
        gluPerspective(90.0, (double)width / (double)height, 1.5, 50.0);
    }
    else {
        if (width > height)
            glOrtho(-40.0, 40.0, -40.0 * (float)height / (float)width,
                40.0 * (float)height / (float)width,
                -0.0, 100.0);
        else
            glOrtho(-40.0 * (float)width / (float)height,
                40.0 * (float)width / (float)height, -40.0, 40.0,
                -0.0, 100.0);
    }
    glMatrixMode(GL_MODELVIEW);
}

/*---------------------------------------------------------
 * Procedure to draw view volume, eye position, focus ,...
 * for perspective projection
 */
void draw_view()
{
    int    i;

    glMatrixMode(GL_MODELVIEW);

    /*----Draw Eye position-----*/
    glPushMatrix();
    glTranslatef(eye[0], eye[1], eye[2]);
    glColor3f(0.0, 1.0, 0.0);
    glutWireSphere(1.0, 10, 10);
    glPopMatrix();

    /*----Draw eye coord. axes -----*/
    glColor3f(1.0, 1.0, 0.0);           /* Draw Xe */
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * u[0][0], eye[1] + 20.0 * u[0][1], eye[2] + 20.0 * u[0][2]);
    glEnd();

    glColor3f(1.0, 0.0, 1.0);          /* Draw Ye */
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * u[1][0], eye[1] + 20.0 * u[1][1], eye[2] + 20.0 * u[1][2]);
    glEnd();

    glColor3f(0.0, 1.0, 1.0);          /* Draw Ze */
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * u[2][0], eye[1] + 20.0 * u[2][1], eye[2] + 20.0 * u[2][2]);
    glEnd();
}

/*--------------------------------------------------------
 * Display callback procedure to draw the scene
 */
void display()
{
    int i, j;
    /*Clear previous frame and the depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (style) {
    case 0:
        make_view(4);
        make_projection(4);
        glViewport(width / 2, 0, width / 2, height / 2);
        draw_scene();

        make_view(1);
        make_projection(1);
        glViewport(0, height / 2, width / 2, height / 2);
        draw_scene();
        make_view(1);
        draw_view();

        make_view(2);
        glViewport(width / 2, height / 2, width / 2, height / 2);
        draw_scene();
        make_view(2);
        draw_view();

        make_view(3);
        glViewport(0, 0, width / 2, height / 2);
        draw_scene();
        make_view(3);
        draw_view();
        break;

    case 1:
        glViewport(0, 0, width, height);
        make_view(1);
        make_projection(1);
        draw_scene();
        break;

    case 2:
        glViewport(0, 0, width, height);
        make_view(2);
        make_projection(2);
        draw_scene();
        break;

    case 3:
        glViewport(0, 0, width, height);
        make_view(3);
        make_projection(3);
        draw_scene();
        break;

    case 4:
        glViewport(0, 0, width, height);
        make_view(4);
        make_projection(4);
        draw_scene();
        break;
    }
    /*-------Swap the back buffer to the front --------*/
    glutSwapBuffers();
    return;
}

void draw_scene()
{
    float metal[] = { 0.19,0.07,0.02,1.0 };
    //float metal2[] = { 0.70,0.27,0.08,1.0 };
    float metal3[] = { 0.26,0.14,0.09,1.0 };
    float cube_color[] = { 0.20, 0.75, 0.0, 1.0 };
    float white[] = { 0.95, 0.95, 0.95, 1.0 };

    glPushMatrix();
    glLoadIdentity();
    /*----position light1 (fixed) in eye coord sys ---*/
    glLightfv(GL_LIGHT1, GL_POSITION, lit1_position);  /*fixed position in eye space---*/
    /*---draw the light source ---*/
    glTranslatef(lit1_position[0], lit1_position[1], lit1_position[2]);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glColor3f(0.80, 0.80, 0.0);
    glutSolidSphere(0.3, 12, 12);
    glPopMatrix();
    glEnable(GL_CULL_FACE);

    /*-------Rotate light position----*/
    glPushMatrix();
    glTranslatef(8.0, 0.0, 8.0);          /* Move to [8, 0, 8] */
    glRotatef(lit_angle, 0.0, 1.0, 0.0);  /* Rotate about y-axis */
    /*-------Draw light source as a red cube----*/
    glPushMatrix();
    glTranslatef(lit_position[0], lit_position[1], lit_position[2]);
    glColor3f(1.0, 0.0, 0.0);
    glutWireSphere(0.5, 8, 8);
    glPopMatrix();

    glEnable(GL_LIGHTING);      /* Turn on lighting */
    /*----Redefine position and direction of light0-----*/
    glLightfv(GL_LIGHT0, GL_POSITION, lit_position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lit_direction);
    glPopMatrix();

    draw_floor();

    draw_axes();
    /*-------Draw the base of the windmill which is a cube----*/
    glTranslatef(ver_x, ver_y, ver_z);
    glRotatef(ang_y, 0.0, 1.0, 0.0);
    glRotatef(ang_z, 0.0, 0.0, 1.0);
    //glRotatef(-30.0, 0.0, 1.0, 0.0);

    glPushMatrix();
    glScalef(0.3, 0.3, 0.3);

    glPushMatrix();              /* Save M1 coord. sys */
    //glRotatef(30.0, 0.0, 1.0, 0.0);  /* rotate about y-axis by 30' */
    glScalef(7.5, 6.0, 6.0);     /* Scale up the axes by 6 times */
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_SPECULAR, metal3);
    glColor3fv(white);
    glMaterialf(GL_FRONT, GL_SHININESS, 64);//12.8
    //glColorMaterial(GL_FRONT, GL_DIFFUSE, metal2);
    //glColorMaterial(GL_FRONT, GL_AMBIENT, metal);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3fv(cube_color);
    draw_cube();                 /* Draw a unit cube in the new coord. sys. */
    glPopMatrix();               /* Get M1 back */


    glPushMatrix();
    glTranslatef(0.0, 3.0, 0.0); /* Go up by 2 units, move to M2 coord. sys */

    /*-------Draw the cylinder, arm of the windmill------*/
    glColor3f(0.68, 0.68, 0.68);    /* Gray colored */

    if (cylind == NULL) {               /* allocate a quadric object, if necessary */
        cylind = gluNewQuadric();
        gluQuadricDrawStyle(cylind, GLU_FILL);
        gluQuadricNormals(cylind, GLU_SMOOTH);
    }

    glPushMatrix();              /* Save M2 system */
    glRotatef(-90.0, 1.0, 0.0, 0.0);  /* Roate about x axis, z become y,
                                         and y become -z */
                                         /*--- Draw a cylinder ---*/
    gluCylinder(cylind, 0.3, 0.3, /* radius of top and bottom circle */
        2.5,              /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();         /* Get M2 back */

    glTranslatef(0.0, 2.5, 0.0); /* Go up 8 units, become M3 coord. sys*/

    /*----- Draw a unit sphere ---*/
    if (sphere == NULL) {
        sphere = gluNewQuadric();
        gluQuadricDrawStyle(sphere, GLU_FILL);
    }


    glColor3f(1.0, 0.2, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    gluSphere(sphere, 0.5,   /* radius=1.5 */
        12,            /* composing of 12 slices*/
        12);           /* composing of 12 stacks */

  /*------Draw three blades ---*/
    glColor3f(1.0, 1.0, 1.0);

    glRotatef(bld_ang, 0.0, 0.0, 1.0);  /* Rotate about x axis, M5 coord. sys. */
    draw_blade();/* draw the first blade */

    glRotatef(120.0, 0.0, 0.0, 1.0);    /* rotate by 120 degree, M6 coord. sys */
    draw_blade();/* draw 2nd blade */

    glRotatef(120.0, 0.0, 0.0, 1.0);    /* rotate by 120 degree,M7 coord. sys */
    draw_blade();/* draw 3rd blade */

    glPopMatrix();/* Get M1 back */

    /*draw Tripod#1*/
    glPushMatrix();
    glColor3f(0.68, 0.68, 0.68);
    //glRotatef(30.0, 0.0, 1.0, 0.0);  /* rotate about y-axis by 30' */
    glTranslatef(3.0, -3.2, 2.0);
    glScalef(0.5, 2.0, 0.5);
    draw_cube();
    glTranslatef(-6.0, 0.0, 0.0);
    draw_cube();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(0.0, -0.5, 0.0);
    glScalef(16.0, 0.25, 1.5);
    draw_cube();
    glPopMatrix();
    /*draw Tripod#2*/
    glPushMatrix();
    glColor3f(0.68, 0.68, 0.68);
    //glRotatef(30.0, 0.0, 1.0, 0.0);  /* rotate about y-axis by 30' */
    glTranslatef(3.0, -3.2, -2.0);
    glScalef(0.5, 2.0, 0.5);
    draw_cube();
    glTranslatef(-6.0, 0.0, 0.0);
    draw_cube();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(0.0, -0.5, 0.0);
    glScalef(16.0, 0.25, 1.5);
    draw_cube();
    glPopMatrix();

    //draw tail
    glPushMatrix();
    //glRotatef(30.0, 0.0, 1.0, 0.0);  /* rotate about y-axis by 30' */
    glTranslatef(-6.0, 0.0, 0.0);

    glPushMatrix();              /* Save M2 coord. sys */
    glColor3f(0.80, 0.75, 0.0);  /* Set the current color */
    glScalef(6.0, 1.5, 1.5);     /* Scale up the axes by 6 times */
    draw_cube();                 /* Draw a unit cube in the new coord. sys. */

    glPopMatrix();               /* Get M2 back */

    glColor3f(0.68, 0.68, 0.68);
    glTranslatef(-2.0, 0.0, 0.0);
    gluCylinder(cylind, 0.3, 0.3, 2.0, 12, 3);


    glColor3f(1.0, 0.2, 0.0);
    glTranslatef(0.0, 0.0, 2.0);
    gluSphere(sphere, 0.5,   /* radius=1.5 */
        12,            /* composing of 12 slices*/
        12);           /* composing of 12 stacks */

  /*------Draw three blades ---*/
    glColor3f(1.0, 1.0, 1.0);
    glScalef(0.2, 0.2, 0.2);

    glRotatef(bld_ang, 0.0, 0.0, 1.0);  /* Rotate about x axis, M5 coord. sys. */
    draw_blade();/* draw the first blade */

    glRotatef(120.0, 0.0, 0.0, 1.0);    /* rotate by 120 degree, M6 coord. sys */
    draw_blade();/* draw 2nd blade */

    glRotatef(120.0, 0.0, 0.0, 1.0);    /* rotate by 120 degree,M7 coord. sys */
    draw_blade();/* draw 3rd blade */
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
    glPopMatrix();

}


/*--------------------------------------------------
 * Reshape callback function which defines the size
 * of the main window when a reshape event occurrs.
 */
void my_reshape(int w, int h)
{
    width = w;
    height = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, (double)w / (double)h, 1.5, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*--------------------------------------------------
 * Keyboard callback func. When a 'q' key is pressed,
 * the program is aborted.
 */
void my_quit(unsigned char key, int ix, int iy)
{
    int    i;
    float  x[3], y[3], z[3];

    /*--- Control the car. ----*/
    if (key == 'Q' || key == 'q') exit(0);
    else if (key == 'u') {
        eyeDy += 0.5;       /* move up */
        for (i = 0;i < 3;i++) eye[i] -= 0.5 * u[1][i];
    }
    else if (key == 'd') {
        eyeDy += -0.5;       /* move down */
        for (i = 0;i < 3;i++) eye[i] += 0.5 * u[1][i];
    }
    else if (key == 'E') {
        eyeDx += -0.5;       /* move left */
        for (i = 0;i < 3;i++) eye[i] += 0.5 * u[0][i];
    }
    else if (key == 'e') {
        eyeDx += 0.5;        /* move right */
        for (i = 0;i < 3;i++) eye[i] -= 0.5 * u[0][i];
    }
    else if (key == 'F') {
        eyeDz += 0.5;        /* zoom in */
        for (i = 0;i < 3;i++) eye[i] -= 0.5 * u[2][i];
    }
    else if (key == 'f') {
        eyeDz += -0.5;       /* zoom out */
        for (i = 0;i < 3;i++) eye[i] += 0.5 * u[2][i];
    }
    else if (key == 'p') {             /* pitching */
        eyeAngx += 5.0;
        if (eyeAngx > 360.0) eyeAngx -= 360.0;
        y[0] = u[1][0] * cv - u[2][0] * sv;
        y[1] = u[1][1] * cv - u[2][1] * sv;
        y[2] = u[1][2] * cv - u[2][2] * sv;

        z[0] = u[2][0] * cv + u[1][0] * sv;
        z[1] = u[2][1] * cv + u[1][1] * sv;
        z[2] = u[2][2] * cv + u[1][2] * sv;

        for (i = 0;i < 3;i++) {
            u[1][i] = y[i];
            u[2][i] = z[i];
        }
    }
    else if (key == 'P') {
        eyeAngx += -5.0;
        if (eyeAngx < 0.0) eyeAngx += 360.0;
        y[0] = u[1][0] * cv + u[2][0] * sv;
        y[1] = u[1][1] * cv + u[2][1] * sv;
        y[2] = u[1][2] * cv + u[2][2] * sv;

        z[0] = u[2][0] * cv - u[1][0] * sv;
        z[1] = u[2][1] * cv - u[1][1] * sv;
        z[2] = u[2][2] * cv - u[1][2] * sv;

        for (i = 0;i < 3;i++) {
            u[1][i] = y[i];
            u[2][i] = z[i];
        }
    }
    else if (key == 'h') {            /* heading */
        eyeAngy += 5.0;
        if (eyeAngy > 360.0) eyeAngy -= 360.0;
        for (i = 0;i < 3;i++) {
            x[i] = cv * u[0][i] - sv * u[2][i];
            z[i] = sv * u[0][i] + cv * u[2][i];
        }
        for (i = 0;i < 3;i++) {
            u[0][i] = x[i];
            u[2][i] = z[i];
        }
    }
    else if (key == 'H') {
        eyeAngy += -5.0;
        if (eyeAngy < 0.0) eyeAngy += 360.0;
        for (i = 0;i < 3;i++) {
            x[i] = cv * u[0][i] + sv * u[2][i];
            z[i] = -sv * u[0][i] + cv * u[2][i];
        }
        for (i = 0;i < 3;i++) {
            u[0][i] = x[i];
            u[2][i] = z[i];
        }
    }
    else if (key == 'r') {            /* rolling */
        eyeAngz += 5.0;
        if (eyeAngz > 360.0) eyeAngz -= 360.0;
        for (i = 0;i < 3;i++) {
            x[i] = cv * u[0][i] - sv * u[1][i];
            y[i] = sv * u[0][i] + cv * u[1][i];
        }
        for (i = 0;i < 3;i++) {
            u[0][i] = x[i];
            u[1][i] = y[i];
        }
    }
    else if (key == 'R') {
        eyeAngz += -5.0;
        if (eyeAngz < 0.0) eyeAngz += 360.0;
        for (i = 0;i < 3;i++) {
            x[i] = cv * u[0][i] + sv * u[1][i];
            y[i] = -sv * u[0][i] + cv * u[1][i];
        }
        for (i = 0;i < 3;i++) {
            u[0][i] = x[i];
            u[1][i] = y[i];
        }
    }
    else if (key == 'M') {
        ver_y += 0.02;
        if (ver_y > 1000.0) ver_y -= 1000.0;
    }
    else if (key == 'm') {
        ver_y -= 0.02;
        if (ver_y < 0.00) ver_y = 0.0;
    }
    else if (key == ',' && ver_y > 1.0) {
        ang_y += 2.5;
    }
    else if (key == '/' && ver_y > 1.0) {
        ang_y -= 2.5;
    }
    else if (key == ';' && ver_y > 1.0) {
        ver_x += Step * cos(ang_y * PI / 180.0);
        ver_z -= Step * sin(ang_y * PI / 180.0);
        if (ang_z > -15.0)
            ang_z -= 3;
    }
    else if (key == '.' && ver_y > 1.0) {
        ver_x += -(Step * cos(ang_y * PI / 180.0));
        ver_z -= -(Step * sin(ang_y * PI / 180.0));
    }
    else if (key == 'z') {
        lit_diffuse[0] = 0.95;
        lit_diffuse[1] = 0.05;
        lit_diffuse[2] = 0.05;
        setup_light();
    }
    else if (key == 'g') {
        lit_diffuse[0] = 0.05;
        lit_diffuse[1] = 0.95;
        lit_diffuse[2] = 0.05;
        setup_light();
    }
    else if (key == 'b') {
        lit_diffuse[0] = 0.05;
        lit_diffuse[1] = 0.05;
        lit_diffuse[2] = 0.95;
        glClearColor(fog_color[0], fog_color[1], fog_color[2], fog_color[3]);
        glFogfv(GL_FOG_COLOR, fog_color);
        setup_light();
    }
    else if (key == 'w') {
        lit_diffuse[0] = 0.95;
        lit_diffuse[1] = 0.95;
        lit_diffuse[2] = 0.95;
        glClearColor(fog_color2[0], fog_color2[1], fog_color2[2], fog_color2[3]);
        glFogfv(GL_FOG_COLOR, fog_color2);
        setup_light();
    }
    else if (key == 'o') glEnable(GL_FOG);
    else if (key == 'O') glDisable(GL_FOG);
    /*fog factor=GL_LINEAR,GL_EXP,or GL_EXP2*/
    else if (key == 'l')   glFogi(GL_FOG_MODE, GL_LINEAR);
    else if (key == 'k')   glFogi(GL_FOG_MODE, GL_EXP);
    else if (key == 'j')   glFogi(GL_FOG_MODE, GL_EXP2);
    /*else if (key == 'Z') {
    lit1_diffuse[0] = 0.95;
    lit1_diffuse[1] = 0.05;
    lit1_diffuse[2] = 0.05;
    setup_light();
    }
    else if (key == 'G') {
    lit1_diffuse[0] = 0.05;
    lit1_diffuse[1] = 0.95;
    lit1_diffuse[2] = 0.05;
    setup_light();
    }
    else if (key == 'B') {
    lit1_diffuse[0] = 0.05;
    lit1_diffuse[1] = 0.05;
    lit1_diffuse[2] = 0.95;
    setup_light();
    }
    else if (key == 'W') {
    lit1_diffuse[0] = 0.95;
    lit1_diffuse[1] = 0.95;
    lit1_diffuse[2] = 0.95;
    setup_light();
    }*/
    else if (key == '=') {
        dens += 0.01;
        glFogf(GL_FOG_DENSITY, dens);
    }
    else if (key == '+') {
        dens -= 0.01;
        glFogf(GL_FOG_DENSITY, dens);
    }
    else if (key == '0') {
        style = 0;
    }
    else if (key == '1') {
        style = 1;
    }
    else if (key == '2') {
        style = 2;
    }
    else if (key == '3') {
        style = 3;
    }
    else if (key == '4') {
        style = 4;
    }
    else if (key == '8') {
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
    }
    else if (key == '9') {
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
    }
    else if (key == '[') {
        lit_exponent += 5;
        setup_light();
    }
    else if (key == ']') {
        lit_exponent -= 5;
        setup_light();
    }
    /*---Rotate light position by SPACE key ----*/
    if (key == ' ') lit_angle += 5.0;
    display();
}


/*---------------------------------------------
 * Idle event callback function, increase blade and arm
 * rotation angles.
 */
void idle_func()
{
    if (ver_y < 0.01)
        bld_ang = 0;
    else if (ver_y < 0.5)
        bld_ang += 0.2;
    else if (ver_y < 0.8)
        bld_ang += 0.5;
    else if (ver_y < 1.0)
        bld_ang += 1.0;
    else
        bld_ang += 5.0;
    if (bld_ang > 360.0) {
        bld_ang -= 360.0;
    }
    if (ang_z < 0)
        ang_z += 0.01;
    display(); /* show the scene again */
}

/*---------------------------------------------------
 * Main procedure which defines the graphics environment,
 * the operation style, and the callback func's.
 */
void main(int argc, char** argv)
{
    /*-----Initialize the GLUT environment-------*/
    glutInit(&argc, argv);

    /*-----Depth buffer is used, be careful !!!----*/
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("windmill");

    myinit();      /*---Initialize other state varibales----*/

    /*----Associate callback func's whith events------*/
    glutDisplayFunc(display);
    glutIdleFunc(idle_func);    /* Add idle event */
    glutReshapeFunc(my_reshape);
    glutKeyboardFunc(my_quit);


    glutMainLoop();
}

/*------------------------------------------
 * Procedure to compute norm of vector v[]
 */
float norm2(float v[])
{
    double  sum;
    float   temp;

    sum = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    temp = (float)sqrt(sum);
    return temp;
}