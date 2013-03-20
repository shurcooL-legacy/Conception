#pragma once
#ifndef __FlowLayoutWidget_H__
#define __FlowLayoutWidget_H__

class FlowLayoutWidget
	: public CompositeWidget
{
public:
	enum class LayoutType : uint8 { Horizontal, Vertical };

	FlowLayoutWidget(Vector2n Position, std::initializer_list<std::shared_ptr<Widget>> Widgets, std::vector<std::shared_ptr<Behavior>> Behaviors, LayoutType Layout = LayoutType::Horizontal);
	FlowLayoutWidget(Vector2n Position, Vector2n Dimensions, std::initializer_list<std::shared_ptr<Widget>> Widgets, std::vector<std::shared_ptr<Behavior>> Behaviors, LayoutType Layout);
	~FlowLayoutWidget();

	void Render() override;

private:
	FlowLayoutWidget(const FlowLayoutWidget &) = delete;
	FlowLayoutWidget & operator = (const FlowLayoutWidget &) = delete;

	LayoutType		m_Layout;
};

#endif // __FlowLayoutWidget_H__
