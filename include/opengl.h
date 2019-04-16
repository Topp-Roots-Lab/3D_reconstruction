#pragma once

////////////////////////////////////////////////////////////
// INCLUDE FILES
////////////////////////////////////////////////////////////

// Standard include files

#include <math.h>
#include <time.h>


// Windows include files 

#ifdef _WIN32
#include <windows.h>
#endif


// OpenGL include files 

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// include mesh
#include "mesh.h"

//debug variable
static int step = 0;

////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
////////////////////////////////////////////////////////////

// GLUT variables 

static int GLUTwindow = 0;
static int GLUTwindow_posX = 100;
static int GLUTwindow_posY = 100;
static int GLUTwindow_width = 640;
static int GLUTwindow_height = 480;
static int GLUTmouse[2] = { 0, 0 };
static int GLUTbutton[3] = { 0, 0, 0 };
static int GLUTarrows[4] = { 0, 0, 0, 0 };
static int GLUTmodifiers = 0;



// Display variables

static int scaling = 0;
static int translating = 0;
static int rotating = 0;
static float scale = 1.0;
static float center[3] = { 0.0, 0.0, 0.0 };
static float rotation[3] = { 0.0, 0.0, 0.0f };
static float translation[3] = { 0.0, 0.0, -4.0 };
static float sphere_radius = 0.0f;
static bool change_model_scale = true;
static bool show_critical_net = false;
static int using_material = 0;


// Mesh variables
Mesh			*mesh			= NULL;


Vertex			*boundingBox	= NULL;
unsigned int	*quadIndex		= NULL;
void DrawCube(){

	if ( boundingBox == NULL ){	
		//initialize the bounding box
		boundingBox = new Vertex[8];			//8 vertices
		quadIndex   = new unsigned int[ 24 ];	//6 faces, each with 4 vertices
		boundingBox[0].Set(mesh->bbox[0][0], mesh->bbox[0][1], mesh->bbox[0][2]);
		boundingBox[1].Set(mesh->bbox[1][0], mesh->bbox[0][1], mesh->bbox[0][2]);
		boundingBox[2].Set(mesh->bbox[1][0], mesh->bbox[1][1], mesh->bbox[0][2]);
		boundingBox[3].Set(mesh->bbox[0][0], mesh->bbox[1][1], mesh->bbox[0][2]);
		boundingBox[4].Set(mesh->bbox[0][0], mesh->bbox[0][1], mesh->bbox[1][2]);
		boundingBox[5].Set(mesh->bbox[1][0], mesh->bbox[0][1], mesh->bbox[1][2]);
		boundingBox[6].Set(mesh->bbox[1][0], mesh->bbox[1][1], mesh->bbox[1][2]);
		boundingBox[7].Set(mesh->bbox[0][0], mesh->bbox[1][1], mesh->bbox[1][2]);

		int index = 0;
		quadIndex[ index ++ ] = 0;
		quadIndex[ index ++ ] = 1;
		quadIndex[ index ++ ] = 2;
		quadIndex[ index ++ ] = 3;

		quadIndex[ index ++ ] = 4;
		quadIndex[ index ++ ] = 5;
		quadIndex[ index ++ ] = 6;
		quadIndex[ index ++ ] = 7;

		quadIndex[ index ++ ] = 0;
		quadIndex[ index ++ ] = 1;
		quadIndex[ index ++ ] = 5;
		quadIndex[ index ++ ] = 4;

		quadIndex[ index ++ ] = 2;
		quadIndex[ index ++ ] = 6;
		quadIndex[ index ++ ] = 7;
		quadIndex[ index ++ ] = 3;

		quadIndex[ index ++ ] = 0;
		quadIndex[ index ++ ] = 4;
		quadIndex[ index ++ ] = 7;
		quadIndex[ index ++ ] = 3;

		quadIndex[ index ++ ] = 1;
		quadIndex[ index ++ ] = 5;
		quadIndex[ index ++ ] = 6;
		quadIndex[ index ++ ] = 2;
	}

	glLineWidth( 2.0f );
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glColor3f( 0.5f, 0.5f, 0.5f );
	glVertexPointer( 3, GL_FLOAT, 0 , boundingBox );
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, quadIndex );
	glLineWidth( 1.0f );
	glEnable(GL_CULL_FACE);

}


