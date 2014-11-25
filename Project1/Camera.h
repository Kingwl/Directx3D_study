#pragma once
#ifndef _CAMERA_
#define _CAMERA_
#include "d3dx9.h"
#include "windows.h"
class Camera
{
public:
	Camera(IDirect3DDevice9 *Device);
	~Camera();
	void strafe(float units);
	void fly(float units);
	void walk(float units);
	void pitch(float units);
	void yaw(float units);
	void roll(float units);
	void getViewMatrix(D3DXMATRIX *V);
	void getPosition(D3DXVECTOR3 *pos);
	void setPosition(D3DXVECTOR3 *pos);
	void getRight(D3DXVECTOR3 *	right);
	void getUp(D3DXVECTOR3 *up);
	void getLook(D3DXVECTOR3 *look);
	void reset();

private:
	D3DXVECTOR3 _right, _up, _look, _pos;
	IDirect3DDevice9 *_Device;
};
#endif
