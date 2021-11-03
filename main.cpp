//
//  main.cpp
//  FerrisWheel
//
//  Created by Noushin Gauhar on 16/5/21.
//

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "BmpLoader.h"

GLUquadric *quad;

using namespace std;



double Txval=0,Tyval=0,Tzval=0;
double windowHeight=1000, windowWidth=1000;
GLfloat alpha = 0.0, theta = 0.0, orbiterAlpha = -45.0, orbiterTheta = 0.0, testTheta = -45.0, pirateBoatTheta = 0.0, cmOrbiterAlpha = 0.0, cmOrbiterTheta = 0.0, skyDropPos = 0.0 ;
double eyeX=-10, eyeY=5.0, eyeZ=100, refX = 0, refY=0,refZ=0;
GLboolean bRotate = false, uRotate = false, fanSwitch = false, door1 = false, orbiterFlag = false, testFlag = true, pirateBoatFlag = false, pirateBoatCheck = false, cmOrbiterFlag = false, skyDropFlag = false, upFlag = true, downFlag1 = true, downFlag2 = false, downFlag3 = false, show = false, day = true, switchOne = false, switchTwo = false, switchThree = false, switchFour = false;
unsigned int ID;
unsigned int ID2[40];

GLfloat yf = 0, xf = 0;
GLboolean yflag = false, xflag = false;

static GLfloat colors[4][6] =
{
    {1,0,0, 0.5,0,0}, //red
    {0,1,0, 0,0.5,0}, //green
    {0,0,1, 0,0,0.5}, //blue
    {1,1,0, 0.5,0.5,0} //yellow
};

static GLfloat v_cube[8][3] =
{
    {0.0, 0.0, 0.0}, //0
    {0.0, 0.0, 3.0}, //1
    {3.0, 0.0, 3.0}, //2
    {3.0, 0.0, 0.0}, //3
    {0.0, 3.0, 0.0}, //4
    {0.0, 3.0, 3.0}, //5
    {3.0, 3.0, 3.0}, //6
    {3.0, 3.0, 0.0}  //7
};

static GLubyte quadIndices[6][4] =
{
    {0, 1, 2, 3}, //bottom
    {4, 5, 6, 7}, //top
    {5, 1, 2, 6}, //front
    {3, 7, 4, 0}, // back
    {2, 3, 7, 6}, //right
    {0, 4, 5,1}  //left
};

/* //This is the loadtexture() written for Windows
 void LoadTexture(const char*filename, int index)
{
    glGenTextures(1, &ID[index]);
    glBindTexture(GL_TEXTURE_2D, ID[index]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID[index]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData);
} */

void LoadTexture2(const char*filename,GLint num)
{
    TK_RGBImageRec             *texture1;
    
    texture1 = tkRGBImageLoad(filename);

    glPixelStorei(GL_UNPACK_ALIGNMENT,   1   );
    
    glTexImage2D(GL_TEXTURE_2D,         //target
            0,                             //level
            GL_RGB,                     //internalFormat
            texture1->sizeX,             //w
            texture1->sizeY,            //h
            0,                             //border
            GL_RGB, GL_UNSIGNED_BYTE,     //format,type
            texture1->data);            //data
                
    // Create Texture
    glGenTextures(1, &ID2[num]);
    glBindTexture(GL_TEXTURE_2D, ID2[num]);
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R,GL_MIRRORED_REPEAT);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 texture1->sizeX, texture1->sizeY, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, texture1->data);
    
}
void materialProperty()
{
    glColor3f(1,1,1);
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {60};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
}

void matCurve(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambfactor = 1.0, GLfloat specfactor = 1.0, GLfloat shine=50)
{
    glColor3f(1,1,1);
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { difX*ambfactor, difY*ambfactor, difZ*ambfactor, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {60};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
}

static void getNormal3p
(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}

void drawCube1(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX=0, GLfloat ambY=0, GLfloat ambZ=0, GLfloat shine=50)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {shine};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    
    glBegin(GL_QUADS);
    
    for (GLint i = 0; i <6; i++)
    {
        getNormal3p(v_cube[quadIndices[i][0]][0], v_cube[quadIndices[i][0]][1], v_cube[quadIndices[i][0]][2],
                    v_cube[quadIndices[i][1]][0], v_cube[quadIndices[i][1]][1], v_cube[quadIndices[i][1]][2],
                    v_cube[quadIndices[i][2]][0], v_cube[quadIndices[i][2]][1], v_cube[quadIndices[i][2]][2]);
        glVertex3fv(&v_cube[quadIndices[i][0]][0]);
        glVertex3fv(&v_cube[quadIndices[i][1]][0]);
        glVertex3fv(&v_cube[quadIndices[i][2]][0]);
        glVertex3fv(&v_cube[quadIndices[i][3]][0]);
    }
    glEnd();
}

void drawSphere(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX, GLfloat ambY, GLfloat ambZ, GLfloat shine=90)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {shine};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    glutSolidSphere (1.0, 16, 16);
}

void drawTorus(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX, GLfloat ambY, GLfloat ambZ, GLdouble innerRadius, GLdouble outerRadius, GLint nsides, GLint rings, GLfloat shine=90)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {shine};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    
    glutSolidTorus(innerRadius, outerRadius, nsides, rings);
    
    //glutSolidTorus(0.5, 10.0, 16, 12);
}

void drawCylinder(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX, GLfloat ambY, GLfloat ambZ, GLfloat shine=90)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {shine};
    
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv( GL_FRONT, GL_EMISSION, no_mat);
        
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    gluCylinder(quadratic, 1.5, 1.5, 19, 32, 32);

}

static GLfloat v_box[8][3] =
{
    {0.0, 0.0, 0.0}, //0
    {3.0, 0.0, 0.0}, //1
    {0.0, 0.0, 3.0}, //2
    {3.0, 0.0, 3.0}, //3
    {0.0, 3.0, 0.0}, //4
    {3.0, 3.0, 0.0}, //5
    {0.0, 3.0, 3.0}, //6
    {3.0, 3.0, 3.0}, //7
    
};

static GLubyte BoxquadIndices[6][4] =
{
    {0,2,3,1},
    {0,2,6,4},
    {2,3,7,6},
    {1,3,7,5},
    {1,5,4,0},
    {6,7,5,4}
};

void drawBox()
{
    materialProperty();
    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {
        //glColor3f(colors[4][0],colors[4][1],colors[4][2]);
        getNormal3p(v_box[BoxquadIndices[i][0]][0], v_box[BoxquadIndices[i][0]][1], v_box[BoxquadIndices[i][0]][2],
                    v_box[BoxquadIndices[i][1]][0], v_box[BoxquadIndices[i][1]][1], v_box[BoxquadIndices[i][1]][2],
                    v_box[BoxquadIndices[i][2]][0], v_box[BoxquadIndices[i][2]][1], v_box[BoxquadIndices[i][2]][2]);

        glVertex3fv(&v_box[BoxquadIndices[i][0]][0]);glTexCoord2f(1,1);
        glVertex3fv(&v_box[BoxquadIndices[i][1]][0]);glTexCoord2f(1,0);
        glVertex3fv(&v_box[BoxquadIndices[i][2]][0]);glTexCoord2f(0,0);
        glVertex3fv(&v_box[BoxquadIndices[i][3]][0]);glTexCoord2f(0,1);
    }
    glEnd();
    //glutSolidSphere (3.0, 20, 16);

}


//Drawing pyramid *************************
static GLfloat v_pyramid[5][3] =
{
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 2.0},
    {2.0, 0.0, 2.0},
    {2.0, 0.0, 0.0},
    {1.0, 4.0, 1.0}
};

static GLubyte p_Indices[4][3] =
{
    {4, 1, 2},
    {4, 2, 3},
    {4, 3, 0},
    {4, 0, 1}
};

static GLubyte PquadIndices[1][4] =
{
    {0, 3, 2, 1}
};

void drawpyramid(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX, GLfloat ambY, GLfloat ambZ, GLfloat shine)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {shine};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv( GL_FRONT, GL_EMISSION, no_mat);

    glBegin(GL_TRIANGLES);
    for (GLint i = 0; i <4; i++)
    {
        getNormal3p(v_pyramid[p_Indices[i][0]][0], v_pyramid[p_Indices[i][0]][1], v_pyramid[p_Indices[i][0]][2],
                    v_pyramid[p_Indices[i][1]][0], v_pyramid[p_Indices[i][1]][1], v_pyramid[p_Indices[i][1]][2],
                    v_pyramid[p_Indices[i][2]][0], v_pyramid[p_Indices[i][2]][1], v_pyramid[p_Indices[i][2]][2]);
        glVertex3fv(&v_pyramid[p_Indices[i][0]][0]);
        glVertex3fv(&v_pyramid[p_Indices[i][1]][0]);
        glVertex3fv(&v_pyramid[p_Indices[i][2]][0]);
    }
    glEnd();

    glBegin(GL_QUADS);
    for (GLint i = 0; i <1; i++)
    {
        getNormal3p(v_pyramid[PquadIndices[i][0]][0], v_pyramid[PquadIndices[i][0]][1], v_pyramid[PquadIndices[i][0]][2],
                    v_pyramid[PquadIndices[i][1]][0], v_pyramid[PquadIndices[i][1]][1], v_pyramid[PquadIndices[i][1]][2],
                    v_pyramid[PquadIndices[i][2]][0], v_pyramid[PquadIndices[i][2]][1], v_pyramid[PquadIndices[i][2]][2]);
        glVertex3fv(&v_pyramid[PquadIndices[i][0]][0]);
        glVertex3fv(&v_pyramid[PquadIndices[i][1]][0]);
        glVertex3fv(&v_pyramid[PquadIndices[i][2]][0]);
        glVertex3fv(&v_pyramid[PquadIndices[i][3]][0]);
    }
    glEnd();

}

static GLfloat v_trapezoid[8][3] =
{
    {0.0, 0.0, 0.0}, //0
    {0.0, 0.0, 3.0}, //1
    {3.0, 0.0, 3.0}, //2
    {3.0, 0.0, 0.0}, //3
    {0.5, 3.0, 0.5}, //4
    {0.5, 3.0, 2.5}, //5
    {2.5, 3.0, 2.5}, //6
    {2.5, 3.0, 0.5}  //7
};

static GLubyte TquadIndices[6][4] =
{
    {0, 1, 2, 3}, //bottom
    {4, 5, 6, 7}, //top
    {5, 1, 2, 6}, //front
    {3, 7, 4, 0}, // back
    {2, 3, 7, 6}, //right
    {0, 4, 5, 1}  //left
};

