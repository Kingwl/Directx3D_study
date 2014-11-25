#include "Camera.h"
Camera::Camera(IDirect3DDevice9 *Device)
:_Device(Device)
{
	reset();
}
Camera::~Camera()
{

}
void Camera::strafe(float units)
{
	_pos += _right * units;
}
void Camera::fly(float units)
{
	_pos += _up * units;
}
void Camera::walk(float units)
{
	_pos += _look * units;
		 
}
void Camera::getViewMatrix(D3DXMATRIX *V)
{
	D3DXVec3Normalize(&_look, &_look);
	D3DXVec3Cross(&_up, &_look, &_right);
	D3DXVec3Normalize(&_up, &_up);
	D3DXVec3Cross(&_right, &_up, &_look);
	D3DXVec3Normalize(&_right, &_right);
	D3DXVECTOR3 look(
		_pos.x + _look.x,
		_pos.y + _look.y,
		_pos.z + _look.z
		);
	D3DXMatrixLookAtLH(V, &_pos, &look, &_up);

}
void Camera::getPosition(D3DXVECTOR3 *pos)
{
	*pos = _pos;
}
void Camera::setPosition(D3DXVECTOR3 *pos)
{
	_pos = *pos;
}
void Camera::getRight(D3DXVECTOR3 *	right)
{
	*right = _right;
}
void Camera::getUp(D3DXVECTOR3 *up)
{
	*up = _up;
}
void Camera::getLook(D3DXVECTOR3 *look)
{
	*look = _look;
}
void Camera::pitch(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &_right, angle);
	D3DXVec3TransformCoord(&_up, &_up, &T);
	D3DXVec3TransformCoord(&_look, &_look, &T);

}
void Camera::yaw(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &_up, angle);
	D3DXVec3TransformCoord(&_right, &_right, &T);
	D3DXVec3TransformCoord(&_look, &_look, &T);
}
void Camera::roll(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &_look, angle);
	D3DXVec3TransformCoord(&_right, &_right, &T);
	D3DXVec3TransformCoord(&_up, &_up, &T);
}
void Camera::reset()
{
	_pos = D3DXVECTOR3(0.0f, 0.0f, -15.0f);
	_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}