////////////////////////////////////////////////////////////
// PAINT TEXT HERE
////////////////////////////////////////////////////////////
void DisplayText(int x, int y, char* s, float r, float g, float b){

	int lines;
	char* p;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH), 
		0.0, glutGet(GLUT_WINDOW_HEIGHT), -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(r,g,b);
	glRasterPos2i(x, y);
	for(p = s, lines = 0; *p; p++) {
		if (*p == '\n') {
			lines++;
			glRasterPos2i(x, y-(lines*18));
		}
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

static bool show_help = false;
void ShowHelp(){

	glDisable( GL_LIGHTING );
	glDisable( GL_COLOR_MATERIAL );

	DisplayText(10,GLUTwindow_height - 20,"r: rendering",1,1,1);
	DisplayText(10,GLUTwindow_height - 40,"m: material",1,1,1);
	DisplayText(10,GLUTwindow_height - 60,"s: zoom",1,1,1);
	DisplayText(10,GLUTwindow_height - 80,"w: rotation",1,1,1);
	DisplayText(10,GLUTwindow_height - 100,"s: zooming",1,1,1);
	DisplayText(10,GLUTwindow_height - 120,"c: critical net",1,1,1);
	DisplayText(10,GLUTwindow_height - 140,"b: bounding box",1,1,1);
	DisplayText(10,GLUTwindow_height - 160,"q: exit",1,1,1);

	glEnable( GL_COLOR_MATERIAL );
	glEnable( GL_LIGHTING );
}

////////////////////////////////////////////////////////////
// PAINT OBJECT HERE
////////////////////////////////////////////////////////////
static int render_mode = 0;				//light + shading
static bool show_bounding_box = false;	//show bounding box?
static bool show_text = false;			//show text?
static int debug_info = 0;
void Paint(){

	//render basic mesh
	if ( render_mode == 0){	//material only with cloud of points

		glDisable(GL_LIGHTING);
		glPointSize( 2.0f );
		glNormalPointer( GL_FLOAT, 0, mesh->normals );
		glVertexPointer( 3, GL_FLOAT, 0 , mesh->vertices );
		cout << mesh->numVertices << " vertices." << endl;
		glColorPointer(3,GL_FLOAT,0, mesh->material );
		glDrawArrays(GL_POINTS,0, mesh->numVertices);
		if ( show_text) DisplayText(10,10,"cloud of points",1,1,1);
		glPointSize( 1.0f );

	}else if ( render_mode == 1){	//material only with wire-frame

		glDisable(GL_LIGHTING);
		glNormalPointer( GL_FLOAT, 0, mesh->normals );
		glVertexPointer( 3, GL_FLOAT, 0 , mesh->vertices );
		glColorPointer(3,GL_FLOAT,0, mesh->material );
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, mesh->numFaces + mesh->numFaces + mesh->numFaces,
			GL_UNSIGNED_INT, mesh->faces );
		if ( show_text ) DisplayText(10,10,"mesh view",1,1,1);

	}else if ( render_mode == 2){	//material only with smooth interpolated surface

		glDisable(GL_LIGHTING);
		glNormalPointer( GL_FLOAT, 0, mesh->normals );
		glVertexPointer( 3, GL_FLOAT, 0 , mesh->vertices );
		glColorPointer(3,GL_FLOAT,0, mesh->material );
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, mesh->numFaces + mesh->numFaces + mesh->numFaces,
			GL_UNSIGNED_INT, mesh->faces );
		if ( show_text ) DisplayText(10,10,"interpolated mesh view",1,1,1);

	}
	else{	// material + lighting + interpolated surface

		glEnable(GL_LIGHTING);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
		glNormalPointer( GL_FLOAT, 0, mesh->normals );
		glVertexPointer( 3, GL_FLOAT, 0 , mesh->vertices );
		glColorPointer(3,GL_FLOAT,0, mesh->material );
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, mesh->numFaces + mesh->numFaces + mesh->numFaces,
			GL_UNSIGNED_INT, mesh->faces );
		glDisable(GL_COLOR_MATERIAL);
		if ( show_text ) DisplayText(10,10,"mesh view with lighting and shading",1,1,1);

	}


	//show bounding box if activated
	if ( show_bounding_box ) 
		DrawCube();
}


