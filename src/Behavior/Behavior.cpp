#include "../Main.h"

Behavior::Behavior(Widget & Widget)
	: GestureHandler(),
	  m_Widget(Widget)
{
}

Behavior::~Behavior()
{
}
