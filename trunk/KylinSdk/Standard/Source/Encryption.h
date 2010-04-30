#pragma once

#ifndef _DEBUG
#define ENCRYPTION
#endif

template<class T>
struct Guard_Var
{
	static KINT key;
	KINT x;
	Guard_Var(const T& a)
	{
		x = (*(KINT *)(&a))^key;
	}
	Guard_Var()
	{
		x = 0;
	}
	void operator=(const T & a)
	{
		x = (*(KINT *)(&a))^key;
	}
	operator const T() const
	{
		KINT t = (x^key);
		return *(T *)(&t);
	}
	void operator ()(const T& a)
	{
		x = (*(KINT *)(&a))^key;
	}
	void operator += (const T& a)
	{
		this->operator=(*this + a);
	}
	void operator -= (const T& a)
	{
		this->operator=(*this - a);
	}
	void operator *= (const T& a)
	{
		this->operator=(*this * a);
	}
	void operator /= (const T& a)
	{
		this->operator=(*this / a);
	}

};
template <typename T> KINT Guard_Var <T>::key;

//more secured version
template<typename T>
struct Guard2_Var
{
	static KINT key;
	KINT * pX;
	void operator=(const Guard2_Var& a)
	{
		T t = a;
		this->operator =(t);
	}

	void operator=(const T & a)
	{
		KDEL pX;
		pX = KNEW KINT;
		*pX = (*(KINT *)(&a))^key;
	}
	operator const T() const
	{
		KINT t = (*pX^key);
		return *(T *)(&t);
	}

	Guard2_Var(const T& a)
	{
		pX = KNEW KINT;
		*pX = (*(KINT *)(&a))^key;
	}
	Guard2_Var()
	{
		pX = KNEW KINT;
	}

	void operator += (const T& a)
	{
		this->operator=(*this + a);
	}
	void operator -= (const T& a)
	{
		this->operator=(*this - a);
	}
	void operator *= (const T& a)
	{
		this->operator=(*this * a);
	}
	void operator /= (const T& a)
	{
		this->operator=(*this / a);
	}
};
template <typename T> KINT Guard2_Var <T>::key;

#ifdef ENCRYPTION
#define guard_int		Guard_Var<KINT>
#define guard_float		Guard_Var<KFLOAT>
#define guard_bool		Guard_Var<KBOOL>
#define guard2_int		Guard2_Var<KINT>
#define guard2_float	Guard2_Var<KFLOAT>
#define guard2_bool		Guard2_Var<KBOOL>

#else
#define guard_int		KINT
#define guard_float		KFLOAT
#define guard_bool		KBOOL
#define guard2_int		KINT
#define guard2_float	KFLOAT
#define guard2_bool		KBOOL
#endif

#endif //__ENCRYPTION_H__