#pragma once
#ifndef __LabelWidget_H__
#define __LabelWidget_H__

class LabelWidget
	: public Widget
{
public:
	LabelWidget(Vector2n Position, std::string Content);
	~LabelWidget();

	void Render() override;

private:
	std::string		m_Content;
};

#endif // __LabelWidget_H__
