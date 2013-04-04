#pragma once
#ifndef __FolderListingPureWidget_H__
#define __FolderListingPureWidget_H__

class FolderListingPureWidget
  : public MenuWidget<std::string>,
	public MutuallyConnectable<FolderListingPureWidget, ConnectionWidget<FolderListingPureWidget>>
{
public:
	FolderListingPureWidget(Vector2n Position, std::vector<std::string> & List, std::string Path);
	~FolderListingPureWidget();

	std::string GetPath() const;

private:
	FolderListingPureWidget(const FolderListingPureWidget &) = delete;
	FolderListingPureWidget & operator = (const FolderListingPureWidget &) = delete;

	std::string m_Path;
};

#endif // __FolderListingPureWidget_H__
