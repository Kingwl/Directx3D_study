#include "d3dUtility.h"

const int Height = 600;
const int Width = 800;

IDirect3DVertexBuffer9 *Quad = 0;
IDirect3DTexture9 *Tex = 0;
IDirect3DDevice9 *D3DDevice = nullptr;
bool state = true;

struct Vertex{
	Vertex(){};
	Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
		:_x(x), _y(y), _z(z), _nx(nx), _ny(ny), _nz(nz), _u(u), _v(v)
	{};
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v;
	static const DWORD FVF;

};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;


bool Setup()
{
	D3DDevice->CreateVertexBuffer(
		3 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&Quad,
		0);

	Vertex *v;
	Quad->Lock(0, 0, (void**)&v, 0);
	v[0] = Vertex(-1.0f, -1.0f, 1.25f,
					0.0f, 0.0f, -1.0f,
					0.0f, 1.0f);

	v[1] = Vertex(-1.0f, 1.0f, 1.25f,
					0.0f, 0.0f, -1.0f,
					0.0f, 0.0f);

	v[2] = Vertex(1.0f, 1.0f, 1.25f,
					0.0f, 0.0f, -1.0f,
					1.0f, 0.0f);

	v[3] = Vertex(-1.0f, -1.0f, 1.25f,
					0.0f, 0.0f, -1.0f,
					0.0f, 1.0f);

	v[4] = Vertex(1.0f, 1.0f, 1.25f, 
					0.0f, 0.0f, -1.0f,
					1.0f, 0.0f);

	v[5] = Vertex(1.0f, -1.0f, 1.25f,
					0.0f, 0.0f, -1.0f, 
					1.0f, 1.0f);
	Quad->Unlock();

	D3DXCreateTextureFromFile(
		D3DDevice,
		"logo.bmp",
		&Tex);

	D3DDevice->SetTexture(0, Tex);
	D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	D3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	D3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f,
		(float)Width / (float)Height,
		1.0f,
		1000.0f);
	D3DDevice->SetTransform(D3DTS_PROJECTION, &proj);
	
	return true;
}
void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(Quad);
	d3d::Release<IDirect3DTexture9*>(Tex);
}
bool Display(float timeDelta)
{
	if (D3DDevice)
	{
		
		D3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		D3DDevice->BeginScene();

		D3DDevice->SetStreamSource(0, Quad, 0, sizeof(Vertex));
		D3DDevice->SetFVF(Vertex::FVF);
		D3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);


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
		else if (wParam == VK_SPACE)
		{
			state = !state;
			D3DDevice->SetRenderState(D3DRS_LIGHTING, state);
		}
		break;
	default:
		break;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!d3d::initD3D(hInstance, Width, Height, true, D3DDEVTYPE_HAL, &D3DDevice))
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