#include "Main.h"

TextFieldWidget * volatile g_OutputWidget = nullptr;

ConceptionApp::ConceptionApp(InputManager & InputManager)
	: App(InputManager),
	  m_CurrentProject(),
	  m_TypingModule(new TypingModule())		// Gets cleaned up via unique_ptr when pushed back to m_Widgets
{
	PopulateConcepts();

	{
		auto MainCanvas = new CanvasWidget(Vector2n(0, 0), true, true, CanvasWidget::BehaviourScrolling::Zooming, m_TypingModule, &m_CurrentProject);
		//MainCanvas->MoveView(0, 336);
		MainCanvas->MoveView(1, -64);

#if 0
		// DEBUG: Focus on one widget at a time
		{
			MainCanvas->AddWidget(new LiveProgramFileWidget(Vector2n(-100, -300), "./GoLand/src/TestProgram.go", *m_TypingModule, m_CurrentProject));
			m_Widgets.push_back(std::unique_ptr<Widget>(MainCanvas));
			m_Widgets.push_back(std::unique_ptr<Widget>(new DebugOverlayWidget(MainCanvas)));		// DEBUG: Print debug info
			goto DebugSkipOtherWidgets;
		}
#endif

#if 1
		{
			auto StdIncludesList = new ListWidget<ConceptId>(Vector2n::ZERO, m_CurrentProject.GetStdIncludes(), *m_TypingModule);
			StdIncludesList->m_TapAction = [=](Vector2n LocalPosition, std::vector<ConceptId> & Entries)
			{
				auto Entry = m_TypingModule->TakeString();

				if (!Entry.empty())
				{
					auto ConceptId = FindOrCreateConcept(Entry);

					//Insert(ConceptId);

					// TEST
					auto Spot = Entries.begin() + (LocalPosition.Y() / lineHeight);
					Entries.insert(Spot, ConceptId);
				}
				else
				{
					auto ListEntry = static_cast<decltype(Entries.size())>(LocalPosition.Y() / lineHeight);

					if (ListEntry < Entries.size())
					{
						m_TypingModule->SetString(GetConcept(Entries[ListEntry]).GetContent());
						Entries.erase(Entries.begin() + ListEntry);
					}
				}
			};

			auto LabelledStdIncludesList = new FlowLayoutWidget(Vector2n(-280, -250), {
				std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string("#include <"), LabelWidget::Background::None)),
				std::shared_ptr<Widget>(StdIncludesList),
				std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string(">"), LabelWidget::Background::None))
			}, {});
			LabelledStdIncludesList->AddBehavior(new DraggablePositionBehavior(*LabelledStdIncludesList));
			MainCanvas->AddWidget(LabelledStdIncludesList);
		}