void drawTrapezoid(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX, GLfloat ambY, GLfloat ambZ, GLfloat shine=50)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {shine};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv( GL_FRONT, GL_EMISSION, no_mat);

    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {
        getNormal3p(v_trapezoid[TquadIndices[i][0]][0], v_trapezoid[TquadIndices[i][0]][1], v_trapezoid[TquadIndices[i][0]][2],
        v_trapezoid[TquadIndices[i][1]][0], v_trapezoid[TquadIndices[i][1]][1], v_trapezoid[TquadIndices[i][1]][2],
        v_trapezoid[TquadIndices[i][2]][0], v_trapezoid[TquadIndices[i][2]][1], v_trapezoid[TquadIndices[i][2]][2]);
        
        glVertex3fv(&v_trapezoid[TquadIndices[i][0]][0]);
        glVertex3fv(&v_trapezoid[TquadIndices[i][1]][0]);
        glVertex3fv(&v_trapezoid[TquadIndices[i][2]][0]);
        glVertex3fv(&v_trapezoid[TquadIndices[i][3]][0]);
    }
    glEnd();
}


void light()
{
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 20.0, 50.0, 0.0, 1.0 };

    //glEnable( GL_LIGHT0);
    glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv( GL_LIGHT0, GL_POSITION, light_position);

   /* GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
    glLightf( GL_LIGHT0, GL_SPOT_CUTOFF, 10.0); */
}

 void sky(float x, float y, float z, float width, float height, float length)
{
    materialProperty();
    glDisable(GL_DEPTH_TEST);
    x = x - width  / 2;
    y = y - height / 2;
    z = z - length / 2;
    
    
    //front
    glEnable(GL_TEXTURE_2D);
    if(day == true)
    {
        glBindTexture(GL_TEXTURE_2D,ID2[8]);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D,ID2[28]);
    }
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y, z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y, z+length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height, z+length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height, z);
    glEnd();
    
    //up;
    if(day == true)
    {
        glBindTexture(GL_TEXTURE_2D,ID2[27]);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D,ID2[28]);
    }
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+height, z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+height, z+length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y+height, z+length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x,y+height,z);
    glEnd();

    //back
    
    if(day == true)
    {
        glBindTexture(GL_TEXTURE_2D,ID2[9]);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D,ID2[28]);
    }
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y+height,z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x,y+height,z+length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x,y,z+length);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x,y,z);
    glEnd();

    //right
    if(day == true)
    {
        glBindTexture(GL_TEXTURE_2D,ID2[10]);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D,ID2[28]);
    } //10
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y, z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height, z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x,y+height, z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
    glEnd();
     
    //left
    
    if(day == true)
    {
        glBindTexture(GL_TEXTURE_2D,ID2[11]);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D,ID2[28]);
    } //11
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x,          y,        z+length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x,          y+height, z+length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height, z+length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,        z+length);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    
    //
}

void ground()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, ID2[14]);
    glPushMatrix();
    materialProperty();
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(150,-20,150);
    glTexCoord2f(5.0f, 0.0f); glVertex3f(150,-20,-150);
    glTexCoord2f(5.0f, 5.0f); glVertex3f(-100,-20,-100);
    glTexCoord2f(0.0f, 5.0f); glVertex3f(-100,-20,100);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void ground2()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, ID2[2]); //2
    glPushMatrix();
    materialProperty();
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(10,-19.8,10);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(10,-19.8,-10);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-10,-19.8,-10);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-10,-19.8,10);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void ferrisWheelSeat()
{
    //seat
    glPushMatrix();
    glTranslatef(0, -0.5, 0);
    glScalef(0.5, 0.2, 1.5);
    drawCube1(0.804, 0.361, 0.361, 0.403,0.1805,0.1805);
    glPopMatrix();
    
    //seat belt rod
    glPushMatrix();
    glTranslatef(1.3, 0.7, 0);
    glScalef(0.02, 0.02, 1.5);
    drawCube1(0,0,0, 0,0,0.0);
    glPopMatrix();
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,ID2[4]);
    
    //back
    glPushMatrix();
    glScalef(0.2, 0.5, 1.5);
    drawBox();
    glPopMatrix();
    
    //seat right side
    glPushMatrix();
    glScalef(0.5, 0.5, 0.02);
    drawBox();
    glPopMatrix();
    
    //seat left side
    glPushMatrix();
    glTranslatef(0, 0, 4.445);
    glScalef(0.5, 0.5, 0.02);
    drawBox();
    glPopMatrix();
    
    //bottom bent part
    glPushMatrix();
    glTranslatef(1.48, -0.5, 0);
    glRotatef(-45, 0, 0, 1);
    glScalef(0.15, 0.02, 1.5);
    drawBox();
    glPopMatrix();
    
    //bottom straight part
    glPushMatrix();
    glTranslatef(1.8, -0.8, 0);
    //glRotatef(-20, 0, 0, 1);
    glScalef(0.16, 0.02, 1.5);
    drawBox();
    glPopMatrix();
    
    //bottom farthest part
    glPushMatrix();
    glTranslatef(2.25, -0.8, 0);
    glScalef(0.02, 0.1, 1.5);
    drawBox();
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
    
}

void wheel()
{
    glPushMatrix();
    glScalef(1, 1, 2);
    drawSphere(1,0,0, 0.5,0,0);
    glPopMatrix();
    

    glPushMatrix();
    drawTorus(1,1,0.3, 0.5,0.5,0.15,  0.5,10.0,32,64); //0.859,0.439,0.576, 0.4295,0.2195,0.288
    glPopMatrix();

    
    //the big lines
    for(int i = 0; i < 180; i+=30)
    {
        glPushMatrix();
        glRotatef(i, 0,0, 1 );
        glScalef(6.6,0.1,0.5);
        glTranslatef(-1.5, -1.5, -1.5);
        drawCube1(0.867, 0.627, 0.867, 0.4335, 0.3135, 0.4335, 100);
        glPopMatrix();
    }

}

void bulbsOnFerrisWheel()
{
    for(int i = 0; i <= 360; i+= 45)
    {
        glPushMatrix();
        glRotatef(i, 0, 0, 1);
        glTranslatef(10, 0, 0);
        glScalef(0.5, 0.5, 0.5);
        drawSphere(1,1,1, 0.5,0.5,0.5);
        glPopMatrix();
    }
    
    for(int i = 15; i <= 360; i+= 45)
    {
        glPushMatrix();
        glRotatef(i, 0, 0, 1);
        glTranslatef(10, 0, 0);
        glScalef(0.5, 0.5, 0.5);
        drawSphere(1, 0, 0, 0.5, 0, 0);
        glPopMatrix();
    }
    
    for(int i = 30; i <= 360; i+= 45)
    {
        glPushMatrix();
        glRotatef(i, 0, 0, 1);
        glTranslatef(10, 0, 0);
        glScalef(0.5, 0.5, 0.5);
        drawSphere(0,0, 1, 0, 0, 0.5);
        glPopMatrix();
    }
}

void ferrisWheel()
{
    //right stand on the back
    glPushMatrix();
    glTranslatef(-.2, 0, -1);
    glRotatef(-75, 0, 0, 1);
    glScalef(7,0.28,0.1);
    //glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.545,0.000,0.545, 0.2725,0.0,0.2725);
    glPopMatrix();
    
    //left stand on the back
    glPushMatrix();
    glTranslatef(-0.6, 0, -1);
    glRotatef(-105, 0, 0, 1);
    glScalef(7,0.28,0.1);
    //glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.545,0.000,0.545, 0.2725,0.0,0.2725);
    glPopMatrix();
    
    //right stand on the front
    glPushMatrix();
    glTranslatef(-.2, 0, 6);
    glRotatef(-75, 0, 0, 1);
    glScalef(7,0.28,0.1);
    drawCube1(0.545,0.000,0.545, 0.2725,0.0,0.2725);
    glPopMatrix();
    
    //left stand on the front
    glPushMatrix();
    glTranslatef(-0.6, 0, 6);
    glRotatef(-105, 0, 0, 1);
    glScalef(7,0.28,0.1);
    //glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.545,0.000,0.545, 0.2725,0.0,0.2725);
    glPopMatrix();
    
    //base stand
    glPushMatrix();
    glTranslatef(0, -19.5, 2.5);
    //glRotatef(-105, 0, 0, 1);
    glScalef(4,0.5,3);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.545,0.271,0.075,  0.2725,0.1355,0.0375);
    glPopMatrix();
    
    //fence in the front
    for(float j = -9; j <= 19; j+=2)
    {
        glPushMatrix();
        glTranslatef(j, -19.5, 11);
        glScalef(0.1,2.5,0.1);
        glTranslatef(-1.5, -1.5, -1.5);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(j, -16.1, 11);
        glScalef(.4, .4, .4);
        drawSphere(0.855,0.439, 0.839, 0.4275, 0.2195, 0.4195);
        glPopMatrix();
    }
    
    ///the horizontal lines of the front fence
    glPushMatrix();
    glTranslatef(4, -17, 11);
    glScalef(10,0.05,0.1);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(4, -18, 11);
    glScalef(10,0.05,0.1);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(4, -19, 11);
    glScalef(10,0.05,0.1);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    //gate
    glPushMatrix();
    glTranslatef(-15, -20, 11);
    glRotatef(-alpha, 0, 1, 0);
    for(float j = 0; j <= 4; j+=2)
    {
        glPushMatrix();
        glTranslatef(j, 0, 0);
        glScalef(0.1,1.5,0.1);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(j, 4.4, 0.2);
        glScalef(.4, .4, .4);
        drawSphere(0.855,0.439, 0.839, 0.4275, 0.2195, 0.4195);
        glPopMatrix();
    }
    
    for(float j = 1; j <=3; j+=1)
    {
        glPushMatrix();
        glTranslatef(0, j, 0);
        glScalef(1.5,0.05,0.1);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
    }
    glPopMatrix();
    
    //fence in the back
    
    for(float j = -15; j <= 19; j+=2)
    {
        glPushMatrix();
        glTranslatef(j, -19.5, -5);
        glScalef(0.1,2.5,0.1);
        glTranslatef(-1.5, -1.5, -1.5);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(j, -16.1, -5);
        glScalef(.4, .4, .4);
        drawSphere(0.855,0.439, 0.839, 0.4275, 0.2195, 0.4195);
        glPopMatrix();
    }
    
    
    glPushMatrix();
    glTranslatef(2, -17, -5);
    glScalef(11.5,0.05,0.1);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(2, -18, -5);
    glScalef(11.5,0.05,0.1);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(2, -19, -5);
    glScalef(11.5,0.05,0.1);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    //fence in the left
    for(float j = -3 ; j <= 9; j+=2)
    {
        glPushMatrix();
        glTranslatef(-15, -19.5, j);
        glScalef(0.1,2.5,0.1);
        glTranslatef(-1.5, -1.5, -1.5);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(-15, -16.1, j);
        glScalef(.4, .4, .4);
        drawSphere(0.855,0.439, 0.839, 0.4275, 0.2195, 0.4195);
        glPopMatrix();
    }
    
    glPushMatrix();
    glTranslatef(-15, -17, 3);
    glScalef(.1,0.05,5.5);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-15, -18, 3);
    glScalef(.1,0.05,5.5);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-15, -19, 3);
    glScalef(.1,0.05,5.5);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
   
    
    //fence in the right
    for(float j = -3 ; j <= 9; j+=2)
    {
        glPushMatrix();
        glTranslatef(19, -19.5, j);
        glScalef(0.1,2.5,0.1);
        glTranslatef(-1.5, -1.5, -1.5);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(19, -16.1, j);
        glScalef(.4, .4, .4);
        drawSphere(0.855,0.439, 0.839, 0.4275, 0.2195, 0.4195);
        glPopMatrix();
    }
    
    glPushMatrix();
    glTranslatef(19, -17, 3);
    glScalef(.1,0.05,5.5);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(19, -18, 3);
    glScalef(.1,0.05,5.5);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(19, -19, 3);
    glScalef(.1,0.05,5.5);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    
    //rotating part
    glPushMatrix();
    glRotatef(-theta, 0,0, 1);
    glScalef(1.5, 1.5,1);
    wheel();
    glPushMatrix();
    glTranslatef(0, 0, 5);
    wheel();
    glPopMatrix();
    
    //bulbs
    glPushMatrix();
    glTranslatef(0, 0, 5.5);
    bulbsOnFerrisWheel();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 0, -0.5);
    bulbsOnFerrisWheel();
    glPopMatrix();
    
    
    //the middle line between two spheres
    glPushMatrix();
    glScalef(0.1,0.05,1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    //the smaller lines in between
    for(int j = 0; j <= 360; j+=30)
    {
        glPushMatrix();
        glRotatef(j, 0, 0, 1);
        for(int i = 1; i <= 7; i++)
        {
            glPushMatrix();
            glTranslatef(i, 0, 0);
            glScalef(0.1,0.05,1.5);
            drawCube1(0.780,0.082,0.522, 0.39, 0.041, 0.261);
            glPopMatrix();
        }
        glPopMatrix();
    }
    
    //the seats
    for(int i = 0; i <360; i+=30)
    {
        glPushMatrix();
        glRotatef(i, 0,0, 1);
        glTranslatef(10, 0, 0.9);
        glRotatef(-i, 0,0, 1);
        glRotatef(theta, 0,0, 1);
        glScalef(1, 1, 0.8);
        ferrisWheelSeat();
        glPopMatrix();
    }
    
    glPopMatrix();
    
    //ground
    glPushMatrix();
    glTranslatef(2, 0, 0);
    glScalef(2, 1, 1.5);
    ground2();
    glPopMatrix();
    
}


