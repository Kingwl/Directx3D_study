#include "d3dUtility.h"

const int Height = 600;
const int Width = 800;
struct VertexNormal{
	VertexNormal() = default;
	VertexNormal(float x, float y, float z, float nx, float ny, float nz)
		:_x(x), _y(y), _z(z), _nx(nx), _ny(ny), _nz(nz){}
	float _x, _y, _z;
	float _nx, _ny, _nz;
	static const DWORD FVF;
}VertexNormalInfo;
const DWORD VertexNormal::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;
IDirect3DVertexBuffer9 *Pyramid;

IDirect3DDevice9 *D3DDevice = nullptr;
bool state = true;

bool Setup()
{
	D3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	D3DDevice->CreateVertexBuffer(
		12 * sizeof(VertexNormal),
		D3DUSAGE_WRITEONLY,
		VertexNormal::FVF,
		D3DPOOL_MANAGED,
		&Pyramid,
		0);

	VertexNormal* v = nullptr ;

	Pyramid->Lock(0,0,(void**)&v,0);
	v[0] = VertexNormal(-1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f);
	v[1] = VertexNormal(0.0f, 1.0f, 0.0f, 0.0f, 0.707f, -0.707f);
	v[2] = VertexNormal(1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f);

	v[3] = VertexNormal(-1.0f, 0.0f, 1.0f, -0.707f, 0.707f, 0.0f);
	v[4] = VertexNormal(0.0f, 1.0f, 0.0f, -0.707f, 0.707f, 0.0f);
	v[5] = VertexNormal(-1.0f, 0.0f, -1.0f, -0.707f, 0.707f, 0.0f);

	v[6] = VertexNormal(1.0f, 0.0f, -1.0f, 0.707f, 0.707f, 0.0f);
	v[7] = VertexNormal(0.0f, 1.0f, 0.0f, 0.707f, 0.707f, 0.0f);
	v[8] = VertexNormal(1.0f, 0.0f, 1.0f, 0.707f, 0.707f, 0.0f);

	v[9] = VertexNormal(1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f);
	v[10] = VertexNormal(0.0f, 1.0f, 0.0f, 0.0f, 0.707f, 0.707f);
	v[11] = VertexNormal(-1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f);

	Pyramid->Unlock();

	D3DMATERIAL9 mtrl;
	mtrl = d3d::InitMtrl(d3d::WHITE, d3d::WHITE, d3d::WHITE, d3d::RED,5.0f);
	D3DDevice->SetMaterial(&mtrl);

	D3DLIGHT9 dir;
	::ZeroMemory(&dir, sizeof(dir));
	dir.Type = D3DLIGHT_DIRECTIONAL;
	dir.Diffuse = d3d::WHITE;
	dir.Specular = d3d::WHITE * 0.3f;
	dir.Ambient = d3d::WHITE * 0.6f;
	dir.Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	D3DDevice->SetLight(0, &dir);
	D3DDevice->LightEnable(0, true);

	D3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	D3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

	D3DXVECTOR3 pos(0.0f, 1.0f, -3.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &pos, &target, &up);
	D3DDevice->SetTransform(D3DTS_VIEW, &V);

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
	d3d::Release<IDirect3DVertexBuffer9*>(Pyramid);
}
bool Display(float timeDelta)
{
	if (D3DDevice)
	{

		
		D3DXMATRIX yRot;

		static float y = 0.0f;

		D3DXMatrixRotationY(&yRot, y);
		y += timeDelta;

		if (y >= 6.28f)
			y = 0.0f;

		D3DDevice->SetTransform(D3DTS_WORLD, &yRot);

		//
		// Draw the scene:
		//

		D3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		D3DDevice->BeginScene();

		D3DDevice->SetStreamSource(0, Pyramid, 0, sizeof(VertexNormal));
		D3DDevice->SetFVF(VertexNormal::FVF);
		D3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4);

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