#endif

		MainCanvas->AddWidget(new ConceptStringBoxWidget(Vector2n(-400, 100 + 400), *m_TypingModule));
		MainCanvas->AddWidget(new ButtonWidget(Vector2n(-200, -350), []() { std::cout << "Hi from anon func.\n"; } ));
		MainCanvas->AddWidget(new ButtonWidget(Vector2n(-160, -350), []() { std::cout << "Second button.\n"; }, "Button Label" ));
		MainCanvas->AddWidget(new ToggleWidget(Vector2n(-20, -350), [](bool State) { std::cout << "Testing this toggle widget! It's now set to " << State << ".\n"; }, true));
		MainCanvas->AddWidget(new TankWidget(Vector2n(40, -350)));
		MainCanvas->AddWidget(new TankWidget(Vector2n(80, -350)));
		for (auto i = 0; i < 3; ++i)
			MainCanvas->AddWidget(new ProgramWidget(Vector2n(-200, 100), *m_TypingModule, m_CurrentProject));
		MainCanvas->AddWidget(new LiveFunctionWidget(Vector2n(-100, 160), *m_TypingModule, m_CurrentProject));
		MainCanvas->AddWidget(new LiveCodeWidget(Vector2n(-100, 220), *m_TypingModule, m_CurrentProject));
		MainCanvas->AddWidget(new LiveProgramWidget(Vector2n(-270, -300), *m_TypingModule, m_CurrentProject));
		MainCanvas->AddWidget(new LiveProgramWidget(Vector2n(-270, -100), *m_TypingModule, m_CurrentProject));
		AddWidgetForPath("./GoLand/src/TestProgram2.go", *MainCanvas, *m_TypingModule, m_CurrentProject, Vector2n(-100, -100));
		AddWidgetForPath("./GoLand/src/TestProgram.go", *MainCanvas, *m_TypingModule, m_CurrentProject, Vector2n(-100, -300));
		MainCanvas->AddWidget(new ShellWidget(Vector2n(-460, 60), *m_TypingModule));
		MainCanvas->AddWidget(new SayWidget(Vector2n(-560, -100), *m_TypingModule));
		MainCanvas->AddWidget(new GitStatusWidget(Vector2n(-380, 280), *m_TypingModule));
		MainCanvas->AddWidget(new GitDiffWidget(Vector2n(-380, 300), *m_TypingModule));

		// LiveGodocWidget
		{
			auto SourceWidget = new TextFieldWidget(Vector2n::ZERO, *m_TypingModule);
			auto GodocWidget = new class GodocWidget(Vector2n::ZERO, *m_TypingModule, SourceWidget);
			GodocWidget->RemoveAllBehaviors();

			auto LiveGodocWidget = new FlowLayoutWidget(Vector2n(400, -400), {
				std::shared_ptr<Widget>(SourceWidget),
				std::shared_ptr<Widget>(GodocWidget)
			}, {});
			LiveGodocWidget->AddBehavior(new DraggablePositionBehavior(*LiveGodocWidget));
			MainCanvas->AddWidget(LiveGodocWidget);
		}

		// LiveGodoc2Widget
		{
			auto SourceWidget = new TextFieldWidget(Vector2n::ZERO, *m_TypingModule);
			auto Godoc2Widget = new class Godoc2Widget(Vector2n::ZERO, *m_TypingModule, SourceWidget);
			Godoc2Widget->RemoveAllBehaviors();

			auto LiveGodoc2Widget = new FlowLayoutWidget(Vector2n(400, -380), {
				std::shared_ptr<Widget>(SourceWidget),
				std::shared_ptr<Widget>(Godoc2Widget)
			}, {});
			LiveGodoc2Widget->AddBehavior(new DraggablePositionBehavior(*LiveGodoc2Widget));
			MainCanvas->AddWidget(LiveGodoc2Widget);
		}

		// LiveGoForcedUseWidget
		{
			auto SourceWidget = new TextFieldWidget(Vector2n::ZERO, *m_TypingModule);
			auto GoForcedUseWidget = new class GoForcedUseWidget(Vector2n::ZERO, *m_TypingModule, SourceWidget);
			GoForcedUseWidget->RemoveAllBehaviors();

			auto LiveGoForcedUseWidget = new FlowLayoutWidget(Vector2n(400, -360), {
				std::shared_ptr<Widget>(SourceWidget),
				std::shared_ptr<Widget>(GoForcedUseWidget)
			}, {});
			LiveGoForcedUseWidget->AddBehavior(new DraggablePositionBehavior(*LiveGoForcedUseWidget));
			MainCanvas->AddWidget(LiveGoForcedUseWidget);
		}

		// Gists Summary Widget
		{
			auto X = new TextFieldWidget(Vector2n::ZERO, *m_TypingModule);

			auto MinimizeToggle = new ToggleWidget(Vector2n::ZERO, Vector2n(12, 12), [X](bool State) { if (!State) g_InputManager->RequestTypingPointer(*static_cast<GestureRecognizer *>(nullptr)); else {
				auto Shell = std::unique_ptr<ShellWidget>(new ShellWidget(Vector2n::ZERO, *static_cast<TypingModule *>(nullptr)));
				Shell->m_CommandWidget->SetContent("goe --quiet 'gist.github.com/5645828.git' 'PrintPackageSummariesInDir(\"gist.github.com\")'");
				Shell->m_ExecuteWidget->GetAction()();
				X->SetContent(Shell->m_OutputWidget->GetContent());
			} }, false);
			X->m_MinimizeToggle = MinimizeToggle;

			auto Y = new FlowLayoutWidget(Vector2n(400, -340), {
				std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string("Gists"), LabelWidget::Background::Normal)),
				std::shared_ptr<Widget>(MinimizeToggle),
				std::shared_ptr<Widget>(X)
			}, {});
			Y->AddBehavior(new DraggablePositionBehavior(*Y));
			MainCanvas->AddWidget(Y);
		}

		// TEST: Goal-driven small task interface (inputs/code/actual output/desired output)
		{
			static std::vector<std::string> vA({"(string)(\"Abcde\")", "(string)(\"Abc zzz\")"}),
											vC({"Error", "Error"}),
											vD({"(string)(\"edcbA\")", "(string)(\"zzz cbA\")"});

			auto A = new ListWidget<std::string>(Vector2n::ZERO, vA, *m_TypingModule);
			A->m_TapAction = [=](Vector2n LocalPosition, std::vector<std::string> & Entries)
			{
				auto Entry = m_TypingModule->TakeString();

				if (!Entry.empty())
				{
					// TEST
					auto Spot = Entries.begin() + (LocalPosition.Y() / lineHeight);
					Entries.insert(Spot, Entry);
				}
				else
				{
					auto ListEntry = static_cast<decltype(Entries.size())>(LocalPosition.Y() / lineHeight);

					if (ListEntry < Entries.size())
					{
						m_TypingModule->SetString(Entries[ListEntry]);
						Entries.erase(Entries.begin() + ListEntry);
					}
				}
			};
			auto B = new TextFieldWidget(Vector2n::ZERO, *m_TypingModule);
			auto C = new ListWidget<std::string>(Vector2n::ZERO, vC, *m_TypingModule);
			C->m_TapAction = A->m_TapAction;
			auto D = new ListWidget<std::string>(Vector2n::ZERO, vD, *m_TypingModule);
			D->m_TapAction = A->m_TapAction;

			auto Y = new FlowLayoutWidget(Vector2n(400, -320), {
				std::shared_ptr<Widget>(A),
				std::shared_ptr<Widget>(B),
				std::shared_ptr<Widget>(C),
				std::shared_ptr<Widget>(D)
			}, {});
			Y->AddBehavior(new DraggablePositionBehavior(*Y));
			MainCanvas->AddWidget(Y);
		}

		{
			auto FlowLayout = new FlowLayoutWidget(Vector2n(-200, -400), {}, {});
			auto Username = new TextFieldWidget(Vector2n::ZERO, *m_TypingModule); FlowLayout->AddWidget(Username);
			auto Password = new TextFieldWidget(Vector2n::ZERO, *m_TypingModule); FlowLayout->AddWidget(Password);
			Password->m_Private = true;
			auto Action = [=]()
			{
				// Create a gist
				auto Shell = std::unique_ptr<ShellWidget>(new ShellWidget(Vector2n::ZERO, *m_TypingModule));
				Shell->m_CommandWidget->SetContent("curl -d '{\"public\":true,\"files\":{\"main.go\":{\"content\":\"package main\\n\\nimport (\\n)\\n\\nfunc main() {\\n}\"}}}' 'https://api.github.com/gists' --config - 2> /dev/null");
				Shell->m_StdInWidget->SetContent("-u \"" + Username->GetContent() + ":" + Password->GetContent() + "\"");
				Shell->m_ExecuteWidget->GetAction()();
				Shell->m_StdInWidget->SetContent("");

				std::string GistId;
				try {
					GistId = ParseGistId(Shell->m_OutputWidget->GetContent());
					std::cerr << "Gist created, id = " << GistId << endl;
				} catch (int) {
					std::cerr << "Error creating gist!\n" << Shell->m_OutputWidget->GetContent() << endl;
					return;
				}

				// Clone the gist repo
				std::string Command = "cd \"./GoLand\"";
				Command += "\nmkdir -p \"./src/gist.github.com\"";
				Command += "\ncd \"./src/gist.github.com\"";
				Command += "\ngit clone https://gist.github.com/" + GistId + ".git \"./" + GistId + ".git\"";
				Shell->m_CommandWidget->SetContent(Command);
				Shell->m_ExecuteWidget->GetAction()();
				std::cerr << "Cloned gist successfully!\n" << Shell->m_OutputWidget->GetContent() << endl;

				// Open it in a new LiveProgramFileWidget
				const auto FullPath = "./GoLand/src/gist.github.com/" + GistId + ".git/main.go";
				AddWidgetForPath(FullPath, *MainCanvas, *m_TypingModule, m_CurrentProject);

				// Place import statement in the typing module, if it's empty...
				if (m_TypingModule->GetString().empty()) {
					const auto ImportStatement = ". \"gist.github.com/" + GistId + ".git\"";
					m_TypingModule->SetString(ImportStatement);
				}
			};
			FlowLayout->AddWidget(new ButtonWidget(Vector2n::ZERO, Action, "+ Gist"));
			MainCanvas->AddWidget(FlowLayout);
		}

		for (auto i = 0; i < 5; ++i) {
			auto DraggableTextField = new TextFieldWidget(Vector2n(-460, 160), *m_TypingModule);
			DraggableTextField->AddBehavior(new DraggablePositionBehavior(*DraggableTextField));
			MainCanvas->AddWidget(DraggableTextField);
		}
		MainCanvas->AddWidget(new LiveGofmtWidget(Vector2n(-460, 200), *m_TypingModule));
		MainCanvas->AddWidget(new GofmtWidget(Vector2n(-460, 240), *m_TypingModule));
		MainCanvas->AddWidget(new LiveRubyWidget(Vector2n(-270, 200), *m_TypingModule));
		MainCanvas->AddWidget(new RubyWidget(Vector2n(-270, 240), *m_TypingModule));

		for (auto i = 0; i < 3; ++i)
			MainCanvas->AddWidget(new DiffWidget(Vector2n(-450, 340), *m_TypingModule));

		// TEST: Connection
		if (false)
		{
			auto Connection = new ConnectionWidget<TextFieldWidget>(Vector2n(0, 0));

			auto Content = [Connection]() -> std::string {
				if (nullptr != Connection->Target())
					return Connection->Target()->GetContent();
				else
					return "<Unconnected>";
			};
			auto Label = new class LabelWidget(Vector2n(16, 0), Content, LabelWidget::Background::Normal);

			auto FlowLayout = new FlowLayoutWidget(Vector2n(0, 0), {
				std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string("Blah blah"))), std::shared_ptr<Widget>(Connection),
				std::shared_ptr<Widget>(Label)
			}, {});
			FlowLayout->AddBehavior(new DraggablePositionBehavior(*FlowLayout));
			MainCanvas->AddWidget(FlowLayout);
		}

		// TEST: Open a file via path (read-only at first)
		if (false)
		{
			auto Path = new TextFieldWidget(Vector2n(40, -400), *m_TypingModule);
			MainCanvas->AddWidget(Path);

			MainCanvas->AddWidget(new ButtonWidget(Vector2n(0, -400), [=]() {
				std::cout << "Adding a new widget...\n";

				auto PathString = Path->GetContent();
#if 0
				MainCanvas->AddWidget(new TextFileWidget(Vector2n::ZERO, PathString, *m_TypingModule));
#else
				auto Content = [PathString]() -> std::string {
					return FromFileToString(PathString);
				};
				auto LabelWidget = new class LabelWidget(Vector2n::ZERO, Content, LabelWidget::Background::Normal);
				LabelWidget->AddBehavior(new DraggablePositionBehavior(*LabelWidget));
				MainCanvas->AddWidget(LabelWidget);
#endif
			} ));
		}

		// Folder Listing
		AddWidgetForPath("./", *MainCanvas, *m_TypingModule, m_CurrentProject, Vector2n(-610, -400));
		AddWidgetForPath("./GoLand/src/gist.github.com/", *MainCanvas, *m_TypingModule, m_CurrentProject, Vector2n(-950, -390));
		AddWidgetForPath("/Users/Dmitri/Dropbox/Work/2013/", *MainCanvas, *m_TypingModule, m_CurrentProject, Vector2n(-600, -590));

		// TEST: Two dependencies
		{
			auto s1 = new TextFieldWidget(Vector2n(-620, 340), *m_TypingModule);
			auto s2 = new TextFieldWidget(Vector2n(-620, 340+lineHeight+2), *m_TypingModule);
			auto o = new TextFieldWidget(Vector2n::ZERO, *m_TypingModule);
			ConnectionWidget<TextFieldWidget> * c1, * c2;
			auto oc = new FlowLayoutWidget(Vector2n(-560, 340), {
				std::shared_ptr<Widget>(c1 = new ConnectionWidget<TextFieldWidget>(Vector2n::ZERO, s1)),
				std::shared_ptr<Widget>(c2 = new ConnectionWidget<TextFieldWidget>(Vector2n::ZERO, s2)),
				std::shared_ptr<Widget>(o)
			}, {});

			auto R = [=]() {
				if (c1->Target() && c2->Target())
				{
					o->SetContent(c1->Target()->GetContent() + "\n" + c2->Target()->GetContent());
				}
			};

			c1->m_OnChange = R;
			c2->m_OnChange = R;
			R();

			MainCanvas->AddWidget(s1);
			MainCanvas->AddWidget(s2);
			MainCanvas->AddWidget(oc);
		}

		// TEST: Modify some Concept
		{
			auto Widget = new TextFieldWidget(Vector2n(-320, 470), *m_TypingModule);
			Widget->SetContent(GetConcept(47).GetContent());
			Widget->m_OnChange = [=]() {
				static_cast<ConceptBasic &>(ModifyConcept(47)).SetContentTEST(Widget->GetContent());
			};
			Widget->AddBehavior(new DraggablePositionBehavior(*Widget));
			MainCanvas->AddWidget(Widget);
		}

		// Label resizing test
		if (false)
		{
			auto SourceWidget = new TextFieldWidget(Vector2n::ZERO, *m_TypingModule);

			auto Content = [=]() -> std::string {
				return SourceWidget->GetContent();
			};
			auto LabelWidget = new class LabelWidget(Vector2n::ZERO, Content, LabelWidget::Background::Normal);

			MainCanvas->AddWidget(new FlowLayoutWidget(Vector2n(-100, -450), {
				std::shared_ptr<Widget>(SourceWidget),
				std::shared_ptr<Widget>(LabelWidget)
			}, {}));
		}

		// Time widget
		/*{
			auto Content = []() -> std::string {
				auto now = std::chrono::system_clock::now();

				auto duration = now.time_since_epoch();

				auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();

				return std::to_string(seconds);
			};
			auto LabelWidget = new class LabelWidget(Vector2n(360, -340), Content, LabelWidget::Background::Normal);
			LabelWidget->AddBehavior(new DraggablePositionBehavior(*LabelWidget));

			MainCanvas->AddWidget(LabelWidget);
		}*/