void bush()
{
    quad = gluNewQuadric();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,ID2[24]);
    
    glPushMatrix();
    gluQuadricTexture(quad,1);
    gluSphere(quad,1,100,100);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void tree()
{
    quad = gluNewQuadric();
    glEnable(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D,ID2[30]);
    
    glPushMatrix();
    glScalef(1, 2, 1);
    gluQuadricTexture(quad,1);
    gluSphere(quad,4,100,100);
    glPopMatrix();
    
    glBindTexture(GL_TEXTURE_2D,ID2[29]);
    glPushMatrix();
    glTranslatef(0, -7, 0);
    glRotatef(90, 1, 0, 0);
    gluQuadricTexture(quad,1);
    gluCylinder(quad, 1, 1, 10, 32, 32);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void rings()
{
    for(float i = -3.5; i >= -17.5; i -= 1)
    {
        glPushMatrix();
        glTranslatef(0, i, 0);
        glRotatef(90, 1, 0, 0);
        glScalef(0.2, 0.2, 0.2);
        drawTorus(1,0,0, 0.5,0,0,  1.5,7.5,32,64);
        glPopMatrix();
    }
    
    for(float i = -3; i >= -17; i -= 1)
    {
        glPushMatrix();
        glTranslatef(0, i, 0);
        glRotatef(90, 1, 0, 0);
        glScalef(0.2, 0.2, 0.2);
        drawTorus(1,1,1, 0.5,0.5,0.5,  1.5,7.5,32,64);
        glPopMatrix();
    }
    
}

void orbiter()
{
    
    glPushMatrix();
    glScalef(2, 1, 2);
    ground2();
    glPopMatrix();
   /* //the base
    glPushMatrix();
    glTranslatef(0, -19.5, 0);
    glScalef(10,0.5,10);
    glTranslatef(-1.5, -1.5, -1.5);
    //drawBox();
    drawCube1(0.545, 0.271, 0.075,  0.2725,0.1355,0.0375);
    glPopMatrix(); */
    
    //the 1st torus at the bottom

    rings();
    
     glPushMatrix();
     glTranslatef(0, -18, 0);
     glRotatef(90, 1, 0, 0);
     glScalef(0.2, 0.2, 0.2);
     drawTorus(1,1,1, 0.5,0.5,0.5,  2,8,32,64);
     glPopMatrix();
    
   //the 2nd torus at the bottom
    glPushMatrix();
    glTranslatef(0, -18.5, 0);
    glRotatef(90, 1, 0, 0);
    glScalef(0.2, 0.2, 0.2);
    drawTorus(1,0,0, 0.5,0,0,  2,10,32,64);
    glPopMatrix();
    
  /*  //the cylinder stand
    glPushMatrix();
    glTranslatef(0, -3, 0);
    glRotatef(90, 1, 0, 0);
    drawCylinder(0, 1, 0, 0, 0.5, 0);
    glPopMatrix(); */
    
    //the sphere
    glPushMatrix();
    glTranslatef(0, -1, 0);
    glScalef(2.5, 2.5, 2.5);
    drawSphere(1,0,0 ,0.5,0,0);
    glPopMatrix();
    
    //fence in the front
    for(float j = -10; j <= 16; j+=2)
    {
        glPushMatrix();
        glTranslatef(j, -19.5, 17);
        glScalef(0.1,2.5,0.1);
        glTranslatef(-1.5, -1.5, -1.5);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(j, -16.1, 17);
        glScalef(.4, .4, .4);
        drawSphere(0.855,0.439, 0.839, 0.4275, 0.2195, 0.4195);
        glPopMatrix();
    }
    
    //the horizontal lines of the front fence
    glPushMatrix();
    glTranslatef(2.5, -17, 17);
    glScalef(9,0.05,0.1);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(2.5, -18, 17);
    glScalef(9,0.05,0.1);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(2.5, -19, 17);
    glScalef(9,0.05,0.1);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    //gate
    glPushMatrix();
    glTranslatef(-15.5, -20, 17);
    glRotatef(-alpha, 0, 1, 0);
    for(float j = 0; j <= 4; j+=2)
    {
        glPushMatrix();
        glTranslatef(j, 0, 0);
        glScalef(0.1,1.5,0.1);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(j, 4.4, 0.2);
        glScalef(.4, .4, .4);
        drawSphere(0.855,0.439, 0.839, 0.4275, 0.2195, 0.4195);
        glPopMatrix();
    }
    
    for(float j = 1; j <=3; j+=1)
    {
        glPushMatrix();
        glTranslatef(0, j, 0);
        glScalef(1.5,0.05,0.1);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
    }
    glPopMatrix();
    
    
    //fence in the back
    
    for(float j = -14; j <= 16; j+=2)
    {
        glPushMatrix();
        glTranslatef(j, -19.5, -17);
        glScalef(0.1,2.5,0.1);
        glTranslatef(-1.5, -1.5, -1.5);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(j, -16.1, -17);
        glScalef(.4, .4, .4);
        drawSphere(0.855,0.439, 0.839, 0.4275, 0.2195, 0.4195);
        glPopMatrix();
    }
    
    ///the horizontal lines of the back fence
    glPushMatrix();
    glTranslatef(0, -17, -17);
    glScalef(10.5,0.05,0.1);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, -18, -17);
    glScalef(10.5,0.05,0.1);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, -19, -17);
    glScalef(10.5,0.05,0.1);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    //fence in the right
    for(float j = -17 ; j <= 15; j+=2)
    {
        glPushMatrix();
        glTranslatef(16, -19.5, j);
        glScalef(0.1,2.5,0.1);
        glTranslatef(-1.5, -1.5, -1.5);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(16, -16.1, j);
        glScalef(.4, .4, .4);
        drawSphere(0.855,0.439, 0.839, 0.4275, 0.2195, 0.4195);
        glPopMatrix();
    }
    
    //the horizontal lines
    glPushMatrix();
    glTranslatef(16, -17, 0);
    glScalef(.1,0.05,11.5);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(16, -18, 0);
    glScalef(.1,0.05,11.5);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(16, -19, 0);
    glScalef(.1,0.05,11.5);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    //fence in the left
    for(float j = -17 ; j <= 17; j+=2)
    {
        glPushMatrix();
        glTranslatef(-16, -19.5, j);
        glScalef(0.1,2.5,0.1);
        glTranslatef(-1.5, -1.5, -1.5);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(-16, -16.1, j);
        glScalef(.4, .4, .4);
        drawSphere(0.855,0.439, 0.839, 0.4275, 0.2195, 0.4195);
        glPopMatrix();
    }
    
    //
    glPushMatrix();
    glTranslatef(-16, -17, 0);
    glScalef(.1,0.05,11.5);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-16, -18, 0);
    glScalef(.1,0.05,11.5);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-16, -19, 0);
    glScalef(.1,0.05,11.5);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    //translating the rotating part down
    glPushMatrix();
    glTranslatef(0, -5, 0);
    
    //rotating part
    glPushMatrix();
    glRotatef(orbiterTheta, 0, 1, 0);
    //seat
    for(int i = 0; i <=360; i+=45)
    {
            glPushMatrix();
            glRotatef(i, 0, 1, 0);
            
            glPushMatrix();
            glRotatef(orbiterAlpha, 0, 0, 1);
            glRotatef(0, 0,1, 0);
            glTranslatef(15, 0, -2);
            glRotatef(-0, 0,1, 0);
            glRotatef(-orbiterAlpha, 0, 0, 1);
            ferrisWheelSeat();
            glPopMatrix();
            
            glPushMatrix();
            glRotatef(orbiterAlpha, 0, 0, 1);
            glScalef(5.1, 0.2, 0.2);
            drawCube1(0, 0, 1, 0,0,0.5);
            glPopMatrix();
            
            glPopMatrix();
    }
    
    glPopMatrix();
    
    glPopMatrix();
    
}


