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


IDirect3DDevice9 *D3DDevice = nullptr;
IDirect3DTexture9 *texture = nullptr;
IDirect3DVertexBuffer9 *VertexBuffer = nullptr;
IDirect3DIndexBuffer9 *IndexBuffer = nullptr;
bool state = true;
void initCube()
{
	if (D3DDevice)
	{
		D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		D3DDevice->CreateVertexBuffer(
			24*sizeof(Vertex),
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

		Vertex *v = nullptr;
		VertexBuffer->Lock(0, 0, (void**)&v, 0);
		//top
		v[0] = Vertex(-1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f);
		v[1] = Vertex(-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f);
		v[2] = Vertex( 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f);
		v[3] = Vertex( 1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f);

		//bottom

		v[4] = Vertex(-1.0f, -1.0f,  -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f);
		v[5] = Vertex(-1.0f,  1.0f,  -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f);  
		v[6] = Vertex( 1.0f,  1.0f,  -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f);
		v[7] = Vertex( 1.0f, -1.0f,  -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f);

		//left

		v[8] = Vertex(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[9] = Vertex(-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		v[10] = Vertex(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
		v[11] = Vertex(-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);


		//right

		v[12] = Vertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[13] = Vertex(1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		v[14] = Vertex(1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
		v[15] = Vertex(1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);


		//front

		v[16] = Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		v[17] = Vertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
		v[18] = Vertex(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
		v[19] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);


		//back

		v[20] = Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		v[21] = Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
		v[22] = Vertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
		v[23] = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);

		VertexBuffer->Unlock();

		WORD *i = nullptr;
		IndexBuffer->Lock(0, 0, (void**)&i, 0);

		//top index
		i[0] = 0; i[1] = 1; i[2] = 2;
		i[3] = 0; i[4] = 2; i[5] = 3;

		//bottom index

		i[6] = 4; i[7] = 5; i[8] = 6;
		i[9] = 4; i[10] = 6; i[11] = 7;

		//left index

		i[12] = 8; i[13] = 9; i[14] = 10;
		i[15] = 8; i[16] = 10; i[17] = 11;

		//right index

		i[18] = 12; i[19] = 13; i[20] = 14;
		i[21] = 12; i[22] = 14; i[23] = 15;

		//front index

		i[24] = 16; i[25] = 17; i[26] = 18;
		i[27] = 16; i[28] = 18; i[29] = 19;

		//back index

		i[30] = 20; i[31] = 21; i[32] = 22;
		i[33] = 20; i[34] = 22; i[35] = 23;

		IndexBuffer->Unlock();

	}
}


bool Setup()
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	light.Direction = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	D3DDevice->SetLight(0, &light);
	D3DDevice->LightEnable(0, true);

	D3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	D3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	D3DXCreateTextureFromFile(D3DDevice, "logo.bmp", &texture);
	D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	D3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	initCube();
	
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
	d3d::Release<IDirect3DIndexBuffer9*>(IndexBuffer);
	d3d::Release<IDirect3DVertexBuffer9*>(VertexBuffer);

}
bool Display(float timeDelta)
{
	if (D3DDevice)
	{
		static float angle = (0.0f * D3DX_PI) / 2.0f;
		static float height = 0.0f;

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			angle -= 0.5f * timeDelta;

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			angle += 0.5f * timeDelta;

		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			height += 5.0f * timeDelta;

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			height -= 5.0f * timeDelta;

		D3DXVECTOR3 position(cosf(angle) * 3.0f, height, sinf(angle) * 3.0f);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);
		D3DDevice->SetTransform(D3DTS_VIEW, &V);
		D3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x11111111, 1.0f, 0);
		D3DDevice->BeginScene();

		D3DDevice->SetTexture(0,texture);
		D3DDevice->SetStreamSource(0, VertexBuffer, 0, sizeof(Vertex));
		D3DDevice->SetIndices(IndexBuffer);
		D3DDevice->SetFVF(Vertex::FVF);
		D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);

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