#include "fcyBinaryHelper.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////

fcyBinaryReader::fcyBinaryReader(fcyStream* pStream)
	: m_pStream(pStream)
{
	if (!m_pStream)
		throw runtime_error("fcyBinaryReader::fcyBinaryReader");
	m_pStream->retain();
}

fcyBinaryReader::~fcyBinaryReader()
{
	m_pStream->release();
}

fcyStream* fcyBinaryReader::GetBaseStream() const
{
	return m_pStream;
}

char fcyBinaryReader::ReadChar() const
{
	char tRet = 0;
	m_pStream->ReadBytes((uint8_t*)&tRet, sizeof tRet);
	return tRet;
}

void fcyBinaryReader::ReadChars(fStr OutBuffer, uint64_t Length) const
{
	m_pStream->ReadBytes((uint8_t*)OutBuffer, Length);
}

wchar_t fcyBinaryReader::ReadCharW() const
{
	char tRet = 0;
	m_pStream->ReadBytes((uint8_t*)&tRet, sizeof tRet);
	return tRet;
}

void fcyBinaryReader::ReadCharsW(wchar_t* OutBuffer, uint64_t Length) const
{
	m_pStream->ReadBytes((uint8_t*)OutBuffer, Length * sizeof(wchar_t));
}

uint8_t fcyBinaryReader::ReadByte() const
{
	uint8_t tRet = 0;
	m_pStream->ReadBytes((uint8_t*)&tRet, sizeof tRet);
	return tRet;
}

void fcyBinaryReader::ReadBytes(uint8_t* OutBuffer, uint64_t Length) const
{
	m_pStream->ReadBytes((uint8_t*)OutBuffer, Length);
}

int16_t fcyBinaryReader::ReadInt16() const
{
	int16_t tRet = 0;
	m_pStream->ReadBytes((uint8_t*)&tRet, sizeof tRet);
	return tRet;
}

uint16_t fcyBinaryReader::ReadUInt16() const
{
	uint16_t tRet = 0;
	m_pStream->ReadBytes((uint8_t*)&tRet, sizeof tRet);
	return tRet;
}

int32_t fcyBinaryReader::ReadInt32() const
{
	int32_t tRet = 0;
	m_pStream->ReadBytes((uint8_t*)&tRet, sizeof tRet);
	return tRet;
}

uint32_t fcyBinaryReader::ReadUInt32() const
{
	uint32_t tRet = 0;
	m_pStream->ReadBytes((uint8_t*)&tRet, sizeof tRet);
	return tRet;
}

int64_t fcyBinaryReader::ReadInt64() const
{
	int64_t tRet = 0;
	m_pStream->ReadBytes((uint8_t*)&tRet, sizeof tRet);
	return tRet;
}

uint64_t fcyBinaryReader::ReadUInt64() const
{
	uint64_t tRet = 0;
	m_pStream->ReadBytes((uint8_t*)&tRet, sizeof tRet);
	return tRet;
}

float fcyBinaryReader::ReadFloat() const
{
	float tRet = 0;
	m_pStream->ReadBytes((uint8_t*)&tRet, sizeof tRet);
	return tRet;
}

double fcyBinaryReader::ReadDouble() const
{
	double tRet = 0;
	m_pStream->ReadBytes((uint8_t*)&tRet, sizeof tRet);
	return tRet;
}

////////////////////////////////////////////////////////////////////////////////

fcyBinaryWriter::fcyBinaryWriter(fcyStream* pStream)
	: m_pStream(pStream)
{
	if (!m_pStream||!m_pStream->CanWrite())
		throw runtime_error("fcyBinaryWriter::fcyBinaryWriter");
	m_pStream->retain();
}

fcyBinaryWriter::~fcyBinaryWriter()
{
	m_pStream->release();
}

fcyStream* fcyBinaryWriter::GetBaseStream() const
{
	return m_pStream;
}

void fcyBinaryWriter::Write(char Value) const
{
	m_pStream->WriteBytes((uint8_t*)&Value, sizeof Value);
}

void fcyBinaryWriter::Write(fcStr Buffer, uint64_t Length) const
{
	m_pStream->WriteBytes((uint8_t*)Buffer, Length);
}

void fcyBinaryWriter::Write(wchar_t Value) const
{
	m_pStream->WriteBytes((uint8_t*)&Value, sizeof Value);
}

void fcyBinaryWriter::Write(fcStrW Buffer, uint64_t Length) const
{
	m_pStream->WriteBytes((uint8_t*)Buffer, Length);
}

void fcyBinaryWriter::Write(uint8_t Value) const
{
	m_pStream->WriteBytes((uint8_t*)&Value, sizeof Value);
}

void fcyBinaryWriter::Write(fcData Buffer, uint64_t Length) const
{
	m_pStream->WriteBytes((uint8_t*)Buffer, Length);
}

void fcyBinaryWriter::Write(int16_t Value) const
{
	m_pStream->WriteBytes((uint8_t*)&Value, sizeof Value);
}

void fcyBinaryWriter::Write(uint16_t Value) const
{
	m_pStream->WriteBytes((uint8_t*)&Value, sizeof Value);
}

void fcyBinaryWriter::Write(int32_t Value) const
{
	m_pStream->WriteBytes((uint8_t*)&Value, sizeof Value);
}

void fcyBinaryWriter::Write(uint32_t Value) const
{
	m_pStream->WriteBytes((uint8_t*)&Value, sizeof Value);
}

void fcyBinaryWriter::Write(int64_t Value) const
{
	m_pStream->WriteBytes((uint8_t*)&Value, sizeof Value);
}

void fcyBinaryWriter::Write(uint64_t Value) const
{
	m_pStream->WriteBytes((uint8_t*)&Value, sizeof Value);
}

void fcyBinaryWriter::Write(float Value) const
{
	m_pStream->WriteBytes((uint8_t*)&Value, sizeof Value);
}

void fcyBinaryWriter::Write(double Value) const
{
	m_pStream->WriteBytes((uint8_t*)&Value, sizeof Value);
}
