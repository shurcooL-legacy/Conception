#include "../Main.h"

PointerMapping::PointerMapping(Pointer & Owner)
	: InputHandler(),
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

	MutuallyConnectable<Pointer, GestureRecognizer>::Connect(m_Owner, Target);
}

// TODO: Consider moving this to AddMapping()
// Call this function after all modules are added, it makes sure to enable one of them if it should get enabled
void PointerMapping::DoneAdding()
{
	// TODO
}

void PointerMapping::RemoveMapping(GestureRecognizer & Target)
{
	// Deactivate the current m_Capturer if it was removed
	if (&Target == m_Capturer)
	{
		ChangeCapturer(nullptr);
	}

	for (auto Entry = m_Entries.begin(); m_Entries.end() != Entry; ++Entry)
	{
		if (*Entry == &Target)
		{
			m_Entries.erase(Entry);
			break;
		}
	}

	MutuallyConnectable<Pointer, GestureRecognizer>::Disconnect(m_Owner, Target);
}

void PointerMapping::RemoveAllMappings()
{
	// Deactivate the current m_Capturer one existed
	if (nullptr != m_Capturer)
	{
		ChangeCapturer(nullptr);
	}

	m_Entries.clear();

	while (!m_Owner.GetConnected().empty())
	{
		MutuallyConnectable<Pointer, GestureRecognizer>::Disconnect(m_Owner, **m_Owner.GetConnected().begin());
	}
}

void PointerMapping::ProcessEvent(InputEvent & InputEvent)
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

	//return true;
}

void PointerMapping::ProcessTimePassed(const double TimePassed)
{
	for (auto & Entry : m_Entries)
	{
		Entry->ProcessTimePassed(TimePassed);
	}
}

void PointerMapping::RequestPointerCapture(GestureRecognizer * Requester)
{
std::cout << "RequestPointerCapture()\n";
	if (nullptr == m_Capturer)
	{
		ChangeCapturer(Requester);
	}
}

void PointerMapping::RequestPointerRelease(GestureRecognizer * Requester)
{
std::cout << "RequestPointerRelease()\n";
	if (m_Capturer == Requester)
	{
		ChangeCapturer(nullptr);
	}
}

GestureRecognizer * PointerMapping::GetCapturer() const
{
	return m_Capturer;
}

GestureRecognizer * PointerMapping::GetHoverer() const
{
	if (!m_Entries.empty())
		return m_Entries.front();
	else
		return nullptr;
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

	//if (nullptr != m_Capturer) m_Capturer->ProcessPointerRelease(m_Owner);
	m_Capturer = Capturer;
	//if (nullptr != Capturer) Capturer->ProcessPointerCapture(m_Owner);

	//g_InputManager->SetMouseCursorVisibility(nullptr == m_Capturer || m_Capturer->ShouldMouseCursorVisible());
}
