#include "Main.h"

SentienceApp::SentienceApp(InputManager & InputManager)
	: App(InputManager)
{
	{
		auto MainCanvas = new Canvas(Vector2n(0, 0), true, true);

		MainCanvas->AddWidget(new ButtonWidget(Vector2n(100, -300)));
		MainCanvas->AddWidget(new ButtonWidget(Vector2n(140, -300)));
		
		MainCanvas->AddWidget(new LifeFormWidget(Vector2n(0, 0)));

		m_Widgets.push_back(std::unique_ptr<Widget>(MainCanvas));
	}
}

SentienceApp::~SentienceApp()
{
}

void SentienceApp::UpdateWindowDimensions(Vector2n WindowDimensions)
{
	// TODO: This is a hack, I should create a WindowResize listener type of thing and take care within Widget itself
	static_cast<Canvas *>(m_Widgets[0].get())->SetDimensions(WindowDimensions);
}

bool SentienceApp::ProcessEvent(InputEvent & InputEvent)
{
	// DEBUG, TEST: System key handling
	if (false == InputEvent.m_Handled)
	{
		if (InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::BUTTON_EVENT))
		{
			if (Pointer::VirtualCategory::TYPING == InputEvent.m_Pointer->GetVirtualCategory())
			{
				auto ButtonId = InputEvent.m_InputId;
				bool Pressed = InputEvent.m_Buttons[0];		// TODO: Check if there are >1 buttons

				if (Pressed)
				{
					switch (ButtonId)
					{
					default:
						break;
					}
				}
			}
		}
	}

	return App::ProcessEvent(InputEvent);
}
