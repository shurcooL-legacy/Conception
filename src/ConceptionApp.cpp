#include "Main.h"

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
			auto * StdIncludesList = new ListWidget<ConceptId>(Vector2n(-200, -300), m_CurrentProject.GetStdIncludes(), m_TypingModule);
			StdIncludesList->m_TapAction = [=](Vector2n LocalPosition, std::vector<ConceptId> & m_List)
				/*{
					auto Entry = m_TypingModule.TakeString();

					if (Entry.length() > 0)
					{
						auto ConceptId = FindOrCreateConcept(Entry);

						StdIncludesList->Insert(ConceptId);
					}
					else
					{
						//auto ListEntry =
						//StdIncludesList->In
					}
				};*/
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

			MainCanvas->AddWidget(StdIncludesList);
		}
#endif
		MainCanvas->AddWidget(new ButtonWidget(Vector2n(-100, -350), []() { std::cout << "Hi from anon func.\n"; } ));
		MainCanvas->AddWidget(new ButtonWidget(Vector2n(-60, -350), []() { std::cout << "Second button.\n"; } ));
		MainCanvas->AddWidget(m_OutputWidget = new TextFieldWidget(Vector2n(200, -200), m_TypingModule));
		MainCanvas->AddWidget(m_SourceWidget = new TextFieldWidget(Vector2n(-400, -200), m_TypingModule));

		// DEBUG: Irregular starting state, for testing
		{
			m_SourceWidget->m_OnChange = [&]()
			{
				//printf("m_SourceWidget->m_OnChange\n");
				//m_OutputWidget->SetContent(m_OutputWidget->GetContent() + "+");

				m_CurrentProject.GenerateProgram(m_SourceWidget->GetContent());
				uint8 Status;
				m_OutputWidget->SetContent(m_CurrentProject.RunProgram(Status));
				if (0 == Status)
					m_OutputWidget->SetBackground(Color(1.0, 1, 1));
				else
					m_OutputWidget->SetBackground(Color(1.0, 0, 0));
			};

			//m_Content = "int main(int argc, char * argv[])\n{\n\tPrintHi();\n\treturn 0;\n}";
			m_SourceWidget->SetContent(
#if 0
				"{""\n"
				"	// Skip non-spaces to the right""\n"
				"	auto LookAt = m_CaretPosition;""\n"
				"	while (   LookAt < m_Content.length()""\n"
				"		   && IsCoreCharacter(m_Content[LookAt]))""\n"
				"	{""\n"
				"		++LookAt;""\n"
				"	}""\n"
				"""\n"
				"	SetCaretPosition(LookAt, false);""\n"
				"}"
#else
				FromFileToString("GenProgram.go")
#endif
			);
		}

		MainCanvas->AddWidget(new ConceptStringBoxWidget(Vector2n(-400, 100 + 400), m_TypingModule));

#if 1
		{
			MainCanvas->AddWidget(new ListWidget<Concept *>(Vector2n(-730 - 300, -250), Concepts, m_TypingModule));
		}
#endif

		m_Widgets.push_back(std::unique_ptr<Widget>(MainCanvas));
	}

	{
		// Load program
		m_CurrentProject.LoadSampleGenProgram(*static_cast<Canvas *>(m_Widgets[0].get()));
	}
}

ConceptionApp::~ConceptionApp()
{
	CleanConcepts();
}

void ConceptionApp::UpdateWindowDimensions(Vector2n WindowDimensions)
{
	// TODO: This is a hack, I should create a WindowResize listener type of thing and take care within Widget itself
	static_cast<Canvas *>(m_Widgets[0].get())->SetDimensions(WindowDimensions);
}

void ConceptionApp::Render()
{
	App::Render();

	// TODO, LOWER_PRIORITY: Perhaps generalize TypingModule to a Renderable object (rather than Widget) and standardize back into App, removing need for overloaded Render()
	{
		m_TypingModule.Render(GetInputManager());
	}
}

void ConceptionApp::ProcessEvent(InputEvent & InputEvent)
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
					//case GLFW_KEY_F5:
					case 'R':
						if (   InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_LSUPER)
							|| InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_RSUPER))
						{
							m_CurrentProject.GenerateProgram(m_SourceWidget->GetContent());
							uint8 Status;
							m_OutputWidget->SetContent(m_CurrentProject.RunProgram(Status));

							InputEvent.m_Handled = true;
						}
						break;
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
		if (InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::CHARACTER_EVENT))
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
