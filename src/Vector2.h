#pragma once
#ifndef __Vector2_H__
#define __Vector2_H__

template <typename T> class Vector2
{
public:
	Vector2();
	Vector2(T x, T y);
	Vector2(const Vector2<T> & Other);
	~Vector2();

	T X() const;
    T & X ();
    T Y() const;
    T & Y();

	T operator [] (const uint8 & Index) const;
	T & operator [] (const uint8 & Index);

	Vector2<T> & operator = (const Vector2<T> & Other);

	bool operator == (const Vector2<T> & Other) const;
	bool operator != (const Vector2<T> & Other) const;

	Vector2<T> & operator += (const Vector2<T> & Other);
	Vector2<T> & operator -= (const Vector2<T> & Other);

	Vector2<T> operator + (const Vector2<T> & Other) const;
	Vector2<T> operator - (const Vector2<T> & Other) const;
	Vector2<T> operator * (const T Value) const;
	Vector2<T> operator / (const T Value) const;

	Vector2<T> Transpose() const;

	T LengthSquared() const;

	static const Vector2		ZERO;

private:
	T m_Tuple[2];
};

#include "Vector2.hpp"

typedef Vector2<sint32> Vector2n;
typedef Vector2<double> Vector2d;

#endif // __Vector2_H__
