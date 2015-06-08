#ifndef CSE167_Cube_h
#define CSE167_Cube_h

#include "Drawable.h"

class Shader;

class Cube : public Drawable
{

public:
	Shader *shader;
	Shader *lighting, *shadow, *shadowMap;

	float size;

	Matrix4 depthMVP;
	Matrix4 depthBiasMVP;

	Cube(float);
	virtual ~Cube(void);

	virtual void draw(DrawData&);
	virtual void update(UpdateData&);

	void depthRender(DrawData& data);
	
	void spin(float);

};
#endif
