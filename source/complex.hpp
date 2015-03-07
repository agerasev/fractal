#pragma once

#include <cmath>
#include <type_traits>

template <typename T>
class complex
{
public:
	T data[2];
	
	complex()
	{

	}
	
	template <typename S>
	complex(const S &a)
	{
		data[0] = static_cast<T>(a);
		data[1] = static_cast<T>(0);
	}
	
	template <typename SR, typename ST>
	complex(const SR &a, const ST &b)
	{
		data[0] = a;
		data[1] = b;
	}
	
	template <typename S>
	complex(const complex<S> &c)
	{
		data[0] = c.data[0];
		data[1] = c.data[1];
	}

	T &re()
	{
		return data[0];
	}
	T &im()
	{
		return data[1];
	}
	T re() const
	{
		return data[0];
	}
	T im() const
	{
		return data[1];
	}
	T arg() const
	{
		return atan2(data[1],data[0]);
	}
	T mod() const
	{
		return sqrt(data[0]*data[0] + data[1]*data[1]);
	}
};

template <typename T, typename S>
complex<typename std::common_type<T,S>::type> polar(T mod, S arg)
{
	return complex<typename std::common_type<T,S>::type>(mod*cos(arg),mod*sin(arg));
}

template <typename T>
complex<T> operator + (const complex<T> &c)
{
	return c;
}

template <typename T>
complex<T> operator - (const complex<T> &c)
{
	return complex<T>(-c.re(),-c.im());
}

template <typename T, typename S>
complex<typename std::common_type<T,S>::type> operator + (const complex<T> &a, const complex<S> &b)
{
	return complex<typename std::common_type<T,S>::type>(a.re() + b.re(), a.im() + b.im());
}

template <typename T, typename S>
complex<typename std::common_type<T,S>::type> operator - (const complex<T> &a, const complex<S> &b)
{
	return a+(-b);
}

template <typename T, typename S>
complex<typename std::common_type<T,S>::type> operator * (const S &a, const complex<T> &b)
{
	return complex<typename std::common_type<T,S>::type>(a*b.re(),a*b.im());
}

template <typename T, typename S>
complex<typename std::common_type<T,S>::type> operator * (const complex<T> &a, const S &b)
{
	return b*a;
}

template <typename T, typename S>
complex<typename std::common_type<T,S>::type> operator / (const complex<T> &a, const S &b)
{
	return a*(1/static_cast<typename std::common_type<T,S>::type>(b));
}

/* Conjugation */
/* If conjugation operation isn't defined for type */
template <typename T>
T conj(const T &a)
{
	return a;
}

template <typename T>
complex<T> conj(const complex<T> &c)
{
	return complex<T>(conj(c.re()),-c.im());
}

/* Normalizing operation */
template <typename T>
T abs2(const complex<T> &c)
{
	return c.re()*conj(c.re()) + c.im()*c.im();
}

template <typename T>
T abs(const complex<T> &c)
{
	return sqrt(abs2(c));
}

/* Multiplication */
/* Not symmetric. Is not commutative if a != conj(a) */
template <typename T, typename S>
complex<typename std::common_type<T,S>::type> operator * (const complex<T> &a, const complex<S> &b)
{
	return complex<typename std::common_type<T,S>::type>(a.re()*b.re() - conj(b.im())*a.im(), b.im()*a.re() + a.im()*conj(b.re()));
}

/* Division */
template <typename T, typename S>
complex<typename std::common_type<T,S>::type> operator / (const complex<T> &a, const complex<S> &b)
{
	a*conj(b)/abs2(b);
}

/* Assign operations */
template <typename T, typename S>
complex<T> &operator += (complex<T> &a, const complex<S> &b)
{
	return a = a + b;
}

template <typename T, typename S>
complex<T> &operator -= (complex<T> &a, const complex<S> &b)
{
	return a = a - b;
}

template <typename T, typename S>
complex<T> &operator *= (complex<T> &a, const complex<S> &b)
{
	return a = a*b;
}

template <typename T, typename S>
complex<T> &operator /= (complex<T> &a, const complex<S> &b)
{
	return a = a/b;
}

typedef long double real;

typedef complex<real> creal;
typedef complex<double> cdouble;
typedef complex<float> cfloat;
