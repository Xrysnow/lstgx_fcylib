#include "fcyStream.h"
#include <algorithm>
#include <utility>

using namespace std;

void fcyStream::Lock()
{
	mut.lock();
}

bool fcyStream::TryLock()
{
	return mut.try_lock();
}

void fcyStream::Unlock()
{
	mut.unlock();
}

////////////////////////////////////////////////////////////////////////////////

fcyFileStream::fcyFileStream(std::string Path, bool Writable)
	:path(std::move(Path))
{
	writable = Writable;
	auto flag = ios::in | ios::binary;
	if (writable)
		flag = flag | ios::out;
	file.open(path, flag);
	file.seekg(0, std::fstream::end);
	size = file.tellg();
	if (!file.is_open())
		throw runtime_error("fcyFileStream::fcyFileStream CreateFile Failed");
}

fcyFileStream::~fcyFileStream()
{
	file.close();
}

bool fcyStream::FillStream(fcyStream* Src, fcyStream* Dest, uint64_t DataLength)
{
	uint8_t tBuffer[8192];
	uint64_t tDataReaded = 0;
	uint64_t tTotalReaded = 0;

	while (tTotalReaded != DataLength && (Src->ReadBytes(tBuffer,
		(DataLength - tTotalReaded > 8192) ? 8192 : DataLength - tTotalReaded, &tDataReaded)))
	{
		tTotalReaded += tDataReaded;

		Dest->WriteBytes(tBuffer, tDataReaded);
	}

	return tTotalReaded == DataLength;
}

bool fcyFileStream::SetLength(uint64_t /*Length*/)
{
	return false;
}

bool fcyFileStream::SetPosition(FCYSEEKORIGIN Origin, int64_t Offset)
{
	auto tOrigin = ios::cur;
	switch(Origin)
	{
	case CUR:
		tOrigin = ios::cur;
		break;
	case BEG:
		tOrigin = ios::beg;
		break;
	case END:
		tOrigin = ios::end;
		break;
	default:
		return false;
	}
	auto last_pos = GetPosition();
	file.seekg(Offset, tOrigin);
	if (writable)
		file.seekp(Offset, tOrigin);

	if(file.fail())
	{
		file.clear();
		SetPosition(BEG, last_pos);
		return false;
	}
	pos = file.tellg();
	return true;
}

bool fcyFileStream::ReadBytes(uint8_t* pData, uint64_t Length, uint64_t* pBytesRead)
{
	if (pBytesRead)*pBytesRead = 0;
	if(Length == 0)
		return true;
	if(!pData)
		return false;
	file.read((char*)pData, Length);
	uint64_t real_read = file.gcount();
	pos += real_read;
	if (pBytesRead)*pBytesRead = real_read;
	return Length == real_read;
}

bool fcyFileStream::WriteBytes(const uint8_t* pSrc, uint64_t Length, uint64_t* pBytesWrite)
{
	if (pBytesWrite)*pBytesWrite = 0;
	if(!writable)
		return false;
	if(Length == 0)
		return true;
	if(!pSrc)
		return false;
	file.write((char*)pSrc, Length);
	if (file.fail())
	{
		file.clear();
		return false;
	}
	pos = file.tellp();
	if (pBytesWrite)*pBytesWrite = Length;

	// update size
	file.seekg(0, std::fstream::end);
	size = file.tellg();
	file.seekg(pos, ios::beg);
	return true;
}

fcyFileStream* fcyFileStream::create(const string& Path, bool Writable)
{
	auto ret = new (nothrow) fcyFileStream(Path, Writable);
	if (!ret)
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
	return ret;
}

////////////////////////////////////////////////////////////////////////////////

fcyMemStream::fcyMemStream(uint8_t* Src, uint64_t Length, bool copy, bool Writable, bool Resizable)
	: isOwner(copy)
{
	resizable = Resizable&&copy;
	writable = Writable;
	data = new cocos2d::Data();
	if (copy && Src)
		data->copy(Src, Length);
	else if (Src)
		data->fastSet(Src, Length);
	else
		data->fastSet((unsigned char*)malloc(Length), Length);
	size = Length;
}

