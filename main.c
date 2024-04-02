#include "headers.h"

int fth = 0;    //View angle for first person
int fov = 90;
double fEx=50;
double fEz=50;
double dx = 0;
double dz = 0;
double asp = 1;
double dim = 20.0;
double dt = 0.2;
int len;

int move = 1;
int zh = 60;

// Lighting
const float Emission[]  = {0.0,0.0,0.0,1.0};
const float Ambient[]   = {0.15,0.15,0.15,1.0};
const float Diffuse[]   = {1.0,1.0,1.0,1.0};
const float Specular[]  = {0.2,0.2,0.2,0.2};
const float Shinyness[] = {.1};
//  Transformation matrixes
int local     =   0;  // Local Viewer Model

GLuint texture[2];

int shaders[2] = {0,0};



float Position[]  = {25,50,25,1};


/*
###################################
Functions for objects in scene
###################################
*/

/*
Ground
*/

// Initialize array and get height values

double ground[100][100];
double normals[100][100][3];
double trees[100][100][3];

void terrain()
{  
    // Calculate normals
    groundNormals(ground, 100, normals);
    glPushMatrix();

    glColor3f(.4, .6, .5);
    glUseProgram(shaders[1]);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    for(int i = 0; i < 99; i++)
    {
        for(int k = 0; k < 99; k++)
        {
            // Changed to quads for easier textures.
            glBegin(GL_QUADS);
            
            glNormal3f(normals[i][k][0],normals[i][k][1],normals[i][k][2]);
            glTexCoord2f(0, 0);
            glVertex3d(i, ground[i][k], k);
            
            
            glNormal3f(normals[i][k+1][0],normals[i][k+1][1],normals[i][k+1][2]);
            glTexCoord2f(0, .3);
            glVertex3d(i, ground[i][k+1], (k+1)); 

            
            glNormal3f(normals[i+1][k+1][0],normals[i+1][k+1][1],normals[i+1][k+1][2]);
            glTexCoord2f(.3, .3);
            glVertex3d((i+1), ground[i+1][k+1], (k+1));

            
            glNormal3f(normals[i+1][k][0],normals[i+1][k][1],normals[i+1][k][2]);
            glTexCoord2f(.3, 0);
            glVertex3d((i+1), ground[i+1][k], k);
            glEnd();
            
        }
    }

    glUseProgram(shaders[0]);
    glDisable(GL_TEXTURE_2D);
    
    //  Undo transformations
    glPopMatrix();
    
}

/*
Trees
*/
// Function to build the triangle fan for trees
static void TrunkVertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph)*.2 ;
   double y = 0;
   double z = Cos(th)*Cos(ph)*.2;
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

// Function to build the triangle fan for leaves
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = .75;
   double z = Cos(th)*Cos(ph);
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

// Function to build trunk of the trees
static void trunk(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
    int d = 60;
    //Save transformation
    glPushMatrix();

    // Transform matrix
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);

    // Begin Drawing triangles

    glUseProgram(shaders[1]);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    //  South pole cap
    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLE_STRIP);
    
    for (int th=0;th<=360;th+=d)
    {
        glTexCoord2f(1, .5);
        glVertex3f(0,2,0);
        glTexCoord2f(0, 0);
        TrunkVertex(th,d-90);
        glTexCoord2f(1, 1);
        TrunkVertex(th+d,d-90);
    }


    //  End
    glEnd();
    glUseProgram(shaders[0]);
    glDisable(GL_TEXTURE_2D);
   //  Undo transformations
   glPopMatrix();
}

// Function to make leaves of tree
static void leaves(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
    int d = 60;
   //Save transformation
    glPushMatrix();

    // Transform matrix
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);

    // Begin Drawing triangles
    glColor3f(0.278, 0.569, 0.278);
    glBegin(GL_TRIANGLE_STRIP);
    
    for (int th=0;th<=360;th+=d)
    {
       glVertex3f(0,3,0);
        Vertex(th,d-90);
    }
    //  End
    glEnd();

    //  Undo transformations
    glPopMatrix();
}

// Tree function to build the tree.
static void tree(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
    trunk(0.0+x, 0.0*dy+y, 0.0+z, 1.0*dx, 1.0*dy, 1.0*dz, 0.0+th);
    leaves(0.0+x, 0.0*dy+y, 0.0+z, 1.0*dx, 1.0*dy, 1.0*dz, 0.0+th);
    leaves(0.0+x, 1.25*dy+y, 0.0+z, .75*dx, .75*dy, .75*dz, 0.0+th);
    leaves(0.0+x, 2.5*dy+y, 0.0+z, .5*dx, .5*dy, .5*dz, 0.0+th);
}

void forest()
{

    for(int i = 0; i < 100; i++)
    {
        for(int j = 0; j <100; j++)
        {
            if(trees[i][j][0])
            {
                tree(i,ground[i][j] - .1, j, trees[i][j][1], trees[i][j][1], trees[i][j][1], trees[i][j][2]);
            }
        }
    }
}

/*
Rock
*/

// Function to build the triangle fan for leaves
static void hVertex(double th,double ph, double y)
{
   double x = Sin(th)*Cos(ph);
   double z = Cos(th)*Cos(ph);
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

void rock(double x,double y,double z,
            double dx,double dy,double dz,
            double th)
{
    int d = 40;
    //Save transformation
    glPushMatrix();

    // Transform matrix
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);

    // Begin Drawing triangles
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_QUADS);
    




    glEnd();
    

    //  Undo transformations
    glPopMatrix();
}

/*
###################################
Scene build
###################################
*/

void scene()
{
    terrain();
    forest();
}


