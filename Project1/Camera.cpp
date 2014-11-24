#include "Camera.h"


Camera::Camera(IDirect3DDevice9 *D3DDevice, float height, float width)
:_D3DDevice(D3DDevice), _height(height), _width(width)
{
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f,
		(float)_width / (float)height,
		1.0f,
		1000.0f);
	D3DDevice->SetTransform(D3DTS_PROJECTION, &proj);

}


Camera::~Camera()
{
}
void Camera::setLootAt(D3DXVECTOR3 *pos, D3DXVECTOR3 *tar, D3DXVECTOR3 *up)
{

}