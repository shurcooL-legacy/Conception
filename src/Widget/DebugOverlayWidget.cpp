#include "../Main.h"

DebugOverlayWidget::DebugOverlayWidget(CanvasWidget * MainCanvas)
	: CanvasWidget(Vector2n::ZERO, false, false)
{
	{
		auto Content = []() -> std::string
		{
			std::ostringstream out;

			out << "Mouse.PntrMppng.m_Entries.size(): " << g_InputManager->m_MousePointer->ModifyPointerMapping().m_Entries.size();
			for (auto & i : g_InputManager->m_MousePointer->ModifyPointerMapping().m_Entries)
			{
				PrintName(out, i);

				auto LocalPosition = dynamic_cast<Widget *>(&i->GetOwner())->GlobalToLocal(Vector2n(g_InputManager->m_MousePointer->GetPointerState().GetAxisState(0).GetPosition(), g_InputManager->m_MousePointer->GetPointerState().GetAxisState(1).GetPosition()));
				out << " [" << static_cast<Widget *>(&i->GetOwner())->GetDimensions().X() << ", " << static_cast<Widget *>(&i->GetOwner())->GetDimensions().Y() << "]";
				out << " (" << LocalPosition.X() << ", " << LocalPosition.Y() << ")";
				out << " <" << static_cast<Widget *>(&i->GetOwner())->m_DebugDescription << ">";
			}

			out << "\nKB.PntrMppng.m_Entries.size(): " << g_InputManager->m_TypingPointer->ModifyPointerMapping().m_Entries.size();
			for (auto & i : g_InputManager->m_TypingPointer->ModifyPointerMapping().m_Entries)
			{
				PrintName(out, i);
			}

			return out.str();
		};

		AddWidget(new LabelWidget(Vector2n(0, 18), Content));
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

		AddWidget(new LabelWidget(Vector2n(0, 240), Content));
	}

	// Visualizer of currently available shortcuts
	{
		auto Content = []() -> std::string
		{
			std::ostringstream out;

			std::map<std::pair<PointerState::Modifiers, Input::InputId>, GestureRecognizer::ShortcutEntry &> AvailShortcuts;

			// Find all non-overlapping available shortcuts
			for (auto & i : g_InputManager->m_TypingPointer->ModifyPointerMapping().m_Entries)
			{
				for (auto & S : i->m_Shortcuts)
				{
					// TODO: Highlight shortcuts that can be activated in current state (e.g. Cmd+O does nothing when a folder is selected, etc.)

					// Only add this shortcut if there isn't any other overlapping one (i.e., with the same signature <Modifier, InputId>)
					auto Key = decltype(AvailShortcuts)::key_type(S.Modifiers, S.InputId);
					if (AvailShortcuts.end() == AvailShortcuts.find(Key))
						AvailShortcuts.insert(decltype(AvailShortcuts)::value_type(Key, S));
				}
			}

			// Print out all available shortcuts
			for (auto & aS : AvailShortcuts)
			{
				auto & S = aS.second;

				if (PointerState::Modifiers::Super == S.Modifiers)		// TODO: Make this more general
					out << "Cmd+";
				out << static_cast<char>(S.InputId);
				out << " - " << S.Description;
				out << endl;
			}

			return out.str();
		};

		// TODO: Make it right aligned to edge of screen
		AddWidget(new LabelWidget(Vector2n(1536 - 19 * charWidth, lineHeight + 2), Content));
	}

	// Time in top right corner
	// TODO: Add support for reference frames other than top left corner as origin
	auto TimeWidget = new class TimeWidget(Vector2n(1536 - 13 * charWidth - 1, 1));
	//TimeWidget->SetPosition(TimeWidget->GetPosition() - Vector2n(TimeWidget->GetDimensions().X(), 0));		// This doesn't work because its dimensions are updated after render...
	AddWidget(TimeWidget);

	// Visibility toggle
	{
		auto ToggleWidget = new class ToggleWidget(Vector2n(1, 1), [=](bool State) {
				// Toggle visibility of all widgets but the last one (i.e. this toggle)
				// TODO: Fix problem where upon initialize, OnChange is called before this widget has been added, so 2nd last widget is not hidden
				for (auto Widget = GetWidgets().begin(); GetWidgets().end() != Widget && GetWidgets().end() - 3 != Widget; ++Widget)
				{
					(*Widget)->m_Visible = State;
				}
			}, false);
		AddWidget(ToggleWidget);
		ProcessTimePassed(0);		// HACK: Get all the widgets actually added
		ToggleWidget->UpdateHACK();
	}
}