/*
###################################
OpenGl Functions
###################################
*/
static void Project()
{
    //  Tell OpenGL we want to manipulate the projection matrix
    glMatrixMode(GL_PROJECTION);
    //  Undo previous transformations
    glLoadIdentity();
    //  Perspective transformation
    gluPerspective(fov,asp,.1,4*dim);

    glMatrixMode(GL_MODELVIEW);
    //  Undo previous transformations
    glLoadIdentity();
}

void display()
{
    //  Erase the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    //  Undo previous transformations
    glLoadIdentity();

    // First person view
    dx = Cos(fth);
    dz = Sin(fth);
    int i = round(fEx);
    int k = round(fEz);
    gluLookAt(fEx, ground[i][k] + 1 ,fEz , fEx+dx, ground[i][k] + 1, fEz + dz, 0,1.3,0);
    // Show what's in front
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glEnable(GL_NORMALIZE);
    //  Enable lighting
    glEnable(GL_LIGHTING);
    //  Location of viewer for specular calculations
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
    //  glColor sets ambient and diffuse color materials
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    //  Enable light 0
    glEnable(GL_LIGHT0);
    //  Set ambient, diffuse, specular components and position of light 0
    glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
    glLightfv(GL_LIGHT0,GL_POSITION,Position);
    //  Set materials
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,Shinyness);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Specular);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

    // Render scene 
    scene();

    glWindowPos2i(5,5);
    Print("X=%.1f  Z=%.1f, zh=%d",fEx,fEz, zh);
    

    //  Render the scene
    glFlush();
    glutSwapBuffers();
}

void idle()
{
    //  Elapsed time in seconds
    double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
    zh = fmod(90*t,360.0);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

void special(int key, int x, int y)
{
    Project();
    glutIdleFunc(move?idle:NULL);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

void key(unsigned char ch, int x, int y)
{
    //  Exit on ESC
    if (ch == 27)
        exit(0);
    // Manage movement in first person
    else if (ch == 'a' || ch == 'A')
        fth -= 5;
    else if (ch == 'd' || ch == 'D')
        fth += 5;
    else if (ch == 'w' || ch == 'W')
    {
        double tempX = fEx + (dx*dt);
        double tempZ = fEz + (dz*dt);

        if (!collisionCilinder(trees, fEx, fEz, tempX, tempZ))
        {
            fEx = tempX;
            fEz = tempZ;
        }
    }
    else if (ch == 's' || ch == 'S')
    {
        
        double tempX = fEx - (dx*dt);
        double tempZ = fEz - (dz*dt);

        if (!collisionCilinder(trees, fEx, fEz, tempX, tempZ))
        {
            fEx = tempX;
            fEz = tempZ;
        }
    }
    else if (ch == 'g' || ch == 'G')
    {
        groundGeneration(ground, 100);
        treeGeneration(trees);
    }
    fth %= 360;
    Project();
    glutIdleFunc(move?idle:NULL);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();  
}

void reshape(int width, int height)
{
    //  Ratio of the width to the height of the window
    asp = (height>0) ? (double)width/height : 1;
    //  Set the viewport to the entire window
    glViewport(0,0, RES*width,RES*height);
    //  Set projection
    Project();
}

char* ReadText(char *file)
{
   char* buffer;
   //  Open file
   FILE* f = fopen(file,"rt");
   if (!f) Fatal("Cannot open text file %s\n",file);
   //  Seek to end to determine size, then rewind
   fseek(f,0,SEEK_END);
   int n = ftell(f);
   rewind(f);
   //  Allocate memory for the whole file
   buffer = (char*)malloc(n+1);
   if (!buffer) Fatal("Cannot allocate %d bytes for text file %s\n",n+1,file);
   //  Snarf the file
   if (fread(buffer,n,1,f)!=1) Fatal("Cannot read %d bytes for text file %s\n",n,file);
   buffer[n] = 0;
   //  Close and return
   fclose(f);
   return buffer;
}


int CreateShader(GLenum type,char* file)
{
   //  Create the shader
   int shader = glCreateShader(type);
   //  Load source code from file
   char* source = ReadText(file);
   glShaderSource(shader,1,(const char**)&source,NULL);
   free(source);
   //  Compile the shader
   fprintf(stderr,"Compile %s\n",file);
   glCompileShader(shader);

   //  Return name
   return shader;
}


int CreateShaderProg(char* VertFile,char* FragFile)
{
   //  Create program
   int prog = glCreateProgram();
   //  Create and compile vertex shader
   int vert = CreateShader(GL_VERTEX_SHADER,VertFile);
   //  Create and compile fragment shader
   int frag = CreateShader(GL_FRAGMENT_SHADER,FragFile);
   //  Attach vertex shader
   glAttachShader(prog,vert);
   //  Attach fragment shader
   glAttachShader(prog,frag);
   //  Link program
   glLinkProgram(prog);
   //  Return name
   return prog;
}

int main(int argc, char* argv[])
{
    //  Initialize GLUT
    glutInit(&argc,argv);
    //  Request double buffered, true color window with Z buffering at 600x600
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(600,600);
    glutCreateWindow("Jesus Carnero - Final Project");
#ifdef USEGLEW
    //  Initialize GLEW
    if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
    //  Set callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    shaders[0] = CreateShaderProg("phong.vert","phong.frag");
    shaders[1] = CreateShaderProg("texture.vert","texture.frag");
    texture[0] = LoadTexBMP("grass.bmp");
    texture[1] = LoadTexBMP("tree.bmp");

    ErrCheck("init");
    glutMainLoop();
    return 0;
}

