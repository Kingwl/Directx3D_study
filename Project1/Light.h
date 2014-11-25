#pragma once
#ifndef _LIGHT_
#define _LIGHT_
#include "ColorMtrl.h"
namespace d3d{

	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3 *direction, D3DXCOLOR *color);
	D3DLIGHT9 InitPointlLight(D3DXVECTOR3 *position, D3DXCOLOR *color);
	D3DLIGHT9 InitSpotLight(D3DXVECTOR3 *position, D3DXVECTOR3 *direction, D3DXCOLOR *color);

}
#endif