fcyMemStream::fcyMemStream(cocos2d::Data* Src, uint64_t Length, bool copy, bool Writable)
	: isOwner(copy)
{
	resizable = copy;
	writable = Writable;
	data = new cocos2d::Data();
	if (Length == 0) Length = Src->getSize();
	if (copy&&Src)
		data->copy(Src->getBytes(), Length);
	else if (Src)
		data->fastSet(Src->getBytes(), Length);
	else
		data->fastSet((unsigned char*)malloc(Length), Length);
	size = Length;
}

fcyMemStream::~fcyMemStream()
{
	if (!isOwner)
		data->fastSet(nullptr, 0);
	delete data;
}

uint8_t* fcyMemStream::GetInternalBuffer()
{
	return (uint8_t*)data->getBytes();
}

bool fcyMemStream::SetLength(uint64_t Length)
{
	if(resizable)
	{
		if (Length == size) return true;
		auto buf = malloc(Length);
		if (!buf)
			return false;
		auto new_data = new cocos2d::Data();
		new_data->fastSet((unsigned char*)buf, Length);
		memcpy(new_data->getBytes(), data->getBytes(), min(size, Length));
		// resizable means isOwner
		//if (!isOwner)
		//	data->fastSet(nullptr, 0);
		delete data;
		data = new_data;
		size = Length;
		if(pos > size)
			pos = size;
		return true;
	}
	return false;
}

uint64_t fcyMemStream::GetPosition()
{
	return pos;
}

bool fcyMemStream::SetPosition(FCYSEEKORIGIN Origin, int64_t Offset)
{
	switch(Origin)
	{
	case CUR:
		break;
	case BEG:
		pos = 0;
		break;
	case END:
		pos = size;
		break;
	default:
		return false;
	}
	if (Offset<0 && uint32_t(-Offset)>pos)
	{
		pos = 0;
		return false;
	}
	else if (Offset > 0 && Offset + pos >= size)
	{
		pos = size;
		return false;
	}
	pos += Offset;
	return true;
}

bool fcyMemStream::ReadBytes(uint8_t* pData, uint64_t Length, uint64_t* pBytesRead)
{
	if (pBytesRead) *pBytesRead = 0;
	if(Length == 0) return true;
	if(!pData) return false;

	uint64_t tRestSize = size - pos;
	if(tRestSize == 0) return false;

	size_t real_read = min(Length, tRestSize);
	memcpy(pData, GetInternalBuffer() + pos, real_read);
	pos += real_read;
	if (pBytesRead) *pBytesRead = real_read;

	return tRestSize >= Length;
}

bool fcyMemStream::WriteBytes(const uint8_t* pSrc, uint64_t Length, uint64_t* pBytesWrite)
{
	if(!writable)
		return false;
	
	if(pBytesWrite) *pBytesWrite = 0;
	if(Length == 0) return true;
	if(!pSrc) return false;

	uint64_t tRestSize = size - pos;

	if(tRestSize < Length)
	{
		if(!resizable)
		{
			if(tRestSize == 0)
				return false;
			memcpy(GetInternalBuffer() + pos, pSrc, (size_t)tRestSize);
			pos += tRestSize;
			if (pBytesWrite) *pBytesWrite = tRestSize;
			return false;
		}
		else
		{
			auto ret = SetLength(max(uint64_t(size*1.5f), size + Length - tRestSize));
			if (!ret)
				return ret;
		}
	}
	memcpy(GetInternalBuffer() + pos, pSrc, (size_t)Length);
	pos += Length;
	if(pBytesWrite) *pBytesWrite = Length;
	return true;
}

fcyMemStream* fcyMemStream::create(uint8_t* Src, uint64_t Length, bool copy, bool Writable, bool Resizable)
{
	auto ret = new (nothrow) fcyMemStream(Src, Length, copy, Writable, Resizable);
	if (!ret)
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
	return ret;
}

fcyMemStream* fcyMemStream::create(cocos2d::Data* Src, uint64_t Length, bool copy, bool Writable)
{
	auto ret = new (nothrow) fcyMemStream(Src, Length, copy, Writable);
	if (!ret)
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
	return ret;
}

