#pragma once
#ifndef __ConceptStream_H__
#define __ConceptStream_H__

class ConceptStream
{
public:
	virtual ConceptStream & operator << (const ConceptId ConceptId) = 0;
	virtual ConceptStream & operator << (const Concept * Concept) = 0;
	virtual ConceptStream & operator << (const Concept & Concept) = 0;
	virtual ConceptStream & operator << (const ConceptInstance & ConceptInstance) = 0;
	virtual ConceptStream & operator << (const std::string & String) = 0;
	ConceptStream & operator << (ConceptStream & (* Function)(ConceptStream &));

protected:
	ConceptStream();
	virtual ~ConceptStream();

	virtual void NewLine() = 0;

private:
	ConceptStream(const ConceptStream &) = delete;
	ConceptStream & operator = (const ConceptStream &) = delete;

	friend ConceptStream & endl(ConceptStream & stream);
};

ConceptStream & endl(ConceptStream & stream);

#endif // __ConceptStream_H__
