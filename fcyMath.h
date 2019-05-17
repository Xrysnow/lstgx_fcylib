////////////////////////////////////////////////////////////////////////////////
/// @file  fcyMath.h
/// @brief 数学库，描述数学相关结构
////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "fcyType.h"
#include <cmath>

#define FCYMAX(a,b) (((a)>(b))?(a):(b))
#define FCYMIN(a,b) (((a)<(b))?(a):(b))

/// @addtogroup fancy数学库
/// @brief 提供数学运算支持
/// @note 若无特殊说明，角度单位均为弧度
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief 矩形
////////////////////////////////////////////////////////////////////////////////
class fcyRect
{
public:
	fcyVec2 a;  ///< @brief 矩形左上角
	fcyVec2 b;  ///< @brief 矩形右下角
public:
	fcyRect() {}
	/// @brief     创建矩形
	/// @param[in] A 矩形左上角
	/// @param[in] B 矩形右下角
	fcyRect(const fcyVec2& A, const fcyVec2& B)
		: a(A), b(B) {}
	/// @brief     创建矩形
	/// @param[in] x1 左边
	/// @param[in] y1 顶边
	/// @param[in] x2 底边
	/// @param[in] y2 右边
	fcyRect(float x1, float y1, float x2, float y2)
		: a(x1,y1), b(x2,y2) {}
public:
	/// @brief 取宽度
	float GetWidth()const
	{
		return fabs(a.x - b.x);
	}
	/// @brief 取高度
	float GetHeight()const
	{
		return fabs(a.y - b.y);
	}
	/// @brief 是否包含点
	/// @note  含边界
	bool Contain(const fcyVec2& x)const
	{
		return (x.x>=a.x && x.x<=b.x && x.y>=a.y && x.y<=b.y);
	}
	/// @brief 是否与圆相交
	/// @note  含相切
	bool Contain(const fcyVec2& x, float R)const
	{
		return (x.x + R >= a.x && x.x - R <=b.x && x.y + R >=a.y && x.y - R <=b.y);
	}
	/// @brief 获得中心点
	fcyVec2 GetCenter()const
	{
		return ((a+b)*0.5);
	}
	/// @brief      求是否相交，若相交返回公共部分
	/// @param[in]  Rect 待判断的矩形
	/// @param[out] pOut 输出的相交部分
	bool Intersect(const fcyRect& Rect, fcyRect* pOut)const
	{
		fcyRect tRet(
			FCYMAX(a.x, Rect.a.x),
			FCYMAX(a.y, Rect.a.y),
			FCYMIN(b.x, Rect.b.x),
			FCYMIN(b.y, Rect.b.y)
			);

		if(tRet.a.x <= tRet.b.x && tRet.a.y <= tRet.b.y)
		{
			// 相交
			if(pOut)
				*pOut = tRet;
			return true;
		}
		else
			return false;
	}

	bool Intersect(const fcyRect& Rect)const
	{
		return FCYMAX(a.x, Rect.a.x) <= FCYMIN(b.x, Rect.b.x) && FCYMAX(a.y, Rect.a.y) <= FCYMIN(b.y, Rect.b.y);
	}
};

/// @}

