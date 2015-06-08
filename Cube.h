#ifndef CSE167_Cube_h
#define CSE167_Cube_h

#include "Drawable.h"
#include "glm/glm.hpp"

class Shader;

class Cube : public Drawable
{

public:
	Shader *shader;
	Shader *lighting, *shadow, *shadowMap;

	float size;

	glm::mat4 depthMVP;
	glm::mat4 depthBiasMVP;

	Cube(float);
	virtual ~Cube(void);

	virtual void draw(DrawData&);
	virtual void update(UpdateData&);

	void depthRender();

	void spin(float);

};
#endif
