#include "Main.h"

TextFieldWidget * volatile g_OutputWidget = nullptr;

ConceptionApp::ConceptionApp(InputManager & InputManager)
	: App(InputManager),
	  m_CurrentProject(),
	  m_TypingModule()
{
	PopulateConcepts();

	{
		auto MainCanvas = new Canvas(Vector2n(0, 0), true, true);
		//MainCanvas->MoveView(0, 336);
		MainCanvas->MoveView(1, -64);

#if 1
		{
			auto StdIncludesList = new ListWidget<ConceptId>(Vector2n::ZERO, m_CurrentProject.GetStdIncludes(), m_TypingModule);
			StdIncludesList->m_TapAction = [=](Vector2n LocalPosition, std::vector<ConceptId> & m_List)
			{
				auto Entry = m_TypingModule.TakeString();

				if (!Entry.empty())
				{
					auto ConceptId = FindOrCreateConcept(Entry);

					//Insert(ConceptId);

					// TEST
					auto Spot = m_List.begin() + (LocalPosition.Y() / lineHeight);
					m_List.insert(Spot, ConceptId);
				}
				else
				{
					auto ListEntry = static_cast<decltype(m_List.size())>(LocalPosition.Y() / lineHeight);

					if (ListEntry < m_List.size())
					{
						m_TypingModule.SetString(GetConcept(m_List[ListEntry]).GetContent());
						m_List.erase(m_List.begin() + ListEntry);
					}
				}
			};

			auto LabelledStdIncludesList = new FlowLayoutWidget(Vector2n(-280, -250), { std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, "#include <", LabelWidget::Background::None)),
																						std::shared_ptr<Widget>(StdIncludesList),
																						std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, ">", LabelWidget::Background::None)) }, {});
			LabelledStdIncludesList->AddBehavior(std::shared_ptr<Behavior>(new DraggablePositionBehavior(*LabelledStdIncludesList)));
			MainCanvas->AddWidget(LabelledStdIncludesList);
		}
#endif

		MainCanvas->AddWidget(new ButtonWidget(Vector2n(-100, -350), []() { std::cout << "Hi from anon func.\n"; } ));
		MainCanvas->AddWidget(new ButtonWidget(Vector2n(-60, -350), []() { std::cout << "Second button.\n"; } ));
		MainCanvas->AddWidget(new LiveFunctionWidget(Vector2n(-100, 100), m_TypingModule, m_CurrentProject));
		MainCanvas->AddWidget(new LiveProgramWidget(Vector2n(-100, -300), m_TypingModule, m_CurrentProject));
		MainCanvas->AddWidget(new LiveProgramWidget(Vector2n(-100, -100), m_TypingModule, m_CurrentProject));

		MainCanvas->AddWidget(new ConceptStringBoxWidget(Vector2n(-400, 100 + 400), m_TypingModule));

		// Label resizing test
		{
			auto SourceWidget = new TextFieldWidget(Vector2n::ZERO, m_TypingModule);

			auto Content = [=]() -> std::string {
				return SourceWidget->GetContent();
			};
			auto LabelWidget = new class LabelWidget(Vector2n::ZERO, Content, LabelWidget::Background::Normal);

			MainCanvas->AddWidget(new FlowLayoutWidget(Vector2n(-100, -450), { std::shared_ptr<Widget>(SourceWidget), std::shared_ptr<Widget>(LabelWidget) }, {}));
		}

#if 1
		{
			MainCanvas->AddWidget(new ListWidget<Concept *>(Vector2n(-730 - 450, -250), Concepts, m_TypingModule));
		}
