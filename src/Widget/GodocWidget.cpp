#include "../Main.h"

GodocWidget::GodocWidget(Vector2n Position, TypingModule & TypingModule, TextFieldWidget * Target)
  : FlowLayoutWidget(Position, {
		std::shared_ptr<Widget>(m_SourceWidget = new ConnectionWidget<TextFieldWidget>(Vector2n::ZERO, Target)),
		std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string("godoc"), LabelWidget::Background::Normal)),
		std::shared_ptr<Widget>(m_OutputWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule))
	}, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) }),
	m_ShellWidget(new ShellWidget(Vector2n::ZERO, TypingModule))
{
	m_ShellWidget->m_CommandWidget->SetContent("IN=`cat`\n\
OUT=`echo -n \"$IN\" | xargs godoc 2>&1`\n\
if [ $? -eq 0 ]\n\
then\n\
	echo -n \"$OUT\"\n\
else\n\
	echo -n \"$IN\" | xargs godoc -q | head -n 1000\n\
fi");

	m_SourceWidget->m_OnChange = [&]()
	{
		std::string Out = "";

		if (nullptr != m_SourceWidget->Target())
		{
			m_ShellWidget->m_StdInWidget->SetContent(m_SourceWidget->Target()->GetContent());
			m_ShellWidget->m_ExecuteWidget->GetAction()();
			Out = m_ShellWidget->m_OutputWidget->GetContent();
		}

		m_OutputWidget->SetContent(Out);
	};

	m_SourceWidget->m_OnChange();

	// TODO: Make this work in LiveGodocWidget too (currently the Cmd+R event doesn't go from LiveGodocWidget to this GodocWidget)
	ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('R', PointerState::Modifiers::Super, m_SourceWidget->m_OnChange, "Run/Refresh"));
}

GodocWidget::~GodocWidget()
{
}
