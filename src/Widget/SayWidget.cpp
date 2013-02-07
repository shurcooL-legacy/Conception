#include "../Main.h"

SayWidget::SayWidget(Vector2n Position, TypingModule & TypingModule)
	: FlowLayoutWidget(Position, { std::shared_ptr<Widget>(m_InputWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)),
								   std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string("say"), LabelWidget::Background::Normal)),
								   std::shared_ptr<Widget>(m_ExecuteWidget = new ButtonWidget(Vector2n::ZERO, Vector2n(16, 16), [&](){} )) }, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) })
{
	// TEST
	m_InputWidget->SetContent("Shall we play a game?");

	{
		m_ExecuteWidget->SetAction([&]()
		{
			{
				auto Pid = fork();

				if (0 == Pid)
				{
					execl("/usr/bin/say", "/usr/bin/say", m_InputWidget->GetContent().c_str(), (char *)0);

					// TODO: Add error checking on above execl(), and do exit() in case execution reaches here
					//exit(1);		// Not needed, just in case I comment out the above
				}
				else if (-1 == Pid)
				{
					std::cerr << "Error forking.\n";
					throw 0;
				}
				else
				{
				}
			}
		});
	}

	ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('R', PointerState::Modifiers::Super, m_ExecuteWidget->GetAction()));
}

SayWidget::~SayWidget()
{
}
