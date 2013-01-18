#include "../Main.h"

DebugOverlayWidget::DebugOverlayWidget()
	: Canvas(Vector2n::ZERO, false, false)
{
	{
		auto Content = []() -> std::string
		{
			std::ostringstream out;

			out << "Mouse.PntrMppng.m_Entries.size(): " << g_InputManager->m_MousePointer->ModifyPointerMapping().m_Entries.size();
			for (auto & i : g_InputManager->m_MousePointer->ModifyPointerMapping().m_Entries)
			{
				if (dynamic_cast<Canvas *>(&i->GetOwner())) out << "\n Canvas";
				else if (dynamic_cast<MultitouchTestBoxWidget *>(&i->GetOwner())) out << "\n MultitouchTestBoxWidget, color: " << static_cast<uint16>(static_cast<MultitouchTestBoxWidget *>(&i->GetOwner())->m_Color);
				else if (dynamic_cast<ContextMenuWidget<std::string> *>(&i->GetOwner())) out << "\n ContextMenuWidget<std::string>";
				else if (dynamic_cast<TextFieldWidget *>(&i->GetOwner())) out << "\n TextFieldWidget";
				else if (dynamic_cast<ButtonWidget *>(&i->GetOwner())) out << "\n ButtonWidget";
				else if (dynamic_cast<ListWidget<ConceptId> *>(&i->GetOwner())) out << "\n ListWidget<ConceptId>";
				else if (dynamic_cast<ListWidget<Concept> *>(&i->GetOwner())) out << "\n ListWidget<Concept>";
				else if (dynamic_cast<LiveProgramWidget *>(&i->GetOwner())) out << "\n LiveProgramWidget";
				else if (dynamic_cast<TypingModule *>(&i->GetOwner())) out << "\n TypingModule";
				else if (dynamic_cast<FlowLayoutWidget *>(&i->GetOwner())) out << "\n FlowLayoutWidget";
				else if (dynamic_cast<TimeWidget *>(&i->GetOwner())) out << "\n TimeWidget";
				else if (dynamic_cast<LabelWidget *>(&i->GetOwner())) out << "\n LabelWidget";
				else if (dynamic_cast<ConceptStringBoxWidget *>(&i->GetOwner())) out << "\n ConceptStringBoxWidget";
				else out << "\n (Unknown)";

				auto LocalPosition = dynamic_cast<Widget *>(&i->GetOwner())->GlobalToLocal(Vector2n(g_InputManager->m_MousePointer->GetPointerState().GetAxisState(0).GetPosition(), g_InputManager->m_MousePointer->GetPointerState().GetAxisState(1).GetPosition()));
				out << " (" << LocalPosition.X() << ", " << LocalPosition.Y() << ")";
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

		AddWidget(new LabelWidget(Vector2n(0, 180), Content));
	}

	// Visibility toggle
	{
		AddWidget(new ToggleWidget(Vector2n(1, 1), [=](bool State) {
															// Toggle visibility of all widgets but the last one (i.e. this toggle)
															// TODO: Fix problem where upon initialize, OnChange is called before this widget has been added, so 2nd last widget is not hidden
															for (auto Widget = GetWidgets().begin(); GetWidgets().end() != Widget && GetWidgets().end() - 1 != Widget; ++Widget)
															{
																(*Widget)->m_Visible = State;
															}
														}, false));
	}
}

DebugOverlayWidget::~DebugOverlayWidget()
{
}
