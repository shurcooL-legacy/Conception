#pragma once
#ifndef __LabelWidget_H__
#define __LabelWidget_H__

class LabelWidget
	: public Widget
{
public:
	enum class Background : uint8 {
		None,
		Normal
	};

	LabelWidget(const Vector2n Position, const std::function<std::string()> & Content, Background Background = Background::None);
	LabelWidget(const Vector2n Position, const std::string & Content, Background Background = Background::None);
	~LabelWidget();

	void Render() override;

	bool IsHit(const Vector2n ParentPosition) const override;

private:
	std::function<std::string()> m_Content;

	Background m_Background;
};

#endif // __LabelWidget_H__
