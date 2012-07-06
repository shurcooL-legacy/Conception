#include "../Main.h"

PointerMapping::PointerMapping(Pointer & Owner)
	: InputResponder(),
	  //m_HoverWidget(nullptr),
	  m_Capturer(nullptr),
	  m_Entries(),
	  m_Owner(Owner)
{
}

PointerMapping::~PointerMapping()
{
}

void PointerMapping::AddMapping(GestureRecognizer & Target)
{
	//Widget->SetActiveWidgetPointer(&m_ActiveWidget);

	m_Entries.push_back(&Target);

	MutuallyConnectable<Pointer, class GestureRecognizer>::Connect(m_Owner, Target);
}

// TODO: Consider moving this to AddMapping()
// Call this function after all modules are added, it makes sure to enable one of them if it should get enabled
void PointerMapping::DoneAdding()
{
	// TODO
}

void PointerMapping::RemoveAllMappings()
{
	// Deactivate the current m_Capturer if it was removed
	if (nullptr != m_Capturer)
	{
		ChangeCapturer(nullptr);
	}

	m_Entries.clear();

	// TODO: There is duplication of information between m_Entries and GetConnected(), so remove one of them
	while (!m_Owner.GetConnected().empty())
	{
		MutuallyConnectable<Pointer, GestureRecognizer>::Disconnect(m_Owner, **m_Owner.GetConnected().begin());
	}
}

/*void PointerMapping::ProcessPointerCreation(Pointer * Pointer)
{
}

void PointerMapping::ProcessPointerDeletion(Pointer * Pointer)
{
	RemoveHoverPointer(Pointer);
}

void PointerMapping::ProcessPointerActivation(Pointer * Pointer)
{
}

void PointerMapping::ProcessPointerDeactivation(Pointer * Pointer)
{
}

void PointerMapping::ProcessButton(Pointer * Pointer, Input::InputId ButtonId, bool Pressed)
{
	if (nullptr == m_ActiveWidget)
	{
		for (auto & Entry : m_Entries)
		{
			Entry->ProcessButton(Pointer, ButtonId, Pressed);
		}
	}
	else
	{
		m_ActiveWidget->ProcessButton(Pointer, ButtonId, Pressed);
	}
}

void PointerMapping::ProcessSlider(Pointer * Pointer, Input::InputId SliderId, double MovedAmount)
{
	if (nullptr == m_ActiveWidget)
	{
		for (auto & Entry : m_Entries)
		{
			Entry->ProcessSlider(Pointer, SliderId, MovedAmount);
		}
	}
	else
	{
		m_ActiveWidget->ProcessSlider(Pointer, SliderId, MovedAmount);
	}
}

void PointerMapping::ProcessAxis(Pointer * Pointer, Input::InputId AxisId, Input::AxisState AxisState)
{
}

void PointerMapping::Process2Axes(Pointer * Pointer, Input::InputId FirstAxisId, Input::AxisState AxisState[2])
{
}

void PointerMapping::ProcessCharacter(Pointer * Pointer, int Character)
{
}*/

bool PointerMapping::ProcessEvent(InputEvent & InputEvent)
{
	if (nullptr == m_Capturer)
	{
		for (auto & Entry : m_Entries)
		{
			// TODO: Care about return result?
			Entry->ProcessEvent(InputEvent);

			// DECISION
			//break;
		}
	}
	else
	{
		// TODO: Care about return result?
		m_Capturer->ProcessEvent(InputEvent);
	}

	return true;
}

void PointerMapping::ProcessTimePassed(const double TimePassed)
{
	for (auto & Entry : m_Entries)
	{
		Entry->ProcessTimePassed(TimePassed);
	}
}

void PointerMapping::RequestPointerCapture(GestureRecognizer * Candidate)
{
	printf("PointerMapping::RequestPointerCapture()\n");

	if (nullptr == m_Capturer)
	{
		ChangeCapturer(Candidate);
	}
}

void PointerMapping::RequestPointerRelease(GestureRecognizer * Candidate)
{
	printf("PointerMapping::RequestPointerRelease()\n");

	if (m_Capturer == Candidate)
	{
		ChangeCapturer(nullptr);
	}
}

GestureRecognizer * PointerMapping::GetCapturer() const
{
	return m_Capturer;
}

/*void PointerMapping::RemoveHoverPointer(Pointer * Pointer)
{
	for (auto & Entry : m_Entries)
	{
		Entry->RemoveHoverPointer(Pointer);		// TODO: Maybe only remove/add when there's change, rather than starting from scratch each time
	}
}*/

void PointerMapping::ChangeCapturer(GestureRecognizer * Capturer)
{
/*if (nullptr == ActiveModule) printf("ChangeActiveModule(nullptr)\n");
else if (typeid(UnrealCameraModule) == typeid(*ActiveModule)) printf("ChangeActiveModule(UnrealCameraModule)\n");
else if (typeid(WidgetModule) == typeid(*ActiveModule)) printf("ChangeActiveModule(WidgetModule)\n");
else if (typeid(TypingModule) == typeid(*ActiveModule)) printf("ChangeActiveModule(TypingModule)\n");*/

	if (m_Capturer == Capturer) {
		std::cerr << "Exception: ChangeCapturer() called with same Capturer.\n";
		throw 0;
	}

	//if (nullptr != m_Capturer) m_Capturer->ProcessPointerRelease();
	m_Capturer = Capturer;
	//if (nullptr != Capturer) Capturer->ProcessPointerCapture();

	//g_InputManager->SetMouseCursorVisibility(nullptr == m_Capturer || m_Capturer->ShouldMouseCursorVisible());
}
