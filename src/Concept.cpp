#include "Main.h"

void Concept::Draw(Vector2n Position) const
{
#if DECISION_CONCEPTS_DISPLAYED_SMALL
	DrawInnerBox(Position, Vector2n(charWidth * static_cast<sint32>(m_Concept.size()), lineHeight), Color(static_cast<uint8>(233), 239, 250), Color(static_cast<uint8>(195), 212, 242));

	if (HasLabel(26))
		glColor3d(0, 0, 1);
	else
		glColor3d(0, 0, 0);

	OpenGLStream OpenGLStream(Position);
	OpenGLStream << m_Concept;
#else
	DrawInnerRoundedBox(Position, Vector2n(static_cast<sint32>(m_Concept.size() + 1) * charWidth, lineHeight), lineHeight / 2, Color(static_cast<uint8>(233), 239, 250), Color(static_cast<uint8>(195), 212, 242));

	if (HasLabel(26))
		glColor3d(0, 0, 1);
	else
		glColor3d(0, 0, 0);

	OpenGLStream OpenGLStream(Position + Vector2n(charWidth / 2, 0));
	OpenGLStream << m_Concept;
#endif
}

Vector2n Concept::GetDimensions() const
{
#if DECISION_CONCEPTS_DISPLAYED_SMALL
	return Vector2n(static_cast<sint32>(m_Concept.length()) * charWidth, lineHeight);
#else
	return Vector2n(static_cast<sint32>(m_Concept.length() + 1) * charWidth, lineHeight);
#endif
}

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

	// FIX: It doesn't make sense that FindOrCreateConcept() calls this and gets an error often, does it?
	std::cerr << "Concept not found: " << Concept << endl;
	return 0;
}

ConceptId FindOrCreateConcept(std::string Concept)
{
	auto ConceptId = FindConcept(Concept);

	if (0 == ConceptId)
	{
		Concepts.push_back(::Concept("", Concept));
		ConceptId = LastConceptId();
	}

	return ConceptId;
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
