#pragma once
#ifndef __OpenGLStream_H__
#define __OpenGLStream_H__

class OpenGLStream
{
public:
	OpenGLStream(Vector2n CaretPosition);
	~OpenGLStream();
	
	OpenGLStream & operator << (const Concept & Concept);
	OpenGLStream & operator << (const std::string & String);
	OpenGLStream & operator << (OpenGLStream & (* Function)(OpenGLStream &));

	void SetBackgroundColor(Color BackgroundColor);

	const Vector2n GetCaretPosition() const;

private:
	OpenGLStream(const OpenGLStream &) = delete;
	OpenGLStream & operator =(const OpenGLStream &) = delete;

	void PrintLine(const std::string & Line);
	void PrintSegment(const std::string & String);

	void NewLine();
	void Tab();

	Vector2n		m_CaretPosition;
	sint32			m_LineStartX;

	Color			m_BackgroundColor;

	friend OpenGLStream & endl(OpenGLStream & stream);
};

OpenGLStream & endl(OpenGLStream & stream);

#endif // __OpenGLStream_H__
