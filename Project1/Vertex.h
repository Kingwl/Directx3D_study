#pragma once
#ifndef _VERTEX_
#define _VERTEX_
#endif

#include <windows.h>
#include "d3dx9.h"

class Vertex
{
public:
	Vertex(){};
	Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
		:_x(x), _y(y), _z(z), _nx(nx), _ny(ny), _nz(nz), _u(u), _v(v){};
	~Vertex(){};
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v;
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
};

