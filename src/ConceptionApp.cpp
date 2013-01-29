#include "Main.h"

TextFieldWidget * volatile g_OutputWidget = nullptr;

ConceptionApp::ConceptionApp(InputManager & InputManager)
	: App(InputManager),
	  m_CurrentProject(),
	  m_TypingModule(new TypingModule())		// Gets cleaned up via unique_ptr when pushed back to m_Widgets
{
	PopulateConcepts();

	{
		auto MainCanvas = new Canvas(Vector2n(0, 0), true, true);
		//MainCanvas->MoveView(0, 336);
		MainCanvas->MoveView(1, -64);

#if 1
		{
			auto StdIncludesList = new ListWidget<ConceptId>(Vector2n::ZERO, m_CurrentProject.GetStdIncludes(), *m_TypingModule);
			StdIncludesList->m_TapAction = [=](Vector2n LocalPosition, std::vector<ConceptId> & m_List)
			{
				auto Entry = m_TypingModule->TakeString();

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
						m_TypingModule->SetString(GetConcept(m_List[ListEntry]).GetContent());
						m_List.erase(m_List.begin() + ListEntry);
					}
				}
			};

			auto LabelledStdIncludesList = new FlowLayoutWidget(Vector2n(-280, -250), { std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string("#include <"), LabelWidget::Background::None)),
																						std::shared_ptr<Widget>(StdIncludesList),
																						std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string(">"), LabelWidget::Background::None)) }, {});
			LabelledStdIncludesList->AddBehavior(std::shared_ptr<Behavior>(new DraggablePositionBehavior(*LabelledStdIncludesList)));
			MainCanvas->AddWidget(LabelledStdIncludesList);
		}
#endif

		MainCanvas->AddWidget(new ButtonWidget(Vector2n(-100, -350), []() { std::cout << "Hi from anon func.\n"; } ));
		MainCanvas->AddWidget(new ButtonWidget(Vector2n(-60, -350), []() { std::cout << "Second button.\n"; } ));
		MainCanvas->AddWidget(new ToggleWidget(Vector2n(-20, -350), [](bool State) { std::cout << "Testing this toggle widget! It's now set to " << State << ".\n"; }, true));
		MainCanvas->AddWidget(new LiveFunctionWidget(Vector2n(-100, 100), *m_TypingModule, m_CurrentProject));
		MainCanvas->AddWidget(new LiveProgramWidget(Vector2n(-100, -300), *m_TypingModule, m_CurrentProject));
		MainCanvas->AddWidget(new LiveProgramWidget(Vector2n(-100, -100), *m_TypingModule, m_CurrentProject));
		MainCanvas->AddWidget(new LiveGofmtWidget(Vector2n(-460, 200), *m_TypingModule, m_CurrentProject));
		MainCanvas->AddWidget(new TextFieldWidget(Vector2n(-460, 160), *m_TypingModule));
		MainCanvas->AddWidget(new ShellWidget(Vector2n(-460, 60), *m_TypingModule));
		MainCanvas->AddWidget(new SayWidget(Vector2n(-460, -100), *m_TypingModule));

		MainCanvas->AddWidget(new ConceptStringBoxWidget(Vector2n(-400, 100 + 400), *m_TypingModule));

		// TEST: Modify some Concept
		{
			auto Widget = new TextFieldWidget(Vector2n(-320, 470), *m_TypingModule);
			Widget->SetContent(GetConcept(47).GetContent());
			Widget->m_OnChange = [=]() {
				static_cast<ConceptBasic &>(ModifyConcept(47)).SetContentTEST(Widget->GetContent());
			};
			Widget->AddBehavior(std::shared_ptr<Behavior>(new DraggablePositionBehavior(*Widget)));
			MainCanvas->AddWidget(Widget);
		}

		// Label resizing test
		{
			auto SourceWidget = new TextFieldWidget(Vector2n::ZERO, *m_TypingModule);

			auto Content = [=]() -> std::string {
				return SourceWidget->GetContent();
			};
			auto LabelWidget = new class LabelWidget(Vector2n::ZERO, Content, LabelWidget::Background::Normal);

			MainCanvas->AddWidget(new FlowLayoutWidget(Vector2n(-100, -450), { std::shared_ptr<Widget>(SourceWidget), std::shared_ptr<Widget>(LabelWidget) }, {}));
		}

		// Time widget
		{
			auto Content = []() -> std::string {
				auto now = std::chrono::system_clock::now();

				auto duration = now.time_since_epoch();

				auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();

				return std::to_string(seconds);
			};
			auto LabelWidget = new class LabelWidget(Vector2n(360, -340), Content, LabelWidget::Background::Normal);
			LabelWidget->AddBehavior(std::shared_ptr<Behavior>(new DraggablePositionBehavior(*LabelWidget)));

			MainCanvas->AddWidget(LabelWidget);
		}

		MainCanvas->AddWidget(new TimeWidget(Vector2n(360, -360)));		// Time widget

#if 0
		// "./GenProgram.go" file contents displayed (in real-time) in this Label Widget
		{
			auto Content = []() -> std::string {
				return FromFileToString("./GenProgram.go");
			};
			auto LabelWidget = new class LabelWidget(Vector2n(-546, -186), Content, LabelWidget::Background::Normal);
			LabelWidget->AddBehavior(std::shared_ptr<Behavior>(new DraggablePositionBehavior(*LabelWidget)));

			MainCanvas->AddWidget(LabelWidget);
		}
#endif

#if 1
		{
			MainCanvas->AddWidget(new ListWidget<Concept *>(Vector2n(-730 - 450, -250), Concepts, *m_TypingModule));
		}
#endif

		m_Widgets.push_back(std::unique_ptr<Widget>(m_TypingModule));

		m_Widgets.push_back(std::unique_ptr<Widget>(MainCanvas));

		m_Widgets.push_back(std::unique_ptr<Widget>(new DebugOverlayWidget()));		// DEBUG: Print debug info
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
#if (defined(__APPLE__) && defined(__MACH__)) || defined(__linux)
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
		m_TypingModule->Render(GetInputManager());
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
	if (   false		// HACK: Since I have a TimeWidget...
		&& 0 == m_CurrentProject.m_BackgroundState
		&& glfwGetTime() >= m_CurrentProject.m_ProcessEndedTime + 1
		&& !GetInputManager().AnyActivePointers()
		&& GetInputManager().EmptyInputEventQueue())
		return false;		// If idle, don't redraw regardless of input
	else
		return true;		// If background thread is doing something, we should redraw
}
