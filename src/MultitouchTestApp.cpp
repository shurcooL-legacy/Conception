#include "Main.h"

MultitouchTestApp::MultitouchTestApp(InputManager & InputManager)
	: App(InputManager)
{
	{
#if 0
		auto MainCanvas = new Canvas(Vector2n(300, 0), true, true);
		MainCanvas->m_BlackBackgroundTEST = true;

		MainCanvas->AddWidget(new MultitouchTestBoxWidget(Vector2n(-200 - 5, -200 - 5)));
		MainCanvas->AddWidget(new MultitouchTestBoxWidget(Vector2n(0 + 5, -200 - 5)));
		MainCanvas->AddWidget(new MultitouchTestBoxWidget(Vector2n(-200 - 5, 0 + 5)));
		MainCanvas->AddWidget(new MultitouchTestBoxWidget(Vector2n(0 + 5, 0 + 5)));
		MainCanvas->AddWidget(new MultitouchTestBoxWidget(Vector2n(100 + 5, 100 + 5)));

		m_Widgets.push_back(std::unique_ptr<Widget>(MainCanvas));		// MainCanvas
#else
		/// Canvas inside Canvas test

		auto OuterCanvasTEST = new Canvas(Vector2n(0, 0), true, true);

		auto MainCanvas = new Canvas(Vector2n(-100, -340), true, true);
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

		// DEBUG: Print debug info
		{
			auto OverlayCanvas = new Canvas(Vector2n(0, 0), false, false);

			{
				auto Content = []() -> std::string {
					std::ostringstream out;

					out << "Mouse.PntrMppng.m_Entries.size(): " << g_InputManager->m_MousePointer->ModifyPointerMapping().m_Entries.size();
					for (auto & i : g_InputManager->m_MousePointer->ModifyPointerMapping().m_Entries)
					{
						if (dynamic_cast<Canvas *>(&i->GetOwner())) out << "\n Canvas";
						else if (dynamic_cast<MultitouchTestBoxWidget *>(&i->GetOwner())) out << "\n MultitouchTestBoxWidget, color: " << static_cast<uint16>(static_cast<MultitouchTestBoxWidget *>(&i->GetOwner())->m_Color);
						else if (dynamic_cast<TextFieldWidget *>(&i->GetOwner())) out << "\n TextFieldWidget";
						else if (dynamic_cast<ButtonWidget *>(&i->GetOwner())) out << "\n ButtonWidget";
						else if (dynamic_cast<ListWidget<ConceptId> *>(&i->GetOwner())) out << "\n ListWidget<ConceptId>";
						else if (dynamic_cast<LiveProgramWidget *>(&i->GetOwner())) out << "\n LiveProgramWidget";
						else out << "\n (Unknown)";

						auto LocalPosition = dynamic_cast<Widget *>(&i->GetOwner())->GlobalToLocal(Vector2n(g_InputManager->m_MousePointer->GetPointerState().GetAxisState(0).GetPosition(), g_InputManager->m_MousePointer->GetPointerState().GetAxisState(1).GetPosition()));
						out << " (" << LocalPosition.X() << ", " << LocalPosition.Y() << ")";
					}

					return out.str();
				};

				OverlayCanvas->AddWidget(new LabelWidget(Vector2n(0, 0), Content));
			}

			{
				auto Content = []() -> std::string {
					std::ostringstream out;

					out << "InputManager.m_IEQueue.m_Queue" << std::endl;
					for (auto & i : g_InputManager->m_InputEventQueue.m_Queue)
					{
						out << i.ToString() << std::endl;
					}

					return out.str();
				};

				OverlayCanvas->AddWidget(new LabelWidget(Vector2n(0, 140), Content));
			}

			m_Widgets.push_back(std::unique_ptr<Widget>(OverlayCanvas));
		}
#endif
	}
}

MultitouchTestApp::~MultitouchTestApp()
{
}

void MultitouchTestApp::UpdateWindowDimensions(Vector2n WindowDimensions)
{
	// TODO: This is a hack, I should create a WindowResize listener type of thing and take care within Widget itself
	static_cast<Canvas *>(m_Widgets[0].get())->SetDimensions(WindowDimensions);
	static_cast<Canvas *>(m_Widgets[1].get())->SetDimensions(WindowDimensions);
}
