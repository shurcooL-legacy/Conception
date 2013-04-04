#include "../Main.h"

FolderListingPureWidget::FolderListingPureWidget(Vector2n Position, std::vector<std::string> & List, std::string Path)
  : MenuWidget<std::string>(Position, List),
	m_Path(Path)
{}

FolderListingPureWidget::~FolderListingPureWidget()
{}

std::string FolderListingPureWidget::GetPath() const
{
	return m_Path;
}
