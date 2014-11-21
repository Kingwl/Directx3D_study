#include "d3dUtility.h"
#include "Vertex.h"
#include "Cube.h"

const int Height = 600;
const int Width = 800;


IDirect3DDevice9 *D3DDevice = nullptr;

IDirect3DTexture9 *WallText = nullptr;
IDirect3DTexture9 *FloorText = nullptr;
IDirect3DTexture9 *MirrorText = nullptr;

D3DMATERIAL9 WallMtrl = d3d::WHITE_MTRL;
D3DMATERIAL9 FloorMtrl = d3d::WHITE_MTRL;
D3DMATERIAL9 MirrorMtrl = d3d::WHITE_MTRL;

ID3DXMesh *teaPort = nullptr;
D3DXVECTOR3 teaPortPosition(0.0f, 2.0f, -2.0f);
D3DMATERIAL9 teaportMtrl;

IDirect3DVertexBuffer9 *FloorBuffer = nullptr;
IDirect3DVertexBuffer9 *WallBuffer = nullptr;
IDirect3DVertexBuffer9 *MirrorBuffer = nullptr;

bool lightState = true;

void setupTexture()
{
	if (D3DDevice)
	{
		D3DDevice->CreateVertexBuffer(
			6 * sizeof(Vertex),
			D3DUSAGE_WRITEONLY,
			Vertex::FVF,
			D3DPOOL_MANAGED,
			&FloorBuffer,0);

		D3DDevice->CreateVertexBuffer(
			12 * sizeof(Vertex),
			D3DUSAGE_WRITEONLY,
			Vertex::FVF,
			D3DPOOL_MANAGED,
			&WallBuffer,0);

		D3DDevice->CreateVertexBuffer(
			6 * sizeof(Vertex),
			D3DUSAGE_WRITEONLY,
			Vertex::FVF,
			D3DPOOL_MANAGED,
			&MirrorBuffer, 
			0);

		Vertex *v = nullptr;

		FloorBuffer->Lock(0, 0, (void**)&v, 0);
		v[0] = Vertex(-7.0f,0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		v[1] = Vertex(7.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		v[2] = Vertex(-7.0f, 0.0f, -4.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);

		v[3] = Vertex(7.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		v[4] = Vertex(7.0f, 0.0f, -4.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
		v[5] = Vertex(-7.0f, 0.0f, -4.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);

		FloorBuffer->Unlock();

		WallBuffer->Lock(0, 0, (void**)&v, 0);

		v[0] = Vertex(-7.0f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		v[1] = Vertex(-3.0f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
		v[2] = Vertex(-7.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);

		v[3] = Vertex(-3.0f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
		v[4] = Vertex(-3.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
		v[5] = Vertex(-7.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);

		v[6] = Vertex(3.0f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		v[7] = Vertex(7.0f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
		v[8] = Vertex(3.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);

		v[9] = Vertex(7.0f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
		v[10] = Vertex(7.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
		v[11] = Vertex(3.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);

		WallBuffer->Unlock();

		MirrorBuffer->Lock(0, 0, (void**)&v, 0);
		
		v[0] = Vertex(-3.0f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		v[1] = Vertex(3.0f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
		v[2] = Vertex(-3.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);

		v[3] = Vertex(3.0f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
		v[4] = Vertex(3.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
		v[5] = Vertex(-3.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);

		MirrorBuffer->Unlock();

		D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

		D3DXCreateTextureFromFile(
			D3DDevice,
			"floor.jpg",
			&FloorText);

		D3DXCreateTextureFromFile(
			D3DDevice,
			"wall.jpg",
			&WallText);

		D3DXCreateTextureFromFile(
			D3DDevice,
			"mirror.bmp",
			&MirrorText);

		D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
		D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
		D3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	}
}

void DrawVertex(IDirect3DVertexBuffer9* VertexBuffer, D3DMATERIAL9* Mtrl, IDirect3DTexture9 *Texture, int num)
{
	D3DXMATRIX I;
	D3DXMatrixTranslation(&I, 0.0f, 0.0f, 0.0f);
	D3DDevice->SetTransform(D3DTS_WORLD, &I);
	if (VertexBuffer && D3DDevice)
	{
		if (Mtrl)
		{
			D3DDevice->SetMaterial(Mtrl);
		}
		if (Texture)
		{
			D3DDevice->SetTexture(0,Texture);
		}

		D3DDevice->SetStreamSource(0, VertexBuffer, 0, sizeof(Vertex));
		D3DDevice->SetFVF(Vertex::FVF);
		D3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2 * num);
	}
}
void initLight()
{
	D3DLIGHT9 dir;
	::ZeroMemory(&dir, sizeof(dir));

	auto d = D3DXVECTOR3(-0.707f, 0.707f, -0.707f);
	auto c = d3d::WHITE;
	dir = d3d::InitDirectionalLight(&d, &c);

	D3DDevice->SetLight(0, &dir);
	D3DDevice->LightEnable(0, true);
	D3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	D3DDevice->SetRenderState(D3DRS_NORMALDEGREE, true);
	D3DDevice->SetRenderState(D3DRS_SPECULARENABLE, false);
}

void initObject()
{
	D3DXCreateTeapot(D3DDevice, &teaPort, 0);
	teaportMtrl = d3d::RED_MTRL;
}

void DrawObject()
{
	D3DDevice->SetMaterial(&teaportMtrl);
	D3DDevice->SetTexture(0, 0);

	D3DXMATRIX W;
	D3DXMatrixTranslation(&W, teaPortPosition.x, teaPortPosition.y, teaPortPosition.z);
	D3DDevice->SetTransform(D3DTS_WORLD, &W);

	teaPort->DrawSubset(0);
}
void RenderShadow()
{
	D3DDevice->SetRenderState(D3DRS_STENCILENABLE, true);
	D3DDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	D3DDevice->SetRenderState(D3DRS_STENCILREF, 0x0);
	D3DDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	D3DDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	D3DDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	D3DDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	D3DDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);

	D3DXVECTOR4 lightDirection(0.707f, -0.707f, 0.707f, 0.0f);
	D3DXPLANE groundPlane(0.0f, -1.0f, 0.0f, 0.0f);

	D3DXMATRIX S;
	D3DXMatrixShadow(&S, &lightDirection, &groundPlane);

	D3DXMATRIX T;
	D3DXMatrixTranslation(&T, teaPortPosition.x, teaPortPosition.y, teaPortPosition.z);

	D3DXMATRIX W = T * S;
	D3DDevice->SetTransform(D3DTS_WORLD, &W);

	D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	D3DMATERIAL9 mtrl = d3d::InitMtrl(d3d::BLACK, d3d::BLACK, d3d::BLACK, d3d::BLACK, 0.0f);
	mtrl.Diffuse.a = 0.5f;

	D3DDevice->SetRenderState(D3DRS_ZENABLE, false);

	D3DDevice->SetMaterial(&mtrl);
	D3DDevice->SetTexture(0, 0);

	teaPort->DrawSubset(0);

	D3DDevice->SetRenderState(D3DRS_ZENABLE, true);
	D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	D3DDevice->SetRenderState(D3DRS_STENCILENABLE, false);

}
void RenderMirror()
{
	D3DDevice->SetRenderState(D3DRS_STENCILENABLE, true);
	D3DDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	D3DDevice->SetRenderState(D3DRS_STENCILREF, 0x1);
	D3DDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	D3DDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	D3DDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	D3DDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	D3DDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

	D3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
	D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	D3DDevice->SetStreamSource(0, MirrorBuffer, 0, sizeof(Vertex));
	D3DDevice->SetFVF(Vertex::FVF);
	D3DDevice->SetMaterial(&MirrorMtrl);
	D3DDevice->SetTexture(0, MirrorText);

	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	D3DDevice->SetTransform(D3DTS_WORLD, &I);
	D3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	D3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	D3DDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	D3DDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	D3DXMATRIX W, T, R;
	D3DXPLANE plane(0.0f, 0.0f, 1.0f, 0.0f); 
	D3DXMatrixReflect(&R, &plane);

	D3DXMatrixTranslation(&T,
		teaPortPosition.x,
		teaPortPosition.y,
		teaPortPosition.z);

	W = T * R;

	D3DDevice->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
	D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	D3DDevice->SetTransform(D3DTS_WORLD, &W);
	D3DDevice->SetMaterial(&teaportMtrl);
	D3DDevice->SetTexture(0, 0);

	D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	teaPort->DrawSubset(0);

	D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	D3DDevice->SetRenderState(D3DRS_STENCILENABLE, false);
	D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
bool Setup()
{
	FloorMtrl = d3d::WHITE_MTRL;
	WallMtrl = d3d::WHITE_MTRL;
	MirrorMtrl = d3d::WHITE_MTRL;

	initLight(); 
	setupTexture();
	initObject();

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
	d3d::Release<IDirect3DTexture9*>(WallText);
	d3d::Release<IDirect3DTexture9*>(MirrorText);
	d3d::Release<IDirect3DTexture9*>(FloorText);
	d3d::Release<IDirect3DVertexBuffer9*>(WallBuffer);
	d3d::Release<IDirect3DVertexBuffer9*>(MirrorBuffer);
	d3d::Release<IDirect3DVertexBuffer9*>(FloorBuffer);
	d3d::Release<ID3DXMesh*>(teaPort);
}
bool Display(float timeDelta)
{
	if (D3DDevice)
	{
		static float radius = 10.0f;

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			if(teaPortPosition.x > -7.0 ) teaPortPosition.x -= 3.0f * timeDelta;

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			if (teaPortPosition.x < 7.0) teaPortPosition.x += 3.0f * timeDelta;

		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			radius -= 2.0f * timeDelta;

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			radius += 2.0f * timeDelta;

		static float angle = (3.0f * D3DX_PI) / 2.0f;

		if (::GetAsyncKeyState('A') & 0x8000f)
			angle -= 0.5f * timeDelta;

		if (::GetAsyncKeyState('S') & 0x8000f)
			angle += 0.5f * timeDelta;

		D3DXVECTOR3 position(cosf(angle) * radius, 5.0f, sinf(angle) * radius);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);
		D3DDevice->SetTransform(D3DTS_VIEW, &V);

		D3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0xff000000, 1.0f, 0);
		D3DDevice->BeginScene();

		DrawVertex(FloorBuffer, &FloorMtrl, FloorText, 1);
		DrawVertex(WallBuffer, &WallMtrl, WallText, 2);
		DrawVertex(MirrorBuffer, &MirrorMtrl, MirrorText, 1);
		DrawObject();
		RenderShadow();
		RenderMirror();


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