DebugOverlayWidget::~DebugOverlayWidget()
{
}

void DebugOverlayWidget::PrintName(std::ostringstream & out, GestureRecognizer *& i)
{
	if (dynamic_cast<CanvasWidget *>(&i->GetOwner())) out << "\n CanvasWidget";
	else if (dynamic_cast<MultitouchTestBoxWidget *>(&i->GetOwner())) out << "\n MultitouchTestBoxWidget, color: " << static_cast<uint16>(static_cast<MultitouchTestBoxWidget *>(&i->GetOwner())->m_Color);
	else if (dynamic_cast<ConnectionWidget<TextFileWidget> *>(&i->GetOwner())) out << "\n ConnectionWidget<TextFileWidget>";
	else if (dynamic_cast<ConnectionWidget<TextFieldWidget> *>(&i->GetOwner())) out << "\n ConnectionWidget<TextFieldWidget>";
	else if (dynamic_cast<FolderListingWidget *>(&i->GetOwner())) out << "\n FolderListingWidget";
	else if (dynamic_cast<MenuWidget<std::string> *>(&i->GetOwner())) out << "\n MenuWidget<std::string>";
	else if (dynamic_cast<ContextMenuWidget<std::string> *>(&i->GetOwner())) out << "\n ContextMenuWidget<std::string>";
	else if (dynamic_cast<TextFileWidget *>(&i->GetOwner())) out << "\n TextFileWidget";
	else if (dynamic_cast<TextFieldWidget *>(&i->GetOwner())) out << "\n TextFieldWidget";
	else if (dynamic_cast<ToggleWidget *>(&i->GetOwner())) out << "\n ToggleWidget";
	else if (dynamic_cast<ButtonWidget *>(&i->GetOwner())) out << "\n ButtonWidget";
	else if (dynamic_cast<ListWidget<ConceptId> *>(&i->GetOwner())) out << "\n ListWidget<ConceptId>";
	else if (dynamic_cast<ListWidget<Concept> *>(&i->GetOwner())) out << "\n ListWidget<Concept>";
	else if (dynamic_cast<LiveFunctionWidget *>(&i->GetOwner())) out << "\n LiveFunctionWidget";
	else if (dynamic_cast<LiveProgramWidget *>(&i->GetOwner())) out << "\n LiveProgramWidget";
	else if (dynamic_cast<LiveGofmtWidget *>(&i->GetOwner())) out << "\n LiveGofmtWidget";
	else if (dynamic_cast<GofmtWidget *>(&i->GetOwner())) out << "\n GofmtWidget";
	else if (dynamic_cast<TypingModule *>(&i->GetOwner())) out << "\n TypingModule";
	else if (dynamic_cast<GitFileWidget *>(&i->GetOwner())) out << "\n GitFileWidget";
	else if (dynamic_cast<LiveProgramFileWidget *>(&i->GetOwner())) out << "\n LiveProgramFileWidget";
	else if (dynamic_cast<LiveCodeWidget *>(&i->GetOwner())) out << "\n LiveCodeWidget";
	else if (dynamic_cast<FlowLayoutWidget *>(&i->GetOwner())) out << "\n FlowLayoutWidget";
	else if (dynamic_cast<TimeWidget *>(&i->GetOwner())) out << "\n TimeWidget";
	else if (dynamic_cast<LabelWidget *>(&i->GetOwner())) out << "\n LabelWidget";
	else if (dynamic_cast<ConceptStringBoxWidget *>(&i->GetOwner())) out << "\n ConceptStringBoxWidget";
	else out << "\n (Unknown)";
}
