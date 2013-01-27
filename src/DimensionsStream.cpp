#include "Main.h"

DimensionsStream::DimensionsStream()
	: ConceptStream(),
	  m_CaretPosition(Vector2n::ZERO),
	  m_LineStartX(0),
	  m_MaxLineLengthX(0)
{
}

DimensionsStream::~DimensionsStream()
{
}

DimensionsStream & DimensionsStream::operator << (const ConceptId ConceptId)
{
	*this << GetConcept(ConceptId);

	return *this;
}

DimensionsStream & DimensionsStream::operator << (const Concept * Concept)
{
	*this << *Concept;

	return *this;
}

DimensionsStream & DimensionsStream::operator << (const Concept & Concept)
{
	m_CaretPosition.X() += Concept.GetDimensions().X();
	m_CaretPosition.Y() += Concept.GetDimensions().Y() - lineHeight;

	return *this;
}

DimensionsStream & DimensionsStream::operator << (const ConceptInstance & ConceptInstance)
{
	m_CaretPosition.X() += ConceptInstance.GetDimensions().X();
	m_CaretPosition.Y() += ConceptInstance.GetDimensions().Y() - lineHeight;

	return *this;
}

DimensionsStream & DimensionsStream::operator << (const std::string & String)
{
	glColor3d(0, 0, 0);

	std::string::size_type Start = 0, End;
	do
	{
		End = String.find_first_of('\n', Start);

		auto Length = ((std::string::npos != End) ? End : String.length()) - Start;
		PrintLine(String.substr(Start, Length));
		if (std::string::npos != End)
		{
			NewLine();
		}

		Start = End + 1;
	}
	while (std::string::npos != End);

	return *this;
}

// Prints a string that doesn't contain line breaks
void DimensionsStream::PrintLine(const std::string & Line)
{
	std::string::size_type Start = 0, End;
	do
	{
		End = Line.find_first_of('\t', Start);

		auto Length = ((std::string::npos != End) ? End : Line.length()) - Start;
		PrintSegment(Line.substr(Start, Length));
		if (std::string::npos != End)
		{
			Tab();
		}

		Start = End + 1;
	}
	while (std::string::npos != End);
}

// Prints a segment that doesn't contain line breaks, tabs
void DimensionsStream::PrintSegment(const std::string & Segment)
{
	m_CaretPosition.X() += Segment.length() * charWidth;
}

void DimensionsStream::NewLine()
{
	if (m_MaxLineLengthX < m_CaretPosition.X())
		m_MaxLineLengthX = m_CaretPosition.X();

	m_CaretPosition.X() = m_LineStartX;
	m_CaretPosition.Y() += lineHeight;
}

void DimensionsStream::Tab()
{
	auto NewCaretPositionX = m_LineStartX + (((m_CaretPosition.X() - m_LineStartX) / charWidth / 4) + 1) * 4 * charWidth;
	m_CaretPosition.X() = NewCaretPositionX;
}

const Vector2n DimensionsStream::GetDimensions() const
{
	return Vector2n(std::max<sint32>(m_CaretPosition.X(), m_MaxLineLengthX), m_CaretPosition.Y() + lineHeight);
}

DimensionsStream & endl(DimensionsStream & stream)
{
	stream.NewLine();

	return stream;
}
