////////////////////////////////////////////////////////////////////////////////
/// @file  fcyHash.h
/// @brief fancy哈希
////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <functional>

/// @addtogroup fancy杂项
/// @{

/// @brief fcyHash支持
namespace fcyHash
{
	/// @brief     SuperFastHash算法
	/// @param[in] pData   原始数据
	/// @param[in] DataLen 原始数据长度
	uint32_t SuperFastHash(const uint8_t* pData, uint32_t DataLen);

	/// @brief SuperFastHash算法模板
	template<typename T>
	uint32_t SuperFastHash(const T& Data)
	{
		return SuperFastHash((const uint8_t*)&Data, sizeof(T));
	}

	/// @brief HASH值组合模板
	template <typename T>
	inline void HashCombine(uint32_t& seed, const T & v)
	{
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
};

/// @}
