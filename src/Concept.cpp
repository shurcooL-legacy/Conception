#include "Main.h"

std::vector<Concept> Concepts;

void PopulateConcepts()
{
#	include "Concepts.h"

	VerifyNoDuplicateConcepts(Concepts);
}

ConceptId FindConcept(std::string Concept)
{
	for (auto it0 = Concepts.begin(); it0 != Concepts.end(); ++it0)
	{
		if (Concept == it0->m_Concept)
			return (it0 - Concepts.begin());
	}

	std::cerr << "Concept not found: " << Concept << endl;
	return 0;
}

Concept & LastConcept()
{
	return Concepts.back();
}

ConceptId LastConceptId()
{
	return Concepts.size() - 1;
}

void VerifyNoDuplicateConcepts(std::vector<Concept> & Concepts)
{
	std::set<std::string> ConceptSet;
	uint32 ExpectedConceptCount = 0;

	for (auto & Concept : Concepts)
	{
		if (0 != Concept.m_Concept.length())
		{
			ConceptSet.insert(Concept.m_Concept);
			++ExpectedConceptCount;
		}
	}

	if (ConceptSet.size() != ExpectedConceptCount)
	{
		throw std::string("Duplicate concepts found!");
	}
}
