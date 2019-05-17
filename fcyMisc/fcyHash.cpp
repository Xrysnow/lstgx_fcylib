#include "fcyHash.h"

////////////////////////////////////////////////////////////////////////////////

#define Get16Bits(d) (*((const uint16_t *) (d)))

uint32_t fcyHash::SuperFastHash(const uint8_t* pData, uint32_t DataLen)
{
	uint32_t tRet = DataLen, t;
	int32_t tRem;

	if (DataLen <= 0 || pData == NULL) return 0;

	tRem = DataLen & 3;
	DataLen >>= 2;

	// Hash算法主循环
	for ( ;DataLen > 0; DataLen--) 
	{
		tRet  += Get16Bits(pData);
		t      = ( Get16Bits(pData+2) << 11 ) ^ tRet;
		tRet   = (tRet << 16) ^ t;
		pData += 2 * sizeof(uint16_t);
		tRet  += tRet >> 11;
	}

	// 处理结尾
	switch (tRem)
	{
	case 3:
		tRet += Get16Bits(pData);
		tRet ^= tRet << 16;
		tRet ^= pData[ sizeof(uint16_t) ] << 18;
		tRet += tRet >> 11;
		break;
	case 2: 
		tRet += Get16Bits(pData);
		tRet ^= tRet << 11;
		tRet += tRet >> 17;
		break;
	case 1: 
		tRet += *pData;
		tRet ^= tRet << 10;
		tRet += tRet >> 1;
	}

	/* Force "avalanching" of final 127 bits */
	tRet ^= tRet << 3;
	tRet += tRet >> 5;
	tRet ^= tRet << 4;
	tRet += tRet >> 17;
	tRet ^= tRet << 25;
	tRet += tRet >> 6;

	return tRet;
}
