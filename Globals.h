#ifndef CSE167_Globals_h
#define CSE167_Globals_h

#include "Camera.h"
#include "Light.h"
#include "DrawData.h"
#include "UpdateData.h"

class Globals
{
    
public:
    
    static Camera camera;
    //static Cube cube;
	//static Cube room;
    static Light ptLight;
	static Light spotL;
	static Light dirLight;
    static DrawData drawData;
    static UpdateData updateData;
    //Feel free to add more member variables as needed
    
};

#endif
