#pragma once
#ifndef __MutuallyConnectable_H__
#define __MutuallyConnectable_H__

// Usage: Suppose you want to have two classes A and B that are mutually connected to each other.
//
//        Derive the class A from MutuallyConnectable<A, B> where A is the class you're deriving
//        and B is the other class you want to make mutual connections with.
//        Derive the class B from MutuallyConnectable<B, A>.
//
//        Call MutuallyConnectable<A, B>::Connect(A, B) to create a connection.
//        Call MutuallyConnectable<A, B>::Disconnect(A, B) to destroy a connection.
//
//        Call A::GetConnected() to get a const reference to std::set<B *>.
//        Call B::GetConnected() to get a const reference to std::set<A *>.
//
//        If A or B is destroyed while it has non-zero connections, it will automatically remove the connections.

template <typename T, typename U> class MutuallyConnectable
{
private:
	std::set<U *>		m_Connected;

public:
	// TODO: Change return type to `const decltype(m_Connected) &` or something
	const std::set<U *> & GetConnected() const;

	static void Connect(MutuallyConnectable<T, U> & MutuallyConnectable0, MutuallyConnectable<U, T> & MutuallyConnectable1);
	static void Disconnect(MutuallyConnectable<T, U> & MutuallyConnectable0, MutuallyConnectable<U, T> & MutuallyConnectable1);

protected:
	MutuallyConnectable();
	virtual ~MutuallyConnectable();

private:
	MutuallyConnectable(const MutuallyConnectable<T, U> &) = delete;
	MutuallyConnectable<T, U> & operator = (const MutuallyConnectable<T, U> &) = delete;

	void ConnectWithoutReciprocating(MutuallyConnectable<U, T> & MutuallyConnectable);
	void DisconnectWithoutReciprocating(MutuallyConnectable<U, T> & MutuallyConnectable);

	friend class MutuallyConnectable<U, T>;
};

#include "MutuallyConnectable.hpp"

#endif // __MutuallyConnectable_H__
