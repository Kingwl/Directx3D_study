#include "d3dUtility.h"
#define FVF_COLOR (D3DFVF_XYZ | D3DFVF_DIFFUSE)
struct Vertex{
	Vertex(float x = 0.0f, float y = 0.0f, float z = 0.0f)
	:_x(x), _y(y), _z(z){}
	float _x, _y, _z;
	static const DWORD FVF;
};
struct ColorVertex{
	ColorVertex(float x = 0.0f, float y = 0.0f, float z = 0.0f)
	:_x(x), _y(y), _z(z){}
	float _x, _y, _z;
	D3DCOLOR color;
	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ;
const DWORD ColorVertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
const int Height = 600;
const int Width = 800;
D3DXMATRIX World;
IDirect3DVertexBuffer9 *D3DPT_TRIANGLEFAN = nullptr;

IDirect3DDevice9 *D3DDevice = nullptr;
IDirect3DVertexBuffer9 *VertexBuffer = nullptr;
IDirect3DIndexBuffer9 *IndexBuffer = nullptr;

bool Setup()
{
	D3DDevice->CreateVertexBuffer(
		8 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&VertexBuffer,
		0);
	D3DDevice->CreateIndexBuffer(
		36 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&IndexBuffer,
		0);
	Vertex *vertexs = nullptr;

	VertexBuffer->Lock(0, 0, (void**)&vertexs, 0);
	vertexs[0] = Vertex(-1.0f, -1.0f, -1.0f);
	vertexs[1] = Vertex(-1.0f, 1.0f, -1.0f);
	vertexs[2] = Vertex(1.0f, 1.0f, -1.0f);
	vertexs[3] = Vertex(1.0f, -1.0f, -1.0f);
	vertexs[4] = Vertex(-1.0f, -1.0f, 1.0f);
	vertexs[5] = Vertex(-1.0f, 1.0f, 1.0f);
	vertexs[6] = Vertex(1.0f, 1.0f, 1.0f);
	vertexs[7] = Vertex(1.0f, -1.0f, 1.0f);
	VertexBuffer->Unlock();

	WORD* indexs = nullptr;
	IndexBuffer->Lock(0, 0, (void**)&indexs, 0);
	indexs[0] = 0; indexs[1] = 1; indexs[2] = 2;
	indexs[3] = 0; indexs[4] = 2; indexs[5] = 3;

	indexs[6] = 4; indexs[7] = 6; indexs[8] = 5;
	indexs[9] = 4; indexs[10] = 7; indexs[11] = 6;

	indexs[12] = 4; indexs[13] = 5; indexs[14] = 1;
	indexs[15] = 4; indexs[16] = 1; indexs[17] = 0;

	indexs[18] = 3; indexs[19] = 2; indexs[20] = 6;
	indexs[21] = 3; indexs[22] = 6; indexs[23] = 7;

	indexs[24] = 1; indexs[25] = 5; indexs[26] = 6;
	indexs[27] = 1; indexs[28] = 6; indexs[29] = 2;

	indexs[30] = 4; indexs[31] = 0; indexs[32] = 3;
	indexs[33] = 4; indexs[34] = 3; indexs[35] = 7;

	IndexBuffer->Unlock();


	D3DXVECTOR3 pos(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX View;
	D3DXMatrixLookAtLH(&View, &pos, &target, &up);
	D3DDevice->SetTransform(D3DTS_VIEW, &View);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f,
		(float)Width / (float)Height,
		1.0f,
		10.0f);
	D3DDevice->SetTransform(D3DTS_PROJECTION, &proj);

	D3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return true;
}
void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(VertexBuffer);
	d3d::Release<IDirect3DIndexBuffer9*>(IndexBuffer);
}
bool Display(float timeDelta)
{
	if (D3DDevice)
	{
		D3DXMATRIX Rx, Ry;
		D3DXMatrixRotationX(&Rx, 3.14f / 4.0f);
		static float y = 0.0f;
		D3DXMatrixRotationY(&Ry, y);
		y += timeDelta;

		if (y >= 6.28f) y = 0.0f;
		D3DXMATRIX p = Rx * Ry;
		D3DDevice->SetTransform(D3DTS_WORLD, &p);

		D3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

		D3DDevice->BeginScene();

		D3DDevice->SetStreamSource(0, VertexBuffer, 0, sizeof(Vertex));
		D3DDevice->SetIndices(IndexBuffer);
		D3DDevice->SetFVF(Vertex::FVF);

		D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

		D3DDevice->EndScene();
		D3DDevice->Present(0, 0, 0, 0);

	}
	
	return true;
}
LRESULT CALLBACK d3d::winProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			::DestroyWindow(hWnd);
		}
		break;
	default:
		break;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!d3d::initD3D(hInstance, 300, 300, true, D3DDEVTYPE_HAL, &D3DDevice))
	{
		::MessageBox(nullptr, "initD3d failed", "error", 0);
		return 0;
	}
	if (!Setup())
	{
		::MessageBox(nullptr, "Setup failed", "error", 0);
		return 0;
	}
	d3d::EnterMsgLoop(Display);
	Cleanup();
	D3DDevice->Release();
	return 0;
}