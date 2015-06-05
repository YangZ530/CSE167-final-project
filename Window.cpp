#include "GLee.h"
#include "Shader.h"
#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Window.h"
#include "Room.h"
#include "Cube.h"
#include "Matrix4.h"
#include "Globals.h"

#define RADIANS 0.0005
#define X 0
#define Y 1
#define Z 2
#define MOUSE_LEFT 0
#define MOUSE_MIDDLE 1
#define MOUSE_RIGHT 2

int Window::width = 512;   //Set window width in pixels here
int Window::height = 512;   //Set window height in pixels here
float Window::radians = RADIANS;
int Window::pressedMouse = -1;
int Window::mouse_x = 0;
int Window::mouse_y = 0;
bool Window::move_forward = false;
bool Window::move_backward = false;
bool Window::move_left = false;
bool Window::move_right = false;
bool Window::fpsMode = false;

Room room = Room(100);
Cube cube = Cube(10);

void Window::initialize(void)
{
	//Setup the light
	Vector4 lightPos(0.0, 30.0, 20.0, 1.0);
	Globals::ptLight.position = lightPos;
	Globals::ptLight.quadraticAttenuation = 0;
	Globals::ptLight.diffuseColor = Color(0.7, 0.7, 0.7);
	Globals::ptLight.specularColor = Color(0.7, 0.7, 0.7);
	Globals::ptLight.ambientColor = Color(0.2, 0.2, 0.2);

	Globals::spotL.position = Matrix4().makeTranslate(5,0,1) * Globals::camera.e.toVector4(1);
	Globals::spotL.direction = (Globals::camera.d - Globals::camera.e).normalize().toVector4(0);
	Globals::spotL.diffuseColor = Color(1, 1, 1);
	Globals::spotL.specularColor = Color(1, 1, 1);
	Globals::spotL.ambientColor = Color(0, 0, 0);
	Globals::spotL.cutoff = 10;
	Globals::spotL.exponent = 5;
	Globals::spotL.quadraticAttenuation = 0;

	Globals::dirLight.quadraticAttenuation = 0.002;
	Globals::dirLight.position = Vector4(0.0, 100.0, 50.0, 0.0);
	Globals::dirLight.diffuseColor = Color(0.3, 0.3, 0.3);
	Globals::dirLight.ambientColor = Color(0.6, 0.6, 0.6);
	Globals::dirLight.specularColor = Color(0.7, 0.7, 0.7);

	glCullFace(GL_BACK);
	room = Room(100);
	cube = Cube(5);
	//Initialize room matrix:
	room.toWorld.identity();
	cube.toWorld.identity();
	cube.toWorld = cube.toWorld * Matrix4().makeTranslate(0, -40, 0);

	//Setup the room's material properties
	//Color color(0x23ff27ff);
	Color color(0.7, 0.7, 0.7);
	room.material = Material(1);
	cube.material = Material(2);
}

