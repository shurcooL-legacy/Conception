#pragma once
#ifndef __DimensionsStream_H__
#define __DimensionsStream_H__

class DimensionsStream
	: public ConceptStream
{
public:
	DimensionsStream();
	~DimensionsStream();

	DimensionsStream & operator << (const ConceptId ConceptId) override;
	DimensionsStream & operator << (const Concept * Concept) override;
	DimensionsStream & operator << (const Concept & Concept) override;
	DimensionsStream & operator << (const ConceptInstance & ConceptInstance) override;
	DimensionsStream & operator << (const std::string & String) override;

	const Vector2n GetDimensions() const;

private:
	DimensionsStream(const DimensionsStream &) = delete;
	DimensionsStream & operator = (const DimensionsStream &) = delete;

	void PrintLine(const std::string & Line);
	void PrintSegment(const std::string & String);

	void NewLine() override;
	void Tab();

	Vector2n		m_CaretPosition;
	sint32			m_LineStartX;
	sint32			m_MaxLineLengthX;

	friend DimensionsStream & endl(DimensionsStream & stream);
};

DimensionsStream & endl(DimensionsStream & stream);

#endif // __DimensionsStream_H__
