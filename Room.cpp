#include "Room.h"
#include "Shader.h"
#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "Window.h"
#include "math.h"

Room::Room(float size) : Drawable()
{
    this->size = size;
	lighting = new Shader("lighting.vert", "lighting.frag", true);
	shadow = new Shader("shadowMap.vert", "shadowMap.frag", true);
	shadowMap = new Shader("shadowMapping.vert", "shadowMapping.frag", true);
	shader = lighting;
}

Room::~Room()
{
    //Delete any dynamically allocated memory/objects here
}


void Room::draw(DrawData& data)
{
	//shader = lighting;
	//shader = shadowMap;

    float halfSize = size/2.0;
    
    //Apply the material properties
    //From here forward anything drawn will be drawn with these material
    material.apply();
    
    glMatrixMode(GL_MODELVIEW);
    
    glPushMatrix();
    glMultMatrixf(toWorld.ptr());
    
	shadowMap->bind();

	GLuint depthBiasMatrixID = glGetUniformLocation(shadowMap->getPid(), "depthBiasMVP");
	glUniformMatrix4fv(depthBiasMatrixID, 1, GL_FALSE, depthBiasMVP.ptr());
	GLuint ShadowMapID = glGetUniformLocation(shadowMap->getPid(), "shadowMap");
	glUniform1i(ShadowMapID, 0);

    glBegin(GL_QUADS);
    
    // Draw front face:
	glColor3f(1, 0, 0); // red
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(-halfSize,  halfSize,  halfSize);
    glVertex3f( halfSize,  halfSize,  halfSize);
    glVertex3f( halfSize, -halfSize,  halfSize);
    glVertex3f(-halfSize, -halfSize,  halfSize);
    
    // Draw left side:
	glColor3f(1, 1, 0); // yellow
    glNormal3f(1.0, 0.0, 0.0);
    glVertex3f(-halfSize,  halfSize,  halfSize);
    glVertex3f(-halfSize,  halfSize, -halfSize);
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f(-halfSize, -halfSize,  halfSize);
    
    // Draw right side:
	glColor3f(1, 0, 1); // purple
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f( halfSize,  halfSize,  halfSize);
    glVertex3f( halfSize,  halfSize, -halfSize);
    glVertex3f( halfSize, -halfSize, -halfSize);
    glVertex3f( halfSize, -halfSize,  halfSize);
    
    // Draw back face:
	glColor3f(0, 1, 1); // cyan
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(-halfSize,  halfSize, -halfSize);
    glVertex3f( halfSize,  halfSize, -halfSize);
    glVertex3f( halfSize, -halfSize, -halfSize);
    glVertex3f(-halfSize, -halfSize, -halfSize);
    
    // Draw top side:
	glColor3f(0, 1, 0); // green
    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(-halfSize,  halfSize,  halfSize);
    glVertex3f( halfSize,  halfSize,  halfSize);
    glVertex3f( halfSize,  halfSize, -halfSize);
    glVertex3f(-halfSize,  halfSize, -halfSize);
    
    // Draw bottom side:
	glColor3f(0, 0, 1); // green
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f( halfSize, -halfSize, -halfSize);
    glVertex3f( halfSize, -halfSize,  halfSize);
    glVertex3f(-halfSize, -halfSize,  halfSize);
    
    glEnd();

	shadowMap->unbind();
    
    //The above glBegin, glEnd, glNormal and glVertex calls can be replaced with a glut convenience function
    //glutSolidRoom(size);
    
    //Pop the save state off the matrix stack
    //This will undo the multiply we did earlier
    glPopMatrix();
}


void Room::update(UpdateData& data)
{
    //
}

void Room::spin(float radians)
{
    Matrix4 rotation;
    rotation.makeRotateY(radians);
    
    toWorld = toWorld * rotation;
}

void Room::depthRender(){
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glMultMatrixf(toWorld.ptr());
	//shader = shadow;

	float halfSize = size / 2.0;

	shadow->bind();

	GLuint depthMatrixID = glGetUniformLocation(shadow->getPid(), "depthMVP");
	glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, depthMVP.ptr());

	glBegin(GL_QUADS);

	glVertex3f(-halfSize, halfSize, halfSize);
	glVertex3f(halfSize, halfSize, halfSize);
	glVertex3f(halfSize, -halfSize, halfSize);
	glVertex3f(-halfSize, -halfSize, halfSize);

	glVertex3f(-halfSize, halfSize, halfSize);
	glVertex3f(-halfSize, halfSize, -halfSize);
	glVertex3f(-halfSize, -halfSize, -halfSize);
	glVertex3f(-halfSize, -halfSize, halfSize);

	glVertex3f(halfSize, halfSize, halfSize);
	glVertex3f(halfSize, halfSize, -halfSize);
	glVertex3f(halfSize, -halfSize, -halfSize);
	glVertex3f(halfSize, -halfSize, halfSize);

	glVertex3f(-halfSize, halfSize, -halfSize);
	glVertex3f(halfSize, halfSize, -halfSize);
	glVertex3f(halfSize, -halfSize, -halfSize);
	glVertex3f(-halfSize, -halfSize, -halfSize);

	glVertex3f(-halfSize, halfSize, halfSize);
	glVertex3f(halfSize, halfSize, halfSize);
	glVertex3f(halfSize, halfSize, -halfSize);
	glVertex3f(-halfSize, halfSize, -halfSize);

	glVertex3f(-halfSize, -halfSize, -halfSize);
	glVertex3f(halfSize, -halfSize, -halfSize);
	glVertex3f(halfSize, -halfSize, halfSize);
	glVertex3f(-halfSize, -halfSize, halfSize);

	glEnd();

	shadow->unbind();

	glPopMatrix();
}




