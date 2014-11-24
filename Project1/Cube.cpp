#include "Cube.h"


Cube::Cube(IDirect3DDevice9 *D3DDevice)
:isDifferent(false)
{
	_D3DDevice = D3DDevice;

	_D3DDevice->CreateVertexBuffer(
		24 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&_VertexBuffer,
		0);

	Vertex* v;
	_VertexBuffer->Lock(0, 0, (void**)&v, 0);


	v[0] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[1] = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[2] = Vertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	v[3] = Vertex(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[4] = Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[5] = Vertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[6] = Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[7] = Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

	v[8] = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[9] = Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[10] = Vertex(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	v[11] = Vertex(1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	v[12] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	v[13] = Vertex(1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	v[14] = Vertex(1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
	v[15] = Vertex(-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

	v[16] = Vertex(-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[17] = Vertex(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[18] = Vertex(-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[19] = Vertex(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	v[20] = Vertex(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[21] = Vertex(1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[22] = Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[23] = Vertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	_VertexBuffer->Unlock();

	_D3DDevice->CreateIndexBuffer(
		36 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_IndexBuffer,
		0);

	WORD* i = nullptr;
	_IndexBuffer->Lock(0, 0, (void**)&i, 0);

	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	_IndexBuffer->Unlock();
}
Cube::Cube(IDirect3DDevice9 *D3DDevice, IDirect3DTexture9 *Texture[])
:Cube(D3DDevice)
{
	isDifferent = true;
	_Texture = Texture;

	
	//front
	_D3DDevice->CreateIndexBuffer(
		6 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_FaceIndexBuffer[frontFace],
		0);
	WORD* i = nullptr;
	_FaceIndexBuffer[frontFace]->Lock(0, 0, (void**)&i, 0);
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;
	_FaceIndexBuffer[frontFace]->Unlock();

	//back
	_D3DDevice->CreateIndexBuffer(
		6 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_FaceIndexBuffer[backFace],
		0);
	_FaceIndexBuffer[backFace]->Lock(0, 0, (void**)&i, 0);
	i[0] = 4; i[1] = 5; i[2] = 6;
	i[3] = 4; i[4] = 6; i[5] = 7;
	_FaceIndexBuffer[backFace]->Unlock();

	//left
	_D3DDevice->CreateIndexBuffer(
		6 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_FaceIndexBuffer[leftFace],
		0);
	_FaceIndexBuffer[leftFace]->Lock(0, 0, (void**)&i, 0);
	i[0] = 12; i[1] = 13; i[2] = 14;
	i[3] = 12; i[4] = 14; i[5] = 15;
	_FaceIndexBuffer[leftFace]->Unlock();


	//right

	_D3DDevice->CreateIndexBuffer(
		6 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_FaceIndexBuffer[rightFace],
		0);
	_FaceIndexBuffer[rightFace]->Lock(0, 0, (void**)&i, 0);
	i[0] = 8; i[1] = 9; i[2] = 10;
	i[3] = 8; i[4] = 10; i[5] = 11;
	_FaceIndexBuffer[rightFace]->Unlock();


	//top
	_D3DDevice->CreateIndexBuffer(
		6 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_FaceIndexBuffer[topFace],
		0);
	_FaceIndexBuffer[topFace]->Lock(0, 0, (void**)&i, 0);
	i[0] = 16; i[1] = 17; i[2] = 18;
	i[3] = 16; i[4] = 18; i[5] = 19;
	_FaceIndexBuffer[topFace]->Unlock();


	//bottom
	_D3DDevice->CreateIndexBuffer(
		6 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_FaceIndexBuffer[bottomFace],
		0);
	_FaceIndexBuffer[bottomFace]->Lock(0, 0, (void**)&i, 0);
	i[0] = 20; i[1] = 21; i[2] = 22;
	i[3] = 20; i[4] = 22; i[5] = 23;
	_FaceIndexBuffer[bottomFace]->Unlock();

}
Cube::~Cube()
{
	if (_VertexBuffer)
	{
		_VertexBuffer->Release();
		_VertexBuffer = nullptr;
	}
	if (_IndexBuffer)
	{
		_IndexBuffer->Release();
		_IndexBuffer = nullptr;
	}
}
bool Cube::drawCube(D3DXMATRIX* World, D3DMATERIAL9* Mtrl, IDirect3DTexture9* Texture)
{
	if (World)
	{
		_D3DDevice->SetTransform(D3DTS_WORLD, World);
	}
	if (Mtrl)
	{
		_D3DDevice->SetMaterial(Mtrl);
	}

	_D3DDevice->SetFVF(Vertex::FVF);

	if (isDifferent)
	{
		_D3DDevice->SetStreamSource(0, _VertexBuffer, 0, sizeof(Vertex));
		for (int i = 0; i < 6; i++)
		{
			_D3DDevice->SetIndices(_FaceIndexBuffer[i]);
			_D3DDevice->SetTexture(0, _Texture[i]);
			_D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 6);
		}

	}
	else{
		if (Texture)
		{
			_D3DDevice->SetTexture(0, Texture);
		}

		_D3DDevice->SetStreamSource(0, _VertexBuffer, 0, sizeof(Vertex));
		_D3DDevice->SetIndices(_IndexBuffer);
		_D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);
	}
	

	return true;
}

bool Cube::drawCube()
{
	if (_D3DDevice)
	{
		_D3DDevice->SetFVF(Vertex::FVF);
		_D3DDevice->SetStreamSource(0, _VertexBuffer, 0, sizeof(Vertex));
		_D3DDevice->SetIndices(_IndexBuffer);
		_D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);
	}
	return true;
}