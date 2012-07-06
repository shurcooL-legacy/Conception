#pragma once
#ifndef __ListWidget_H__
#define __ListWidget_H__

class ListWidget
	: public Widget
{
public:
	ListWidget(Vector2n Position, std::vector<ConceptId> & List);
	virtual ~ListWidget();
	
	void Render() override;

	void ProcessTap() override;
	
private:
	void UpdateDimensions();

	// TODO: Template this specific class to typename <T>
	std::vector<ConceptId>		& m_List;
};

#endif // __ListWidget_H__
