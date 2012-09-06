#include "Main.h"

std::vector<Concept *> Concepts;

ConceptInstance::ConceptInstance(ConceptId ConceptId)
	: m_ConceptId(ConceptId),
	  m_Parameters()
{}

ConceptInstance::ConceptInstance(ConceptId ConceptId, std::initializer_list<::ConceptId> Parameters)
	: m_ConceptId(ConceptId),
	  m_Parameters(Parameters)
{}
/*ConceptInstance::ConceptInstance(ConceptId ConceptId)
	: m_ConceptId(ConceptId),
	  m_Parameters(nullptr)
{}

ConceptInstance::ConceptInstance(ConceptId ConceptId, std::initializer_list<::ConceptId> Parameters)
	: m_ConceptId(ConceptId),
	  m_Parameters(std::unique_ptr<std::vector<::ConceptId>>(new std::vector<::ConceptId>(Parameters)))
{}

ConceptInstance::ConceptInstance(const ConceptInstance & Other)
	: m_ConceptId(Other.m_ConceptId)
{
	if (nullptr != Other.GetParameters())
	{
		m_Parameters = std::unique_ptr<std::vector<ConceptId>>(new std::vector<ConceptId>(*Other.GetParameters()));
	}
	else
	{
		m_Parameters = nullptr;
	}
}

ConceptInstance & ConceptInstance::operator = (const ConceptInstance & Other)
{
	m_ConceptId = Other.m_ConceptId;

	if (nullptr != Other.GetParameters())
	{
		m_Parameters = std::unique_ptr<std::vector<ConceptId>>(new std::vector<ConceptId>(*Other.GetParameters()));
	}
	else
	{
		m_Parameters = nullptr;
	}

	return *this;
}*/

void ConceptInstance::Draw(Vector2n Position) const
{
	GetConcept(m_ConceptId).Draw(*this, Position);
}

Vector2n ConceptInstance::GetDimensions() const
{
	return GetConcept(m_ConceptId).GetDimensions(*this);
}

std::string ConceptInstance::GetContent() const
{
	return GetConcept(m_ConceptId).GetContent(*this);
}

void Concept::Draw(const ConceptInstance & ConceptInstance, Vector2n Position) const
{
#if DECISION_CONCEPTS_DISPLAYED_SMALL
	DrawInnerBox(Position, GetDimensions(ConceptInstance), Color(static_cast<uint8>(233), 239, 250), Color(static_cast<uint8>(195), 212, 242));
#else
	DrawInnerRoundedBox(Position, GetDimensions(ConceptInstance), lineHeight / 2, Color(static_cast<uint8>(233), 239, 250), Color(static_cast<uint8>(195), 212, 242));
#endif
}

Vector2n Concept::GetDimensions(const ConceptInstance & ConceptInstance) const
{
#if DECISION_CONCEPTS_DISPLAYED_SMALL
	return Vector2n(charWidth, lineHeight);
#else
	return Vector2n(charWidth * 2, lineHeight);
#endif
}

std::string Concept::GetContent(const ConceptInstance & ConceptInstance) const
{
	return "";
}

const Vector2n Concept::GetDimensions(ConceptId ConceptId)
{
	// HACK
	ConceptInstance ConceptInstance(ConceptId);

	return ConceptInstance.GetDimensions();
}

const Vector2n Concept::GetDimensions(Concept * Concept)
{
	// HACK
	ConceptInstance ConceptInstance(0);

	return Concept->GetDimensions(ConceptInstance);
}

/*static const Vector2n Concept::GetDimensions(const ConceptInstance & ConceptInstance)
{
	return ConceptInstance.GetDimensions();
}*/

const Concept & GetConcept(ConceptId ConceptId)
{
	return *Concepts[ConceptId];
}

Concept & ModifyConcept(ConceptId ConceptId)
{
	return *Concepts[ConceptId];
}

void PopulateConcepts()
{
#	include "Concepts.h"

	/*{
		std::vector<std::unique_ptr<Concept>> ConceptsX;
		ConceptsX.push_back(std::unique_ptr<Concept>(new Concept("Null concept")));
		ConceptsX.push_back(std::unique_ptr<Concept>(new ConceptBasic("", "std::cout")));

		auto & C0 = ConceptsX[0];
		auto & C1 = ConceptsX[1];
		printf(">>>>>>>>> '%s'\n", C0->GetContent().c_str());
		printf(">>>>>>>>> '%s'\n", C1->GetContent().c_str());
	}*/

	VerifyNoDuplicateConcepts(Concepts);
}

void CleanConcepts()
{
	while (!Concepts.empty())
	{
		delete Concepts.back();
		Concepts.pop_back();
	}
}

ConceptId FindConcept(std::string Content)
{
	// HACK
	ConceptInstance ConceptInstance(0);

	for (auto it0 = Concepts.begin(); it0 != Concepts.end(); ++it0)
	{
		if (Content == (*it0)->GetContent(ConceptInstance))
			return (it0 - Concepts.begin());
	}

	// FIX: It doesn't make sense that FindOrCreateConcept() calls this and gets an error often, does it?
	std::cerr << "Concept not found: " << Content << endl;
	return 0;
}

ConceptId FindOrCreateConcept(std::string Content)
{
	auto ConceptId = FindConcept(Content);

	if (0 == ConceptId)
	{
		Concepts.push_back(new ConceptBasic("", Content));
		ConceptId = LastConceptId();
	}

	return ConceptId;
}

Concept & LastConcept()
{
	return *Concepts.back();
}

ConceptId LastConceptId()
{
	return Concepts.size() - 1;
}

