#pragma once
#ifndef _CUBE_
#define _CUBE_


#include <windows.h>
#include "d3dx9.h"
#include "Vertex.h"
class Cube
{
public:
	Cube(IDirect3DDevice9 *D3DDevice);
	~Cube();
	bool drawCube(D3DXMATRIX* World, D3DMATERIAL9* Mtrl, IDirect3DTexture9* Texture);
private:
	IDirect3DDevice9 *_D3DDevice;
	IDirect3DVertexBuffer9 *_VertexBuffer;
	IDirect3DIndexBuffer9 *_IndexBuffer;
};
#endif
