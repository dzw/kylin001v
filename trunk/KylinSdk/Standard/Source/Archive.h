#pragma once
#include "standard.h"

#define MAKE_ARCHIVE_VERSION(major, minor)	((major << 16) + minor)

#define TECH_VERSION Version_GetTech()
extern KINT Version_GetTech();

#define MARK				0xBEEF

namespace Kylin
{

	class ArchiveException : public exception
	{
	public:
		ArchiveException(KCCHAR * msg): m_sMsg(msg){}
		KCCHAR * ErrorMsg() { return m_sMsg;};
	private:
		KCCHAR * m_sMsg;
	};

	class MemoryArchive
	{
	public:
		MemoryArchive();
		MemoryArchive(KBOOL bReading, KINT nInitialSize);
		virtual	~MemoryArchive();

		KBOOL			IsReading() {return m_bReading;}

		MemoryArchive &		operator<<(KBYTE &v);
		MemoryArchive &		operator<<(KWORD &v);
		MemoryArchive &		operator<<(KBOOL &v);
		MemoryArchive &		operator<<(KINT &v);
		MemoryArchive &		operator<<(unsigned &v);
		MemoryArchive &		operator<<(KCHAR &v);
		MemoryArchive &		operator<<(KFLOAT &v);
		MemoryArchive &		operator<<(KDOUBLE &v);
		MemoryArchive &		operator<<(KCHAR *v);
		MemoryArchive &		operator<<(KCCHAR *v);
		MemoryArchive &		operator<<(KDWORD &v);
		MemoryArchive &		operator<<(MemoryArchive &ar);

		MemoryArchive & operator=(MemoryArchive &a);
		KINT Serialize(KCHAR * pBuf);

		KBOOL			Tag(KCCHAR *value);
		KINT			BeginChunk();
		KVOID			EndChunk(KINT nBeginMark);
		KVOID			CancelChunk(KINT nBeginMark);
		KINT			Length() {return m_pTail - m_pData;}
		KVOID			Resize(KINT nNewSize);
		///return the buffer ptr and give up the ownership of the data
		KCHAR *					ExtractData(KVOID);

	protected:
		KUINT					m_uAllocatedSize;
		KCHAR					* m_pData;
		KCHAR					* m_pTail;
		KBOOL					m_bReading;
	};
	typedef MemoryArchive Archive;
}
