#include "ColorMtrl.h"
D3DMATERIAL9 d3d::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = a;
	mtrl.Diffuse = d;
	mtrl.Emissive = e;
	mtrl.Specular = s;
	mtrl.Power = p;
	return mtrl;
}