void boatBody()
{
    glPushMatrix();
    glTranslatef(-1.3, 0, 0);
    glScalef(3.7, 0.1, 1);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.412, 0.412, 0.412,  0.0,0.0,0.0);
    glPopMatrix();
    
    
    glPushMatrix();
    glTranslatef(0, 1.5, 1.5);
    glScalef(5.5, 1, 0.1);
    glRotatef(180, 0, 0, 1);
    glTranslatef(-1.25, -1.5, -1.25);
    drawTrapezoid(0.412, 0.412, 0.412,  0.0,0.0,0.0);
    //drawTrapezoid(0.4,0.4,0.4, 0.0,0.0,0.0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 1.5, -1.5);
    glScalef(5.5, 1, 0.1);
    glRotatef(180, 0, 0, 1);
    glTranslatef(-1.25, -1.5, -1.25);
    drawTrapezoid(0.412, 0.412, 0.412,  0.0,0.0,0.0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(5.5, 1.5, 0);
    glRotatef(-42, 0, 0, 1);
    glScalef(0.1, 1.3, 1);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.412, 0.412, 0.412,  0.0,0.0,0.0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-8.1, 1.5, 0);
    glRotatef(42, 0, 0, 1);
    glScalef(0.1, 1.3, 1);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.412, 0.412, 0.412,  0.0,0.0,0.0);
    glPopMatrix();
    
    for(float i=-6; i<=2; i+=2)
    {
        glPushMatrix();
        glTranslatef(i, 0, -1.5);
        glScalef(0.1, 1, 1);
        drawCube1(0.412, 0.412, 0.412,  0.0,0.0,0.0);
        glPopMatrix();
    }
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, ID2[3] );
    glPushMatrix();
    glTranslatef(-6.9, 0, 1.7);
    glScalef(3.7, 1, 0.01);
    drawBox();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-6.9, 0, -1.7);
    glScalef(3.7, 1, 0.01);
    drawBox();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void pirateBoat()
{
    glPushMatrix();
    glTranslatef(1, 0, 0);
    glScalef(1.3, 1, 1.3);
    ground2();
    glPopMatrix();
    //base
    //base stand
    glPushMatrix();
    glTranslatef(0.5, -19.5, 0);
    //glRotatef(-105, 0, 0, 1);
    glScalef(6,0.5,4);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.545,0.271,0.075,  0.2725,0.1355,0.0375);
    glPopMatrix();
    
    //translate down
    glPushMatrix();
    glTranslatef(0, -5.5, 0);
    
    glPushMatrix();
    glTranslatef(0, 0, -4);
    glScalef(0.2, 0.2, 0.5);
    drawCylinder(1,0,0, 0.5,0,0.5);
    glPopMatrix();
    
    //boat body
    glPushMatrix();
    glRotatef(pirateBoatTheta, 0, 0, 1);
    glTranslatef(1.5, -12, 0);
    boatBody();
    glPopMatrix();
    
    //stand on the front
    glPushMatrix();
    glTranslatef(0, 0, 1.4);
    glRotatef(pirateBoatTheta, 0, 0, 1);
    drawSphere(0, 0, 1, 0, 0, 0.5);
    //left stand
    glPushMatrix();
    glRotatef(-55, 0, 0, 1);
    glScalef(4,0.28,0.1);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    //right stand
    glPushMatrix();
    glRotatef(-125, 0, 0, 1);
    glScalef(4,0.28,0.1);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    glPopMatrix();
    
    //stand on the back
    glPushMatrix();
    glTranslatef(0, 0, -1.6);
    glRotatef(pirateBoatTheta, 0, 0, 1);
    drawSphere(0, 0, 1, 0, 0, 0.5);
    //left stand
    glPushMatrix();
    glRotatef(-55, 0, 0, 1);
    glScalef(4,0.28,0.1);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    //right stand
    glPushMatrix();
    glRotatef(-125, 0, 0, 1);
    glScalef(4,0.28,0.1);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    glPopMatrix();
    
    //base stand at the front
    glPushMatrix();
    glTranslatef(0, 0, 5);
    drawSphere(0, 0, 1, 0, 0, 0.5);
    //left stand
    glPushMatrix();
    glRotatef(-60, 0, 0, 1);
    glScalef(6,0.28,0.1);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    //right stand
    glPushMatrix();
    glRotatef(-120, 0, 0, 1);
    glScalef(6,0.28,0.1);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.055);
    glPopMatrix();
    glPopMatrix();
    
    //base stand at the back
    glPushMatrix();
    glTranslatef(0, 0, -5);
    drawSphere(0, 0, 1, 0, 0, 0.5);
    //left stand
    glPushMatrix();
    glRotatef(-60, 0, 0, 1);
    glScalef(6,0.28,0.1);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    
    //right stand
    glPushMatrix();
    glRotatef(-120, 0, 0, 1);
    glScalef(6,0.28,0.1);
    drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
    glPopMatrix();
    glPopMatrix();
    
    glPopMatrix();
}

void chair()
{
    //seat part
    glPushMatrix();
    glScalef(0.5, 0.05, 0.5);
    drawCube1(0.8,0.2,0.4,  0.4,0.1,0.2);
    glPopMatrix();
    
    //seat left back
    glPushMatrix();
    glTranslatef(0, -1.5, 0);
    glScalef(0.05, 1.4, 0.05);
    drawCube1(0.8,0.2,0.4,  0.4,0.1,0.2);
    glPopMatrix();
    
    //seat right back
    glPushMatrix();
    glTranslatef(1.35, -1.5, 0);
    glScalef(0.05, 1.4, 0.05);
    drawCube1(0.8,0.2,0.4,  0.4,0.1,0.2);
    glPopMatrix();
    
    //seat horizontal up back
    glPushMatrix();
    glTranslatef(0, 2, 0);
    glScalef(0.5, 0.05, 0.05);
    drawCube1(0.8,0.2,0.4,  0.4,0.1,0.2);
    glPopMatrix();
    
    //seat horizontal up back
    glPushMatrix();
    glTranslatef(0, 1.5 , 0);
    glScalef(0.5, 0.05, 0.05);
    drawCube1(0.8,0.2,0.4,  0.4,0.1,0.2);
    glPopMatrix();
    
    //seat horizontal up back
    glPushMatrix();
    glTranslatef(0, 1, 0);
    glScalef(0.5, 0.05, 0.05);
    drawCube1(0.8,0.2,0.4,  0.4,0.1,0.2);
    glPopMatrix();
    
    //seat left front leg
    glPushMatrix();
    glTranslatef(0, -1.5, 1.3);
    glScalef(0.05, .55, 0.05);
    drawCube1(0.8,0.2,0.4,  0.4,0.1,0.2);
    glPopMatrix();
    
    //seat right front leg
    glPushMatrix();
    glTranslatef(1.35, -1.5, 1.3);
    glScalef(0.05, .55, 0.05);
    drawCube1(0.8,0.2,0.4,  0.4,0.1,0.2);
    glPopMatrix();
    
}

void table()
{
    //table
    glPushMatrix();
    glScalef(4, 0.3, 4);
    drawSphere(0.8,0.4,0.00,  0.4,0.2,0);
    glPopMatrix();
    
    //stand
    glPushMatrix();
    glScalef(0.1, -1, -0.1);
    drawCube1(0,0,0, 0,0,0.5);
    glPopMatrix();
    
    //stand bottom
    glPushMatrix();
    glTranslatef(0, -2.8, 0);
    glScalef(1, 0.2, 1);
    drawSphere(1,0.549,0.00,  0.5,0.2745,0);
    glPopMatrix();
}

void diningSet()
{
    glPushMatrix();
    glTranslatef(0, -16, 0);
    table();
    glPopMatrix();
    
    for(int i = 0; i <= 360; i+=72)
     {
         glPushMatrix();
         glRotatef(i, 0, 1, 0);
         glTranslatef(0, -17, -4);
         chair();
         glPopMatrix();
     }
}

void quad1()
{
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(10,4,3);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0,4,3);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0,0,3);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(10,0,3);
    glEnd();
}

void quad2()
{
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0,8,5);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0,8,0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0,0,0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0,0,5);
    glEnd();
}

void icecreamParlor()
{
    materialProperty();
    glEnable(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D,ID2[16]);
    glPushMatrix();
    quad1();
    glPopMatrix();
        
    glPushMatrix();
    glTranslatef(0, 11, -2);
    glRotatef(90, 1, 0, 0);
    glScalef(1, 1.5, 1);
    quad1();
    glPopMatrix();
        
    glBindTexture(GL_TEXTURE_2D,ID2[17]);
    glPushMatrix();
    glTranslatef(0, 0, -5);
    glScalef(1, 2, 1);
    quad1();
    glPopMatrix();
    
    glBindTexture(GL_TEXTURE_2D,ID2[18]);
        
    glPushMatrix();
    glTranslatef(0, 0, -2);
    quad2();
    glPopMatrix();
        
    glPushMatrix();
    glTranslatef(10, 0, -2);
    quad2();
    glPopMatrix();
        
    glPushMatrix();
    glTranslatef(0, 3, -2);
    glRotatef(90, 1, 0, 0);
    quad1();
    glPopMatrix();
        
    glBindTexture(GL_TEXTURE_2D,ID2[5]);
    glPushMatrix();
    glTranslatef(0, 6, 1);
    glScalef(1, 0.5, 1);
    quad1();
    glPopMatrix();
        
    glDisable(GL_TEXTURE_2D);


}



void pizzaHut()
{
    materialProperty();
    glEnable(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D,ID2[15]);
    glPushMatrix();
    quad1();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 11, -2);
    glRotatef(90, 1, 0, 0);
    glScalef(1, 1.5, 1);
    quad1();
    glPopMatrix();
    
    glBindTexture(GL_TEXTURE_2D,ID2[22]);
    glPushMatrix();
    glTranslatef(0, 0, -5);
    glScalef(1, 2, 1);
    quad1();
    glPopMatrix();
    
    glBindTexture(GL_TEXTURE_2D,ID2[4]);
    glPushMatrix();
    glTranslatef(0, 0, -2);
    quad2();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(10, 0, -2);
    quad2();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 3, -2);
    glRotatef(90, 1, 0, 0);
    quad1();
    glPopMatrix();
    
    glBindTexture(GL_TEXTURE_2D,ID2[6]);
    glPushMatrix();
    glTranslatef(0, 6, 1);
    glScalef(1, 0.5, 1);
    quad1();
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);

}

void dunkinDonuts()
{
    materialProperty();
    glEnable(GL_TEXTURE_2D);
        
    glBindTexture(GL_TEXTURE_2D,ID2[19]);
    glPushMatrix();
    quad1();
    glPopMatrix();
        
    glPushMatrix();
    glTranslatef(0, 11, -2);
    glRotatef(90, 1, 0, 0);
    glScalef(1, 1.5, 1);
    quad1();
    glPopMatrix();
        
    glBindTexture(GL_TEXTURE_2D,ID2[21]);
    glPushMatrix();
    glTranslatef(0, 0, -5);
    glScalef(1, 2, 1);
    quad1();
    glPopMatrix();
    
    glBindTexture(GL_TEXTURE_2D,ID2[20]);
    glPushMatrix();
    glTranslatef(0, 0, -2);
    quad2();
    glPopMatrix();
        
    glPushMatrix();
    glTranslatef(10, 0, -2);
    quad2();
    glPopMatrix();
        
    glPushMatrix();
    glTranslatef(0, 3, -2);
    glRotatef(90, 1, 0, 0);
    quad1();
    glPopMatrix();
        
    glBindTexture(GL_TEXTURE_2D,ID2[7]);
    glPushMatrix();
    glTranslatef(0, 6, 1);
    glScalef(1, 0.5, 1);
    quad1();
    glPopMatrix();
        
    glDisable(GL_TEXTURE_2D);


}

