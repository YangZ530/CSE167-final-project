#ifndef CSE167_Window_h
#define CSE167_Window_h

#include "Vector3.h"

class Shader;

class Window	  // OpenGL output window related routines
{
protected:


	static Vector3 trackBall(float, float);

	static Shader* shader;
	static Shader* bloom;

	static void genFBO();
	static void setupLightMatrices(float position_x, float position_y, float position_z, float lookAt_x, float lookAt_y, float lookAt_z);
	static void setupMatrices(float position_x, float position_y, float position_z, float lookAt_x, float lookAt_y, float lookAt_z);
	static void setTextureMatrix(void);

public:
    
	static int width, height, pressedMouse, mouse_x, mouse_y;
	static float radians;
	static bool move_forward, move_backward, move_left, move_right, fpsMode;

    static void initialize(void);
    static void idleCallback(void);
    static void reshapeCallback(int, int);
    static void displayCallback(void);
	static void processNormalKeys(unsigned char key, int x, int y);
	static void processMouse(int button, int state, int x, int y);
	static void processMotion(int x, int y);
	static void processKeyup(unsigned char key, int x, int y);
};

#endif

