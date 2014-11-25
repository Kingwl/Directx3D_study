#include "d3dUtility.h"
#include "Vertex.h"
#include "Cube.h"
#include "Camera.h"
#include "Light.h"
#include "vector"

const int Height = 600;
const int Width = 800;


IDirect3DDevice9 *D3DDevice = nullptr;
Camera *camera;
Cube *cube;
std::vector<Cube*>rendVec;

bool lightState = true;

bool Setup()
{
	D3DLIGHT9 light;
	D3DXVECTOR3 d(0.717f, 0.0f, 0.717f);
	D3DXCOLOR c = d3d::WHITE;
	light = d3d::InitDirectionalLight(&d, &c);
	D3DDevice->SetLight(0, &light);
	D3DDevice->LightEnable(0, true);


	cube = new Cube(D3DDevice);
	camera = new Camera(D3DDevice);
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

}
bool Display(float timeDelta)
{
	if (D3DDevice)
	{

		if (::GetAsyncKeyState('W') & 0x8000f)
		{
			camera->walk(4.0f * timeDelta);
		}
		if (::GetAsyncKeyState('S') & 0x8000f)
		{
			camera->walk(-4.0f * timeDelta);
		}
		if (::GetAsyncKeyState('A') & 0x8000f)
		{
			camera->strafe(4.0f * timeDelta);
		}
		if (::GetAsyncKeyState('D') & 0x8000f)
		{
			camera->strafe(-4.0f * timeDelta);
		}
		if (::GetAsyncKeyState(VK_SHIFT) & 0x8000f)
		{
			camera->fly(4.0f * timeDelta);
		}
		if (::GetAsyncKeyState(VK_CONTROL) & 0x8000f)
		{
			camera->fly(-4.0f * timeDelta);
		}
		D3DXMATRIX V;
		camera->getViewMatrix(&V);
		D3DDevice->SetTransform(D3DTS_VIEW, &V);

		D3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0xff000000, 1.0f, 0);
		D3DDevice->BeginScene();

		D3DMATERIAL9 mtrl = d3d::WHITE_MTRL;
		D3DXMATRIX C;
		D3DXMatrixIdentity(&C);
		cube->drawCube(&C,&mtrl,0);
		

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
			lightState = !lightState;
			D3DDevice->SetRenderState(D3DRS_LIGHTING, lightState);
		}
		break;
	case WM_MOUSEMOVE:
		
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