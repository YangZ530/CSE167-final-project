#ifndef CSE167_Cube_h
#define CSE167_Cube_h

#include "Drawable.h"

class Shader;

class Cube : public Drawable
{
    
public:
	Shader *shader;
    
    float size;
    
    Cube(float);
    virtual ~Cube(void);
    
    virtual void draw(DrawData&);
    virtual void update(UpdateData&);
    
    void spin(float);
    
};

#endif