#if 1
		{
			MainCanvas->AddWidget(new ListWidget<Concept *>(Vector2n(-730 - 450, -250), Concepts, *m_TypingModule));
		}
#endif

		m_Widgets.push_back(std::unique_ptr<Widget>(m_TypingModule));

		m_Widgets.push_back(std::unique_ptr<Widget>(MainCanvas));

		m_Widgets.push_back(std::unique_ptr<Widget>(new DebugOverlayWidget(MainCanvas)));		// DEBUG: Print debug info
	}

DebugSkipOtherWidgets:
	// Prepare and start the thread
	{
		m_CurrentProject.StartBackgroundThread();
	}

	{
		// Load program
		m_CurrentProject.LoadSampleGenProgram(*static_cast<CanvasWidget *>(m_Widgets[0].get()));
	}
}

ConceptionApp::~ConceptionApp()
{
	CleanConcepts();

	// Clean up temporary files
#if (defined(__APPLE__) && defined(__MACH__)) || defined(__linux)
	system("rm ./GenProgram");
	system("rm ./GenProgram.go");
	system("rm ./GenDiff1.txt ./GenDiff2.txt");
	system("./bin/gocode/gocode drop-cache");
	system("./bin/gocode/gocode close");
#endif
}

void ConceptionApp::Render()
{
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
	if (   false &&		// HACK: Since I have a TimeWidget...
		   0 == m_CurrentProject.m_BackgroundState
		&& glfwGetTime() >= m_CurrentProject.m_ProcessEndedTime + 1
		&& !GetInputManager().AnyActivePointers()
		&& GetInputManager().EmptyInputEventQueue())
		return false;		// If idle, don't redraw regardless of input
	else
		return true;		// If background thread is doing something, we should redraw
}
