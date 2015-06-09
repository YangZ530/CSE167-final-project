#include "Cube.h"
#include "Shader.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Window.h"
#include "math.h"

Cube::Cube(float size) : Drawable()
{
	this->size = size;
	//lighting = new Shader("lighting.vert", "lighting.frag", true);
	shadow = new Shader("shadowMap.vert", "shadowMap.frag", true);
	shadowMap = new Shader("shadowMapping.vert", "shadowMapping.frag", true);
//	tex = Texture("companion_cube.ppm");
	tex = Texture("Doge_Tex.ppm");
}

Cube::~Cube()
{
	//Delete any dynamically allocated memory/objects here
	//delete shadow;
	//delete lighting;
	//delete shadowMap;
}


void Cube::draw(DrawData& data)
{
	//shader = lighting;
	//shader = shadowMap;

	float halfSize = size / 2.0;

	//Apply the material properties
	//From here forward anything drawn will be drawn with these material
	material.apply();

	//Se the OpenGL Matrix mode to ModelView (used when drawing geometry)
	glMatrixMode(GL_MODELVIEW);

	//Push a save state onto the matrix stack, and multiply in the toWorld matrix
	glPushMatrix();
	glMultMatrixf(toWorld.ptr());

	//Make Cube!
	//Note: The glBegin, and glEnd should always be as close to the glNormal/glVertex calls as possible
	//These are special calls that 'freeze' many internal states of OpenGL.
	//Once the glBegin state is active many of the calls made to OpenGL (like glMultMatrixf) will be IGNORED!
	//As a good habit, only call glBegin just before you need to draw, and call end just after you finish

	glBegin(GL_QUADS);

	// Draw front face:
	glColor3f(1, 0, 0); // red
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0, 0); glVertex3f(-halfSize, halfSize, halfSize);
	glTexCoord2f(1, 0); glVertex3f(halfSize, halfSize, halfSize);
	glTexCoord2f(1, 1); glVertex3f(halfSize, -halfSize, halfSize);
	glTexCoord2f(0, 1); glVertex3f(-halfSize, -halfSize, halfSize);

	// Draw left side:
	glColor3f(1, 1, 0); // yellow
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(-halfSize, halfSize, halfSize);
	glVertex3f(-halfSize, halfSize, -halfSize);
	glVertex3f(-halfSize, -halfSize, -halfSize);
	glVertex3f(-halfSize, -halfSize, halfSize);

	// Draw right side:
	glColor3f(1, 0, 1); // purple
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(halfSize, halfSize, halfSize);
	glVertex3f(halfSize, halfSize, -halfSize);
	glVertex3f(halfSize, -halfSize, -halfSize);
	glVertex3f(halfSize, -halfSize, halfSize);

	// Draw back face:
	glColor3f(0, 1, 1); // cyan
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(-halfSize, halfSize, -halfSize);
	glVertex3f(halfSize, halfSize, -halfSize);
	glVertex3f(halfSize, -halfSize, -halfSize);
	glVertex3f(-halfSize, -halfSize, -halfSize);

	// Draw top side:
	glColor3f(0, 1, 0); // green
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(-halfSize, halfSize, halfSize);
	glVertex3f(halfSize, halfSize, halfSize);
	glVertex3f(halfSize, halfSize, -halfSize);
	glVertex3f(-halfSize, halfSize, -halfSize);

	// Draw bottom side:
	glColor3f(0, 0, 1); // green
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(-halfSize, -halfSize, -halfSize);
	glVertex3f(halfSize, -halfSize, -halfSize);
	glVertex3f(halfSize, -halfSize, halfSize);
	glVertex3f(-halfSize, -halfSize, halfSize);

	glEnd();

	tex.unbind();

	//The above glBegin, glEnd, glNormal and glVertex calls can be replaced with a glut convenience function
	//glutSolidCube(size);

	//Pop the save state off the matrix stack
	//This will undo the multiply we did earlier
	glPopMatrix();
}


void Cube::update(UpdateData& data)
{
	//
}

void Cube::spin(float radians)
{
	Matrix4 rotation;
	rotation.makeRotateY(radians);

	toWorld = toWorld * rotation;
}

void Cube::depthRender(DrawData& data){
	material.apply();
	//Se the OpenGL Matrix mode to ModelView (used when drawing geometry)
	glMatrixMode(GL_MODELVIEW);

	//Push a save state onto the matrix stack, and multiply in the toWorld matrix
	glPushMatrix();
	glMultMatrixf(toWorld.ptr());

	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPushMatrix();
	glMultMatrixf(toWorld.ptr());

	float halfSize = size / 2.0;
	glBegin(GL_QUADS);
	glColor3f(1,1,1);
	// Draw front face:
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0, 0);	glVertex3f(-halfSize, halfSize, halfSize);
	glTexCoord2f(1, 0);	glVertex3f(halfSize, halfSize, halfSize);
	glTexCoord2f(1, 1);	glVertex3f(halfSize, -halfSize, halfSize);
	glTexCoord2f(0, 1);	glVertex3f(-halfSize, -halfSize, halfSize);

	// Draw left side:
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(1, 0);	glVertex3f(-halfSize, halfSize, halfSize);
	glTexCoord2f(0, 0); glVertex3f(-halfSize, halfSize, -halfSize);
	glTexCoord2f(0, 1); glVertex3f(-halfSize, -halfSize, -halfSize);
	glTexCoord2f(1, 1); glVertex3f(-halfSize, -halfSize, halfSize);

	// Draw right side:
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(0, 0); glVertex3f(halfSize, halfSize, halfSize);
	glTexCoord2f(1, 0); glVertex3f(halfSize, halfSize, -halfSize);
	glTexCoord2f(1, 1); glVertex3f(halfSize, -halfSize, -halfSize);
	glTexCoord2f(0, 1); glVertex3f(halfSize, -halfSize, halfSize);

	// Draw back face:
	glNormal3f(0.0, 0.0, -1.0);
	glTexCoord2f(1, 0); glVertex3f(-halfSize, halfSize, -halfSize);
	glTexCoord2f(0, 0); glVertex3f(halfSize, halfSize, -halfSize);
	glTexCoord2f(0, 1); glVertex3f(halfSize, -halfSize, -halfSize);
	glTexCoord2f(1, 1); glVertex3f(-halfSize, -halfSize, -halfSize);

	// Draw top side:
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0, 1); glVertex3f(-halfSize, halfSize, halfSize);
	glTexCoord2f(1, 1); glVertex3f(halfSize, halfSize, halfSize);
	glTexCoord2f(1, 0); glVertex3f(halfSize, halfSize, -halfSize);
	glTexCoord2f(0, 0); glVertex3f(-halfSize, halfSize, -halfSize);

	// Draw bottom side:
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(0, 1); glVertex3f(-halfSize, -halfSize, -halfSize);
	glTexCoord2f(1, 1); glVertex3f(halfSize, -halfSize, -halfSize);
	glTexCoord2f(1, 0); glVertex3f(halfSize, -halfSize, halfSize);
	glTexCoord2f(0, 0); glVertex3f(-halfSize, -halfSize, halfSize);

	glEnd();

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}