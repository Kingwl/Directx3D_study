#include "d3dUtility.h"

const int Height = 600;
const int Width = 800;
struct Vertex{
	Vertex(){};
	Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
	:_x(x),_y(y),_z(z),_nx(nx),_ny(ny),_nz(nz),_u(u),_v(v){};
	~Vertex(){};
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v;
	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

ID3DXMesh *Object = nullptr;
D3DMATERIAL9 Mtrl,BkMtrl;

IDirect3DDevice9 *D3DDevice = nullptr;
IDirect3DTexture9 *texture[4] = { 0, 0, 0, 0 };
IDirect3DVertexBuffer9 *Quad = nullptr;
bool state = true;



bool Setup()
{
	Mtrl = d3d::RED_MTRL;
	Mtrl.Diffuse.a = 0.5f;
	BkMtrl = d3d::WHITE_MTRL;

	D3DXCreateTeapot(D3DDevice, &Object, 0);

	
	D3DDevice->CreateVertexBuffer(
			6 * sizeof(Vertex),
			D3DUSAGE_WRITEONLY,
			Vertex::FVF,
			D3DPOOL_MANAGED,
			&Quad,
			0);

	Vertex *v;

	Quad->Lock(0, 0, (void**)&v, 0);
	v[0] = Vertex(-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = Vertex(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex(1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	
	v[3] = Vertex(1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[4] = Vertex(1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	v[5] = Vertex(-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);



	Quad->Unlock();


	D3DLIGHT9 dir;
	::ZeroMemory(&dir, sizeof(dir));

	dir.Type = D3DLIGHT_DIRECTIONAL;
	dir.Diffuse = d3d::WHITE;
	dir.Specular = d3d::WHITE * 0.2f;
	dir.Ambient = d3d::WHITE * 0.6f;
	dir.Direction = D3DXVECTOR3(0.707f, 0.0f, 0.707f);

	D3DDevice->SetLight(0, &dir);
	D3DDevice->LightEnable(0, true);

	D3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	D3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	D3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	D3DXCreateTextureFromFile(
		D3DDevice,
		"logo.bmp",
		&texture[0]);

	D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	D3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	D3DXVECTOR3 pos(0.0f, 0.0f, -3.0f);
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
	d3d::Release<IDirect3DVertexBuffer9*>(Quad);
	d3d::Release<ID3DXMesh*>(Object);
	d3d::Release<IDirect3DTexture9*>(texture[0]);
}
bool Display(float timeDelta)
{
	if (D3DDevice)
	{
		
		if (::GetAsyncKeyState('A') & 0x8000f)
			Mtrl.Diffuse.a += 0.01f;
		if (::GetAsyncKeyState('S') & 0x8000f)
			Mtrl.Diffuse.a -= 0.01f;

		// force alpha to [0, 1] interval
		if (Mtrl.Diffuse.a > 1.0f)
			Mtrl.Diffuse.a = 1.0f;
		if (Mtrl.Diffuse.a < 0.0f)
			Mtrl.Diffuse.a = 0.0f;

		D3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		D3DDevice->BeginScene();

		D3DXMATRIX W;
		D3DXMatrixIdentity(&W);
		D3DDevice->SetTransform(D3DTS_WORLD, &W);
		D3DDevice->SetFVF(Vertex::FVF);
		D3DDevice->SetStreamSource(0, Quad, 0, sizeof(Vertex));
		D3DDevice->SetMaterial(&BkMtrl);
		D3DDevice->SetTexture(0,texture[0]);

		D3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);


		D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		D3DDevice->SetMaterial(&Mtrl);
		D3DDevice->SetTransform(D3DTS_WORLD, &W);
		D3DDevice->SetTexture(0, 0);
		Object->DrawSubset(0);
		D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);





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