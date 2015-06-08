#ifndef CSE167_Room_h
#define CSE167_Room_h

#include "Drawable.h"
#include "glm/glm.hpp"

class Shader;

class Room : public Drawable
{
    
public:
	Shader *shader;
	Shader *lighting, *shadow, *shadowMap;
    
    float size;

	glm::mat4 depthMVP;
	glm::mat4 depthBiasMVP;
    
    Room(float);
    virtual ~Room(void);
    
    virtual void draw(DrawData&);
    virtual void update(UpdateData&);

	void depthRender();
    
    void spin(float);
    
};

#endif

