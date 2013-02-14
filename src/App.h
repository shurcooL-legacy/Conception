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

	const InputManager & GetInputManager();
	InputManager & ModifyInputManager();

	virtual void Render();

	virtual void UpdateWindowDimensions(Vector2n WindowDimensions);

	void ProcessEventQueue(InputEventQueue & InputEventQueue);
	MatchResult MatchEventQueue(InputEventQueue::FilteredQueue & UnreservedEvents);
	//void ProcessEvent(InputEvent & InputEvent) override;

	void ProcessTimePassed(const double TimePassed) override;

	virtual bool ShouldRedrawRegardless();

	virtual std::string GetTitle();

protected:
	// DEBUG: Made m_Widgets public for temporary debugging purposes
	public:std::vector<std::unique_ptr<Widget>>		m_Widgets;		// Top-most widgets are last

private:
	App(const App &) = delete;
	App & operator = (const App &) = delete;

	InputManager &		m_InputManager;
};

#endif // __App_H__
