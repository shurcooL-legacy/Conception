#include "Main.h"

ConceptionTestApp::ConceptionTestApp(InputManager & InputManager)
	: App(InputManager),
	  m_CurrentProject()
	  /*OverlayCanvas(Vector2n(0, 0), false),
	  SystemInputListener(),
	  ControlModuleMapping0(),
	  ControlModuleMapping1(),
	  WidgetManager(),
	  WidgetModule(WidgetManager),
	  UnrealCameraModule(MainCanvas),
	  UnrealCameraModule2TEST(MainCanvas2TEST),
	  TypingModule(),*/
{
	/*{
		{ auto Widget = new ButtonWidget(Vector2n(10, 10)); WidgetManager.Add(Widget); Widget->SetOwnerCanvas(OverlayCanvas); }
		{ auto Widget = new ButtonWidget(Vector2n(50, 10)); WidgetManager.Add(Widget); Widget->SetOwnerCanvas(OverlayCanvas); }
		{ auto Widget = new TextFieldWidget(Vector2n(10, 50)); WidgetManager.Add(Widget); Widget->SetOwnerCanvas(OverlayCanvas); }

		{ auto Widget = new ListWidget(Vector2n(-200, -292), CurrentProject.GetStdIncludes()); WidgetManager.Add(Widget); Widget->SetOwnerCanvas(MainCanvas); }
	}

	{
		std::vector<InputManager::InputId> ButtonMappings; ButtonMappings.push_back(InputManager::InputId(1000, GLFW_MOUSE_BUTTON_LEFT));
		std::vector<InputManager::InputId> SliderMappings;
		std::vector<InputManager::InputId> AxisMappings; AxisMappings.push_back(InputManager::InputId(1000, 0)); AxisMappings.push_back(InputManager::InputId(1000, 1));
		std::vector<InputManager::InputId> PositiveConstraints;
		std::vector<InputManager::InputId> NegativeConstraints;
		ControlModuleMapping0.AddMapping(&WidgetModule, 0, ButtonMappings, SliderMappings, AxisMappings, PositiveConstraints, NegativeConstraints);
		WidgetModule.SetOwnerCanvas(OverlayCanvas);
	}
	{
		std::vector<InputManager::InputId> ButtonMappings; ButtonMappings.push_back(InputManager::InputId(1000, GLFW_MOUSE_BUTTON_LEFT));
		std::vector<InputManager::InputId> SliderMappings; SliderMappings.push_back(InputManager::InputId(1000, 0)); SliderMappings.push_back(InputManager::InputId(1000, 1)); SliderMappings.push_back(InputManager::InputId(1000, 2));
		std::vector<InputManager::InputId> AxisMappings; AxisMappings.push_back(InputManager::InputId(1000, 0)); AxisMappings.push_back(InputManager::InputId(1000, 1));
		std::vector<InputManager::InputId> PositiveConstraints;
		std::vector<InputManager::InputId> NegativeConstraints;
		ControlModuleMapping0.AddMapping(&UnrealCameraModule2TEST, 0, ButtonMappings, SliderMappings, AxisMappings, PositiveConstraints, NegativeConstraints);
		UnrealCameraModule2TEST.SetOwnerCanvas(MainCanvas2TEST);
	}
	{
		std::vector<InputManager::InputId> ButtonMappings; ButtonMappings.push_back(InputManager::InputId(1000, GLFW_MOUSE_BUTTON_LEFT));
		std::vector<InputManager::InputId> SliderMappings; SliderMappings.push_back(InputManager::InputId(1000, 0)); SliderMappings.push_back(InputManager::InputId(1000, 1)); SliderMappings.push_back(InputManager::InputId(1000, 2));
		std::vector<InputManager::InputId> AxisMappings; AxisMappings.push_back(InputManager::InputId(1000, 0)); AxisMappings.push_back(InputManager::InputId(1000, 1));
		std::vector<InputManager::InputId> PositiveConstraints;
		std::vector<InputManager::InputId> NegativeConstraints;
		ControlModuleMapping0.AddMapping(&UnrealCameraModule, 0, ButtonMappings, SliderMappings, AxisMappings, PositiveConstraints, NegativeConstraints);
		UnrealCameraModule.SetOwnerCanvas(MainCanvas);
	}
	ControlModuleMapping0.DoneAdding();

	g_TypingModuleTEST = &TypingModule;
	{
		std::vector<InputManager::InputId> ButtonMappings; ButtonMappings.push_back(InputManager::InputId(0, GLFW_KEY_BACKSPACE));
		std::vector<InputManager::InputId> SliderMappings;
		std::vector<InputManager::InputId> AxisMappings; AxisMappings.push_back(InputManager::InputId(1000, 0)); AxisMappings.push_back(InputManager::InputId(1000, 1));
		std::vector<InputManager::InputId> PositiveConstraints;
		std::vector<InputManager::InputId> NegativeConstraints;
		ControlModuleMapping1.AddMapping(&TypingModule, 0, ButtonMappings, SliderMappings, AxisMappings, PositiveConstraints, NegativeConstraints);
		TypingModule.SetOwnerCanvas(OverlayCanvas);
	}
	ControlModuleMapping1.DoneAdding();

	m_InputManager.RegisterListener(&SystemInputListener);
	m_InputManager.RegisterListener(&ControlModuleMapping0);
	m_InputManager.RegisterListener(&ControlModuleMapping1);*/

	/*{
		MainCanvas.RenderBackground();

		MainCanvas.SetupTransform();
		{
			CurrentProject.Render();

			WidgetManager.Render(MainCanvas);
		}
		MainCanvas.EndTransform();
	}

	{
		MainCanvas2TEST.RenderBackground();

		MainCanvas2TEST.SetupTransform();
		{
			CurrentProject.Render();

			WidgetManager.Render(MainCanvas2TEST);
		}
		MainCanvas2TEST.EndTransform();
	}

	{
		OverlayCanvas.SetupTransform();
		{
			WidgetManager.Render(OverlayCanvas);

			g_TypingModuleTEST->Render();
		}
		OverlayCanvas.EndTransform();
	}*/

	{
		auto MainCanvas = new Canvas(Vector2n(50, 50), true, true);

		MainCanvas->AddWidget(new ButtonWidget(Vector2n(0, 0), nullptr));
		MainCanvas->AddWidget(new ButtonWidget(Vector2n(10, 10), nullptr));
		MainCanvas->AddWidget(new ListWidget(Vector2n(-200, -292), m_CurrentProject.GetStdIncludes()));
		MainCanvas->AddWidget(new TextFieldWidget(Vector2n(-400, -100)));

		m_Widgets.push_back(std::unique_ptr<Widget>(MainCanvas));
	}

	{
		auto MainCanvas2TEST = new Canvas(Vector2n(800, 100), true, true);

		m_Widgets.push_back(std::unique_ptr<Widget>(MainCanvas2TEST));
	}

	{
		auto OverlayCanvas = new Canvas(Vector2n(0, 0), false, false);

		OverlayCanvas->AddWidget(new ButtonWidget(Vector2n(10, 10), nullptr));
		OverlayCanvas->AddWidget(new ButtonWidget(Vector2n(50, 10), nullptr));
		//OverlayCanvas->AddWidget(new TextFieldWidget(Vector2n(10, 50)));
		//OverlayCanvas->AddWidget(new TextFieldWidget(Vector2n(10, 100)));
		//OverlayCanvas->AddWidget(new TextFieldWidget(Vector2n(10, 150)));

		m_Widgets.push_back(std::unique_ptr<Widget>(OverlayCanvas));
	}

	{
		PopulateConcepts();

		// Load program
		m_CurrentProject.LoadSampleGenProgram(*static_cast<Canvas *>(m_Widgets[0].get()));		// HACK, TEST: Should use MainCanvas or something
	}
}

ConceptionTestApp::~ConceptionTestApp()
{
}

void ConceptionTestApp::UpdateWindowDimensions(Vector2n WindowDimensions)
{
	// TODO: This is a hack, I should create a WindowResize listener type of thing and take care within Widget itself
	static_cast<Canvas *>(m_Widgets[0].get())->SetDimensions(WindowDimensions + Vector2n(-200, -200));
	static_cast<Canvas *>(m_Widgets[1].get())->SetDimensions(Vector2n(300, 500));
	static_cast<Canvas *>(m_Widgets[2].get())->SetDimensions(WindowDimensions);
}
