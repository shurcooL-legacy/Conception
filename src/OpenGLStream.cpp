#include "Main.h"

OpenGLStream::OpenGLStream(Vector2n CaretPosition)
	: m_CaretPosition(CaretPosition),
	  m_LineStartX(CaretPosition.X()),
	  m_BackgroundColor(1, 1, 1)
{
}

OpenGLStream::~OpenGLStream()
{
}

OpenGLStream & OpenGLStream::operator << (const Concept & Concept)
{
	DrawInnerBox(m_CaretPosition, Vector2n(charWidth * Concept.m_Concept.size(), lineHeight), Color(233 / 255.0, 239 / 255.0, 250 / 255.0), Color(195 / 255.0, 212 / 255.0, 242 / 255.0));

	if (Concept.HasLabel(26))
		glColor3d(0, 0, 1);
	else
		glColor3d(0, 0, 0);

	PrintLine(Concept.m_Concept.c_str());

	return *this;
}

OpenGLStream & OpenGLStream::operator << (const std::string & String)
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
	if (Color(1, 1, 1) != m_BackgroundColor)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		DrawBox(m_CaretPosition, Vector2n(Segment.length() * charWidth, lineHeight), m_BackgroundColor, m_BackgroundColor);
		glPopAttrib();
	}

	OglUtilsPrint(m_CaretPosition.X(), m_CaretPosition.Y(), 0, LEFT, Segment.c_str());
	m_CaretPosition.X() += Segment.length() * charWidth;
}

void OpenGLStream::NewLine()
{
	if (Color(1, 1, 1) != m_BackgroundColor)
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

	if (Color(1, 1, 1) != m_BackgroundColor)
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
