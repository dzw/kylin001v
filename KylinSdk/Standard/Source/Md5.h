#pragma once

namespace Kylin
{
	class MD5
	{
	// Methods
	public:
		MD5() { Init(); }
		KVOID    Init();
		KVOID    Update(KBYTE* chInput, KUINT nInputLen);
		KVOID    Finalize();
		KBYTE*  Digest() { return m_Digest; }

		KSTR String2MD5(KCCHAR* szString);
		KSTR PrintMD5(KBYTE md5Digest[16]);

	private:

		KVOID    Transform(KBYTE* block);
		KVOID    Encode(KBYTE* dest, KUINT* src, KUINT nLength);
		KVOID    Decode(KUINT* dest, KBYTE* src, KUINT nLength);


		inline  KUINT   rotate_left(KUINT x, KUINT n)
						 { return ((x << n) | (x >> (32-n))); }

		inline  KUINT   F(KUINT x, KUINT y, KUINT z)
						 { return ((x & y) | (~x & z)); }

		inline  KUINT   G(KUINT x, KUINT y, KUINT z)
						 { return ((x & z) | (y & ~z)); }

		inline  KUINT   H(KUINT x, KUINT y, KUINT z)
						 { return (x ^ y ^ z); }

		inline  KUINT   I(KUINT x, KUINT y, KUINT z)
						 { return (y ^ (x | ~z)); }

		inline  KVOID    FF(KUINT& a, KUINT b, KUINT c, KUINT d, KUINT x, KUINT s, KUINT ac)
						 { a += F(b, c, d) + x + ac; a = rotate_left(a, s); a += b; }

		inline  KVOID    GG(KUINT& a, KUINT b, KUINT c, KUINT d, KUINT x, KUINT s, KUINT ac)
						 { a += G(b, c, d) + x + ac; a = rotate_left(a, s); a += b; }

		inline  KVOID    HH(KUINT& a, KUINT b, KUINT c, KUINT d, KUINT x, KUINT s, KUINT ac)
						 { a += H(b, c, d) + x + ac; a = rotate_left(a, s); a += b; }

		inline  KVOID    II(KUINT& a, KUINT b, KUINT c, KUINT d, KUINT x, KUINT s, KUINT ac)
						 { a += I(b, c, d) + x + ac; a = rotate_left(a, s); a += b; }

	// Data
	private:
		KUINT       m_State[4];
		KUINT       m_Count[2];
		KBYTE       m_Buffer[64];
		KBYTE       m_Digest[16];
		KBYTE       m_Finalized;

	};
}