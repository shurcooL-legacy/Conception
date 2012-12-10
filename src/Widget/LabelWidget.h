#pragma once
#ifndef __LabelWidget_H__
#define __LabelWidget_H__

class LabelWidget
	: public Widget
{
public:
	LabelWidget(const Vector2n Position, const std::function<std::string()> & Content);
	virtual ~LabelWidget();

	void Render() override;

private:
	std::function<std::string()> m_Content;
};

#endif // __LabelWidget_H__
