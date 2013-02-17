#include "../Main.h"

GofmtWidget::GofmtWidget(Vector2n Position, TypingModule & TypingModule, Project & Project)
	: FlowLayoutWidget(Position, { std::shared_ptr<Widget>(m_SourceWidget = new ConnectionWidget<TextFieldWidget>(Vector2n::ZERO, nullptr)),
								   std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string("gofmt"), LabelWidget::Background::Normal)),
								   std::shared_ptr<Widget>(m_OutputWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)) }, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) })
{
	{
		auto Action = [&]()
		{
			std::string InOut = "";

			if (nullptr != m_SourceWidget->Target())
			{
				InOut = m_SourceWidget->Target()->GetContent();

				// gofmt
				auto Op = &Gofmt;
				Op(InOut);

				// Trim last newline, if there is one
				Op = &TrimLastNewline;
				Op(InOut);
			}

			m_OutputWidget->SetContent(InOut);
		};

		ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('R', PointerState::Modifiers::Super, Action));
	}
}

GofmtWidget::~GofmtWidget()
{
}
