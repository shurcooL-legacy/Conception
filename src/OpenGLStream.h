#pragma once
#ifndef __OpenGLStream_H__
#define __OpenGLStream_H__

class OpenGLStream
	: public ConceptStream
{
public:
	OpenGLStream(Vector2n CaretPosition);
	~OpenGLStream();

	OpenGLStream & operator << (const ConceptId ConceptId) override;
	OpenGLStream & operator << (const Concept * Concept) override;
	OpenGLStream & operator << (const Concept & Concept) override;
	OpenGLStream & operator << (const ConceptInstance & ConceptInstance) override;
	OpenGLStream & operator << (const std::string & String) override;
	OpenGLStream & operator << (OpenGLStream & (* Function)(OpenGLStream &));

	void SetBackgroundColor(Color BackgroundColor);

	const Vector2n GetCaretPosition() const;

	void Indent();
	void Unindent();

private:
	OpenGLStream(const OpenGLStream &) = delete;
	OpenGLStream & operator = (const OpenGLStream &) = delete;

	void PrintLine(const std::string & Line);
	void PrintSegment(const std::string & String);

	void NewLine() override;
	void Tab();

	Vector2n		m_CaretPosition;
	sint32			m_LineStartX;

	Color			m_BackgroundColor;

	friend OpenGLStream & endl(OpenGLStream & stream);
};

OpenGLStream & endl(OpenGLStream & stream);

#endif // __OpenGLStream_H__
