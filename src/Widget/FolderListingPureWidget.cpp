#include "../Main.h"

FolderListingPureWidget::FolderListingPureWidget(Vector2n Position, std::vector<std::string> & List, std::string Path, TypingModule & TypingModule)
  : MenuWidget<std::string>(Position, List, &TypingModule),
	m_Path(Path)
{}

FolderListingPureWidget::~FolderListingPureWidget()
{}

std::string FolderListingPureWidget::GetPath() const
{
	return m_Path;
}
