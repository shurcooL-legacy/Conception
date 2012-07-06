template <typename T, typename U> MutuallyConnectable<T, U>::MutuallyConnectable()
{
}

template <typename T, typename U> MutuallyConnectable<T, U>::~MutuallyConnectable()
{
	for (auto & Connected : m_Connected)
	{
		static_cast<MutuallyConnectable<U, T> *>(Connected)->DisconnectWithoutReciprocating(*this);
	}
}

//template <typename T, typename U> auto MutuallyConnectable<T, U>::GetConnected() -> decltype((m_Connected))
template <typename T, typename U> auto MutuallyConnectable<T, U>::GetConnected() const -> const std::set<U *> &
{
	return m_Connected;
}

template <typename T, typename U> void MutuallyConnectable<T, U>::Connect(MutuallyConnectable<T, U> & MutuallyConnectable0, MutuallyConnectable<U, T> & MutuallyConnectable1)
{
	MutuallyConnectable0.ConnectWithoutReciprocating(MutuallyConnectable1);
	MutuallyConnectable1.ConnectWithoutReciprocating(MutuallyConnectable0);
}

template <typename T, typename U> void MutuallyConnectable<T, U>::Disconnect(MutuallyConnectable<T, U> & MutuallyConnectable0, MutuallyConnectable<U, T> & MutuallyConnectable1)
{
	MutuallyConnectable0.DisconnectWithoutReciprocating(MutuallyConnectable1);
	MutuallyConnectable1.DisconnectWithoutReciprocating(MutuallyConnectable0);
}

template <typename T, typename U> void MutuallyConnectable<T, U>::ConnectWithoutReciprocating(MutuallyConnectable<U, T> & MutuallyConnectable)
{
	m_Connected.insert(static_cast<U *>(&MutuallyConnectable));
}

template <typename T, typename U> void MutuallyConnectable<T, U>::DisconnectWithoutReciprocating(MutuallyConnectable<U, T> & MutuallyConnectable)
{
	m_Connected.erase(static_cast<U *>(&MutuallyConnectable));
}
