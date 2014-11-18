#include "d3dUtility.h"

const int Height = 600;
const int Width = 800;

ID3DXMesh* Obj[4] = { 0, 0, 0, 0 };
D3DXMATRIX Worlds[4];
D3DMATERIAL9 Mtrls[4];



IDirect3DDevice9 *D3DDevice = nullptr;
bool state = true;

bool Setup()
{
	D3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	D3DXCreateTeapot(D3DDevice, &Obj[0], 0);
	D3DXCreateSphere(D3DDevice, 1.0f, 20, 20, &Obj[1], 0);
	D3DXCreateTorus(D3DDevice, 0.5f, 1.0f, 20, 20, &Obj[2], 0);
	D3DXCreateCylinder(D3DDevice, 0.5f, 0.5f, 2.0f, 20, 20, &Obj[3], 0);

	D3DXMatrixTranslation(&Worlds[0], 0.0f, 2.0f, 0.0f);
	D3DXMatrixTranslation(&Worlds[1], 0.0f, -2.0f, 0.0f);
	D3DXMatrixTranslation(&Worlds[2], -3.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&Worlds[3], 3.0f, 0.0f, 0.0f);

	Mtrls[0] = d3d::RED_MTRL;
	Mtrls[1] = d3d::BLUE_MTRL;
	Mtrls[2] = d3d::GREEN_MTRL;
	Mtrls[3] = d3d::YELLOW_MTRL;

	D3DXVECTOR3 dir(1.0f, 0.0f, 0.25f);
	D3DXCOLOR c = d3d::WHITE;
	D3DLIGHT9 dirLight = d3d::InitDirectionalLight(&dir, &c);

	D3DDevice->SetLight(0, &dirLight);
	D3DDevice->LightEnable(0,true);

	D3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	D3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

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
	for (int i = 0; i < 4; i++)
		d3d::Release<ID3DXMesh*>(Obj[i]);
}
bool Display(float timeDelta)
{
	if (D3DDevice)
	{

		static float angle = (3.0f * D3DX_PI) / 2.0f;
		static float height = 5.0f;

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			angle -= 0.5f * timeDelta;

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			angle += 0.5f * timeDelta;

		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			height += 5.0f * timeDelta;

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			height -= 5.0f * timeDelta;

		D3DXVECTOR3 position(cosf(angle) * 7.0f, height, sinf(angle) * 7.0f);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);

		D3DDevice->SetTransform(D3DTS_VIEW, &V);
		

		D3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		D3DDevice->BeginScene();

		for (int i = 0; i < 4; i++)
		{

			D3DDevice->SetMaterial(&Mtrls[i]);
			D3DDevice->SetTransform(D3DTS_WORLD, &Worlds[i]);
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