void cafeteriaFence()
{
    for(float i = -12; i <= 24; i+=4)
    {
        glPushMatrix();
        glTranslatef(-15.5, -17, i);
        glScalef(1, 2, 1);
        bush();
        glPopMatrix();
    }
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,ID2[25]);
    
    glPushMatrix();
    glTranslatef(-17, -20, -16.5);
    glScalef(1, 0.5, 15);
    drawBox();
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
}



void cafeteria()
{
    for(float i=-4; i<=32; i+=12)
    {
        for(float j=2; j<=26; j+=12)
        {
            glPushMatrix();
            glTranslatef(i, 10, j);
            glScalef(1, 1.5, 1);
            diningSet();
            glPopMatrix();
        }
    }
    
    glPushMatrix();
    glTranslatef(5, -18, -10);
    glScalef(1.5, 2, 1);
    pizzaHut();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-12, -18, -10);
    glScalef(1.5, 2, 1);
    icecreamParlor();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(22, -18, -10);
    glScalef(1.5, 2, 1);
    dunkinDonuts();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(13, 0, 5);
    glScalef(2.9, 1, 2.4);
    ground2();
    glPopMatrix();
    
    cafeteriaFence();
    
    glPushMatrix();
    glTranslatef(58, 0, 0);
    cafeteriaFence();
    glPopMatrix();
    
}

void complexOrbiterUnit()
{
    glPushMatrix();
    drawSphere(0, 0, 1, 0, 0, 0.5);
    glPopMatrix();

    
    glPushMatrix();
    glRotatef(cmOrbiterTheta, 0, 1, 0);
    
    int j = 0;
    for(int i = 0; i<360; i+=90)
    {
        glPushMatrix();
        glRotatef(i, 0, 1, 0);
        
        glPushMatrix();
        glRotatef(-45, 0, 0, 1);
        glTranslatef(4, 0, -2.4);
        glRotatef(45, 0, 0, 1);
        ferrisWheelSeat();
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0, 0, -0.5);
        glRotatef(-45, 0, 0, 1);
        glScalef(1.6, 0.2, 0.2);
        drawCube1(colors[j][0],colors[j][1],colors[j][2], colors[j][3],colors[j][4],colors[j][5]);
        glPopMatrix();
        
        glPopMatrix();
        
        j++;
    }
    glPopMatrix();
}

void complexOrbiter()
{
    glPushMatrix();
    //glTranslatef(1, 0, 0);
    glScalef(2, 1, 2);
    ground2();
    glPopMatrix();
   /* //the base
    glPushMatrix();
    glTranslatef(0, -19.5, 0);
    glScalef(11,0.5,11);
    glTranslatef(-1.5, -1.5, -1.5);
    //drawBox();
    drawCube1(0.545, 0.271, 0.075,  0.2725,0.1355,0.0375);
    glPopMatrix(); */
    
    //the sphere
    glPushMatrix();
    glTranslatef(0, -1, 0);
    glScalef(2, 2, 2);
    drawSphere(1,0,0 ,0.5,0,0);
    glPopMatrix();
    
    //the cylinder stand
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    drawCylinder(0, 1, 0, 0, 0.5, 0.5);
    glPopMatrix();
        
    //the ride
    glPushMatrix();
    glRotatef(cmOrbiterAlpha, 0, 1, 0);
    
    for(int i=0; i<360; i+=72)
    {
        glPushMatrix();
        glRotatef(i, 0, 1, 0);
        
        glPushMatrix();
        glTranslatef(0, 0, -0.5);
        glRotatef(-45, 0, 0, 1);
        glScalef(5.8, 0.2, 0.2);
        drawCube1(1, 0, 0, 0.5,0,0);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(12, -12, 0);
        complexOrbiterUnit();
        glPopMatrix();
        
        glPopMatrix();
    }
    
    glPopMatrix();
}

void skyDropStructure()
{
    for(float i = 0; i<=90; i+=2.8)
    {
        glPushMatrix();
        glTranslatef(0, i, 0);
        glScalef(0.1, 0.1, -1.5);
        drawCube1(1, 0, 0, 0.5,0,0);
        glPopMatrix();
    }
    
    for(float i = 0; i<=87.2; i+=2.8)
    {
        glPushMatrix();
        glTranslatef(0, i, 0);
        glRotatef(32, 1, 0, 0);
        glScalef(0.1, 0.1, -1.8);
        drawCube1(1, 0, 0, 0.5,0,0);
        glPopMatrix();
    }
    
    
    for(float i = 2.8; i<=90; i+=2.8)
    {
        glPushMatrix();
        glTranslatef(0, i, 0);
        glRotatef(-32, 1, 0, 0);
        glScalef(0.1, 0.1, -1.8);
        drawCube1(1, 0, 0, 0.5,0,0);
        glPopMatrix();
    }
}

void skyDropSeat()
{
    glPushMatrix();
    glTranslatef(-6, 3.2, 1);
    
    glPushMatrix();
    glScalef(6, 1.8, 0.2);
    drawCube1(1,1,0, 0.5,0.5,0);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(6, 0.2, 1.2);
    drawCube1(1,1,0, 0.5,0.5,0.5);
    glPopMatrix();
    
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-0.5, 2, 0);
    glScalef(2.2, 0.4, 0.5);
    drawCube1(1,0,1, 0.5,0.5,0.5);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-0.5, 0.5, 0);
    glScalef(0.3, 3, 0.5);
    drawCube1(0,0,1, 0,0.5,0.5);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(5.2, 0.5, 0);
    glScalef(0.3, 3, 0.5);
    drawCube1(0,0,1, 0,0.5,0.5);
    glPopMatrix();
    
    for(float i = -5.5; i<= 13; i+=2.1)
    {
        glPushMatrix();
        glTranslatef(i, 8, 1.5);
        glRotatef(25, 1, 0, 0);
        glScalef(0.1, 0.1, 0.15);
        drawCylinder(0.412, 0.412, 0.412, 0.2, 0.2, 0.2);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(i, 6.8, 4);
        glRotatef(90, 1, 0, 0);
        glScalef(0.1, 0.1, 0.19);
        drawCylinder(0.412, 0.412, 0.412, 0.2, 0.2, 0.2);
        glPopMatrix();
    }
    
    
}

void skyDropTexture()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, ID2[12]);
    glPushMatrix();
    materialProperty();
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0,0,0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2,0,0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2,20,0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0,20,0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void skyDropLogo()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, ID2[13]);
    glPushMatrix();
    materialProperty();
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0,0,0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(3,0,0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(3,5,0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0,5,0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void skyDrop()
{
    for(float i = 0; i <=5; i+=5)
    {
        glPushMatrix();
        glTranslatef(i, 0, 0);
        glScalef(0.2, 30, 0.2);
        drawCube1(0, 0, 1, 0,0,0.5);
        glPopMatrix();
    }
    
    for(float i = 0; i <=5; i+=5)
    {
        glPushMatrix();
        glTranslatef(i, 0, -5);
        glScalef(0.2, 30, 0.2);
        drawCube1(0, 0, 1, 0,0,0.5);
        glPopMatrix();
    }
    
    
    skyDropStructure();
    
    glPushMatrix();
    glTranslatef(5, 0, 0);
    skyDropStructure();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.7, 0, -5);
    glRotatef(-90, 0, 1, 0);
    skyDropStructure();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, skyDropPos, 0);
    //glTranslatef(-6, 0, 1);
    skyDropSeat();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.8, 0, 0);
    glScalef(2, 4.5, 1);
    skyDropTexture();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-0.5, 90, 1);
    glScalef(2.5, 2, 1);
    skyDropLogo();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(3, 20, 0);
    ground2();
    glPopMatrix();
}

///curve
//const int L=5;
const int nt = 60;                //number of slices along x-direction
const int ntheta = 20;
const double PI = 3.14159265389;



class point1
{
public:
    point1()
    {
        x=0;
        y=0;
    }
    int x;
    int y;
} clkpt[2];

///////////////////////
void setNormal(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(-Nx,-Ny,-Nz);
}

