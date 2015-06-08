#ifndef CSE167_Room_h
#define CSE167_Room_h

#include "Drawable.h"

class Shader;

class Room : public Drawable
{
    
public:
	Shader *shader;
	Shader *lighting, *shadow, *shadowMap;
    
    float size;

	Matrix4 depthMVP;
	Matrix4 depthBiasMVP;
    
    Room(float);
    virtual ~Room(void);
    
    virtual void draw(DrawData&);
    virtual void update(UpdateData&);

	void depthRender(DrawData& data);
    
    void spin(float);
    
};

#endif

