#include "../Main.h"

LiveFunctionWidget::LiveFunctionWidget(Vector2n Position, TypingModule & TypingModule, Project & Project)
	: FlowLayoutWidget(Position, { std::shared_ptr<Widget>(m_InputWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)),
								   std::shared_ptr<Widget>(m_SourceWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)),
								   std::shared_ptr<Widget>(m_OutputWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)) }, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) })
{
#if 0
	m_Project.SetFunctionOnChange(*m_InputWidget, *m_SourceWidget, *m_OutputWidget);
#else
	m_SourceWidget->m_OnChange = [&]()
	{
		Project.GenerateProgramForFunction(m_InputWidget->GetContent(), m_SourceWidget->GetContent());
		m_OutputWidget->SetContent(FromFileToString("./GenProgram.go"));
	};

	m_InputWidget->m_OnChange = m_SourceWidget->m_OnChange;
#endif

	{
		auto ImportList = new ListWidget<ConceptId>(Vector2n::ZERO, Project.GetStdIncludes(), TypingModule);
		auto pTypingModule = &TypingModule;
		ImportList->m_TapAction = [=](Vector2n LocalPosition, std::vector<ConceptId> & m_List)
		{
			auto Entry = pTypingModule->TakeString();

			if (!Entry.empty())
			{
				auto ConceptId = FindOrCreateConcept(Entry);

				//Insert(ConceptId);

				// TEST
				auto Spot = m_List.begin() + (LocalPosition.Y() / lineHeight);
				m_List.insert(Spot, ConceptId);
			}
			else
			{
				auto ListEntry = static_cast<decltype(m_List.size())>(LocalPosition.Y() / lineHeight);

				if (ListEntry < m_List.size())
				{
					pTypingModule->SetString(GetConcept(m_List[ListEntry]).GetContent());
					m_List.erase(m_List.begin() + ListEntry);
				}
			}
		};

		auto LabelledImportList = new FlowLayoutWidget(Vector2n(-280, -100), { std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string("import (\""), LabelWidget::Background::None)),
																			   std::shared_ptr<Widget>(ImportList),
																			   std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string("\")"), LabelWidget::Background::None)) }, {});
		AddWidget(LabelledImportList);
	}
}

LiveFunctionWidget::~LiveFunctionWidget()
{
}
