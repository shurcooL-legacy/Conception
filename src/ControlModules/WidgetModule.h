#pragma once
#ifndef __WidgetModule_H__
#define __WidgetModule_H__

#if 0
class WidgetModule : public ControlModule
{
public:
	WidgetModule(WidgetManager & WidgetManager);
	virtual ~WidgetModule();

	virtual bool ShouldActivate() const;
	virtual bool ShouldDeactivate() const;
	//virtual bool ShouldMouseCursorVisible() { return false; }

	virtual void ProcessActivation(ControlModule * Previous);
	virtual void ProcessDeactivation(ControlModule * Next);

protected:
	virtual void ModuleProcessButton(InputManager::VirtualInputId ButtonId, bool Pressed);
	virtual void ModuleProcess2Axes(InputManager::VirtualInputId FirstAxisId, Input::AxisState AxisState[2]);

private:
	WidgetModule(const WidgetModule &);
	WidgetModule & operator =(const WidgetModule &);

	WidgetManager & m_WidgetManager;
};
#endif

#endif // __WidgetModule_H__
