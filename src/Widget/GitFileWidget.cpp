#include "../Main.h"

GitFileWidget::GitFileWidget(Vector2n Position, std::string Path, TypingModule & TypingModule)
	: CompositeWidget(Position, { std::shared_ptr<Widget>(m_TextFileWidget = new TextFileWidget(Vector2n::ZERO, Path, TypingModule)),
								  std::shared_ptr<Widget>(m_GitDiffWidget = new GitDiffWidget(Vector2n::ZERO, TypingModule, m_TextFileWidget)) },
								{ std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) })
{
	m_TextFileWidget->RemoveAllBehaviors();

	m_OnChange = []()
	{
		PlayBeep();
		printf("Git file changed.\n");
	};
}

GitFileWidget::~GitFileWidget()
{
}

void GitFileWidget::NotifyChange(bool OverrideLiveToggle)
{
	if (nullptr != m_OnChange) {
		m_OnChange();
	}
	NotifyExternalChange(OverrideLiveToggle);
}

void GitFileWidget::NotifyExternalChange(bool OverrideLiveToggle)
{
	for (auto ConnectionWidget : GetConnected()) {
		ConnectionWidget->NotifyChange(OverrideLiveToggle);
	}
}
