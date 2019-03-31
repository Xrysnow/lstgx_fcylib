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
/// @brief 2D向量
////////////////////////////////////////////////////////////////////////////////
class fcyVec2
{
public:
	float x; ///< @brief x坐标
	float y; ///< @brief y坐标
public: // 构造函数
	fcyVec2()
		: x(0.f), y(0.f) {}
	/// @brief     构造函数
	/// @param[in] X x坐标
	/// @param[in] Y y坐标
	fcyVec2(float X, float Y)
		: x(X), y(Y) {}
	/// @brief     构造函数，从data[0]~data[1]取出浮点值
	/// @warning   不安全的函数
	/// @param[in] data 浮点数组指针
	fcyVec2(float *data)
		: x(data[0]), y(data[1]) {}
public: // 运算符重载
	/// @brief 2D向量相等
	bool operator==(const fcyVec2& right)const
	{
		return (x == right.x && y == right.y);
	}
	/// @brief 2D向量不等
	bool operator!=(const fcyVec2& right)const
	{
		return (x != right.x || y != right.y);
	}
	/// @brief 2D向量相加
	fcyVec2 operator+(const fcyVec2& right)const
	{
		return fcyVec2(x + right.x, y + right.y);
	};
	/// @brief 2D向量自加
	fcyVec2& operator+=(const fcyVec2& right)
	{
		x += right.x;
		y += right.y;
		return *this;
	};
	/// @brief 2D向量相减
	fcyVec2 operator-(const fcyVec2& right)const
	{
		return fcyVec2(x - right.x, y - right.y);
	};
	/// @brief 2D向量自减
	fcyVec2& operator-=(const fcyVec2& right)
	{
		x -= right.x;
		y -= right.y;
		return *this;
	};
	/// @brief 2D向量数乘
	fcyVec2 operator*(float right)const
	{
		return fcyVec2(x * right, y * right);
	};
	/// @brief 2D向量数乘（除法）
	fcyVec2 operator/(float right)const
	{
		return fcyVec2(x / right, y / right);
	};
	/// @brief 2D向量自数乘
	fcyVec2& operator*=(float right)
	{
		x = x * right;
		y = y * right;
		return *this;
	};
	/// @brief 2D向量自数乘（除法）
	fcyVec2& operator/=(float right)
	{
		x = x / right;
		y = y / right;
		return *this;
	};
	/// @brief 2D向量点积
	float operator*(const fcyVec2& right)const
	{
		return x * right.x + y * right.y;
	}
	/// @brief 取反
	fcyVec2 operator-()const
	{
		return fcyVec2(-x, -y);
	};
public: // 函数
	// 设置
	/// @brief 设置新值
	void Set(float X,float Y)
	{
		x = X; y = Y;
	}
	/// @brief 平面叉积
	float Cross(const fcyVec2& right)const
	{
		return  x * right.y - right.x * y;
	};
	/// @brief 规范化向量
	void Normalize()
	{
		float mode = sqrt( x * x +  y * y );
		if(mode != 0)
		{
			x /= mode;
			y /= mode;
		}
	};
	/// @brief 返回规范化向量
	fcyVec2 GetNormalize()const
	{
		float mode=sqrt( x *  x +  y *  y);
		if(mode != 0)
			return fcyVec2(x / mode, y / mode);
		else
			return fcyVec2();
	};
	/// @brief 求模
	float Length()const
	{
		return sqrt( x*x + y*y );
	}
	/// @brief 求模的平方
	float Length2()const
	{
		return ( x*x + y*y );
	}
	/// @brief     逆时针旋转
	/// @param[in] angle 旋转角度
	void Rotation(float angle)
	{
		float s = sin(angle),
		       c = cos(angle);
		float tx = x*c - y*s,
		       ty = x*s + y*c;
		x = tx;
		y = ty;
	}
	/// @brief     逆时针旋转
	/// @note      用于批量旋转，减少三角函数运算
	/// @param[in] s Sin值(=sin(angle))
	/// @param[in] c Cos值(=cos(angle))
	void RotationSC(float s, float c)
	{
		float tx = x*c - y*s,
			ty = x*s + y*c;
		x = tx; y = ty;
	}
	/// @brief 计算与x轴的夹角
	float CalcuAngle()
	{
		return atan2(y,x);
	}
};

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

