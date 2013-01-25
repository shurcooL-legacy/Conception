#pragma once
#ifndef __Concept_H__
#define __Concept_H__

typedef std::basic_string<ConceptId> ConceptString;
//typedef std::basic_string<ConceptInstance> ConceptInstanceString;
//typedef std::vector<ConceptInstance> ConceptInstanceString;

//typedef std::vector<ConceptId> ConceptParameters;
class ConceptParameters
{
private:
	//std::unique_ptr<std::vector<ConceptId>>		m_Parameters;
	std::vector<ConceptId>		m_Parameters;

public:
	ConceptParameters()
		: m_Parameters()
	{}
	ConceptParameters(const decltype(m_Parameters) & Parameters)
		: m_Parameters(Parameters)
	{}
	ConceptParameters(std::initializer_list<ConceptId> Parameters)
		: m_Parameters(Parameters)
	{}
	ConceptParameters(const ConceptParameters & Other)
		: m_Parameters(Other.m_Parameters)
	{}

	ConceptParameters operator + (const ConceptParameters & Other) const
	{
		decltype(m_Parameters) CombinedParameters(GetParameters());
		CombinedParameters.insert(CombinedParameters.end(), Other.GetParameters().begin(), Other.GetParameters().end());

		return ConceptParameters(CombinedParameters);
	}

	ConceptId GetParameterIfExists(std::vector<ConceptId>::size_type ParameterId) const
	{
		/*if (   nullptr != GetParameters()
			&& ParameterId < GetParameters()->size())
		{
			return (*GetParameters())[ParameterId];
		}*/
		if (ParameterId < GetParameters().size())
		{
			return GetParameters()[ParameterId];
		}
		else
		{
			return 0;		// Null concept
		}
	}

private:
	const decltype(m_Parameters) & GetParameters() const
	{
		//return m_Parameters.get();
		return m_Parameters;
	}
};

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

	virtual void Draw(const ConceptParameters & ConceptParameters, Vector2n Position) const { Draw(Position); }
	virtual Vector2n GetDimensions(const ConceptParameters & ConceptParameters) const { return GetDimensions(); }
	virtual std::string GetContent(const ConceptParameters & ConceptParameters) const { return GetContent(); }

	void AddLabel(ConceptId Label)
	{
		m_Labels.insert(Label);
	}

	bool HasLabel(ConceptId Label) const
	{
		return (m_Labels.end() != m_Labels.find(Label));
	}

	/*friend std::ostream & operator << (std::ostream & Out, const ConceptInstance & ConceptInstance)
	{
		Out << ConceptInstance.GetContent();

		return Out;
	}*/

	/*friend std::ostream & operator << (std::ostream & Out, const Concept & Concept)
	{
		Out << Concept.GetContent();

		return Out;
	}*/

	static const Vector2n GetDimensions(const std::string & Content);
	static const Vector2n GetDimensions(const ConceptId ConceptId);
	static const Vector2n GetDimensions(const Concept * const Concept);
	//static const Vector2n GetDimensions(const ConceptInstance & ConceptInstance);

private:
	Concept(const Concept &) = delete;
	Concept & operator = (const Concept &) = delete;

	std::string		m_HumanDescription;

	std::set<ConceptId>		m_Labels;
};

void PopulateConcepts();
void CleanConcepts();
ConceptId FindConcept(std::string Content);
ConceptId CreateConcept(std::string Content);
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

		Out << ConceptString[i];

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

/*class ConceptCompound
	: public Concept
{
public:
	ConceptCompound(std::string HumanDescription, const std::function<ConceptString(const std::vector<ConceptId> &)> & Content, const std::vector<ConceptId> & Parameters)
		: Concept(HumanDescription),
		  m_Content(Content(Parameters))
	{
	}

	void Draw(Vector2n Position) const override;
	Vector2n GetDimensions() const override;
	std::string GetContent() const override;

private:
	ConceptString		m_Content;
};*/

class ConceptParameterized
	: public Concept
{
public:
	ConceptParameterized(std::string HumanDescription, const std::function<ConceptString(const ConceptParameters &)> & Content)
		: Concept(HumanDescription),
		  m_Content(Content)
	{
	}

	void Draw(Vector2n Position) const override { Draw(EmptyParameters, Position); }
	Vector2n GetDimensions() const override { return GetDimensions(EmptyParameters); }
	std::string GetContent() const override { return GetContent(EmptyParameters); }

	void Draw(const ConceptParameters & ConceptParameters, Vector2n Position) const override;
	Vector2n GetDimensions(const ConceptParameters & ConceptParameters) const override;
	std::string GetContent(const ConceptParameters & ConceptParameters) const override;

	static ConceptParameters EmptyParameters;

private:
	std::function<ConceptString(const ConceptParameters &)>		m_Content;
};

class ConceptParameterized2
	: public Concept
{
public:
	ConceptParameterized2(std::string HumanDescription, const std::function<void(OpenGLStream &, const ConceptParameters &)> & Content)
		: Concept(HumanDescription),
		  m_Content(Content)
	{
	}

	void Draw(Vector2n Position) const override { Draw(EmptyParameters, Position); }
	Vector2n GetDimensions() const override { return GetDimensions(EmptyParameters); }
	std::string GetContent() const override { return GetContent(EmptyParameters); }

	void Draw(const ConceptParameters & ConceptParameters, Vector2n Position) const override;
	Vector2n GetDimensions(const ConceptParameters & ConceptParameters) const override;
	std::string GetContent(const ConceptParameters & ConceptParameters) const override;

	static ConceptParameters EmptyParameters;

private:
	std::function<void(OpenGLStream &, const ConceptParameters &)>		m_Content;
};

//typedef std::pair<ConceptId, std::vector<ConceptId>> ConceptInstance;
class ConceptInstance
	: public Concept
{
public:
	//ConceptInstance(ConceptId ConceptId);
	ConceptInstance(std::string HumanDescription, ConceptId ConceptId, std::initializer_list<::ConceptId> Parameters)
		: Concept(HumanDescription),
		  m_ConceptId(ConceptId),
		  //m_Parameters(std::unique_ptr<std::vector<::ConceptId>>(new std::vector<::ConceptId>(Parameters)))
		  m_Parameters(Parameters)
	{}
	//ConceptInstance(const ConceptInstance & Other);
	/*ConceptInstance & operator = (const ConceptInstance & Other);*/

	void Draw(Vector2n Position) const override;
	Vector2n GetDimensions() const override;
	std::string GetContent() const override;

	void Draw(const ConceptParameters & ConceptParameters, Vector2n Position) const override;
	Vector2n GetDimensions(const ConceptParameters & ConceptParameters) const override;
	std::string GetContent(const ConceptParameters & ConceptParameters) const override;

private:
	const ConceptParameters & GetParameters() const
	{
		return m_Parameters;
	}

	ConceptId									m_ConceptId;
	ConceptParameters							m_Parameters;
};

#endif // __Concept_H__
