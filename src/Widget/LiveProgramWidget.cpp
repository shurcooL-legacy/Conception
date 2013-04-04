#include "../Main.h"

LiveProgramWidget::LiveProgramWidget(Vector2n Position, TypingModule & TypingModule, Project & Project)
	: FlowLayoutWidget(Position, { std::shared_ptr<Widget>(m_SourceWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)),
								   std::shared_ptr<Widget>(m_ProgramWidget = new ProgramWidget(Vector2n::ZERO, TypingModule, Project, m_SourceWidget)) }, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) })
{
	m_ProgramWidget->RemoveAllBehaviors();

	// TODO: Should I be using the ./GenProgram.go file or something else?
	m_SourceWidget->m_GetAutocompletions = GetAutocompletions("./GenProgram.go");

#if 0
#if DECISION_USE_CPP_INSTEAD_OF_GO
	m_SourceWidget->SetContent(FromFileToString("./GenProgram.cpp"));
#else
	m_SourceWidget->SetContent(FromFileToString("./GenProgram.go"));
#endif
#endif

	ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('R', PointerState::Modifiers::Super, [=]() { m_SourceWidget->NotifyChange(true); }, "Run/Refresh"));
}

LiveProgramWidget::~LiveProgramWidget()
{
}
