#include "Main.h"

OpenGLStream::OpenGLStream(Vector2n CaretPosition)
	: OpenGLStream(CaretPosition, Color::BLACK)
{}

OpenGLStream::OpenGLStream(Vector2n CaretPosition, Color Foreground)
	: ConceptStream(),
	  m_CaretPosition(CaretPosition),
	  m_LineStartX(CaretPosition.X()),
	  m_Foreground(Foreground),
	  m_BackgroundColor(1.0, 1.0, 1.0)
{
}

OpenGLStream::~OpenGLStream()
{
}

OpenGLStream & OpenGLStream::operator << (const ConceptId ConceptId)
{
	*this << GetConcept(ConceptId);

	return *this;
}

OpenGLStream & OpenGLStream::operator << (const Concept * Concept)
{
	*this << *Concept;

	return *this;
}

OpenGLStream & OpenGLStream::operator << (const Concept & Concept)
{
	Concept.Render(m_CaretPosition);

	m_CaretPosition.X() += Concept.GetDimensions().X();
	m_CaretPosition.Y() += Concept.GetDimensions().Y() - lineHeight;

	return *this;
}

OpenGLStream & OpenGLStream::operator << (const ConceptInstance & ConceptInstance)
{
	ConceptInstance.Render(m_CaretPosition);

	m_CaretPosition.X() += ConceptInstance.GetDimensions().X();
	m_CaretPosition.Y() += ConceptInstance.GetDimensions().Y() - lineHeight;

	return *this;
}

OpenGLStream & OpenGLStream::operator << (const std::string & String)
{
	glColor3dv(m_Foreground.GetComponents());

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

OpenGLStream & OpenGLStream::operator << (OpenGLStream & (* Function)(OpenGLStream &))
{
	(*Function)(*this);

	return *this;
}

void OpenGLStream::SetBackgroundColor(Color BackgroundColor)
{
	m_BackgroundColor = BackgroundColor;
}

const Vector2n OpenGLStream::GetCaretPosition() const
{
	return m_CaretPosition;
}

void OpenGLStream::Indent()
{
	m_CaretPosition.X() += 4 * charWidth;
	m_LineStartX += 4 * charWidth;
}

void OpenGLStream::Unindent()
{
	m_CaretPosition.X() -= 4 * charWidth;
	m_LineStartX -= 4 * charWidth;

}

// Prints a string that doesn't contain line breaks
void OpenGLStream::PrintLine(const std::string & Line)
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
void OpenGLStream::PrintSegment(const std::string & Segment)
{
	if (Color(1.0, 1.0, 1.0) != m_BackgroundColor)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		DrawBox(m_CaretPosition, Vector2n(static_cast<sint32>(Segment.length()) * charWidth, lineHeight), m_BackgroundColor, m_BackgroundColor);
		glPopAttrib();
	}

	OglUtilsPrint(m_CaretPosition.X(), m_CaretPosition.Y(), 0, LEFT, Segment.c_str());
	m_CaretPosition.X() += Segment.length() * charWidth;
}

void OpenGLStream::NewLine()
{
	if (Color(1.0, 1.0, 1.0) != m_BackgroundColor)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		DrawBox(m_CaretPosition, Vector2n(charWidth, lineHeight), m_BackgroundColor, m_BackgroundColor);
		glPopAttrib();
	}

	m_CaretPosition.X() = m_LineStartX;
	m_CaretPosition.Y() += lineHeight;
}

void OpenGLStream::Tab()
{
	auto NewCaretPositionX = m_LineStartX + (((m_CaretPosition.X() - m_LineStartX) / charWidth / 4) + 1) * 4 * charWidth;

	if (Color(1.0, 1.0, 1.0) != m_BackgroundColor)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		DrawBox(m_CaretPosition, Vector2n(NewCaretPositionX - m_CaretPosition.X(), lineHeight), m_BackgroundColor, m_BackgroundColor);
		glPopAttrib();
	}

	m_CaretPosition.X() = NewCaretPositionX;
}

OpenGLStream & endl(OpenGLStream & stream)
{
	stream.NewLine();

	return stream;
}
