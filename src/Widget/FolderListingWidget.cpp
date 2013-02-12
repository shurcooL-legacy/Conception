#include "../Main.h"

FolderListingWidget::FolderListingWidget(Vector2n Position, std::string Path, TypingModule & TypingModule)
	: FlowLayoutWidget(Position, {}, { /*std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this))*/ })
{
	auto List = Ls(Path);

	if (!(   List.empty()
		  || (   1 == List.size()
			  && List.front() == "ls: " + Path + ": Not a directory")))
	{
		auto ListingWidget = new MenuWidget<std::string>(Vector2n(-600, -390), List);
		//ListWidget->AddBehavior(new DraggablePositionBehavior(*ListWidget));

		ListingWidget->m_TapAction = [=](Vector2n LocalPosition, std::vector<std::string> & m_List)
		{
			g_InputManager->RequestTypingPointer(ListingWidget->ModifyGestureRecognizer());

			ListingWidget->SetSelectedEntryId(LocalPosition);
		};

		/*auto BeepWidget = new SayWidget(Vector2n::ZERO, *m_TypingModule);		// HACK: Memory leak
		BeepWidget->m_InputWidget->SetContent("Beep.");*/
		ListingWidget->m_OnChange = [&TypingModule, this, ListingWidget, Path](){
			//std::cout << "Beep.\n";
			//BeepWidget->m_ExecuteWidget->GetAction()();
			LaunchProcessInBackground("/usr/bin/afplay", "data/hitsound.wav");		// HACK: OS X dependency

			if (nullptr != m_Child) {
				this->RemoveWidget(m_Child);
			}
			auto NewPath = Path + *ListingWidget->GetSelectedEntry() + "/";
			this->AddWidget(m_Child = new FolderListingWidget(Vector2n(-390, -390), NewPath, TypingModule));
		};

		AddWidget(ListingWidget);
		//g_InputManager->RequestTypingPointer(ListWidget->ModifyGestureRecognizer());
	}
}

FolderListingWidget::~FolderListingWidget()
{
}

void FolderListingWidget::ProcessEvent(InputEvent & InputEvent)
{
	if (InputEvent.HasType(InputEvent::EventType::BUTTON_EVENT))
	{
		auto ButtonId = InputEvent.m_InputId;
		bool Pressed = InputEvent.m_Buttons[0];		// TODO: Check if there are >1 buttons

		if (Pointer::VirtualCategory::TYPING == InputEvent.m_Pointer->GetVirtualCategory())
		{
			if (Pressed)
			{
				bool HandledEvent = true;		// Assume true at first

				switch (ButtonId)
				{
				case GLFW_KEY_LEFT:
					{
						auto ParentFolderListingWidget = dynamic_cast<FolderListingWidget *>(ModifyParent());
						if (nullptr != ParentFolderListingWidget)
						{
							g_InputManager->RequestTypingPointer(ParentFolderListingWidget->GetWidgets()[0]->ModifyGestureRecognizer());
						}
					}
					break;
				case GLFW_KEY_RIGHT:
					{
						if (nullptr != m_Child)
						{
							if (nullptr == static_cast<MenuWidget<std::string> *>(m_Child->GetWidgets()[0].get())->GetSelectedEntry())
								static_cast<MenuWidget<std::string> *>(m_Child->GetWidgets()[0].get())->SetSelectedEntryId(0);

							g_InputManager->RequestTypingPointer(m_Child->GetWidgets()[0]->ModifyGestureRecognizer());
						}
					}
					break;
				default:
					HandledEvent = false;
					break;
				}

				if (HandledEvent)
				{
					InputEvent.m_Handled = true;
				}
			}
		}
	}
}
