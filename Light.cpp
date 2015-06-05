#include "Light.h"
#include "Window.h"
#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define DIRECTIONAL 1
#define POINT 2
#define SPOT 3

Light::Light(int model)
{
	lightSource = model;
	switch (model)
	{
	case DIRECTIONAL:
		ambientColor = Color::black();
		diffuseColor = Color(0x888888ff);
		specularColor = Color::white();

		constantAttenuation = 1.0;
		linearAttenuation = 0.0;
		quadraticAttenuation = 0.0005;
		cutoff = -1;
		break;

	case POINT:
		ambientColor = Color::ambientDefault();
		diffuseColor = Color::green();
		specularColor = Color::green();

		constantAttenuation = 1.0;
		linearAttenuation = 0.0;
		quadraticAttenuation = 0.0005;
		cutoff = -1;
		break;

	case SPOT:
		ambientColor = Color::ambientDefault();

		diffuseColor = Color::blue();
		specularColor = Color::blue();

		constantAttenuation = 1.0;
		linearAttenuation = 0.0;
		//quadraticAttenuation = 0.005;
		quadraticAttenuation = 0.01;
		cutoff = 10;
		exponent = 0;
		break;
	}
}

Light::~Light()
{
	//Delete any dynamically allocated memory/objects here
}

void Light::bind(int id)
{
	if (id < 0 || id > 7) {
		std::cout << "ERROR: The light bind ID " << id << " is not valid!" << std::endl;
		return;
	}

	switch (lightSource)
	{
	case DIRECTIONAL:
		//Set the bindID
		bindID = id;

		//Configure the light at the bindID
		glEnable(GL_LIGHT0 + bindID);

		//Configure the color of the light
		glLightfv(GL_LIGHT0 + bindID, GL_AMBIENT, ambientColor.ptr());
		glLightfv(GL_LIGHT0 + bindID, GL_DIFFUSE, diffuseColor.ptr());
		glLightfv(GL_LIGHT0 + bindID, GL_SPECULAR, Color::white().ptr());

		//Configure the attenuation properties of the light
		//Constant Attenuation
		//Linear Attenuation
		glLightf(GL_LIGHT0 + bindID, GL_CONSTANT_ATTENUATION, constantAttenuation);

		//Position the light
		//glLightfv(GL_LIGHT0 + bindID, GL_POSITION, position.ptr());
		//glLightfv(GL_LIGHT0 + bindID, GL_SPOT_DIRECTION, direction.ptr());
		glLightfv(GL_LIGHT0 + bindID, GL_POSITION, direction.ptr());

		//Setup spotlight direction, angle, and exponent here
		break;

	case POINT:
		//Set the bindID
		bindID = id;

		//Configure the light at the bindID
		glEnable(GL_LIGHT0 + bindID);

		//Configure the color of the light
		glLightfv(GL_LIGHT0 + bindID, GL_AMBIENT, ambientColor.ptr());
		glLightfv(GL_LIGHT0 + bindID, GL_DIFFUSE, diffuseColor.ptr());
		glLightfv(GL_LIGHT0 + bindID, GL_SPECULAR, specularColor.ptr());

		//Configure the attenuation properties of the light
		//Constant Attenuation
		//Linear Attenuation
		glLightf(GL_LIGHT0 + bindID, GL_QUADRATIC_ATTENUATION, quadraticAttenuation);
		glLightf(GL_LIGHT0 + bindID, GL_CONSTANT_ATTENUATION, constantAttenuation);

		//Position the light
		//glLightfv(GL_LIGHT0 + bindID, GL_POSITION, position.ptr());
		//glLightfv(GL_LIGHT0 + bindID, GL_SPOT_DIRECTION, direction.ptr());
		glLightfv(GL_LIGHT0 + bindID, GL_POSITION, position.ptr());

		//Setup spotlight direction, angle, and exponent here
		break;

	case SPOT:
		//Set the bindID
		bindID = id;

		//Configure the light at the bindID
		glEnable(GL_LIGHT0 + bindID);

		//Configure the color of the light
		glLightfv(GL_LIGHT0 + bindID, GL_AMBIENT, ambientColor.ptr());
		glLightfv(GL_LIGHT0 + bindID, GL_DIFFUSE, diffuseColor.ptr());
		glLightfv(GL_LIGHT0 + bindID, GL_SPECULAR, specularColor.ptr());

		//Configure the attenuation properties of the light
		//Constant Attenuation
		//Linear Attenuation
		glLightf(GL_LIGHT0 + bindID, GL_QUADRATIC_ATTENUATION, quadraticAttenuation);
		glLightf(GL_LIGHT0 + bindID, GL_CONSTANT_ATTENUATION, constantAttenuation);

		//Position the light
		glLightfv(GL_LIGHT0 + bindID, GL_POSITION, position.ptr());
		glLightf(GL_LIGHT0 + bindID, GL_SPOT_CUTOFF, cutoff);
		glLightfv(GL_LIGHT0 + bindID, GL_SPOT_DIRECTION, direction.ptr());
		glLightf(GL_LIGHT0 + bindID, GL_SPOT_EXPONENT, exponent);
		//Setup spotlight direction, angle, and exponent here
		break;
	}
}

void Light::unbind(void)
{
	glDisable(GL_LIGHT0 + bindID);
	bindID = -1;
}
