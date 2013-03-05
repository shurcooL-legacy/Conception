#pragma once
#ifndef __FlowLayoutWidget_H__
#define __FlowLayoutWidget_H__

class FlowLayoutWidget
	: public CompositeWidget
{
public:
	FlowLayoutWidget(Vector2n Position, std::initializer_list<std::shared_ptr<Widget>> Widgets, std::vector<std::shared_ptr<Behavior>> Behaviors);
	FlowLayoutWidget(Vector2n Position, Vector2n Dimensions, std::initializer_list<std::shared_ptr<Widget>> Widgets, std::vector<std::shared_ptr<Behavior>> Behaviors);
	~FlowLayoutWidget();

	void Render() override;

private:
	FlowLayoutWidget(const FlowLayoutWidget &) = delete;
	FlowLayoutWidget & operator = (const FlowLayoutWidget &) = delete;
};

#endif // __FlowLayoutWidget_H__
