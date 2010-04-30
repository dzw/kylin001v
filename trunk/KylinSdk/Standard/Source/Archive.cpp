#include "stdpch.h"
#include ".\archive.h"


#ifdef _DEBUG
#define COMPILE_TIME_ASSERT(expr)   static KCHAR constraint[expr]
#else
#define COMPILE_TIME_ASSERT(expr)
#endif

COMPILE_TIME_ASSERT(sizeof(KWORD) == 2 && sizeof(KBYTE) == 1);

#define DEBUG_ARCHIVE

namespace Kylin
{

MemoryArchive::MemoryArchive()
: m_uAllocatedSize(128)
, m_pData(NULL)
, m_bReading(false)
{
	m_pData = KNEW KCHAR[m_uAllocatedSize];
#ifdef DEBUG_ARCHIVE
	memset(m_pData, 0, m_uAllocatedSize);
#endif
	m_pTail = m_pData;
}

MemoryArchive::MemoryArchive(KBOOL bReading, KINT nInitialSize)
: m_uAllocatedSize(nInitialSize)
, m_pData(NULL)
, m_bReading(bReading)
{
	m_pData = KNEW KCHAR[m_uAllocatedSize];
	m_pTail = m_pData;
#ifdef DEBUG_ARCHIVE
	memset(m_pData, 0, m_uAllocatedSize);
#endif 
}

MemoryArchive::~MemoryArchive()
{
	KDEL[] m_pData;
}

///Put or get the a tag to/from the archive, return true if succeed
KBOOL MemoryArchive::Tag(KCCHAR *value)
{
	if(m_bReading)
	{
		KCHAR pBuf[1024];
		(*this) << (KCHAR *)pBuf;
		if(strcmp(pBuf, value))
			return true;
		return false;
	}
	else
	{
		*this << (KCHAR *&)value;
	}
	return true;
}

///Mark the begin of a chunk
KINT MemoryArchive::BeginChunk()
{
	KINT nChunkSize = 0;
	KINT nOffset = m_pTail - m_pData;
	(*this) << nChunkSize; //if it's reading, nChunkSize will get the right value; if it's writing, nChunksize will be changed later in ChunkEnd
	return nOffset;
}

///make sure the chunk has ended at the right offset. This function along with BeginChunk() make it possible to 
//skip a whole chunk if something wrong happened while reading the archive chunk
KVOID MemoryArchive::EndChunk(KINT nBeginMark)
{
	if(m_bReading)
	{
		//read the chunksize from the data stream
		KINT nChunkSize = * reinterpret_cast<KINT *>(m_pData + nBeginMark);
		//put the tail at the right position
		m_pTail = m_pData + nBeginMark + nChunkSize;
		assert(m_pTail <= m_pData + m_uAllocatedSize);
		if(m_pTail > m_pData + m_uAllocatedSize)
			throw ArchiveException("Inner error");
	}
	else
	{
		//calculate the chunk size, this should include the begin mark
		KINT nChunkSize = m_pTail - m_pData - nBeginMark;
		//write it back into the begin mark
		*reinterpret_cast<KINT *>(m_pData + nBeginMark) = nChunkSize;
	}
}

///cancel the data chunk from the between current position and the beginmark; This allows the chunk be canceled if something wrong happend while writing the archive chunk
KVOID MemoryArchive::CancelChunk(KINT nBeginMark)
{
	assert(m_bReading);
	m_pTail = m_pData + nBeginMark;
	assert(m_pTail <= m_pData + m_uAllocatedSize);
}


#define CODE(type)     \
assert(m_pTail>= m_pData);			\
if(m_bReading)	\
{				\
	v = * (type *)(m_pTail);	\
	m_pTail += sizeof(v);			\
}										\
else									\
{										\
	while(m_pTail - m_pData + sizeof(v) >= m_uAllocatedSize)	\
		Resize(m_uAllocatedSize * 2);	\
	* (type *)(m_pTail) = v;	\
	m_pTail += sizeof(v);			\
	assert(m_pTail <= m_pData + m_uAllocatedSize);\
}										\
return *this;	

MemoryArchive & MemoryArchive::operator<<(KBYTE &v)
{
	CODE(KBYTE);
}
MemoryArchive & MemoryArchive::operator<<(KWORD &v)
{
	CODE(KWORD);
}

//optimize, use KCHAR for KBOOL
MemoryArchive &		MemoryArchive::operator<<(KBOOL &v)
{
	KBYTE x = v;
	(*this) << x;
	v = (1 == x);
	return *this;
}

MemoryArchive &		MemoryArchive::operator<<(KINT &v)
{
	CODE(KINT)
}
MemoryArchive &		MemoryArchive::operator<<(unsigned &v)
{
	CODE(unsigned)
}
MemoryArchive &		MemoryArchive::operator<<(KCHAR &v)
{
	CODE(KCHAR)
}
MemoryArchive &		MemoryArchive::operator<<(KFLOAT &v)
{
	CODE(KFLOAT)
}
MemoryArchive &		MemoryArchive::operator<<(KDOUBLE &v)
{
	CODE(KDOUBLE)
}

MemoryArchive &		MemoryArchive::operator <<(KDWORD&v)
{
	CODE(KDWORD)
}

///archive for a string type
MemoryArchive &		MemoryArchive::operator<<(KCHAR *v)
{
	if(m_bReading)
	{
		unsigned uLen;
		*this << uLen;

		//error check
		assert(uLen < 4096);
		if(uLen >= 4096)
			throw ArchiveException("String is too long");

		//we need to make sure there is enough space in v
		memcpy(v, m_pTail, uLen);
		m_pTail += uLen;
		v[uLen] = '\0';
		assert(m_pTail <= m_pData + m_uAllocatedSize);
	}
	else
	{
		KUINT uLen = strlen(v);

		//error check
		assert(uLen < 4096);
		if(uLen >= 4096)
			throw ArchiveException("String is too long");

		*this << uLen;
		while(m_pTail - m_pData + uLen >= m_uAllocatedSize)
			Resize(m_uAllocatedSize * 2);
		memcpy(m_pTail, v, uLen);
		m_pTail += uLen;
	}

	return * this;
}

MemoryArchive &		MemoryArchive::operator<<(KCCHAR *v)
{
	assert(!m_bReading);
	if(m_bReading)
	{
	}
	else
	{
		//c/ped from above function
		KUINT uLen = strlen(v);

		//error check
		assert(uLen < 4096);
		if(uLen >= 4096)
			throw ArchiveException("String is too long");

		*this << uLen;
		while(m_pTail - m_pData + uLen >= m_uAllocatedSize)
			Resize(m_uAllocatedSize * 2);
		memcpy(m_pTail, v, uLen);
		m_pTail += uLen;
	}

	return * this;
}

MemoryArchive& MemoryArchive::operator <<(MemoryArchive& ar)
{
	if(m_bReading)
	{
		assert(!"Read from archive is not supported");
	}
	else
	{
		unsigned uLen = ar.Length();

		while(m_pTail - m_pData + uLen >= m_uAllocatedSize)
			Resize(m_uAllocatedSize * 2);
		memcpy(m_pTail, ar.m_pData, uLen);
		m_pTail += uLen;
	}

	return * this;
}

KVOID MemoryArchive::Resize(KINT nNewSize)
{
	//create a KNEW buffer
	KCHAR * pBuf = KNEW KCHAR[nNewSize];
	KINT nSize = m_pTail - m_pData;
	memcpy(pBuf, m_pData, nSize);
	KDEL[] m_pData;
	m_pData = pBuf;
	m_pTail = m_pData + nSize;
	m_uAllocatedSize = nNewSize;
}

///copy
MemoryArchive & MemoryArchive::operator=(MemoryArchive &a)
{
	KINT nSize = a.Length();
	Resize(nSize);
	m_bReading = a.m_bReading;
	memcpy(m_pData, a.m_pData, nSize);
	m_pTail = m_pData + nSize;
	return (*this);
}

///Put/Get in/from a buffer, the caller has the responsibility to make sure there is enough space in either
//buffer or archive data. Return the bytes serialized
KINT MemoryArchive::Serialize(KCHAR * pBuf)
{
	//caller need to make sure there is enough space
	if(m_bReading)
	{
		memcpy(m_pData, pBuf, m_uAllocatedSize);
	}
	else
		memcpy(pBuf, m_pData, Length());

	return Length();
}

///return the buffer ptr and give up the ownership of the data
KCHAR * MemoryArchive::ExtractData(KVOID)
{
	
	KCHAR * pRet = m_pData;
	m_uAllocatedSize = 0;
	m_pData = m_pTail = NULL;
	return pRet;
}

}