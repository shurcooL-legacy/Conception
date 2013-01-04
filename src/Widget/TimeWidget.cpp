#include "../Main.h"

auto Content = []() -> std::string {
	auto now = std::chrono::system_clock::now();

	auto duration = now.time_since_epoch();

	typedef std::chrono::duration<double, std::ratio<3600 * 24 * 365>> years;

	auto out = 1970 + std::chrono::duration_cast<years>(duration).count();

	//return std::to_string(out);
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(8);
	ss << out;
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
