#include "d3dUtility.h"

const int Height = 600;
const int Width = 800;
ID3DXMesh *Obj[4] = { 0, 0, 0, 0 };
D3DXMATRIX ObjMatrix[4];
IDirect3DDevice9 *D3DDevice = nullptr;

bool Setup()
{
	D3DXCreateTeapot(D3DDevice, &Obj[0],0);
	D3DXCreateBox(D3DDevice,2.0f,2.0f,2.0f,&Obj[1],0);
	D3DXCreateCylinder(D3DDevice, 1.0f, 1.0f, 3.0f, 10, 10, &Obj[2], 0);
	D3DXCreateTorus(D3DDevice, 1.0f, 3.0f, 10, 10, &Obj[3], 0);

	D3DXMatrixTranslation(&ObjMatrix[0], 0.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&ObjMatrix[1], -5.0f, 0.0f, 5.0f);
	D3DXMatrixTranslation(&ObjMatrix[2], 5.0f, 0.0f, 5.0f);
	D3DXMatrixTranslation(&ObjMatrix[3], -5.0f, 0.0f, -5.0f);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f,
		(float)Width / (float)Height,
		1.0f,
		1000.0f);
	D3DDevice->SetTransform(D3DTS_PROJECTION, &proj);

	D3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return true;
}
void Cleanup()
{
	for (int i = 0; i < 4; i++)
	{
		d3d::Release<ID3DXMesh*>(Obj[i]);
	}
}
bool Display(float timeDelta)
{
	if (D3DDevice)
	{

		static float angle = (3.0 * D3DX_PI) / 2.0f;
		static float cameraHeight = 0.0f;
		static float cameraHeightDirection = 5.0f;

		D3DXVECTOR3 pos(cosf(angle) * 10.0f, cameraHeight, sinf(angle)* 1.0f);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

		D3DXMATRIX View;
		D3DXMatrixLookAtLH(&View, &pos, &target, &up);
		
		D3DDevice->SetTransform(D3DTS_VIEW, &View);

		angle += timeDelta;
		if (angle >= 6.28f)
			angle = 0.0f;

		cameraHeight += cameraHeightDirection * timeDelta;
		if (cameraHeight >= 10.0f)
		{
			cameraHeightDirection = -5.0f;
		}
		if (cameraHeight <= -10.0)
		{
			cameraHeightDirection = 5.0f;
		}

		D3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

		D3DDevice->BeginScene();
		for (int i = 0; i < 4; i++)
		{
			D3DDevice->SetTransform(D3DTS_WORLD, &ObjMatrix[i]);
			Obj[i]->DrawSubset(0);
		}

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