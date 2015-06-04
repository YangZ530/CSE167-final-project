#include "Camera.h"
#include <iostream>

Camera::Camera()
{
	vAngle = 0;
	hAngle = 0;

	c2w.identity();
	e.set(0.0, -45.0, 20.0);
	d.set(0.0, -45.0, 0.0);
	up.set(0.0, 1.0, 0.0);

	update();
}

Camera::~Camera()
{
	//Delete and dynamically allocated memory/objects here
}

Matrix4& Camera::getMatrix()
{
	return c2w;
}

Matrix4& Camera::getInverseMatrix()
{
	return w2c;
}

void Camera::update()
{
	//e.print("e");
	//d.print("d");
	Vector3 x, y, z;
	Matrix4 iR, iT;
	//Update the Camera Matrix using d, e, and up
	//Solve for the z, x, and y axes of the camera matrix
	//Use these axes and the e vector to create a camera matrix c
	//Use c to solve for an inverse camera matrix ci
	z = (e - d).normalize();
	x = up.cross(z).normalize();
	y = z.cross(x).normalize();
	c2w.set(x.ptr()[0], x.ptr()[1], x.ptr()[2], 0,
		y.ptr()[0], y.ptr()[1], y.ptr()[2], 0,
		z.ptr()[0], z.ptr()[1], z.ptr()[2], 0,
		e.ptr()[0], e.ptr()[1], e.ptr()[2], 1);

	iR.set(x.ptr()[0], x.ptr()[1], x.ptr()[2], 0,
		y.ptr()[0], y.ptr()[1], y.ptr()[2], 0,
		z.ptr()[0], z.ptr()[1], z.ptr()[2], 0,
		0, 0, 0, 1);
	iR = iR.transpose();
	iT.set(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -e[0], -e[1], -e[2], 1);
	w2c = iR * iT;
}

void Camera::set(Vector3& e, Vector3& d, Vector3& up)
{
	this->e = e;
	this->d = d;
	this->up = up;
	update();
}

void Camera::tiltLeftright(float angle)
{
	Matrix4 trans;
	Vector3 dir;

	dir = d - e;
	trans.makeRotateArbitrary(dir, angle / 100);
	up = trans * up;
	update();
}

void Camera::tiltUpdown(float angle)
{
	Matrix4 trans;
	Vector3 dir;
	Vector3 axis;

	dir = d - e;
	dir = dir.normalize();
	axis = up.cross(dir);
	axis = axis.normalize();

	trans.makeRotateArbitrary(axis, angle / 100);
	dir = trans * dir;
	d = e + dir;
	up = dir.cross(axis);

	d.print("d");
	trans.print("trans");
	update();
}

void Camera::move(float distance)
{
	Matrix4 trans;
	Vector3 dir;

	dir = d - e;
	dir = dir.normalize();
	dir = dir.scale(distance);
	trans.makeTranslate(dir);
	e = (trans * e.toVector4(1)).toVector3();
	d = (trans * d.toVector4(1)).toVector3();

	update();
}


void Camera::arbitraryLook(Matrix4 rotation){
	Vector3 temp = d - e;
	temp = w2c * temp;
	temp = rotation * temp;
	temp = c2w * temp;
	d = e + temp;

	update();
}

void Camera::goForward(float scale){
	Vector3 f;
	Matrix4 tmp;
	f = up.cross((d - e).cross(up)).normalize();
	f = f.scale(scale);
	e = e + f;
	d = d + f;
	update();
}

void Camera::goBack(float scale){
	Vector3 f;
	Matrix4 tmp;
	f = up.cross((d - e).cross(up)).normalize().negate();
	f = f.scale(scale);
	e = e + f;
	d = d + f;
	update();
}

void Camera::goRight(float scale){
	Vector3 f;
	Matrix4 tmp;
	f = (d - e).cross(up).normalize();
	f = f.scale(scale);
	e = e + f;
	d = d + f;
	update();
}

void Camera::goLeft(float scale){
	Vector3 f;
	Matrix4 tmp;
	f = up.cross(d - e).normalize();
	f = f.scale(scale);
	e = e + f;
	d = d + f;
	update();
}