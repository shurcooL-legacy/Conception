#include "../Main.h"

auto Content = []() -> std::string {
	auto now = std::chrono::system_clock::now();

	auto duration = now.time_since_epoch();

	typedef std::chrono::duration<double, std::ratio<3600l * 24 * 3652422, 10000>> shuryears;

	auto shuryears_now = 1970 + std::chrono::duration_cast<shuryears>(duration).count();

	//return std::to_string(out);
	std::ostringstream ss;
	ss << std::setprecision(8) << std::fixed;
	ss << shuryears_now;
	return ss.str();
};

TimeWidget::TimeWidget(Vector2n Position)
	: LabelWidget(Position, Content, LabelWidget::Background::Normal)
{
	AddBehavior(std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)));
}

TimeWidget::~TimeWidget()
{
}
