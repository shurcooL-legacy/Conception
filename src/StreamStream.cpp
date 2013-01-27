#include "Main.h"

StreamStream::StreamStream(std::ostream & Stream)
	: ConceptStream(),
	  m_Stream(Stream)
{
}

StreamStream::~StreamStream()
{
}

StreamStream & StreamStream::operator << (const ConceptId ConceptId)
{
	*this << GetConcept(ConceptId);

	return *this;
}

StreamStream & StreamStream::operator << (const Concept * Concept)
{
	*this << *Concept;

	return *this;
}

StreamStream & StreamStream::operator << (const Concept & Concept)
{
	m_Stream << Concept.GetContent();

	return *this;
}

StreamStream & StreamStream::operator << (const ConceptInstance & ConceptInstance)
{
	m_Stream << ConceptInstance.GetContent();

	return *this;
}

StreamStream & StreamStream::operator << (const std::string & String)
{
	m_Stream << String;

	return *this;
}

void StreamStream::NewLine()
{
	m_Stream << '\n';
}

void StreamStream::Tab()
{
	m_Stream << '\t';
}

StreamStream & endl(StreamStream & stream)
{
	stream.NewLine();

	return stream;
}
