#include "Material.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

Material::Material()
{
	//Material setup goes hurr
	ambientColor = Color::ambientMaterialDefault();
	diffuseColor = Color::diffuseMaterialDefault();
	specularColor = Color::specularMaterialDefault();
	emissionColor = Color::emissionMaterialDefault();
	shininess = 10.0;

	//Set the initial color to a random pastel color
	//Can be removed once materials are implemented
	//color = Color::randomPastel();
}

Material::Material(int i){
	switch (i){
	case 0:
		ambientColor = Color(0.25, 0.25, 0.25);
		diffuseColor = Color(0.4, 0.4, 0.4);
		specularColor = Color(0.774597, 0.774597, 0.774597);
		emissionColor = Color::emissionMaterialDefault();
		shininess = 0.6 * 128.0;
		break;
	case 1:
		ambientColor = Color(1, 1, 1);
		diffuseColor = Color(0.4, 0.4, 0.4);
		specularColor = Color(0.1, 0.1, 0.1);
		emissionColor = Color::emissionMaterialDefault();
		shininess = 0.01 * 128.0;
		break;
	case 2:
		ambientColor = Color(0.19125, 0.0735, 0.0225);
		diffuseColor = Color(0.7038, 0.27048, 0.0828);
		specularColor = Color(0.256777, 0.137622, 0.086014);
		emissionColor = Color::emissionMaterialDefault();
		shininess = 0.1 * 128.0;
		break;
	}
}

Material::~Material()
{
	//Delete any dynamically allocated memory/objects here
}

void Material::apply(void)
{
	//Set the material properties for ambient, diffuse, specular, emission, and shininess
	//Hint: Lookup how glMaterialfv works
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientColor.ptr());
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor.ptr());
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseColor.ptr());
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissionColor.ptr());
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	//Set the color property (can be removed once materials are implemented)
	//Used for projects 1, 2, and 3
	//glColor3fv(color.ptr());
}



