#include "d3dUtility.h"
#define FVF_COLOR (D3DFVF_XYZ | D3DFVF_DIFFUSE)
struct Vertex{
	Vertex(float x = 0.0f, float y = 0.0f, float z = 0.0f)
	:_x(x), _y(y), _z(z){}
	float _x, _y, _z;
};
struct ColorVertex{
	ColorVertex(float x = 0.0f, float y=0.0f, float z = 0.0f, DWORD color = 0)
	:_x(x), _y(y), _z(z), _color(color){}
	float _x, _y, _z;
	DWORD _color;
};
struct QUAD{
	Vertex vertex[4];
};
IDirect3DDevice9 *D3DDevice = nullptr;
bool Setup()
{
	return true;
}
void Cleanup()
{

}
bool Display(float timeDelta)
{
	if (D3DDevice)
	{
		D3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x000000, 1.0f, 0);
		D3DDevice->BeginScene();
		ID3DXMesh *mesh = 0;
		D3DXCreateTeapot(D3DDevice, &mesh, 0);
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