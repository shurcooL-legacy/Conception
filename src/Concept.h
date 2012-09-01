#pragma once
#ifndef __Concept_H__
#define __Concept_H__

typedef std::basic_string<ConceptId> ConceptString;

extern std::vector<Concept *> Concepts;

const Concept & GetConcept(ConceptId ConceptId);
Concept & ModifyConcept(ConceptId ConceptId);

class Concept
{
public:
	Concept(std::string HumanDescription)
		: m_HumanDescription(HumanDescription),
		  m_Labels()
	{
	}

	virtual ~Concept() {}

	virtual void Draw(Vector2n Position) const;
	virtual Vector2n GetDimensions() const;

	virtual std::string GetContent() const;

	void AddLabel(ConceptId Label)
	{
		m_Labels.insert(Label);
	}

	bool HasLabel(ConceptId Label) const
	{
		return (m_Labels.end() != m_Labels.find(Label));
	}

	friend std::ostream & operator << (std::ostream & Out, const Concept & Concept)
	{
		Out << Concept.GetContent();

		return Out;
	}

	static const Vector2n GetDimensions(ConceptId ConceptId)
	{
		return GetConcept(ConceptId).GetDimensions();
	}

	static const Vector2n GetDimensions(Concept * Concept)
	{
		return Concept->GetDimensions();
	}

private:
	Concept(const Concept &) = delete;
	Concept & operator = (const Concept &) = delete;

	std::string		m_HumanDescription;

	std::set<ConceptId>		m_Labels;
};

void PopulateConcepts();
void CleanConcepts();
ConceptId FindConcept(std::string Content);
ConceptId FindOrCreateConcept(std::string Content);
Concept & LastConcept();
ConceptId LastConceptId();
void VerifyNoDuplicateConcepts(std::vector<Concept *> & Concepts);

//void PrintConceptString(vector<ConceptId> & ConceptString, ostream & Out);
//std::ostream & operator << (std::ostream & Out, const ConceptString & ConceptString);
template <typename StreamT> StreamT & operator << (StreamT & Out, const ConceptString & ConceptString)
{
	for (ConceptString::size_type i = 0; i < ConceptString.size(); ++i)
	{
#if 0
		if (   (0 != i)
			&& (11 != ConceptString[i - 1])
			&& !Concepts[ConceptString[i]].HasLabel(12)
			&& !Concepts[ConceptString[i - 1]].HasLabel(15)
			&& (11 != ConceptString[i]))
		{
			Out << " ";
		}
#elif 0
		if (   (0 != i)
			&& (11 != ConceptString[i - 1])
			&& (29 != ConceptString[i - 1]))
		{
			Out << " ";
		}
#endif

		// DECISION
#if 0
		// Put a newline before open curley bracket
		if (29 == ConceptString[i])
			Out << endl;
#endif

		Out << GetConcept(ConceptString[i]);

		// DECISION
#if 0
		if (   11 == ConceptString[i]
			|| 29 == ConceptString[i])
#endif
		{
			// DECISION
#if 0
			Out << endl;
#endif
#if 0
			// Add a tab
			if (i + 1 < ConceptString.size())
				Out << "\t";
#endif
		}
	}

	return Out;
}

/*OpenGLStream & operator << (OpenGLStream & Out, const ConceptString & ConceptString)
{
	for (ConceptString::size_type i = 0; i < ConceptString.size(); ++i)
	{
#if 0
		if (   (0 != i)
			&& (11 != ConceptString[i - 1])
			&& !Concepts[ConceptString[i]].HasLabel(12)
			&& !Concepts[ConceptString[i - 1]].HasLabel(15)
			&& (11 != ConceptString[i]))
		{
			Out << " ";
		}
#elif 0
		if (   (0 != i)
			&& (11 != ConceptString[i - 1])
			&& (29 != ConceptString[i - 1]))
		{
			Out << " ";
		}
#endif

		// Put a newline before open curley bracket, and indent
		if (29 == ConceptString[i])
		{
			Out << endl;
			Out.Indent();
		}

		Out << Concepts[ConceptString[i]];

		if (   11 == ConceptString[i]
			|| 29 == ConceptString[i])
		{
			Out << endl;
			Out.Unindent();
#if 0
			// Add a tab
			if (i + 1 < ConceptString.size())
				Out << "\t";
#endif
		}
	}

	return Out;
}*/

class ConceptBasic
	: public Concept
{
public:
	ConceptBasic(std::string HumanDescription, std::string Content)
		: Concept(HumanDescription),
		  m_Content(Content)
	{
	}

	void Draw(Vector2n Position) const override;
	Vector2n GetDimensions() const override;

	std::string GetContent() const override;

private:
	std::string		m_Content;
};

class ConceptCompound
	: public Concept
{
public:
	ConceptCompound(std::string HumanDescription, ConceptString Content)
		: Concept(HumanDescription),
		  m_Content(Content)
	{
	}

	void Draw(Vector2n Position) const override;
	Vector2n GetDimensions() const override;

	std::string GetContent() const override;

private:
	ConceptString		m_Content;
};

#endif // __Concept_H__
