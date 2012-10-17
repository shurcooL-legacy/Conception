#pragma once
#ifndef __App_H__
#define __App_H__

class App
	: public InputHandler
{
public:
	App(InputManager & InputManager);
	virtual ~App();
	
	void Layout();

	virtual void UpdateWindowDimensions(Vector2n WindowDimensions) {}

	const InputManager & GetInputManager();
	InputManager & ModifyInputManager();

	virtual void Render();

	//void ProcessPosition(Pointer * Pointer, Vector2n Position);
	void ProcessEvent(InputEvent & InputEvent) override;

	void ProcessTimePassed(const double TimePassed) override;

	virtual bool ShouldRedrawRegardless();

	virtual std::string GetTitle();

protected:
	// DEBUG: Made m_Widgets public for temporary debugging purposes
	public:std::vector<std::unique_ptr<Widget>>		m_Widgets;

private:
	App(const App &) = delete;
	App & operator = (const App &) = delete;

	InputManager &		m_InputManager;
};

#endif // __App_H__