////////////////////////////////////////////////////////////
// GLUT USER INTERFACE CODE
////////////////////////////////////////////////////////////
void GLUTRedraw(void)
{
	// Setup viewing transformation
	glLoadIdentity();
	glScalef(scale, scale, scale);
	glTranslatef(translation[0], translation[1], 0.0);

	// Set projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat) GLUTwindow_width /(GLfloat) GLUTwindow_height, 0.1, 100.0);

	// Set camera transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(translation[0], translation[1], translation[2]);
	glScalef(scale, scale, scale);
	glRotatef(rotation[0], 1.0, 0.0, 0.0);
	glRotatef(rotation[1], 0.0, 1.0, 0.0);
	glRotatef(rotation[2], 0.0, 0.0, 1.0);
	glTranslatef(-center[0], -center[1], -center[2]);

	// Clear window 
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set back face culling
	glEnable(GL_CULL_FACE);

	// Set lights
	static GLfloat light0_position[] = { 3.0, 4.0, 5.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	static GLfloat light1_position[] = { -3.0, -2.0, -3.0, 0.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);	

	// Paint objects
	Paint();

	// Swap buffers
	glutSwapBuffers();
}    



void GLUTStop(void)
{
	// Destroy window 
	glutDestroyWindow(GLUTwindow);

	// Release memory
	delete[] mesh->vertices;
	delete[] mesh->faces;
	delete[] mesh->normals;
	delete[] boundingBox;
	delete[] quadIndex;
	vector<Vertex*>::iterator matItr; Vertex* pMat;
	for ( matItr = mesh->materials.begin(); matItr != mesh->materials.end(); ++matItr ){
		pMat = *matItr; delete[] pMat;
	}
	mesh->materials.clear();

	delete(mesh);


	// Exit
	exit(0);
}



void GLUTResize(int w, int h)
{
	// Resize window
	glViewport(0, 0, w, h);

	// Remember window size 
	GLUTwindow_width = w;
	GLUTwindow_height = h;

	// Redraw
	glutPostRedisplay();
}



void GLUTMotion(int x, int y)
{
	// Invert y coordinate
	y = GLUTwindow_height - y;

	// Process mouse motion event
	if (rotating) {
		// Rotate model
		rotation[0] += -0.5 * (y - GLUTmouse[1]);
		rotation[1] += 0.5 * (x - GLUTmouse[0]);
	}
	else if (scaling) {
		// Scale window
		scale *= exp(2.0 * (float) (x - GLUTmouse[0]) / (float) GLUTwindow_width);
	}
	else if (translating) {
		// Translate window
		translation[0] += 2.0 * (float) (x - GLUTmouse[0]) / (float) GLUTwindow_width;
		translation[1] += 2.0 * (float) (y - GLUTmouse[1]) / (float) GLUTwindow_height;
	}

	// Remember mouse position 
	GLUTmouse[0] = x;
	GLUTmouse[1] = y;
}



void GLUTMouse(int button, int state, int x, int y)
{
	// Invert y coordinate
	y = GLUTwindow_height - y;

	// Process mouse button event
	rotating = (button == GLUT_LEFT_BUTTON);
	scaling = (button == GLUT_MIDDLE_BUTTON);
	translating = (button == GLUT_RIGHT_BUTTON);
	if (rotating || scaling || translating) glutIdleFunc(GLUTRedraw);
	else glutIdleFunc(0);

	// Remember button state 
	int b = (button == GLUT_LEFT_BUTTON) ? 0 : ((button == GLUT_MIDDLE_BUTTON) ? 1 : 2);
	GLUTbutton[b] = (state == GLUT_DOWN) ? 1 : 0;

	// Remember modifiers 
	GLUTmodifiers = glutGetModifiers();

	// Remember mouse position 
	GLUTmouse[0] = x;
	GLUTmouse[1] = y;
}



void GLUTSpecial(int key, int x, int y)
{
	// Invert y coordinate
	y = GLUTwindow_height - y;

	// Process keyboard button event 

	// Remember mouse position 
	GLUTmouse[0] = x;
	GLUTmouse[1] = y;

	// Remember modifiers 
	GLUTmodifiers = glutGetModifiers();

	// Redraw
	glutPostRedisplay();
}



void GLUTKeyboard(unsigned char key, int x, int y)
{
	// Process keyboard button event 
	switch (key) {
		case 'Q':
		case 'q':
			GLUTStop();
			break;

		case 'r':
		case 'R':
			render_mode = (render_mode + 1)%4;
			break;
		case 'b':
		case 'B':
			show_bounding_box = !show_bounding_box;
			break;
		case 't':
		case 'T':
			show_text = !show_text;
			break;
		case 's':
			scale /= 1.25;
			glutPostRedisplay();
			break;
		case 'S':
			scale *= 1.25;
			glutPostRedisplay();
			break;
		case 'w':
			rotation[2] += 9;
			glutPostRedisplay();
			break;
		case 'W':
			rotation[2] -= 9;
			glutPostRedisplay();
			break;
		case 'c':
		case 'C':
			show_critical_net = !show_critical_net;
			break;
		case 'm':
		case 'M':
			using_material = (using_material + 1)%mesh->materials.size();
			mesh->material = mesh->materials[using_material];
			break;
		case 'h':
		case 'H':
			show_help = !show_help;
			break;
		case 'n':
		case 'N':
			++step;
			break;
		case 27: // ESCAPE
			GLUTStop();
			break;
		default:
			break;
	}

	// Remember mouse position 
	GLUTmouse[0] = x;
	GLUTmouse[1] = GLUTwindow_height - y;

	// Remember modifiers 
	GLUTmodifiers = glutGetModifiers();
}


void GLUTInit(int argc, char** argv, Mesh* data)
{
	// Get all the program parameters
	GLUTwindow_posX = 100;
	GLUTwindow_posY = 100;
	GLUTwindow_width = 640;
	GLUTwindow_height = 480;

	// Feed in all the data necessary for visualization
	mesh		= data;

	// Setup initial viewing scale
	float dx = mesh->bbox[1][0] - mesh->bbox[0][0];
	float dy = mesh->bbox[1][1] - mesh->bbox[0][1];
	float dz = mesh->bbox[1][2] - mesh->bbox[0][2];
	scale = 3.5 / sqrt(dx*dx + dy*dy + dz*dz);
	float max_axis = dx;
	if ( dy > max_axis ) max_axis = dy;
	if ( dy > max_axis ) max_axis = dz;
	sphere_radius = 0.01 * max_axis;

	rotation[0] = -90;

	// Setup initial viewing center
	center[0] = 0.5 * (mesh->bbox[1][0] + mesh->bbox[0][0]);
	center[1] = 0.5 * (mesh->bbox[1][1] + mesh->bbox[0][1]);
	center[2] = 0.5 * (mesh->bbox[1][2] + mesh->bbox[0][2]);

	// Adjust window size
	change_model_scale = true;
	if ( change_model_scale ){
		if ( dz < dy ){
			if ( dx > dy){
				GLUTwindow_height *= (dy/dx);
			}else{
				GLUTwindow_width *= (dx/dy);
			}
		}else{
			rotation[0] = -90.0f;
			if ( dx > dz){
				GLUTwindow_height *= (dz/dx);
			}else{
				GLUTwindow_width *= (dx/dz);
			}
		}
	}

	// Open window 
	glutInit(&argc, argv);
	glutInitWindowPosition( GLUTwindow_posX, GLUTwindow_posY );
	glutInitWindowSize(GLUTwindow_width, GLUTwindow_height);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // | GLUT_STENCIL
	GLUTwindow = glutCreateWindow("---Root Demo---");

	// Initialize GLUT callback functions 
	glutReshapeFunc(GLUTResize);
	glutDisplayFunc(GLUTRedraw);
	glutKeyboardFunc(GLUTKeyboard);
	glutSpecialFunc(GLUTSpecial);
	glutMouseFunc(GLUTMouse);
	glutMotionFunc(GLUTMotion);
	glutIdleFunc(0);

	// Initialize background color 
	glClearColor(1.0f, 1.0f, 1.0f, 1.0);

	// Initialize lights
	static GLfloat lmodel_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	static GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glEnable(GL_LIGHT0);
	static GLfloat light1_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);

	glEnableClientState(GL_VERTEX_ARRAY);	
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);  

	// Initialize graphics modes 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Initialize graphics modes 
	glEnable(GL_DEPTH_TEST);
}

void GLUTMainLoop(void)
{
	// Run main loop -- never returns 
	glutMainLoop();
}

