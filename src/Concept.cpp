#include "Main.h"

std::vector<Concept *> Concepts;

void Concept::Render(Vector2n Position) const
{
#if DECISION_CONCEPTS_DISPLAYED_SMALL
	DrawInnerBox(Position, GetDimensions(), Color(static_cast<uint8>(233), 239, 250) * 1.075, Color(static_cast<uint8>(195), 212, 242));
#else
	DrawInnerRoundedBox(Position, GetDimensions(), lineHeight / 2, Color(static_cast<uint8>(233), 239, 250) * 1.075, Color(static_cast<uint8>(195), 212, 242));
#endif
}

Vector2n Concept::GetDimensions() const
{
#if DECISION_CONCEPTS_DISPLAYED_SMALL
	return Vector2n(charWidth, lineHeight);
#else
	return Vector2n(charWidth * 2, lineHeight);
#endif
}

std::string Concept::GetContent() const
{
	return "";
}

const Vector2n Concept::GetDimensions(const std::string & Content)
{
	std::string::size_type MaxLineLengthX = 0;
	uint32 Height = 1;
	std::string::size_type Start = 0, End;
	do
	{
		End = Content.find_first_of('\n', Start);

		uint32 LineLengthX = 0;
		auto LineLength = ((std::string::npos != End) ? End : Content.length()) - Start;
		auto Line = Content.substr(Start, LineLength);
		{
			std::string::size_type Start = 0, End;
			do
			{
				End = Line.find_first_of('\t', Start);

				auto SegmentLength = ((std::string::npos != End) ? End : Line.length()) - Start;
				LineLengthX += SegmentLength;
				if (std::string::npos != End)
				{
					LineLengthX = (LineLengthX / 4 + 1) * 4;		// Tab
				}

				Start = End + 1;
			}
			while (std::string::npos != End);
		}
		if (MaxLineLengthX < LineLengthX)
			MaxLineLengthX = LineLengthX;
		if (std::string::npos != End)
		{
			++Height;
		}

		Start = End + 1;
	}
	while (std::string::npos != End);

	return Vector2n(static_cast<sint32>(MaxLineLengthX) * charWidth, Height * lineHeight);
}

const Vector2n Concept::GetDimensions(const ConceptId ConceptId)
{
	return GetConcept(ConceptId).GetDimensions();
}

const Vector2n Concept::GetDimensions(const Concept * const Concept)
{
	return Concept->GetDimensions();
}

