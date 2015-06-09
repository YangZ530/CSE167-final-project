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
Cube cube2 = Cube(4);

static GLuint depthTextureId;
static GLuint fboId;
static GLuint bloomFboId;
static GLuint bloomTexId;
static GLuint shadowMapUniform;
static GLuint textureUniform;
Shader *Window::shader = new Shader("shadow_simple.vert", "shadow_simple.frag", true);
Shader *Window::bloom = new Shader("shadow_simple.vert", "shadow_simple.frag", true);

void Window::initialize(void)
{
	//Setup the light
	Globals::ptLight.position = Vector4(0.0, 45.0, 0.0, 1.0);
	Globals::ptLight.quadraticAttenuation = 0;
	Globals::ptLight.diffuseColor = Color(0.4, 0.4, 0.4);
	Globals::ptLight.specularColor = Color(0.2, 0.2, 0.2);
	Globals::ptLight.ambientColor = Color(0.2, 0.2, 0.2);

//	Globals::spotL.position = Globals::camera.e.toVector4(1);
//	Globals::spotL.direction = (Globals::camera.d - Globals::camera.e).normalize().toVector4(0);

	Globals::spotL.position = Globals::camera.getMatrix() * Matrix4().makeTranslate(3, 0, 1) * Globals::camera.getInverseMatrix() * Globals::camera.e.toVector4(1);
	Globals::spotL.direction = Globals::camera.getMatrix() * Matrix4().makeTranslate(3, 0, 1) * Globals::camera.getInverseMatrix() * (Globals::camera.d - Globals::camera.e).normalize().toVector4(0);

	Globals::spotL.diffuseColor = Color(0.6, 0.65, 0.5);
	Globals::spotL.specularColor = Color(0.7, 0.75, 0.6);
	Globals::spotL.ambientColor = Color(0.1, 0.1, 0.1);
	Globals::spotL.cutoff = 15;
	Globals::spotL.exponent = 100;
	Globals::spotL.quadraticAttenuation = 0;

	Globals::dirLight.position = Vector4(50.0, 50.0, 50.0, 0.0);
	Globals::dirLight.diffuseColor = Color(0.2, 0.2, 0.2);
	Globals::dirLight.ambientColor = Color(0.5,0.5,0.5);
	Globals::dirLight.specularColor = Color(0.2, 0.2, 0.2);

	room = Room(100);
	cube = Cube(3);
	cube2 = Cube(4);
	//Initialize room matrix:
	room.toWorld.identity();
	cube.toWorld.identity();
	cube.toWorld = cube.toWorld * Matrix4().makeTranslate(5, -48.5, -10.0);
	cube2.toWorld = cube2.toWorld * Matrix4().makeTranslate(5, -48, -15.0);

	//Setup the room's material properties
	//Color color(0x23ff27ff);
	Color color(0.7, 0.7, 0.7);
	room.material = Material(1);
	cube.material = Material(0);
	cube2.material = Material(0);
	//shader = new Shader("shadow_simple.vert", "shadow_simple.frag", true);	
	shader = new Shader("shadowMapping.vert", "shadowMapping.frag", true);
	//bloom = new Shader("shadow_simple.vert", "shadow_simple.frag", true);
	genFBO();
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

	genFBO();
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{
	//Update camera
	if (move_forward)
		Globals::camera.goForward(0.04);
	if (move_backward)
		Globals::camera.goBack(0.04);
	if (move_left)
		Globals::camera.goLeft(0.04);
	if (move_right)
		Globals::camera.goRight(0.04);

	Globals::spotL.position = Globals::camera.getMatrix() * Matrix4().makeTranslate(3, 0, 1) * Globals::camera.getInverseMatrix() * Globals::camera.e.toVector4(1);
	Globals::spotL.direction = Globals::camera.getMatrix() * Matrix4().makeTranslate(3, 0, 1) * Globals::camera.getInverseMatrix() * (Globals::camera.d - Globals::camera.e).normalize().toVector4(0);
//	Globals::spotL.position = Globals::camera.e.toVector4(1);
//	Globals::spotL.direction = (Globals::camera.d - Globals::camera.e).normalize().toVector4(0);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId); //start rendering offscreen
	glUseProgramObjectARB(0);
	glViewport(0, 0, 1024, 1024);
	glClear(GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	Vector4 p_light = Globals::spotL.position;
	Vector4 l_light = Globals::spotL.position + Globals::spotL.direction;
	//Vector4 l_light = Globals::spotL.direction;
	setupLightMatrices(p_light[0], p_light[1], p_light[2], l_light[0], l_light[1], l_light[2]);

//	Globals::spotL.bind(0);
	cube.depthRender(Globals::drawData);
	cube2.depthRender(Globals::drawData);
	room.depthRender(Globals::drawData);
	
//	cube.draw(Globals::drawData);
//	room.draw(Globals::drawData);

	setTextureMatrix();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); //start render screen
	glViewport(0, 0, width, height);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgramObjectARB(shader->getPid());
	shadowMapUniform = glGetUniformLocationARB(shader->getPid(), "ShadowMap");
	glUniform1iARB(shadowMapUniform, 7);
	textureUniform = glGetUniformLocationARB(shader->getPid(), "Texture");
	glActiveTextureARB(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, depthTextureId);
	glActiveTextureARB(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cube.tex.id);

	Vector3 p_camera = Globals::camera.e;
	Vector3 l_camera = Globals::camera.d;
	setupMatrices(p_camera[0], p_camera[1], p_camera[2], l_camera[0], l_camera[1], l_camera[2]);

	//Globals::ptLight.bind(2);
	Globals::dirLight.bind(1);
	Globals::spotL.bind(0);

	//Draw the room!
//	room.draw(Globals::drawData);
//	cube.draw(Globals::drawData);

	room.depthRender(Globals::drawData);
	cube.depthRender(Globals::drawData);
	cube2.depthRender(Globals::drawData);

	/*
	// Debug: draw texture to the screen
	glUseProgramObjectARB(0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width / 2, width / 2, -height / 2, height / 2, 1, 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor4f(1, 1, 1, 1);
	glActiveTextureARB(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTextureId);
	//glBindTexture(GL_TEXTURE_2D, bloomTexId);
	glEnable(GL_TEXTURE_2D);
	glTranslated(0, 0, -1);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex3f(-width / 2, -height/2, 0);
	glTexCoord2d(1, 0); glVertex3f(width/2, -height/2, 0);
	glTexCoord2d(1, 1); glVertex3f(width/2, height/2, 0);
	glTexCoord2d(0, 1); glVertex3f(-width/2, height/2, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
//	*/

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
	//position.print("The position of the room is(ignore w):");
	
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
//		Globals::spotL.position = Globals::camera.getMatrix() * Matrix4().makeTranslate(5, 0, 1) * Globals::camera.getInverseMatrix() * Globals::camera.e.toVector4(1);
//		Globals::spotL.direction = Globals::camera.getMatrix() * Matrix4().makeTranslate(5, 0, 1) * Globals::camera.getInverseMatrix() * (Globals::camera.d - Globals::camera.e).normalize().toVector4(0);

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

void Window::genFBO(){
	GLenum FBOstatus;

	// Try to use a texture depth component
	glGenTextures(1, &depthTextureId);
	glBindTexture(GL_TEXTURE_2D, depthTextureId);

	// GL_LINEAR does not make sense for depth texture. However, next tutorial shows usage of GL_LINEAR and PCF
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Remove artefact on the edges of the shadowmap
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor );

	// No need to force GL_DEPTH_COMPONENT24, drivers usually give you the max precision if available 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// create a framebuffer object
	glGenFramebuffersEXT(1, &fboId);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

	// Instruct openGL that we won't bind a color texture with the currently binded FBO
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// attach the texture to FBO depth attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, depthTextureId, 0);

	// check FBO status
	FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT)
		printf("GL_FRAMEBUFFER_COMPLETE_EXT failed, CANNOT use FBO\n");

	// switch back to window-system-provided framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void Window::setupLightMatrices(float position_x, float position_y, float position_z, float lookAt_x, float lookAt_y, float lookAt_z)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 40000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(position_x, position_y, position_z, lookAt_x, lookAt_y, lookAt_z, 0, 1, 0);
}

void Window::setupMatrices(float position_x, float position_y, float position_z, float lookAt_x, float lookAt_y, float lookAt_z)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (double)width / (double)height, 1, 40000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(position_x, position_y, position_z, lookAt_x, lookAt_y, lookAt_z, 0, 1, 0);
}

void Window::setTextureMatrix(void)
{
	static double modelView[16];
	static double projection[16];

	// This is matrix transform every coordinate x,y,z
	// x = x* 0.5 + 0.5 
	// y = y* 0.5 + 0.5 
	// z = z* 0.5 + 0.5 
	// Moving from unit cube [-1,1] to [0,1]  
	const GLdouble bias[16] = {
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0 };

	// Grab modelview and transformation matrices
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);


	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);

	glLoadIdentity();
	glLoadMatrixd(bias);

	// concatating all matrice into one.
	glMultMatrixd(projection);
	glMultMatrixd(modelView);

	// Go back to normal matrix mode
	glMatrixMode(GL_MODELVIEW);
}