#include "../Main.h"

LiveFunctionWidget::LiveFunctionWidget(Vector2n Position, TypingModule & TypingModule, Project & Project)
	: CompositeWidget(Position, {
		std::shared_ptr<Widget>(m_ToggleWidget = new ToggleWidget(Vector2n(-1, -18), [=](bool State) { m_LiveProgramWidget->m_SourceWidget->m_Visible = State; }, true)),
		std::shared_ptr<Widget>(new FlowLayoutWidget(Vector2n::ZERO, { std::shared_ptr<Widget>(m_InputWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)),
								   std::shared_ptr<Widget>(m_SourceWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)),
								   std::shared_ptr<Widget>(m_LiveProgramWidget = new LiveProgramWidget(Vector2n::ZERO, TypingModule, Project)) }, { }))

	}, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) })
{
	m_LiveProgramWidget->RemoveAllBehaviors();

	// With live test execution and all
	m_SourceWidget->m_OnChange = [=, &Project]()
	{
		Project.GenerateProgramForFunction(m_InputWidget->GetContent(), m_SourceWidget->GetContent());
		m_LiveProgramWidget->m_SourceWidget->SetContent(FromFileToString("./GenProgram.go"));
	};

	m_InputWidget->m_OnChange = m_SourceWidget->m_OnChange;

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
		static_cast<FlowLayoutWidget *>(GetWidgets()[1].get())->AddWidget(LabelledImportList);
	}
}

LiveFunctionWidget::~LiveFunctionWidget()
{
}
