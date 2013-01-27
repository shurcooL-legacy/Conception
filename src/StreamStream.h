#pragma once
#ifndef __StreamStream_H__
#define __StreamStream_H__

class StreamStream
	: public ConceptStream
{
public:
	StreamStream(std::ostream & Stream);
	~StreamStream();

	StreamStream & operator << (const ConceptId ConceptId) override;
	StreamStream & operator << (const Concept * Concept) override;
	StreamStream & operator << (const Concept & Concept) override;
	StreamStream & operator << (const ConceptInstance & ConceptInstance) override;
	StreamStream & operator << (const std::string & String) override;

private:
	StreamStream(const StreamStream &) = delete;
	StreamStream & operator = (const StreamStream &) = delete;

	void NewLine() override;
	void Tab();

	std::ostream & m_Stream;

	friend StreamStream & endl(StreamStream & stream);
};

StreamStream & endl(StreamStream & stream);

#endif // __StreamStream_H__