//control points
long long nCr(int n, int r)
{
    if(r > n / 2) r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for(i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

void BezierCurve ( double t,  float xy[2], int L, GLfloat ctrlpoints[L-1][3])
{
    double y=0;
    double x=0;
    t=t>1.0?1.0:t;
    for(int i=0; i<=L; i++)
    {
        int ncr=nCr(L,i);
        double oneMinusTpow=pow(1-t,double(L-i));
        double tPow=pow(t,double(i));
        double coef=oneMinusTpow*tPow*ncr;
        x+=coef*ctrlpoints[i][0];
        y+=coef*ctrlpoints[i][1];

    }
    xy[0] = float(x);
    xy[1] = float(y);

    //return y;
}

void showControlPoints(int L, GLfloat ctrlpoints[L-1][3])
{
    glPointSize(5.0);
    glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_POINTS);
    for (int i = 0; i <=L; i++)
        glVertex3fv(&ctrlpoints[i][0]);
    glEnd();
}

void balloonBezier()
{
    int L = 5;
    GLfloat balloonctrlpoints[6][3] =
    {
        { 0.0, 0.0, 0.0}, {0.7,0.8,0},
        {2,0.9,0}, {2.3,0.5,0},
        {2.5, 0.1,0}, {2.4, 0, 0}
    };
    
    int i, j;
    float x, y, z, r;                //current coordinates
    float x1, y1, z1, r1;            //next coordinates
    float theta;

    const float startx = 0, endx = balloonctrlpoints[L][0];
    //number of angular slices
    const float dx = (endx - startx) / nt;    //x step size
    const float dtheta = 2*PI / ntheta;        //angular step size

    float t=0;
    float dt=1.0/nt;
    float xy[2];
    BezierCurve( t,  xy, 5, balloonctrlpoints);
    x = xy[0];
    r = xy[1];
    //rotate about z-axis
    float p1x,p1y,p1z,p2x,p2y,p2z;
    for ( i = 0; i < nt; ++i )              //step through x
    {
        theta = 0;
        t+=dt;
        BezierCurve( t,  xy, 5, balloonctrlpoints);
        x1 = xy[0];
        r1 = xy[1];

        //draw the surface composed of quadrilaterals by sweeping theta
        glBegin( GL_QUAD_STRIP );
        for ( j = 0; j <= ntheta; ++j )
        {
            theta += dtheta;
            double cosa = cos( theta );
            double sina = sin ( theta );
            y = r * cosa;
            y1 = r1 * cosa;    //current and next y
            z = r * sina;
            z1 = r1 * sina;    //current and next z

            //edge from point at x to point at next x
            glVertex3f (x, y, z);

            if(j>0)
            {
                setNormal(p1x,p1y,p1z,p2x,p2y,p2z,x, y, z);
            }
            else
            {
                p1x=x;
                p1y=y;
                p1z=z;
                p2x=x1;
                p2y=y1;
                p2z=z1;

            }
            glVertex3f (x1, y1, z1);

            //forms quad with next pair of points with incremented theta value
        }
        glEnd();
        x = x1;
        r = r1;
    } //for i
    
    if(show == true)
    {
        showControlPoints(L, balloonctrlpoints);
    }
}

void drawFlag()
{
    int L = 3;
    GLfloat ctrlpoints1[4][3] =
   {
       {0,0,0}, {3.5,0+yf,0}, {3.5,0-yf,0}, {7+xf,0,0}
   };

   GLfloat ctrlpoints2[4][3] =
   {
       {0,5,0}, {3.5,5+yf,0}, {3.5,5-yf,0}, {7+xf,5,0}
   };
    int i;
    float x1, y1, x2, y2;                //current coordinates
    //float x1, y1, z1, r1;            //next coordinates
    const float startx = 0, endx = ctrlpoints1[L][0];
    float t=0;
    float xy1[2];
    float xy2[2];
    float dt=1.0/nt;
    //glColor3f(0, 0, 1);
    glBegin(GL_QUAD_STRIP);
    for(i = 0; i < nt; i++)
    {
        BezierCurve( t,  xy1, L, ctrlpoints1);
        x1 = xy1[0];
        y1 = xy1[1];
        glVertex3f (x1, y1, 0);
        BezierCurve( t,  xy2, L, ctrlpoints2);
        x2 = xy2[0];
        y2 = xy2[1];
        glVertex3f(x2, y2, 0);
        t+=dt;
    }
    
    glEnd();
    
    if(show == true)
    {
        showControlPoints(L, ctrlpoints1);
        showControlPoints(L, ctrlpoints2);
    }
}

void balloon()
{
    glPushMatrix();
    glRotatef(90, 0, 0, 1);
    glScalef(2, 2.5, 2);
    balloonBezier();
    glPopMatrix();
    
}

void balloonLine()
{
    matCurve(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(1, 10);
    glVertex2f(1, 1);
    glEnd();
}

void balloons()
{
    matCurve(1, 0, 0);
    glPushMatrix();
    glTranslatef(3, -10, 0.6);
    balloon();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(1.8, -19, 0.6);
    balloonLine();
    glPopMatrix();
    
    matCurve(0, 0, 1);
    glPushMatrix();
    glTranslatef(6.2, -11, 0.6);
    balloon();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(1.8, -18, 0.6);
    glRotatef(-22, 0, 0, 1);
    balloonLine();
    glPopMatrix();

    
    matCurve(1, 1, 0);
    glPushMatrix();
    glTranslatef(0, -11, 0.6);
    balloon();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(1.8, -18, 0.6);
    glRotatef(22, 0, 0, 1);
    balloonLine();
    glPopMatrix();
    
    matCurve(1, 0.5, 0);
    glPushMatrix();
    glTranslatef(3, -13, 2.6);
    balloon();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(1.8, -20, 0.6);
    glRotatef(16, 1, 0, 0);
    balloonLine();
    glPopMatrix();
    
    matCurve(1, 0, 1);
    glPushMatrix();
    glTranslatef(3, -13, -1.8);
    balloon();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(1.8, -20, 0.6);
    glRotatef(-14, 1, 0, 0);
    balloonLine();
    glPopMatrix();
    
}

void cart()
{
    materialProperty();
    glEnable(GL_TEXTURE_2D);
        
    glBindTexture(GL_TEXTURE_2D,ID2[26]);
    glPushMatrix();
    glScalef(1, 2, 1);
    quad1();
    glPopMatrix();
        
    glPushMatrix();
    glTranslatef(0, 0, -5);
    glScalef(1, 2, 1);
    quad1();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 0, -2);
    quad2();
    glPopMatrix();
        
    glPushMatrix();
    glTranslatef(10, 0, -2);
    quad2();
    glPopMatrix();
        
    glBindTexture(GL_TEXTURE_2D,ID2[4]);
    glPushMatrix();
    glTranslatef(0, 11, -2);
    glRotatef(90, 1, 0, 0);
    glScalef(1, 1.25, 1);
    quad1();
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
}

void balloonCart()
{
    balloons();
    
    glPushMatrix();
    glTranslatef(0, -20, 0);
    glScalef(0.5, 0.5, 1);
    cart();
    glPopMatrix();
}

void flagpole()
{
    glPushMatrix();
    glTranslatef(0, 5.5, 0);
    glRotatef(90, 1, 0, 0);
    glScalef(0.2, 0.2, 1.5);
    drawCylinder(1, 0, 0, 0.5, 0, 0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 5.5, 0);
    glScalef(0.5, 0.5, 0.5);
    drawSphere(.502, 0, 0, 0.26, 0, 0);
    glPopMatrix();
    
    matCurve(0, 0, 1);
    drawFlag();
}

void flagpole1()
{
    glPushMatrix();
    glTranslatef(0, 5.5, 0);
    glRotatef(90, 1, 0, 0);
    glScalef(0.2, 0.2, 1.5);
    drawCylinder(1, 0, 0, 0.5, 0, 0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 5.5, 0);
    glScalef(0.5, 0.5, 0.5);
    drawSphere(.502, 0, 0, 0.26, 0, 0);
    glPopMatrix();
    
    matCurve(1, 1, 0);
    drawFlag();
}

void flagpole2()
{
    glPushMatrix();
    glTranslatef(0, 5.5, 0);
    glRotatef(90, 1, 0, 0);
    glScalef(0.2, 0.2, 1.5);
    drawCylinder(1, 0, 0, 0.5, 0, 0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 5.5, 0);
    glScalef(0.5, 0.5, 0.5);
    drawSphere(.502, 0, 0, 0.26, 0, 0);
    glPopMatrix();
    
    matCurve(1, 0.5, 0);
    drawFlag();
}

void flagpole3()
{
    glPushMatrix();
    glTranslatef(0, 5.5, 0);
    glRotatef(90, 1, 0, 0);
    glScalef(0.2, 0.2, 1.5);
    drawCylinder(1, 0, 0, 0.5, 0, 0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 5.5, 0);
    glScalef(0.5, 0.5, 0.5);
    drawSphere(.502, 0, 0, 0.26, 0, 0);
    glPopMatrix();
    
    matCurve(1, 0, 1);
    drawFlag();
}



void trees()
{
    for(float i= -40; i<=55; i+=15)
    {
        glPushMatrix();
        glTranslatef(-70, -2, i);
        tree();
        glPopMatrix();
    }
    
    for(float i= -40; i<=55; i+=15)
    {
        glPushMatrix();
        glTranslatef(100, -2, i);
        tree();
        glPopMatrix();
    }
}

void wall()
{
    materialProperty();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,ID2[31]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(10,4,3);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0,4,3);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0,0,3);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(10,0,3);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void walls()
{
    for(float i=20; i<=100; i+=10)
    {
        glPushMatrix();
        glTranslatef(i, -20, 60);
        wall();
        glPopMatrix();
    }
    
    for(float i=-80; i<=-10; i+=10)
    {
        glPushMatrix();
        glTranslatef(i, -20, 60);
        wall();
        glPopMatrix();
    }
    
    for(float i=-80; i<=100; i+=10)
    {
        glPushMatrix();
        glTranslatef(i, -20, -70);
        wall();
        glPopMatrix();
    }
    
    for(float i = -57; i<=63; i+=10)
    {
        glPushMatrix();
        glTranslatef(-83, -20, i);
        glRotatef(90, 0, 1, 0);
        wall();
        glPopMatrix();
    }
    
    for(float i = -57; i<=63; i+=10)
    {
        glPushMatrix();
        glTranslatef(107, -20, i);
        glRotatef(90, 0, 1, 0);
        wall();
        glPopMatrix();
    }
    
    
}

void streetLampbody()
{
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glScalef(0.5, 0.5, 1);
    drawCylinder(0.1, 0.1, 0.1, 0.05, 0.05, 0.05);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, -19, 0);
    glRotatef(90, 1, 0, 0);
    glScalef(0.2, 0.2, 0.2);
    drawTorus(0.1,0.1,0.1, 0.05,0.05,0.05,  2,5,32,64);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glRotatef(90, 1, 0, 0);
    glScalef(0.2, 0.2, 0.2);
    drawTorus(0.1,0.1,0.1, 0.05,0.05,0.05,  2,5,32,64);
    glPopMatrix();
}

void spotLight1()
{
    glPushMatrix();
    
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {0.5, 0.5, 0.5, 1.0};
    GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { -5.0, 30, 0.0, 1.0 };  //0.7, 4.5, 9.0
    
    //glEnable( GL_LIGHT1);
    
  /*  if(amb3 == true){glLightfv( GL_LIGHT2, GL_AMBIENT, light_ambient);}
    else{glLightfv( GL_LIGHT2, GL_AMBIENT, no_light);}
    
    if(diff3 == true){glLightfv( GL_LIGHT2, GL_DIFFUSE, light_diffuse);}
    else{glLightfv( GL_LIGHT2, GL_DIFFUSE, no_light);}
    
    if(spec3 == true){glLightfv( GL_LIGHT2, GL_SPECULAR, light_specular);}
    else{glLightfv( GL_LIGHT2, GL_SPECULAR, no_light);} */
    
    glLightfv( GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv( GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv( GL_LIGHT1, GL_SPECULAR, light_specular);

    glLightfv( GL_LIGHT1, GL_POSITION, light_position);
    GLfloat spot_direction[] = { -0.5, -1, 0 };
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf( GL_LIGHT1, GL_SPOT_CUTOFF, 70.0);
    glPopMatrix();
}

void streetLight1()
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
    GLfloat mat_diffuse[] = { 1.000, 0.843, 0.000, 1.0 };
    GLfloat high_shininess[] = { 100.0 };
    GLfloat mat_emission[] = {1,1,1, 1.0};
    
    glPushMatrix();
    glTranslatef (0,2,0);
    glScalef(2, 2, 2);
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    if(switchOne == true){
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    }else{
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    }
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();
    
    streetLampbody();
}

void spotLight2()
{
    glPushMatrix();
    
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {0.5, 0.5, 0.5, 1.0};
    GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 25.0, 30, 0.0, 1.0 };  //0.7, 4.5, 9.0
    
    //glEnable( GL_LIGHT2);
    
  /*  if(amb3 == true){glLightfv( GL_LIGHT2, GL_AMBIENT, light_ambient);}
    else{glLightfv( GL_LIGHT2, GL_AMBIENT, no_light);}
    
    if(diff3 == true){glLightfv( GL_LIGHT2, GL_DIFFUSE, light_diffuse);}
    else{glLightfv( GL_LIGHT2, GL_DIFFUSE, no_light);}
    
    if(spec3 == true){glLightfv( GL_LIGHT2, GL_SPECULAR, light_specular);}
    else{glLightfv( GL_LIGHT2, GL_SPECULAR, no_light);} */
    
    glLightfv( GL_LIGHT2, GL_AMBIENT, light_ambient);
    glLightfv( GL_LIGHT2, GL_DIFFUSE, light_diffuse);
    glLightfv( GL_LIGHT2, GL_SPECULAR, light_specular);

    glLightfv( GL_LIGHT2, GL_POSITION, light_position);
    GLfloat spot_direction[] = { 1, -1, 0.5 };
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glLightf( GL_LIGHT2, GL_SPOT_CUTOFF, 70.0);
    glPopMatrix();
}

