#include "Main.h"

MultitouchTestApp::MultitouchTestApp(InputManager & InputManager)
	: App(InputManager)
{
	{
#if 0
		auto MainCanvas = new CanvasWidget(Vector2n(300, 0), true, true);
		MainCanvas->m_BlackBackgroundTEST = true;

		MainCanvas->AddWidget(new MultitouchTestBoxWidget(Vector2n(-200 - 5, -200 - 5)));
		MainCanvas->AddWidget(new MultitouchTestBoxWidget(Vector2n(0 + 5, -200 - 5)));
		MainCanvas->AddWidget(new MultitouchTestBoxWidget(Vector2n(-200 - 5, 0 + 5)));
		MainCanvas->AddWidget(new MultitouchTestBoxWidget(Vector2n(0 + 5, 0 + 5)));
		MainCanvas->AddWidget(new MultitouchTestBoxWidget(Vector2n(100 + 5, 100 + 5)));

		m_Widgets.push_back(std::unique_ptr<Widget>(MainCanvas));		// MainCanvas
#else
		/// CanvasWidget inside CanvasWidget test

		auto OuterCanvasTEST = new CanvasWidget(Vector2n(0, 0), true, true);

		auto MainCanvas = new CanvasWidget(Vector2n(-100, -340), true, true);
		MainCanvas->m_BlackBackgroundTEST = true;

		OuterCanvasTEST->AddWidget(MainCanvas);

		MainCanvas->AddWidget(new MultitouchTestBoxWidget(Vector2n(-200 - 5, -200 - 5)));
		MainCanvas->AddWidget(new MultitouchTestBoxWidget(Vector2n(0 + 5, -200 - 5)));
		MainCanvas->AddWidget(new MultitouchTestBoxWidget(Vector2n(-200 - 5, 0 + 5)));
		MainCanvas->AddWidget(new MultitouchTestBoxWidget(Vector2n(0 + 5, 0 + 5)));
		MainCanvas->AddWidget(new MultitouchTestBoxWidget(Vector2n(100 + 5, 100 + 5)));

		OuterCanvasTEST->AddWidget(new MultitouchTestBoxWidget(Vector2n(-300, 250)));

		MainCanvas->SetDimensions(Vector2n(500, 500));
		m_Widgets.push_back(std::unique_ptr<Widget>(OuterCanvasTEST));

		m_Widgets.push_back(std::unique_ptr<Widget>(new DebugOverlayWidget()));		// DEBUG: Print debug info
#endif
	}
}

MultitouchTestApp::~MultitouchTestApp()
{
}
