#include "Main.h"

ConceptStream::ConceptStream()
{
}

ConceptStream::~ConceptStream()
{
}

ConceptStream & ConceptStream::operator << (ConceptStream & (* Function)(ConceptStream &))
{
	(*Function)(*this);

	return *this;
}

ConceptStream & endl(ConceptStream & stream)
{
	stream.NewLine();

	return stream;
}