/*const Vector2n Concept::GetDimensions(const ConceptInstance & ConceptInstance)
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
	for (ConceptId ConceptId = 0; ConceptId < Concepts.size(); ++ConceptId)
	{
		if (Content == GetConcept(ConceptId).GetContent())
			return ConceptId;
	}

	return 0;		// Concept not found, return null concept
}

ConceptId CreateConcept(std::string Content)
{
	Concepts.push_back(new ConceptBasic("", Content));
	return LastConceptId();
}

ConceptId FindOrCreateConcept(std::string Content)
{
	auto ConceptId = FindConcept(Content);

	if (0 == ConceptId)
	{
		ConceptId = CreateConcept(Content);
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
	std::set<std::string> ConceptSet;
	uint32 ExpectedConceptCount = 0;

	for (ConceptId ConceptId = 0; ConceptId < Concepts.size(); ++ConceptId)
	{
		auto Content = GetConcept(ConceptId).GetContent();

		if (0 != Content.length())
		{
			ConceptSet.insert(Content);
			++ExpectedConceptCount;
		}
	}

	if (ConceptSet.size() != ExpectedConceptCount)
	{
		std::cerr << "WARNING: Duplicate concepts found!\n";
		throw 0;
	}
}

void ConceptBasic::Render(Vector2n Position) const
{
#if DECISION_CONCEPTS_DISPLAYED_SMALL
	DrawInnerBox(Position, GetDimensions(), Color(static_cast<uint8>(233), 239, 250), Color(static_cast<uint8>(195), 212, 242));

	if (HasLabel(26))
		glColor3d(0, 0, 1);
	else
		glColor3d(0, 0, 0);

	OpenGLStream OpenGLStream(Position);
	OpenGLStream << m_Content;
#else
	DrawInnerRoundedBox(Position, GetDimensions(), lineHeight / 2, Color(static_cast<uint8>(233), 239, 250), Color(static_cast<uint8>(195), 212, 242));

	if (HasLabel(26))
		glColor3d(0, 0, 1);
	else
		glColor3d(0, 0, 0);

	OpenGLStream OpenGLStream(Position + Vector2n(charWidth / 2, 0));
	OpenGLStream << m_Content;
#endif
}

Vector2n ConceptBasic::GetDimensions() const
{
#if DECISION_CONCEPTS_DISPLAYED_SMALL
	return Concept::GetDimensions(m_Content);
#else
	return Concept::GetDimensions(m_Content) + Vector2n(charWidth, 0);
#endif
}

std::string ConceptBasic::GetContent() const
{
	return m_Content;
}

/*void ConceptCompound::Render(Vector2n Position) const
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

ConceptParameters ConceptParameterized::EmptyParameters;

void ConceptParameterized::Render(const ConceptParameters & ConceptParameters, Vector2n Position) const
{
#if DECISION_CONCEPTS_DISPLAYED_SMALL
	DrawInnerBox(Position, GetDimensions(ConceptParameters), Color(static_cast<uint8>(233), 239, 250), Color(static_cast<uint8>(195), 212, 242));
#else
	DrawInnerRoundedBox(Position, GetDimensions(ConceptParameters), lineHeight / 2, Color(static_cast<uint8>(233), 239, 250), Color(static_cast<uint8>(195), 212, 242));
#endif

	//OpenGLStream OpenGLStream(Position);
	//OpenGLStream << m_Content(ConceptParameters);
	// TEST, DUPLICATION
	auto Content = m_Content(ConceptParameters);
	Vector2n CaretPosition = Position;

	for (decltype(Content)::size_type i = 0; i < Content.size(); ++i)
	{
		//OpenGLStream << GetConcept(ConceptString[i]).GetContent(ConceptParameters);
		GetConcept(Content[i]).Render(ConceptParameters, CaretPosition);

		Vector2n InnerDimensions = GetConcept(Content[i]).GetDimensions(ConceptParameters);

		CaretPosition.X() += InnerDimensions.X();
		CaretPosition.Y() += InnerDimensions.Y() - lineHeight;
	}
}

Vector2n ConceptParameterized::GetDimensions(const ConceptParameters & ConceptParameters) const
{
	auto Content = m_Content(ConceptParameters);
	Vector2n CaretPosition;

	for (decltype(Content)::size_type i = 0; i < Content.size(); ++i)
	{
		Vector2n InnerDimensions = GetConcept(Content[i]).GetDimensions(ConceptParameters);

		CaretPosition.X() += InnerDimensions.X();
		CaretPosition.Y() += InnerDimensions.Y() - lineHeight;
	}

	return Vector2n(CaretPosition.X(), CaretPosition.Y() + lineHeight);
}

std::string ConceptParameterized::GetContent(const ConceptParameters & ConceptParameters) const
{
	auto Content = m_Content(ConceptParameters);
	std::string ContentString;

	for (decltype(Content)::size_type i = 0; i < Content.size(); ++i)
	{
		// TODO: Use ConceptInstance?
		ContentString += GetConcept(Content[i]).GetContent(ConceptParameters);
	}

	return ContentString;
}

ConceptParameters ConceptParameterized2::EmptyParameters;

void ConceptParameterized2::Render(const ConceptParameters & ConceptParameters, Vector2n Position) const
{
#if DECISION_CONCEPTS_DISPLAYED_SMALL
	DrawInnerBox(Position, GetDimensions(ConceptParameters), Color(static_cast<uint8>(233), 239, 250), Color(static_cast<uint8>(195), 212, 242));
#else
	DrawInnerRoundedBox(Position, GetDimensions(ConceptParameters), lineHeight / 2, Color(static_cast<uint8>(233), 239, 250), Color(static_cast<uint8>(195), 212, 242));
#endif

	OpenGLStream OpenGLStream(Position);
	m_Content(OpenGLStream, ConceptParameters);
}

Vector2n ConceptParameterized2::GetDimensions(const ConceptParameters & ConceptParameters) const
{
#if 0
	// HACK
	//return Concept::GetDimensions(GetContent(ConceptParameters));
	return Vector2n(620, 300);
#else
	DimensionsStream Stream;
	m_Content(Stream, ConceptParameters);
	return Stream.GetDimensions();
#endif
}

std::string ConceptParameterized2::GetContent(const ConceptParameters & ConceptParameters) const
{
	std::ostringstream oss;
	StreamStream Stream(oss);
	m_Content(Stream, ConceptParameters);
	return oss.str();
}

/*ConceptInstance::ConceptInstance(ConceptId ConceptId)
	: m_ConceptId(ConceptId),
	  m_Parameters()
{}

ConceptInstance::ConceptInstance(ConceptId ConceptId, std::initializer_list<::ConceptId> Parameters)
	: m_ConceptId(ConceptId),
	  m_Parameters(Parameters)
{}*/
/*ConceptInstance::ConceptInstance(std::string HumanDescription, ConceptId ConceptId)
	: Concept()
	  m_ConceptId(ConceptId),
//	  m_Parameters(nullptr)
	  m_Parameters()
{}*/

/*ConceptInstance::ConceptInstance(const ConceptInstance & Other)
	: Concept("TODO: FIX, this should be an identical copy?"),
	  m_ConceptId(Other.m_ConceptId),
//	  m_Parameters((nullptr == Other.GetParameters()) ? nullptr : std::unique_ptr<std::vector<ConceptId>>(new std::vector<ConceptId>(*Other.GetParameters())))
	  m_Parameters(Other.m_Parameters)
{
	/ *if (nullptr != Other.GetParameters())
	{
		m_Parameters = std::unique_ptr<std::vector<ConceptId>>(new std::vector<ConceptId>(*Other.GetParameters()));
	}
	else
	{
		m_Parameters = nullptr;
	}* /
}*/

/*ConceptInstance & ConceptInstance::operator = (const ConceptInstance & Other)
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

void ConceptInstance::Render(Vector2n Position) const
{
	GetConcept(m_ConceptId).Render(GetParameters(), Position);
}

Vector2n ConceptInstance::GetDimensions() const
{
	return GetConcept(m_ConceptId).GetDimensions(GetParameters());
}

std::string ConceptInstance::GetContent() const
{
	return GetConcept(m_ConceptId).GetContent(GetParameters());
}

void ConceptInstance::Render(const ConceptParameters & ConceptParameters, Vector2n Position) const
{
	GetConcept(m_ConceptId).Render(GetParameters() + ConceptParameters, Position);
}

Vector2n ConceptInstance::GetDimensions(const ConceptParameters & ConceptParameters) const
{
	return GetConcept(m_ConceptId).GetDimensions(GetParameters() + ConceptParameters);
}

std::string ConceptInstance::GetContent(const ConceptParameters & ConceptParameters) const
{
	return GetConcept(m_ConceptId).GetContent(GetParameters() + ConceptParameters);
}
