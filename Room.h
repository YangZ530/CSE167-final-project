#ifndef CSE167_Room_h
#define CSE167_Room_h

#include "Drawable.h"

class Shader;

class Room : public Drawable
{
    
public:
	Shader *shader;
    
    float size;
    
    Room(float);
    virtual ~Room(void);
    
    virtual void draw(DrawData&);
    virtual void update(UpdateData&);
    
    void spin(float);
    
};

#endif

