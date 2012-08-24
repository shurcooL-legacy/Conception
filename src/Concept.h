#pragma once
#ifndef __Concept_H__
#define __Concept_H__

typedef std::basic_string<ConceptId> ConceptString;

extern std::vector<Concept> Concepts;

class Concept
{
public:
	Concept(std::string HumanDescription)
		: m_HumanDescription(HumanDescription),
		  m_Concept(),
		  m_Labels()
	{
	}

	Concept(std::string HumanDescription, std::string Concept)
		: m_HumanDescription(HumanDescription),
		  m_Concept(Concept),
		  m_Labels()
	{
	}

	void Draw(Vector2n Position) const;
	Vector2n GetDimensions() const;

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
		Out << Concept.m_Concept;

		return Out;
	}

	static const Vector2n GetDimensions(ConceptId ConceptId)
	{
		return Concepts[ConceptId].GetDimensions();
	}

	static const Vector2n GetDimensions(Concept & Concept)
	{
		return Concept.GetDimensions();
	}

	std::string		m_HumanDescription;
	std::string		m_Concept;

private:
	std::set<ConceptId>		m_Labels;
};

void PopulateConcepts();
ConceptId FindConcept(std::string Concept);
ConceptId FindOrCreateConcept(std::string Concept);
Concept & LastConcept();
ConceptId LastConceptId();
void VerifyNoDuplicateConcepts(std::vector<Concept> & Concepts);

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

		// Put a newline before open curley bracket
		if (29 == ConceptString[i])
			Out << endl;

		Out << Concepts[ConceptString[i]];

		if (   11 == ConceptString[i]
			|| 29 == ConceptString[i])
		{
			Out << endl;
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

#endif // __Concept_H__
