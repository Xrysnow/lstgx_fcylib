////////////////////////////////////////////////////////////////////////////////
/// @file  fcyStream.h
/// @brief 描述并实现了fancy内部的流式接口
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "fcyType.h"

#include <string>
#include <vector>
#include <fstream>
#include <mutex>
#include "base/CCData.h"
#include "base/CCRef.h"

#define FCYSTREAM

/// @addtogroup fancy库IO模块
/// @brief      提供对文件、内存的读写以及额外操作支持
/// @note       基于fcyStream的IO系统，通过实现fcyStream接口来实现其他来源的流
/// @{

/// @brief fcy流寻址方式
enum FCYSEEKORIGIN
{
	BEG = 0,  ///< @brief 从头开始寻址 @note  指针寻址位置开始于0处
	CUR = 1,  ///< @brief 从当前位置开始寻址
	END = 2   ///< @brief 从结尾处开始寻址 @note  指针寻址位置开始于EOF处
};

/// @brief fcy流接口
class fcyStream : public cocos2d::Ref
{
public:
	virtual ~fcyStream() = default;

	/// @brief 返回流是否可写
	virtual bool CanWrite() { return writable; }

	/// @brief 返回流是否可变长
	virtual bool CanResize() { return resizable; }

	/// @brief 返回流长度
	virtual uint64_t GetLength() { return size; }

	/// @brief     设置新长度
	/// @param[in] Length 流的新长度
	virtual bool SetLength(uint64_t Length) =0;
	
	/// @brief 获得读写指针的位置
	virtual uint64_t GetPosition() { return pos; }

	/// @brief     设置读写位置
	/// @param[in] Origin 寻址参考位置
	/// @param[in] Offset 位移
	virtual bool SetPosition(FCYSEEKORIGIN Origin, int64_t Offset) =0;

	/// @brief      从流中读取字节数据
	/// @param[in]  pData      目的缓冲区
	/// @param[in]  Length     数据长度
	/// @param[out] pBytesRead 真实读写长度，可置为NULL
	virtual bool ReadBytes(uint8_t* pData, uint64_t Length, uint64_t* pBytesRead= nullptr)=0;

	/// @brief      向流中写入字节数据
	/// @param[in]  pSrc        原始缓冲区
	/// @param[in]  Length      数据长度
	/// @param[out] pBytesWrite 真实读写长度，可置为NULL
	virtual bool WriteBytes(const uint8_t* pSrc, uint64_t Length, uint64_t* pBytesWrite = nullptr) =0;

	/// @brief   锁定流
	/// @note    该函数可能会造成阻塞
	/// @warning 如果一个流在多线程环境下被使用时必须在读写区块中手动加锁
	virtual void Lock();
	
	/// @brief   试图锁定流
	/// @warning 如果一个流在多线程环境下被使用时必须在读写区块中手动加锁
	/// @return  使用FCYOK和FCYFAILED判断是否成功加锁
	virtual bool TryLock();

	/// @brief   解锁流
	/// @note    该函数必须在Lock和TryLock成功的条件下进行调用
	/// @warning 如果一个流在多线程环境下被使用时必须在读写区块中手动加锁
	virtual void Unlock();
protected:
	uint64_t size = 0;
	uint64_t pos = 0;
	bool resizable = false;               ///< @brief 可变长
	bool writable = false;                ///< @brief 可写
private:
	std::mutex mut;

public:
	/// @brief     填充流
	/// @note      在Src的当前位置读取DataLength个长度到Dest流中
	/// @param[in] Src 原始流
	/// @param[in] Dest 目的流
	/// @param[in] DataLength 数据长度
	/// @return    FCYERR_OK：操作成功完成，FCYERR_INTERNALERR：读取时错误
	static bool FillStream(fcyStream* Src, fcyStream* Dest, uint64_t DataLength);
};

/// @brief fcy文件流
class fcyFileStream :
	public fcyStream
{
private:
	std::fstream file;///< @brief 内部文件句柄
	std::string path; ///< @brief 文件路径
public: // 接口实现
	bool SetLength(uint64_t Length) override;
	bool SetPosition(FCYSEEKORIGIN Origin, int64_t Offset) override;
	bool ReadBytes(uint8_t* pData, uint64_t Length, uint64_t* pBytesRead) override;
	bool WriteBytes(const uint8_t* pSrc, uint64_t Length, uint64_t* pBytesWrite) override;

	/// @brief     构造函数
	/// @param[in] Path     文件路径
	/// @param[in] Writable 可写
	static fcyFileStream* create(const std::string& Path, bool Writable);
private:
	fcyFileStream(std::string Path, bool Writable);
protected:
	~fcyFileStream();
};

/// @brief fcy内存流
class fcyMemStream :
	public fcyStream
{
private:
	cocos2d::Data* data = nullptr;
	bool isOwner;
public: // 接口实现
	bool SetLength(uint64_t Length) override;
	uint64_t GetPosition() override;
	bool SetPosition(FCYSEEKORIGIN Origin, int64_t Offset) override;
	bool ReadBytes(uint8_t* pData, uint64_t Length, uint64_t* pBytesRead) override;
	bool WriteBytes(const uint8_t* pSrc, uint64_t Length, uint64_t* pBytesWrite) override;

	uint8_t* GetInternalBuffer();

	/// @brief     构造函数
	/// @param[in] Src       数据源，置为NULL则不从数据源拷贝
	/// @param[in] Length    数据长度
	/// @param[in] Writable  可写
	/// @param[in] Resizable 可变长
	static fcyMemStream* create(uint8_t* Src, uint64_t Length, bool copy = true ,bool Writable = false, bool Resizable = false);
	static fcyMemStream* create(cocos2d::Data* Src, uint64_t Length = 0, bool copy = true, bool Writable = false);
private:
	fcyMemStream(uint8_t* Src, uint64_t Length, bool copy, bool Writable, bool Resizable);
	fcyMemStream(cocos2d::Data* Src, uint64_t Length, bool copy, bool Writable);
protected:
	~fcyMemStream();
};

/// @brief fcy部分流
/// @note  用来对一个流的一部分进行访问
class fcyPartialStream :
	public fcyStream
{
private:
	fcyStream* oriStream;     ///< @brief 原始流
	uint64_t offset;          ///< @brief 在原始流中的偏移量
public: // 接口实现
	bool CanWrite() override;
	bool CanResize() override;
	bool SetLength(uint64_t Length) override;
	bool SetPosition(FCYSEEKORIGIN Origin, int64_t Offset) override;
	bool ReadBytes(uint8_t* pData, uint64_t Length, uint64_t* pBytesRead) override;
	bool WriteBytes(const uint8_t* pSrc, uint64_t Length, uint64_t* pBytesWrite) override;
	void Lock() override;
	bool TryLock() override;
	void Unlock() override;
public:
	/// @brief     构造函数
	/// @param[in] OrgStream 原始流指针
	/// @param[in] Offset    部分流在原始流中的位移
	/// @param[in] Size      部分流大小
	static fcyPartialStream* create(fcyStream* OrgStream, uint64_t Offset, uint64_t Size);
private:
	fcyPartialStream(fcyStream* OrgStream, uint64_t Offset, uint64_t Size);
protected:
	~fcyPartialStream();
};
