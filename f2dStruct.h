#pragma once

////////////////////////////////////////////////////////////////////////////////
/// @brief 混合因素
/// @note  颜色混合公式：Color = SrcColor * SrcBlend + DestColor * DestBlend
////////////////////////////////////////////////////////////////////////////////
enum F2DBLENDFACTOR
{
	F2DBLENDFACTOR_ZERO = 1,   ///< @brief 混合因素(0, 0, 0, 0)
	F2DBLENDFACTOR_ONE = 2,   ///< @brief 混合因素(1, 1, 1, 1)
	F2DBLENDFACTOR_SRCCOLOR = 3,   ///< @brief 混合因素(Rs, Gs, Bs, As)
	F2DBLENDFACTOR_INVSRCCOLOR = 4,   ///< @brief 混合因素(1 - Rs, 1 - Gs, 1 - Bs, 1 - As)
	F2DBLENDFACTOR_SRCALPHA = 5,   ///< @brief 混合因素(As, As, As, As)
	F2DBLENDFACTOR_INVSRCALPHA = 6,   ///< @brief 混合因素(1 - As, 1 - As, 1 - As, 1 - As)
	F2DBLENDFACTOR_DESTALPHA = 7,   ///< @brief 混合因素(Ad, Ad, Ad, Ad)
	F2DBLENDFACTOR_INVDESTALPHA = 8,   ///< @brief 混合因素(1 - Ad, 1 - Ad, 1 - Ad, 1 - Ad)
	F2DBLENDFACTOR_DESTCOLOR = 9,   ///< @brief 混合因素(Rd, Gd, Bd, Ad)
	F2DBLENDFACTOR_INVDESTCOLOR = 10,  ///< @brief 混合因素(1 - Rd, 1 - Gd, 1 - Bd, 1 - Ad)
	F2DBLENDFACTOR_SRCALPHASAT = 11   ///< @brief 混合因素(f, f, f, 1)，其中f = min(As, 1 - Ad)
};

////////////////////////////////////////////////////////////////////////////////
/// @brief 2D渲染器混合选项
////////////////////////////////////////////////////////////////////////////////
enum F2DGRAPH2DBLENDTYPE
{
	F2DGRAPH2DBLENDTYPE_DISABLE,          ///< @brief 禁用
	F2DGRAPH2DBLENDTYPE_SELECTCOLOR,      ///< @brief 选择颜色(final=color)
	F2DGRAPH2DBLENDTYPE_SELECTTEXTURE,    ///< @brief 选择纹理(final=texture)
	F2DGRAPH2DBLENDTYPE_ADD,              ///< @brief 颜色相加(final=color+texture)
	F2DGRAPH2DBLENDTYPE_SUBTRACT,         ///< @brief 颜色相减(final=color-texture)
	F2DGRAPH2DBLENDTYPE_MODULATE,         ///< @brief 颜色相乘(final=color*texture)
	F2DGRAPH2DBLENDTYPE_MODULATE2X,       ///< @brief 颜色相乘x2(final=color*texture*2)
	F2DGRAPH2DBLENDTYPE_MODULATE4X,       ///< @brief 颜色相乘x4(final=color*texture*4)
	F2DGRAPH2DBLENDTYPE_ADDSIGNED,        ///< @brief 颜色相加并偏移(final=color+texture-0.5)
	F2DGRAPH2DBLENDTYPE_ADDSIGNED2X,      ///< @brief 颜色相加并偏移x2(final=(color+texture-0.5)x2)
	F2DGRAPH2DBLENDTYPE_ADDSMOOTH         ///< @brief 平滑相加(final=color+texture-color*texture)
};

////////////////////////////////////////////////////////////////////////////////
/// @brief 二维渲染器顶点
////////////////////////////////////////////////////////////////////////////////
struct f2dGraphics2DVertex
{
	fFloat x;      ///< @brief x坐标
	fFloat y;      ///< @brief y坐标
	fFloat z;      ///< @brief z坐标
	fuInt color;   ///< @brief 漫反射颜色
	fFloat u;      ///< @brief 纹理u坐标
	fFloat v;      ///< @brief 纹理v坐标
};
