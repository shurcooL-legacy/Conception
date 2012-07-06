#pragma once
#ifndef __Concept_H__
#define __Concept_H__

typedef std::basic_string<ConceptId> ConceptString;

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

	void AddLabel(ConceptId Label)
	{
		m_Labels.insert(Label);
	}

	bool HasLabel(ConceptId Label) const
	{
		return (m_Labels.end() != m_Labels.find(Label));
	}

 	Vector2n GetDimensions() const
	{
		return Vector2n(m_Concept.length() * charWidth, lineHeight);
	}

	friend std::ostream & operator << (std::ostream & out, const Concept & Concept)
	{
		out << Concept.m_Concept.c_str();

		return out;
	}

	std::string		m_HumanDescription;
	std::string		m_Concept;

private:
	std::set<ConceptId>		m_Labels;
};

extern std::vector<Concept> Concepts;

void PopulateConcepts();
ConceptId FindConcept(std::string Concept);
Concept & LastConcept();
ConceptId LastConceptId();
void VerifyNoDuplicateConcepts(std::vector<Concept> & Concepts);

//void PrintConceptString(vector<ConceptId> & ConceptString, ostream & out);
//std::ostream & operator << (std::ostream & out, vector<ConceptId> & ConceptString);
template <typename StreamT> StreamT & operator << (StreamT & out, const ConceptString & ConceptString)
{
	for (ConceptString::size_type i = 0; i < ConceptString.size(); ++i)
	{
#if 0
		if ((0 != i)
			&& (11 != ConceptString[i - 1])
			&& !Concepts[ConceptString[i]].HasLabel(12)
			&& !Concepts[ConceptString[i - 1]].HasLabel(15)
			&& (11 != ConceptString[i]))
		{
			out << " ";
		}
#else
		if ((0 != i)
			&& (11 != ConceptString[i - 1]))
		{
			out << " ";
		}
#endif

		out << Concepts[ConceptString[i]];

		if (11 == ConceptString[i])
		{
			out << endl;
#if 0
			// Add a tab
			if (i + 1 < ConceptString.size())
				out << "\t";
#endif
		}
	}

	return out;
}

#endif // __Concept_H__
