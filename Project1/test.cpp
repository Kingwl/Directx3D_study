#include "d3dUtility.h"
#define FVF_COLOR (D3DFVF_XYZ | D3DFVF_DIFFUSE)
struct ColorVertex
{
	ColorVertex(){}

	ColorVertex(float x, float y, float z, D3DCOLOR c)
	{
		_x = x;	 _y = y;  _z = z;  _color = c;
	}

	float _x, _y, _z;
	D3DCOLOR _color;

	static const DWORD FVF;
};
const DWORD ColorVertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
const int Height = 600;
const int Width = 800;
D3DXMATRIX World;
IDirect3DVertexBuffer9 *Triangel = nullptr;
IDirect3DDevice9 *D3DDevice = nullptr;

bool Setup()
{
	D3DDevice->CreateVertexBuffer(
		3 * sizeof(ColorVertex),
		D3DUSAGE_WRITEONLY,
		ColorVertex::FVF,
		D3DPOOL_MANAGED,
		&Triangel,
		0);

	ColorVertex *vertexs = nullptr;

	Triangel->Lock(0, 0, (void**)&vertexs, 0);

	vertexs[0] = ColorVertex(-1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(255, 0, 0));
	vertexs[1] = ColorVertex(0.0f, 1.0f, 2.0f, D3DCOLOR_XRGB(0, 255, 0));
	vertexs[2] = ColorVertex(1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(0, 0, 255));
	Triangel->Unlock();

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f,
		(float)Width / (float)Height,
		1.0f,
		10.0f);
	D3DDevice->SetTransform(D3DTS_PROJECTION, &proj);

	D3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	return true;
}
void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(Triangel);
}
bool Display(float timeDelta)
{
	if (D3DDevice)
	{

		D3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

		D3DDevice->BeginScene();

		D3DDevice->SetStreamSource(0, Triangel, 0, sizeof(ColorVertex));
		D3DDevice->SetFVF(ColorVertex::FVF);

		D3DXMatrixTranslation(&World, -1.25f, 0.0f, 0.0f);
		D3DDevice->SetTransform(D3DTS_WORLD, &World);

		D3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
		D3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0,1);

		D3DXMatrixTranslation(&World, 1.25f, 0.0f, 0.0f);
		D3DDevice->SetTransform(D3DTS_WORLD, &World);

		D3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		D3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0,1);


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