//----------------------------------------------------------------------------
// Callback method called when system is idle.
// This is called at the start of every new "frame" (qualitatively)
void Window::idleCallback()
{
	//Set up a static time delta for update calls
	Globals::updateData.dt = 1.0 / 60.0;// 60 fps

	//Rotate room; if it spins too fast try smaller values and vice versa
	//Globals::room.spin(radians);

	//Call the update function on room
	room.update(Globals::updateData);

	//Call the display routine to draw the room
	displayCallback();
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when graphics window is resized by the user
void Window::reshapeCallback(int w, int h)
{
	width = w;                                                       //Set the window width
	height = h;                                                      //Set the window height
	glViewport(0, 0, w, h);                                          //Set new viewport size
	glMatrixMode(GL_PROJECTION);                                     //Set the OpenGL matrix mode to Projection
	glLoadIdentity();                                                //Clear the projection matrix by loading the identity
	gluPerspective(40.0, (double)width / (double)height, 1.0, 1000.0); //Set perspective projection viewing frustum
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{
	//Update camera
	if (move_forward)
		Globals::camera.goForward(0.03);
	if (move_backward)
		Globals::camera.goBack(0.03);
	if (move_left)
		Globals::camera.goLeft(0.03);
	if (move_right)
		Globals::camera.goRight(0.03);
	/*
	Globals::spotL.position = Globals::camera.e.toVector4(1);
	Globals::spotL.direction = (Globals::camera.d - Globals::camera.e).normalize().toVector4(0);
	*/

	//if (fpsMode)
		//glutWarpPointer(width / 2, height / 2);

	//Clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set the OpenGL matrix mode to ModelView
	glMatrixMode(GL_MODELVIEW);

	//Push a matrix save point
	//This will save a copy of the current matrix so that we can
	//make changes to it and 'pop' those changes off later.
	glPushMatrix();

	//Replace the current top of the matrix stack with the inverse camera matrix
	//This will convert all world coordiantes into camera coordiantes
	glLoadMatrixf(Globals::camera.getInverseMatrix().ptr());

	//Bind the light to slot 0.  We do this after the camera matrix is loaded so that
	//the light position will be treated as world coordiantes
	//(if we didn't the light would move with the camera, why is that?)

//	Globals::ptLight.bind(0);
	Globals::dirLight.bind(1);
	Globals::spotL.bind(0);

	//Draw the room!
	room.draw(Globals::drawData);
	cube.draw(Globals::drawData);

	//Pop off the changes we made to the matrix stack this frame
	glPopMatrix();

	//Tell OpenGL to clear any outstanding commands in its command buffer
	//This will make sure that all of our commands are fully executed before
	//we swap buffers and show the user the freshly drawn frame
	glFlush();

	//Swap the off-screen buffer (the one we just drew to) with the on-screen buffer
	glutSwapBuffers();
}

void Window::processNormalKeys(unsigned char key, int x, int y) {

	
	Matrix4 trans;
	Vector4 position;
	Vector4 oldPosition;
	Vector4 v;

	v.set(0, 0, 0, 1);
	oldPosition = room.toWorld * v;

	switch (key)
	{
	case 'w':
		move_forward = true;
		break;
	case 'a':
		move_left = true;
		break;
	case 's':
		move_backward = true;
		break;
	case 'd':
		move_right = true;
		break;
	case 'f':
		if (fpsMode)
		{
			fpsMode = false;
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		}
		else
		{
			fpsMode = true;
			glutSetCursor(GLUT_CURSOR_NONE);
			glutWarpPointer(width / 2, height / 2);
		}
		break;
	case 27:
		exit(0);	
	}

	position = room.toWorld * v;
	position.print("The position of the room is(ignore w):");
	
}

void Window::processKeyup(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		move_forward = false;
		break;
	case 'a':
		move_left = false;
		break;
	case 's':
		move_backward = false;
		break;
	case 'd':
		move_right = false;
		break;
	case 27:
		exit(0);
	}

}

void Window::processMouse(int button, int state, int x, int y)
{
	mouse_x = x;
	mouse_y = y;
	if (pressedMouse == -1)
		pressedMouse = button;
	else
		pressedMouse = -1;
	std::cout << "button =  " << button << std::endl;
}

//TODO: Mouse Motion callbacks!
void Window::processMotion(int x, int y)
{
	static bool just_warped = false;
	Matrix4 trans;
	Vector3 lastPoint, currPoint, direction, rotAxis;
	float rot_angle, velocity;
	
	if (just_warped) {
		just_warped = false;
		return;
	}

	if (fpsMode)
	{
		lastPoint = trackBall((float)width / 2, (float)height / 2);
		currPoint = trackBall((float)x, (float)y);
		direction = currPoint - lastPoint;
		velocity = direction.magnitude();
		//rotAxis = lastPoint.cross(currPoint);
		rotAxis = currPoint.cross(lastPoint);
		rot_angle = velocity;

		trans.makeRotateArbitrary(rotAxis, rot_angle);

		Globals::camera.arbitraryLook(trans);

//		Globals::spotL.position = Globals::camera.e.toVector4(1);
//		Globals::spotL.direction = (Globals::camera.d - Globals::camera.e).normalize().toVector4(0);

		glutWarpPointer(width / 2, height / 2);

		just_warped = true;
	}	
}



Vector3 Window::trackBall(float x, float y)    // The CPoint class is a specific Windows class. Either use separate x and y values for the mouse location, or use a Vector3 in which you ignore the z coordinate.
{
	Vector3 v;    // Vector v is the synthesized 3D position of the mouse location on the trackball
	float* vValue[3];
	float d;     // this is the depth of the mouse location: the delta between the plane through the center of the trackball and the z position of the mouse
	v[X] = (2.0 * x - (float)width) / (float)width;   // this calculates the mouse X position in trackball coordinates, which range from -1 to +1
	v[Y] = ((float)height - 2.0 * y) / (float)height;   // this does the equivalent to the above for the mouse Y position
	v[Z] = 0.0;   // initially the mouse z position is set to zero, but this will change below
	v.set(v[X], v[Y], v[Z]);
	d = v.magnitude();    // this is the distance from the trackball's origin to the mouse location, without considering depth (=in the plane of the trackball's origin)
	d = (d<1.0) ? d : 1.0;   // this limits d to values of 1.0 or less to avoid square roots of negative values in the following line
	v[Z] = sqrtf(1.001 - d*d);  // this calculates the Z coordinate of the mouse position on the trackball, based on Pythagoras: v.z*v.z + d*d = 1*1
	v.set(v[X], v[Y], v[Z]);
	v.normalize(); // Still need to normalize, since we only capped d, not v.
	return v;  // return the mouse location on the surface of the trackball
}