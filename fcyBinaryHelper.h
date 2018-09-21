////////////////////////////////////////////////////////////////////////////////
/// @file  fcyBinaryHelper.h
/// @brief fancy二进制读写辅助
////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "fcyStream.h"

/// @brief fcy二进制读取辅助
class fcyBinaryReader
{
private:
	fcyStream* m_pStream;         ///< @原始流
public:
	/// @brief   返回基础流
	/// @warning 本函数不会增加原始流的引用计数，如果需要管理这个指针请手动调用AddRef
	fcyStream* GetBaseStream() const;

	/// @brief 读取一个字符
	char ReadChar() const;
	
	/// @brief      读取多个字符
	/// @param[out] OutBuffer 输出缓冲区
	/// @param[in]  Length    要读取的字符个数
	void ReadChars(char* OutBuffer, uint64_t Length) const;
	
	/// @brief 读取一个宽字符
	wchar_t ReadCharW() const;
	
	/// @brief      读取多个宽字符
	/// @param[out] OutBuffer 输出缓冲区
	/// @param[in]  Length    要读取的字符个数
	void ReadCharsW(wchar_t* OutBuffer, uint64_t Length) const;
	
	/// @brief 读取一个字节
	uint8_t ReadByte() const;

	/// @brief 读取多个字节数据
	/// @param[out] OutBuffer 输出缓冲区
	/// @param[in]  Length    要读取的字节数目
	void ReadBytes(fData OutBuffer, uint64_t Length) const;
	
	/// @brief 读取一个短整数
	int16_t ReadInt16() const;
	
	/// @brief 读取一个无符号短整数
	uint16_t ReadUInt16() const;

	/// @brief 读取一个整数
	int32_t ReadInt32() const;

	/// @brief 读取一个无符号整数
	uint32_t ReadUInt32() const;

	/// @brief 读取一个长整数
	int64_t ReadInt64() const;

	/// @brief 读取一个无符号长整数
	uint64_t ReadUInt64() const;

	/// @brief 读取一个浮点数
	float ReadFloat() const;

	/// @brief 读取一个双精度浮点数
	double ReadDouble() const;
public:
	/// @brief     构造函数
	/// @param[in] pStream 原始流
	fcyBinaryReader(fcyStream* pStream);
	~fcyBinaryReader();
};

/// @brief fcy二进制写入辅助
class fcyBinaryWriter
{
private:
	fcyStream* m_pStream;         ///< @原始流
public:
	/// @brief   返回基础流
	/// @warning 本函数不会增加原始流的引用计数，如果需要管理这个指针请手动调用AddRef
	fcyStream* GetBaseStream() const;
	
	/// @brief 写出一个字符
	void Write(char Value) const;
	
	/// @brief 写出一个字符串
	void Write(fcStr Buffer, uint64_t Length) const;
	
	/// @brief 写出一个宽字符
	void Write(wchar_t Value) const;
	
	/// @brief 写出一个宽字符串
	void Write(fcStrW Buffer, uint64_t Length) const;
	
	/// @brief 写出一个字节
	void Write(uint8_t Value) const;
	
	/// @brief 写出一个内存缓冲
	void Write(fcData Buffer, uint64_t Length) const;
	
	/// @brief 写出一个短整数
	void Write(int16_t Value) const;
	
	/// @brief 写出一个无符号短整数
	void Write(uint16_t Value) const;
	
	/// @brief 写出一个整数
	void Write(int32_t Value) const;
	
	/// @brief 写出一个无符号整数
	void Write(uint32_t Value) const;
	
	/// @brief 写出一个长整数
	void Write(int64_t Value) const;
	
	/// @brief 写出一个无符号长整数
	void Write(uint64_t Value) const;
	
	/// @brief 写出一个浮点数
	void Write(float Value) const;
	
	/// @brief 写出一个双精度浮点数
	void Write(double Value) const;
public:
	/// @brief     构造函数
	/// @param[in] pStream 原始流
	fcyBinaryWriter(fcyStream* pStream);
	~fcyBinaryWriter();
};

