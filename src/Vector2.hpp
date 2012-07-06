template <typename T> Vector2<T>::Vector2()
	: m_Tuple()
{
}

template <typename T> Vector2<T>::Vector2(const Vector2<T> & Other)
{
	m_Tuple[0] = Other.m_Tuple[0];
	m_Tuple[1] = Other.m_Tuple[1];
}

template <typename T> Vector2<T>::Vector2(T x, T y)
{
	m_Tuple[0] = x;
	m_Tuple[1] = y;
}

template <typename T> Vector2<T>::~Vector2()
{
}

template <typename T> T Vector2<T>::X() const
{
	return m_Tuple[0];
}

template <typename T> T & Vector2<T>::X()
{
    return m_Tuple[0];
}

template <typename T> T Vector2<T>::Y() const
{
	return m_Tuple[1];
}

template <typename T> T & Vector2<T>::Y()
{
    return m_Tuple[1];
}

template <typename T> T Vector2<T>::operator [] (const uint8 & Index) const
{
    return m_Tuple[Index];
}

template <typename T> T & Vector2<T>::operator [] (const uint8 & Index)
{
    return m_Tuple[Index];
}

template <typename T> Vector2<T> & Vector2<T>::operator = (const Vector2<T> & Other)
{
	m_Tuple[0] = Other.m_Tuple[0];
	m_Tuple[1] = Other.m_Tuple[1];

	return *this;
}

template <typename T> Vector2<T> Vector2<T>::operator + (const Vector2<T> & Other) const
{
	return Vector2<T>
	(
		m_Tuple[0] + Other.m_Tuple[0],
		m_Tuple[1] + Other.m_Tuple[1]
	);
}

template <typename T> Vector2<T> Vector2<T>::operator - (const Vector2<T> & Other) const
{
	return Vector2<T>
	(
		m_Tuple[0] - Other.m_Tuple[0],
		m_Tuple[1] - Other.m_Tuple[1]
	);
}

template <typename T> Vector2<T> Vector2<T>::operator * (const T Value) const
{
	return Vector2<T>
	(
		m_Tuple[0] * Value,
		m_Tuple[1] * Value
	);
}

template <typename T> Vector2<T> Vector2<T>::operator / (const T Value) const
{
	return Vector2<T>
	(
		m_Tuple[0] / Value,
		m_Tuple[1] / Value
	);
}

template <typename T> T Vector2<T>::LengthSquared() const
{
	return (X() * X() + Y() * Y());
}