void streetLight2()
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
    GLfloat mat_diffuse[] = { 1.000, 0.843, 0.000, 1.0 };
    GLfloat high_shininess[] = { 100.0 };
    GLfloat mat_emission[] = {1,1,1, 1.0};
    
    glPushMatrix();
    glTranslatef (0,2,0);
    glScalef(2, 2, 2);
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    if(switchTwo == true){
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    }else{
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    }
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();
    
    streetLampbody();
}

void spotLight3()
{
    glPushMatrix();
    
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {0.5, 0.5, 0.5, 1.0};
    GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 25.0, 30, 10.0, 1.0 };  //0.7, 4.5, 9.0
    
    //glEnable( GL_LIGHT3);
    
  /*  if(amb3 == true){glLightfv( GL_LIGHT2, GL_AMBIENT, light_ambient);}
    else{glLightfv( GL_LIGHT2, GL_AMBIENT, no_light);}
    
    if(diff3 == true){glLightfv( GL_LIGHT2, GL_DIFFUSE, light_diffuse);}
    else{glLightfv( GL_LIGHT2, GL_DIFFUSE, no_light);}
    
    if(spec3 == true){glLightfv( GL_LIGHT2, GL_SPECULAR, light_specular);}
    else{glLightfv( GL_LIGHT2, GL_SPECULAR, no_light);} */
    
    glLightfv( GL_LIGHT3, GL_AMBIENT, light_ambient);
    glLightfv( GL_LIGHT3, GL_DIFFUSE, light_diffuse);
    glLightfv( GL_LIGHT3, GL_SPECULAR, light_specular);

    glLightfv( GL_LIGHT3, GL_POSITION, light_position);
    GLfloat spot_direction[] = { 1, -1, 0 };
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction);
    glLightf( GL_LIGHT3, GL_SPOT_CUTOFF, 60.0);
    glPopMatrix();
}

void streetLight3()
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
    GLfloat mat_diffuse[] = { 1.000, 0.843, 0.000, 1.0 };
    GLfloat high_shininess[] = { 100.0 };
    GLfloat mat_emission[] = {1,1,1, 1.0};
    
    glPushMatrix();
    glTranslatef (0,2,0);
    glScalef(2, 2, 2);
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    if(switchThree == true){
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    }else{
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    }
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();
    
    streetLampbody();
}

void spotLight4()
{
    glPushMatrix();
    
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {0.5, 0.5, 0.5, 1.0};
    GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { -20.0, 30, 10.0, 1.0 };  //0.7, 4.5, 9.0
    
    //glEnable( GL_LIGHT4);
    
  /*  if(amb3 == true){glLightfv( GL_LIGHT2, GL_AMBIENT, light_ambient);}
    else{glLightfv( GL_LIGHT2, GL_AMBIENT, no_light);}
    
    if(diff3 == true){glLightfv( GL_LIGHT2, GL_DIFFUSE, light_diffuse);}
    else{glLightfv( GL_LIGHT2, GL_DIFFUSE, no_light);}
    
    if(spec3 == true){glLightfv( GL_LIGHT2, GL_SPECULAR, light_specular);}
    else{glLightfv( GL_LIGHT2, GL_SPECULAR, no_light);} */
    
    glLightfv( GL_LIGHT4, GL_AMBIENT, light_ambient);
    glLightfv( GL_LIGHT4, GL_DIFFUSE, light_diffuse);
    glLightfv( GL_LIGHT4, GL_SPECULAR, light_specular);

    glLightfv( GL_LIGHT4, GL_POSITION, light_position);
    GLfloat spot_direction[] = { -0.5, -1, 0 };
    glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spot_direction);
    glLightf( GL_LIGHT4, GL_SPOT_CUTOFF, 60.0);
    glPopMatrix();
}

void streetLight4()
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
    GLfloat mat_diffuse[] = { 1.000, 0.843, 0.000, 1.0 };
    GLfloat high_shininess[] = { 100.0 };
    GLfloat mat_emission[] = {1,1,1, 1.0};
    
    glPushMatrix();
    glTranslatef (0,2,0);
    glScalef(2,2,2);
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    if(switchFour == true){
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    }else{
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    }
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();
    
    streetLampbody();
}

void bench1()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,ID2[16]);
    glScalef(2, 0.5, 0.5);
    drawBox();
    glDisable(GL_TEXTURE_2D);
}

void bench2()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,ID2[19]);
    glScalef(2, 0.5, 0.5);
    drawBox();
    glDisable(GL_TEXTURE_2D);
}



void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(60,1,1,300);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt(eyeX,eyeY,eyeZ,  refX,refY,refZ,  0,1,0);

    //glViewport(0, 0, 800, 600);
    
    
    glEnable(GL_LIGHTING);
    
    glPushMatrix();
    sky(eyeX+(0.05*refX),eyeY+(0.05*refY),eyeZ+(0.05*refZ),250,250,250);
    glPopMatrix();
    
    glEnable( GL_DEPTH_TEST );
    
    spotLight1();
    spotLight2();
    spotLight3();
    spotLight4();
    ground();
    walls();
    trees();
    
    for(float i = -70; i<=-10; i+=20)
    {
        glPushMatrix();
        glTranslatef(i, -20, 55);
        bench1();
        glPopMatrix();
    }
    
    for(float i = -60; i<=-20; i+=20)
    {
        glPushMatrix();
        glTranslatef(i, -20, 55);
        bench2();
        glPopMatrix();
    }
    
    for(float i = 30; i<=100; i+=20)
    {
        glPushMatrix();
        glTranslatef(i, -20, 55);
        bench1();
        glPopMatrix();
    }
    
    for(float i = 40; i<=90; i+=20)
    {
        glPushMatrix();
        glTranslatef(i, -20, 55);
        bench2();
        glPopMatrix();
    }
    
  /*  glPushMatrix();
    glTranslatef(-40, -20, 55);
    bench1();
    glPopMatrix(); */
    
    glPushMatrix();
    glTranslatef(-25, 0, 0);
    streetLight1();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(50, 0, 0);
    streetLight2();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(50, 0, 30);
    streetLight3();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-25, 0, 30);
    streetLight4();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 0, 10);
    cafeteria();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(65, 0, -30);
    ferrisWheel();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(75, 0, 20);
    orbiter();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-50, 0, -30);
    complexOrbiter();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-50, 0, 10);
    pirateBoat();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-20, -20, -40);
    skyDrop();
    glPopMatrix();
 
    glPushMatrix();
    glTranslatef(-30, 0, 40);
    balloonCart();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-60, 0, 40);
    balloonCart();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(45, 0, 48);
    balloonCart();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(80, 0, 48);
    balloonCart();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(15, 0, -3);
    flagpole();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(5, 0, -3);
    flagpole1();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(25, 0, -3);
    flagpole2();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-5, 0, -3);
    flagpole3();
    glPopMatrix();
    
    glDisable(GL_LIGHTING);
        
    glFlush();
    glutSwapBuffers();
}

void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
        case 'w': // move eye point upwards along Y axis
            eyeY+=1.0;
            break;
        case 's': // move eye point downwards along Y axis
            eyeY-=1.0;
            break;
        case 'a': // move eye point left along X axis
            eyeX-=1.0;
            break;
        case 'd': // move eye point right along X axis
            eyeX+=1.0;
            break;
        case 'o':  //zoom out
            eyeZ+=1;
            break;
        case 'i': //zoom in
            eyeZ-=1;
            break;
        case 'q': //back to default eye point and ref point
            eyeX=0.0; eyeY=2.0; eyeZ=30.0;
            refX=0.0; refY=0.0; refZ=0.0;
            break;
    case 'j': // move ref point upwards along Y axis
        refY+=1.0;
        break;
    case 'n': // move ref point downwards along Y axis
        refY-=1.0;
        break;
    case 'b': // move ref point left along X axis
        refX-=1.0;
        break;
    case 'm': // move eye point right along X axis
        refX+=1.0;
        break;
    case 'k':  //move ref point away from screen/ along z axis
        refZ+=1;
        break;
    case 'l': //move ref point towards screen/ along z axis
        refZ-=1;
        break;
        case '1': //orbiter
            if(orbiterFlag == false) {
                orbiterFlag = true; break;
            }
            else if( orbiterFlag == true) {
                orbiterFlag = false; break;
            }
        case '2': //ferriswheel
            if(fanSwitch == false) {
                fanSwitch = true; break;
            }
            else{
                fanSwitch = false; break;
            }
        case '3': //skydrop
            if(skyDropFlag == false) {
                skyDropFlag = true; break;
            }
            else if( skyDropFlag == true) {
                skyDropFlag = false; break;
            }
        case '4': //complex orbiter
            if(cmOrbiterFlag == false) {
                cmOrbiterFlag = true; break;
            }
            else if( cmOrbiterFlag == true) {
                cmOrbiterFlag = false; break;
            }
        case '5': //pirate boat
            if(pirateBoatFlag == false) {
                pirateBoatFlag = true; break;
            }
            else if( pirateBoatFlag == true) {
                pirateBoatFlag = false; break;
            }
        case '6': //spot light 1
            if(switchOne == false) {
                switchOne = true; glEnable( GL_LIGHT1); break;
            }
            else if( switchOne == true) {
                switchOne = false; glDisable( GL_LIGHT1); break;
            }
        case '7': //spot light 2
            if(switchTwo == false) {
                switchTwo = true; glEnable( GL_LIGHT2); break;
            }
            else if( switchTwo == true) {
                switchTwo = false; glDisable(GL_LIGHT2); break;
            }
        case '8': //spot light 3
            if(switchThree == false) {
                switchThree = true; glEnable( GL_LIGHT3); break;
            }
            else if( switchThree == true) {
                switchThree = false; glDisable( GL_LIGHT3); break;
            }
        case '9': //spot light 4
            if(switchFour == false) {
                switchFour = true; glEnable( GL_LIGHT4); break;
            }
            else if( switchFour == true) {
                switchFour = false; glDisable( GL_LIGHT4); break;
            }
        case 'z': //turn on/off fan
            if(show == false) {
                show = true; break;
            }
            else if( show == true) {
                show = false; break;
            }
        case 'g': //turn on/off fan
            if(door1 == false) {
                door1 = true; break;
            }
            else{
                door1 = false; break;
            }
        
        
        case '0': //day/night
            if(day == false) {
                day = true; glEnable( GL_LIGHT0); break;
            }
            else if( day == true) {
                day = false; glDisable( GL_LIGHT0); break;
            }
    case 27:    // Escape key
        exit(1);
    }
    
    glutPostRedisplay();
}


