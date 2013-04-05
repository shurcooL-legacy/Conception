#include "../Main.h"

FolderListingPureWidget::FolderListingPureWidget(Vector2n Position, std::vector<std::string> & List, std::string Path, TypingModule & TypingModule)
  : MenuWidget<std::string>(Position, List, &TypingModule),
	m_Path(Path)
{}

FolderListingPureWidget::~FolderListingPureWidget()
{}

void FolderListingPureWidget::ProcessTimePassed(const double TimePassed)
{
	auto Now = glfwGetTime();
	if (Now >= m_NextRefresh)
	{
		m_NextRefresh = Now + 0.250;		// Refresh again after 250 ms

		auto List = Ls(GetPath());
		UpdateEntries(List);
	}

	MenuWidget<std::string>::ProcessTimePassed(TimePassed);
}

std::string FolderListingPureWidget::GetPath() const
{
	return m_Path;
}
