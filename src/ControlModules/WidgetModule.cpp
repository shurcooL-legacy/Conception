#include "../Main.h"

#if 0
WidgetModule::WidgetModule(WidgetManager & WidgetManager)
	: ControlModule(1, 0, 2),
	  m_WidgetManager(WidgetManager)
{
}

WidgetModule::~WidgetModule()
{
}

bool WidgetModule::ShouldActivate() const
{
	//return (IsButtonPressed(0) && m_WidgetManager.HitTest(Vector2d(GetAxisState(0).GetPosition(), GetAxisState(1).GetPosition())));
	return m_WidgetManager.HitTest(Vector2d(GetAxisState(0).GetPosition(), GetAxisState(1).GetPosition()));
}

bool WidgetModule::ShouldDeactivate() const
{
	//return (!IsButtonPressed(0));
	return (!IsButtonPressed(0) && !m_WidgetManager.HitTest(Vector2d(GetAxisState(0).GetPosition(), GetAxisState(1).GetPosition())));
}

void WidgetModule::ProcessActivation(ControlModule * Previous)
{
	//m_WidgetManager.ProcessPress(Vector2d(GetAxisState(0).GetPosition(), GetAxisState(1).GetPosition()));
	m_WidgetManager.ProcessMove(Vector2d(GetAxisState(0).GetPosition(), GetAxisState(1).GetPosition()));
}

void WidgetModule::ProcessDeactivation(ControlModule * Next)
{
	//m_WidgetManager.ProcessRelease(Vector2d(GetAxisState(0).GetPosition(), GetAxisState(1).GetPosition()));
	m_WidgetManager.ProcessDeactivation();
}

void WidgetModule::ModuleProcessButton(InputManager::VirtualInputId ButtonId, bool Pressed)
{
	if (0 == ButtonId)
	{
		if (Pressed)
		{
			m_WidgetManager.ProcessPress(Vector2d(GetAxisState(0).GetPosition(), GetAxisState(1).GetPosition()));
		}
		else
		{
			m_WidgetManager.ProcessRelease(Vector2d(GetAxisState(0).GetPosition(), GetAxisState(1).GetPosition()));
		}
	}
}

void WidgetModule::ModuleProcess2Axes(InputManager::VirtualInputId FirstAxisId, Input::AxisState AxisState[2])
{
	m_WidgetManager.ProcessMove(Vector2d(GetAxisState(0).GetPosition(), GetAxisState(1).GetPosition()));
}
#endif
