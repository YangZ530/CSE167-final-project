#ifndef CSE167_Camera_h
#define CSE167_Camera_h

#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"

class Camera
{
private:

protected:

	Matrix4 c2w;   //Camera Matrix
	Matrix4 w2c;  //Inverse of Camera Matrix

	float vAngle;
	float hAngle;

public:

	Vector3 e;   //Center of Projection
	Vector3 d;   //Look At
	Vector3 up;  //Up Vector ^

	Camera(void);
	~Camera(void);

	void update(void);

	Matrix4& getMatrix(void);
	Matrix4& getInverseMatrix(void);

	void set(Vector3& d, Vector3& e, Vector3& up);

	void tiltLeftright(float);
	//void tiltRight(float);
	void tiltUpdown(float);
	//void tiltDown(float);
	void move(float);

	void arbitraryLook(Matrix4);
	void goForward(float);
	void goBack(float);
	void goLeft(float);
	void goRight(float);
};

#endif
