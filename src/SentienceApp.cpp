#include "Main.h"

SentienceApp::SentienceApp(InputManager & InputManager)
	: App(InputManager)
{
	{
		auto MainCanvas = new Canvas(Vector2n(0, 0), true, true);
		
		auto * lf = new LifeFormWidget(Vector2n(0, 0));
		MainCanvas->AddWidget(lf);

		MainCanvas->AddWidget(new ButtonWidget(Vector2n(100, -300), [=]() { InputEvent InputEvent; lf->ProcessTap(InputEvent, Vector2n(0, 0)); } ));

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

void SentienceApp::ProcessEvent(InputEvent & InputEvent)
{
	App::ProcessEvent(InputEvent);
}