#endif

		m_Widgets.push_back(std::unique_ptr<Widget>(MainCanvas));

		// HACK: Fix pointer voodoo
		m_Widgets.push_back(std::unique_ptr<Widget>(&m_TypingModule));
		//m_Widgets.push_back(std::unique_ptr<Widget>(new ButtonWidget(Vector2n(10, 10), []() { std::cout << "Hi from anon func AFT!!!.\n"; } )));

		// DEBUG: Print debug info
		{
			auto OverlayCanvas = new Canvas(Vector2n(0, 0), false, false);

			{
				auto Content = []() -> std::string
				{
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
						else if (dynamic_cast<TypingModule *>(&i->GetOwner())) out << "\n TypingModule";
						else out << "\n (Unknown)";

						auto LocalPosition = dynamic_cast<Widget *>(&i->GetOwner())->GlobalToLocal(Vector2n(g_InputManager->m_MousePointer->GetPointerState().GetAxisState(0).GetPosition(), g_InputManager->m_MousePointer->GetPointerState().GetAxisState(1).GetPosition()));
						out << " (" << LocalPosition.X() << ", " << LocalPosition.Y() << ")";
					}

					return out.str();
				};

				OverlayCanvas->AddWidget(new LabelWidget(Vector2n(0, 0), Content));
			}

			{
				auto Content = []() -> std::string
				{
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
	}

	// Prepare and start the thread
	{
		m_CurrentProject.StartBackgroundThread();
	}

	{
		// Load program
		m_CurrentProject.LoadSampleGenProgram(*static_cast<Canvas *>(m_Widgets[0].get()));
	}
}

ConceptionApp::~ConceptionApp()
{
	CleanConcepts();

	// Clean up temporary files
#if defined(__APPLE__) && defined(__MACH__)
	system("rm ./GenProgram");
	system("./bin/gocode/gocode drop-cache");
	system("./bin/gocode/gocode close");
#endif
}

void ConceptionApp::Render()
{
	m_CurrentProject.SomethingFromAppRenderTEST();

	App::Render();

	// TODO, LOWER_PRIORITY: Perhaps generalize TypingModule to a Renderable object (rather than Widget) and standardize back into App, removing need for overloaded Render()
	{
		//m_TypingModule.Render(GetInputManager());
	}
}

#if 0
void ConceptionApp::ProcessEvent(InputEvent & InputEvent)
{
	// DEBUG, TEST: System key handling
	if (false == InputEvent.m_Handled)
	{
		if (InputEvent.HasType(InputEvent::EventType::BUTTON_EVENT))
		{
			if (Pointer::VirtualCategory::TYPING == InputEvent.m_Pointer->GetVirtualCategory())
			{
				auto ButtonId = InputEvent.m_InputId;
				bool Pressed = InputEvent.m_Buttons[0];		// TODO: Check if there are >1 buttons

				if (Pressed)
				{
					switch (ButtonId)
					{
					//case GLFW_KEY_F5:
					/*case 'R':
						if (   InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_LSUPER)
							|| InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_RSUPER))
						{
							/ *m_CurrentProject.GenerateProgram(m_SourceWidget->GetContent());
							m_OutputWidget->SetContent(m_CurrentProject.RunProgram(m_OutputWidget));* /
							m_SourceWidget->m_OnChange();

							InputEvent.m_Handled = true;
						}
						break;*/
					// TEST
					/*case 'B':
						//if (glfwGetKey(GLFW_KEY_LCTRL) || glfwGetKey(GLFW_KEY_RCTRL))
						{
							m_CurrentProject.GetStdIncludes().push_back(FindOrCreateConcept("test"));

							InputEvent.m_Handled = true;
						}
						break;*/
					default:
						break;
					}
				}
			}
		}
	}

	App::ProcessEvent(InputEvent);

	// TODO, LOWER_PRIORITY: Perhaps generalize and standardize this back into App, removing need for overloaded ProcessEvent()
	if (false == InputEvent.m_Handled)
	{
		if (InputEvent.HasType(InputEvent::EventType::CHARACTER_EVENT))
		{
			if (Pointer::VirtualCategory::TYPING == InputEvent.m_Pointer->GetVirtualCategory())
			{
				auto Character = InputEvent.m_InputId;

				m_TypingModule.ProcessCharacter(InputEvent, Character);
			}
		}

		m_TypingModule.ProcessEvent(InputEvent);
	}
}
#endif

bool ConceptionApp::ShouldRedrawRegardless()
{
	if (   0 == m_CurrentProject.m_BackgroundState
		&& glfwGetTime() >= m_CurrentProject.m_ProcessEndedTime + 1
		&& !GetInputManager().AnyActivePointers()
		&& GetInputManager().EmptyInputEventQueue())
		return false;		// If idle, don't redraw regardless of input
	else
		return true;		// If background thread is doing something, we should redraw
}