void VerifyNoDuplicateConcepts(std::vector<Concept *> & Concepts)
{
	// HACK
	ConceptInstance ConceptInstance(0);

	std::set<std::string> ConceptSet;
	uint32 ExpectedConceptCount = 0;

	for (auto & Concept : Concepts)
	{
		if (0 != Concept->GetContent(ConceptInstance).length())
		{
			ConceptSet.insert(Concept->GetContent(ConceptInstance));
			++ExpectedConceptCount;
		}
	}

	if (ConceptSet.size() != ExpectedConceptCount)
	{
		throw std::string("Duplicate concepts found!");
	}
}

void ConceptBasic::Draw(const ConceptInstance & ConceptInstance, Vector2n Position) const
{
#if DECISION_CONCEPTS_DISPLAYED_SMALL
	DrawInnerBox(Position, GetDimensions(ConceptInstance), Color(static_cast<uint8>(233), 239, 250), Color(static_cast<uint8>(195), 212, 242));

	if (HasLabel(26))
		glColor3d(0, 0, 1);
	else
		glColor3d(0, 0, 0);

	OpenGLStream OpenGLStream(Position);
	OpenGLStream << m_Content;
#else
	DrawInnerRoundedBox(Position, GetDimensions(ConceptInstance), lineHeight / 2, Color(static_cast<uint8>(233), 239, 250), Color(static_cast<uint8>(195), 212, 242));

	if (HasLabel(26))
		glColor3d(0, 0, 1);
	else
		glColor3d(0, 0, 0);

	OpenGLStream OpenGLStream(Position + Vector2n(charWidth / 2, 0));
	OpenGLStream << m_Content;
#endif
}

Vector2n ConceptBasic::GetDimensions(const ConceptInstance & ConceptInstance) const
{
	// TEST, TODO: Optimize it by outsourcing it somewhere, turn it into reusable code
	{
		std::string::size_type MaxLineLength = 1;
		uint32 Height = 0;
		std::string::size_type Start = 0, End;
		do
		{
			End = m_Content.find_first_of('\n', Start);

			// TODO: Count tabs properly?
			auto Length = ((std::string::npos != End) ? End : m_Content.length()) - Start;
			if (MaxLineLength < Length)
				MaxLineLength = Length;

			++Height;

			Start = End + 1;
		}
		while (std::string::npos != End);

#if DECISION_CONCEPTS_DISPLAYED_SMALL
		return Vector2n(static_cast<sint32>(MaxLineLength) * charWidth, Height * lineHeight);
#else
		return Vector2n(static_cast<sint32>(MaxLineLength + 1) * charWidth, Height * lineHeight);
#endif
	}
}

std::string ConceptBasic::GetContent(const ConceptInstance & ConceptInstance) const
{
	return m_Content;
}

/*void ConceptCompound::Draw(Vector2n Position) const
{
#if DECISION_CONCEPTS_DISPLAYED_SMALL
	DrawInnerBox(Position, GetDimensions(), Color(static_cast<uint8>(233), 239, 250), Color(static_cast<uint8>(195), 212, 242));
#else
	DrawInnerRoundedBox(Position, GetDimensions(), lineHeight / 2, Color(static_cast<uint8>(233), 239, 250), Color(static_cast<uint8>(195), 212, 242));
#endif

	OpenGLStream OpenGLStream(Position);
	OpenGLStream << m_Content;
}

Vector2n ConceptCompound::GetDimensions() const
{
	Vector2n CaretPosition;

	for (ConceptString::size_type i = 0; i < m_Content.size(); ++i)
	{
		Vector2n InnerDimensions = GetConcept(m_Content[i]).GetDimensions();

		CaretPosition.X() += InnerDimensions.X();
		CaretPosition.Y() += InnerDimensions.Y() - lineHeight;
	}

	return Vector2n(CaretPosition.X(), CaretPosition.Y() + lineHeight);
}

std::string ConceptCompound::GetContent() const
{
	std::string Content;

	for (ConceptString::size_type i = 0; i < m_Content.size(); ++i)
	{
		Content += GetConcept(m_Content[i]).GetContent();
	}

	return Content;
}*/

void ConceptParameterized::Draw(const ConceptInstance & ConceptInstance, Vector2n Position) const
{
#if DECISION_CONCEPTS_DISPLAYED_SMALL
	DrawInnerBox(Position, GetDimensions(ConceptInstance), Color(static_cast<uint8>(233), 239, 250), Color(static_cast<uint8>(195), 212, 242));
#else
	DrawInnerRoundedBox(Position, GetDimensions(ConceptInstance), lineHeight / 2, Color(static_cast<uint8>(233), 239, 250), Color(static_cast<uint8>(195), 212, 242));
#endif

	OpenGLStream OpenGLStream(Position);
	OpenGLStream << m_Content(ConceptInstance);
}

Vector2n ConceptParameterized::GetDimensions(const ConceptInstance & ConceptInstance) const
{
	auto Content = m_Content(ConceptInstance);
	Vector2n CaretPosition;

	for (decltype(Content)::size_type i = 0; i < Content.size(); ++i)
	{
		// TODO: Use ConceptInstance?
		Vector2n InnerDimensions = Content[i].GetDimensions();

		CaretPosition.X() += InnerDimensions.X();
		CaretPosition.Y() += InnerDimensions.Y() - lineHeight;
	}

	return Vector2n(CaretPosition.X(), CaretPosition.Y() + lineHeight);
}

std::string ConceptParameterized::GetContent(const ConceptInstance & ConceptInstance) const
{
	auto Content = m_Content(ConceptInstance);
	std::string ContentString;

	for (decltype(Content)::size_type i = 0; i < Content.size(); ++i)
	{
		// TODO: Use ConceptInstance?
		ContentString += Content[i].GetContent();
	}

	return ContentString;
}
