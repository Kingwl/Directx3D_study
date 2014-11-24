#pragma once
#ifndef _CAMERA_
#define _CAMERA_
#include "d3dx9.h"
#include "windows.h"
class Camera
{
public:
	Camera(IDirect3DDevice9 *D3DDevice,float height, float width);
	void setLootAt(D3DXVECTOR3 *pos, D3DXVECTOR3 *tar, D3DXVECTOR3 *up);
	~Camera();

private:
	float _height;
	float _width;
	IDirect3DDevice9 *_D3DDevice;
	D3DXVECTOR3 *_posotion;
	D3DXVECTOR3 *_target;
	D3DXVECTOR3 *_up;

};
#endif