void animate()
{
    if(skyDropFlag == true)
    {
        if(upFlag == true)
        {
            skyDropPos += 0.5;
            if(skyDropPos >= 80)
            {
                upFlag = false;
            }
        }
        else
        {
            if(downFlag1 == true && downFlag2 == false && downFlag3 == false)
            {
                skyDropPos -= 2;
                if(skyDropPos <= 5)
                {
                    upFlag = true;
                    downFlag1 = false;
                    downFlag2 = true;
                    downFlag3 = false;
                }
            }
            else if(downFlag1 == false && downFlag2 == true && downFlag3 == false)
            {
                skyDropPos -= 3;
                if(skyDropPos <= 45)
                {
                    upFlag = true;
                    downFlag1 = false;
                    downFlag2 = false;
                    downFlag3 = true;
                }
            }
            else if(downFlag1 == false && downFlag2 == false && downFlag3 == true)
            {
                skyDropPos -= 4;
                if(skyDropPos <= 15)
                {
                    upFlag = true;
                    downFlag1 = true;
                    downFlag2 = false;
                    downFlag3 = false;
                }
            }
            
        }
        
    }
    else
    {
        skyDropPos -= 2;
        if(skyDropPos <= 2 )
        {
            skyDropPos = 2;
        }
    }

    
    
    if(cmOrbiterFlag == true)
    {
        cmOrbiterTheta += 10;
        cmOrbiterAlpha += 1;
    }

    if(pirateBoatFlag == true)
    {
        if(pirateBoatCheck == true)
        {
            pirateBoatTheta += 2;
            if(pirateBoatTheta == 60)
            {
                pirateBoatCheck = false;
            }
        }
        else
        {
            pirateBoatTheta -= 2;
            if(pirateBoatTheta == -70)
            {
                pirateBoatCheck = true;
            }
        }
    }
    else
    {
        if(pirateBoatTheta < 0)
        {
            pirateBoatTheta += 1;
            if(pirateBoatTheta == 0)
            {
                pirateBoatTheta = 0;
            }
        }
        else if(pirateBoatTheta > 0)
        {
            pirateBoatTheta -= 1;
            if(pirateBoatTheta == 0)
            {
                pirateBoatTheta = 0;
            }
        }
    }

    if (fanSwitch == true)
    {
        theta += 2;
        if(theta > 360.0)
            theta -= 360.0*floor(theta/360.0);
    }
    
    if(orbiterFlag == true)
    {
        orbiterTheta += 3;
        if(orbiterTheta > 360.0)
            orbiterTheta -= 360.0*floor(theta/360.0);
        
            orbiterAlpha += 2;
         if (orbiterAlpha >= 45)
             orbiterAlpha = 45;
    }
    else
    {
        orbiterAlpha -= 1;
        if (orbiterAlpha <= -45)
            orbiterAlpha = -45;
        
        orbiterTheta += 3;
        if(orbiterAlpha == -45)
            orbiterTheta = 0;
    }

    if (door1 == true)
    {
        alpha += 10;
        if(alpha > 90)
            alpha = 90;
    }
    else if(door1 == false)
    {
        alpha -= 10;
        if(alpha < 0)
            alpha = 0;
    }
    
    if(yflag == true)
    {
        yf += 0.1;
        if(yf >= 2)
        {
            yflag = false;
        }
    }
    else
    {
        yf -= 0.1;
        if(yf <= -2)
        {
            yflag = true;
        }
    }
    
    if(xflag == true)
    {
        xf += 0.1;
        if(xf >= 0.6)
        {
            xflag = false;
        }
    }
    else
    {
        xf -= 0.1;
        if(xf <= -0.6)
        {
            xflag = true;
        }
    }
    
    glutPostRedisplay();

}

void fullScreen(int w, int h)
{
    //Prevent a divide by zero, when window is too short;you cant make a window of zero width.
    if (h == 0)
        h = 1;
    float ratio = (GLfloat)w / (GLfloat)h;         //Calculate aspect ratio of the window

    //Set the perspective coordinate system
    glMatrixMode(GL_PROJECTION);                   //Use the Projection Matrix
    glLoadIdentity();                              //Reset Matrix

    glViewport(0, 0, w, h);                        //Set the viewport to be the entire window
    gluPerspective(60, ratio, 1, 500);             //Set the correct perspective.
    //glFrustum(-2.5,2.5,-2.5,2.5, ratio, 200);
    glMatrixMode(GL_MODELVIEW);                    //Get Back to the Modelview
}


int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(100,100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Demo");

    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_NORMALIZE);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_BLEND);
   // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    light();
    
    cout<<"To move Eye point:"<<endl;
    cout<<"w: up"<<endl;
    cout<<"s: down"<<endl;
    cout<<"a: left"<<endl;
    cout<<"d: right"<<endl;
    cout<<"i: zoom in"<<endl;
    cout<<"o: zoom out"<<endl;
    cout<<"      "<<endl;
    cout<<"To move Camera point:"<< endl;
    cout<<"j: up"<<endl;
    cout<<"n: down"<<endl;
    cout<<"b: left"<<endl;
    cout<<"m: right"<<endl;
    cout<<"l: move nearer"<<endl;
    cout<<"k: move far"<<endl;
    cout<<"      "<<endl;
    cout<<"Press q to move to default position"<<endl;
    cout<<"      "<<endl;
    cout<<"To control Rides: "<<endl;
    cout<<"1: Orbiter"<<endl;
    cout<<"2: Ferris Wheel"<<endl;
    cout<<"3: Sky Drop"<<endl;
    cout<<"4: Complex Orbiter"<<endl;
    cout<<"5: Pirate Boat"<<endl;
    cout<<"      "<<endl;
    cout<<"To control lights: "<<endl;
    cout<<"6: Spotlight 1"<<endl;
    cout<<"7: Spotlight 2"<<endl;
    cout<<"8: Spotlight 3"<<endl;
    cout<<"9: Spotlight 4"<<endl;
    cout<<"      "<<endl;
    cout<<"Other controls: "<<endl;
    cout<<"0: Day/Night"<<endl;
    cout<<"Z: To show control points"<<endl;
    
    LoadTexture2("sgi imageswhiteground.sgi",2);
    
    LoadTexture2("sgi images/whiteground.sgi",3);
    
    LoadTexture2("sgi images/pirate-copy.sgi",4);
    
    LoadTexture2("sgi images/red-metal.sgi",5);
    
    LoadTexture2("sgi images/baskinrobbinslogo.sgi",6);
    
    LoadTexture2("sgi images/pizzahut.sgi",7);
    
    LoadTexture2("sgi images/dunkindonuts.sgi",8);
    
    LoadTexture2("sgi images/front.sgi",9);
    
    LoadTexture2("sgi images/back.sgi",10);
    
    LoadTexture2("sgi images/right.sgi",11);
    
    LoadTexture2("sgi imagesleft.sgi",12);
    
    LoadTexture2("sgi images/skydrop.sgi", 13);
    
    LoadTexture2("sgi images/skydroplogo.sgi", 14);
    
    LoadTexture2("sgi images/grass.sgi", 15);
    
    LoadTexture2("sgi images/blackred.sgi", 16);
    
    LoadTexture2("sgi images/pinkblue.sgi", 17);
    
    LoadTexture2("sgi images/baskin-robin-ad.sgi", 18);
    
    LoadTexture2("sgi images/de5b9e.sgi", 19);
    
    LoadTexture2("sgi images/pinkorange.sgi", 20);
    
    LoadTexture2("sgi images/ff6d0d.sgi", 21);
    
    LoadTexture2("sgi images/dd.sgi", 22);
    
    LoadTexture2("sgi images/pizzahutad.sgi", 23);
    
    LoadTexture2("sgi images/bush.sgi", 24);
    
    LoadTexture2("sgi images/bushflower.sgi", 25);
    
    LoadTexture2("sgi images/brickwall.sgi", 26);
    
    LoadTexture2("sgi images/redwhite.sgi", 27);
    
    LoadTexture2("sgi images/up.sgi", 28);
    
    LoadTexture2("sgi imagesnightsky.sgi", 29);
    
    LoadTexture2("sgi images/treebark.sgi", 30);
    
    LoadTexture2("sgi images/bush.sgi", 31);
    
    LoadTexture2("sgi images/purplewall.sgi", 32);
    
/* This portion is written for Windows.
  
 LoadTexture("whiteground.bmp",2);
 LoadTexture("pirate-copy.bmp",3);
 LoadTexture("red-metal.bmp",4);
 LoadTexture("baskinrobbinslogo.bmp",5);
 LoadTexture("pizzahut.bmp",6);
 LoadTexture("dunkindonuts.bmp",7);
 LoadTexture("front.bmp",8);
 LoadTexture("back.bmp",9);
 LoadTexture("right.bmp",10);
 LoadTexture("left.bmp",11);
 LoadTexture("skydrop.bmp", 12);
 LoadTexture("skydroplogo.bmp", 13);
 LoadTexture("grass.bmp", 14);
 LoadTexture("blackred.bmp", 15);
 LoadTexture("pinkblue.bmp", 16);
 LoadTexture("baskin-robin-ad.bmp", 17);
 LoadTexture("de5b9e.bmp", 18);
 LoadTexture("pinkorange.bmp", 19);
 LoadTexture("ff6d0d.bmp", 20);
 LoadTexture("dd.bmp", 21);
 LoadTexture("pizzahutad.bmp", 22);
 LoadTexture("bush.bmp", 23);
 LoadTexture("bushflower.bmp", 24);
 LoadTexture("brickwall.bmp", 25);
 LoadTexture("redwhite.bmp", 26);
 LoadTexture("up.bmp", 27);
 LoadTexture("nightsky.bmp", 28);
 LoadTexture("treebark.bmp", 29);
 LoadTexture("bush.bmp", 30);
 LoadTexture("purplewall.bmp", 31);
 */
    
    glutReshapeFunc(fullScreen);
    glutKeyboardFunc(myKeyboardFunc);
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutMainLoop();

    return 0;
}
