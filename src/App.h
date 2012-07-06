#pragma once
#ifndef __App_H__
#define __App_H__

class App
	: public InputResponder
{
public:
	App(InputManager & InputManager);
	virtual ~App();
	
	void Layout();

	virtual void UpdateWindowDimensions(Vector2n WindowDimensions) {}

	InputManager & ModifyInputManager();

	void Render();

	//void ProcessPosition(Pointer * Pointer, Vector2n Position);
	bool ProcessEvent(InputEvent & InputEvent) override;

	void ProcessTimePassed(const double TimePassed) override;

protected:
	// DEBUG: Made m_Widgets public for temporary debugging purposes
	public:std::vector<std::unique_ptr<Widget>>		m_Widgets;

private:
	App(const App &) = delete;
	App & operator = (const App &) = delete;

	InputManager &		m_InputManager;
};

#endif // __App_H__
