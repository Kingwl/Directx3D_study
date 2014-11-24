#pragma once
#ifndef _CUBE_
#define _CUBE_


#include <windows.h>
#include "d3dx9.h"
#include "Vertex.h"
class Cube
{
	static const int frontFace = 0;
	static const int backFace = 1;
	static const int leftFace = 2;
	static const int rightFace = 3;
	static const int topFace = 4;
	static const int bottomFace = 5;
public:
	Cube(IDirect3DDevice9 *D3DDevice);
	Cube(IDirect3DDevice9 *D3DDevice, IDirect3DTexture9 *Texture[]);
	~Cube();
	bool drawCube(D3DXMATRIX* World, D3DMATERIAL9* Mtrl, IDirect3DTexture9* Texture);
	bool drawCube();
private:
	bool isDifferent;
	IDirect3DTexture9 **_Texture;
	IDirect3DDevice9 *_D3DDevice;
	IDirect3DVertexBuffer9 *_VertexBuffer;
	IDirect3DIndexBuffer9 *_IndexBuffer;
	IDirect3DIndexBuffer9 *_FaceIndexBuffer[6];

};
#endif