////////////////////////////////////////////////////////////////////////////////

fcyPartialStream::fcyPartialStream(fcyStream* OrgStream, uint64_t Offset, uint64_t Size)
	: oriStream(OrgStream), offset(Offset)
{
	size = Size;
	if(OrgStream == nullptr)
		throw runtime_error("fcyPartialStream::fcyPartialStream Invalid pos.");
	if(Offset + Size > OrgStream->GetLength())
		throw runtime_error("fcyPartialStream::fcyPartialStream Partial stream is bigger than orginal stream.");
	oriStream->retain();
}

fcyPartialStream::~fcyPartialStream()
{
	oriStream->release();
}

bool fcyPartialStream::CanWrite()
{
	return oriStream->CanWrite();
}

bool fcyPartialStream::CanResize()
{
	return oriStream->CanResize();
}

bool fcyPartialStream::SetLength(uint64_t Length)
{
	return false;
}

bool fcyPartialStream::SetPosition(FCYSEEKORIGIN Origin, int64_t Offset)
{
	switch(Origin)
	{
	case CUR:
		break;
	case BEG:
		pos = 0;
		break;
	case END:
		pos = size;
		break;
	default:
		return false;
	}
	if(Offset<0 && uint32_t(-Offset)>pos)
	{
		pos = 0;
		return false;
	}
	else if(Offset>0 && Offset+pos>= size)
	{
		pos = size;
		return false;
	}
	pos += Offset;
	return true;
}

bool fcyPartialStream::ReadBytes(uint8_t* pData, uint64_t Length, uint64_t* pBytesRead)
{
	if(pBytesRead)
		*pBytesRead = 0;
	if(Length == 0)
		return true;
	if(!pData)
		return false;

	uint64_t tRestSize = size - pos;

	if(tRestSize == 0)
		return false;

	if(tRestSize<Length)
	{
		uint64_t tTempPos = oriStream->GetPosition();
		oriStream->SetPosition(BEG, offset + pos);
		oriStream->ReadBytes(pData, tRestSize, pBytesRead);
		oriStream->SetPosition(BEG, tTempPos);

		pos += tRestSize;
		return false;
	}
	else
	{
		uint64_t tTempPos = oriStream->GetPosition();
		oriStream->SetPosition(BEG, offset + pos);
		oriStream->ReadBytes(pData, Length, pBytesRead);
		oriStream->SetPosition(BEG, tTempPos);

		pos += Length;
		if(pBytesRead)
			*pBytesRead = Length;
		return true;
	}
}

bool fcyPartialStream::WriteBytes(const uint8_t* pSrc, uint64_t Length, uint64_t* pBytesWrite)
{
	if(!CanWrite())
		return false;
	
	if(pBytesWrite)
		*pBytesWrite = 0;
	if(Length == 0)
		return true;
	if(!pSrc)
		return false;

	uint64_t tRestSize = size - pos;

	if(tRestSize < Length)
	{
		if(tRestSize == 0)
			return false;
		else
		{
			uint64_t tTempPos = oriStream->GetPosition();
			oriStream->SetPosition(BEG, offset + pos);
			oriStream->WriteBytes(pSrc, tRestSize, pBytesWrite);
			oriStream->SetPosition(BEG, tTempPos);
			
			pos += tRestSize;
			return false;
		}
	}

	uint64_t tTempPos = oriStream->GetPosition();
	oriStream->SetPosition(BEG, offset + pos);
	oriStream->WriteBytes(pSrc, Length, pBytesWrite);
	oriStream->SetPosition(BEG, tTempPos);

	pos += Length;
	if(pBytesWrite)
		*pBytesWrite = Length;
	return true;
}

void fcyPartialStream::Lock()
{
	oriStream->Lock();
}

bool fcyPartialStream::TryLock()
{
	return oriStream->TryLock();
}

void fcyPartialStream::Unlock()
{
	oriStream->Unlock();
}

fcyPartialStream* fcyPartialStream::create(fcyStream* OrgStream, uint64_t Offset, uint64_t Size)
{
	auto ret = new (nothrow) fcyPartialStream(OrgStream, Offset, Size);
	if (!ret)
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
	return ret;
}
