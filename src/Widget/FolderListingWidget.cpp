#include "../Main.h"

FolderListingWidget::FolderListingWidget(Vector2n Position, std::string Path, CompositeWidget & AddTo, TypingModule & TypingModule)
	: FlowLayoutWidget(Position, {}, { /*std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this))*/ })
{
	auto List = Ls(Path);

	if (   1 == List.size()
		&& List.front() == "ls: " + Path + ": Not a directory")
	{
		// TEST: Add a file preview pane
		{
#if 0
			auto TextField = new TextFieldWidget(Vector2n(0, -430), *m_TypingModule);
			TextField->AddBehavior(new DraggablePositionBehavior(*TextField));
			MainCanvas->AddWidget(TextField);

			TextField->SetContent(FromFileToString(Path->GetContent()));
#else
			auto PathString = Path.substr(0, Path.length() - 1);
			auto Content = [PathString]() -> std::string {
				return FromFileToString(PathString);
			};
			auto LabelWidget = new class LabelWidget(Vector2n(0, -430), Content, LabelWidget::Background::Normal);
			AddWidget(LabelWidget);
#endif
		}
	}
	else if (!(   List.empty()
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
		ListingWidget->m_OnChange = [&AddTo, &TypingModule, this, ListingWidget, Path](){
			PlayBeep();

			if (nullptr != m_Child) {
				this->RemoveWidget(m_Child);
			}
			auto NewPath = Path + *ListingWidget->GetSelectedEntry() + "/";
			this->AddWidget(m_Child = new FolderListingWidget(Vector2n(-390, -390), NewPath, AddTo, TypingModule));
		};

		AddWidget(ListingWidget);
		//g_InputManager->RequestTypingPointer(ListWidget->ModifyGestureRecognizer());

		auto Open = [&AddTo, &TypingModule, ListingWidget, Path]() {
			if (nullptr != ListingWidget->GetSelectedEntry())
			{
				std::string FullPath = Path + *ListingWidget->GetSelectedEntry();

				std::cout << "Open sesame '" << FullPath << "'.\n";
				AddTo.AddWidget(new TextFileWidget(Vector2n(240, -230), FullPath, TypingModule));
			}
		};

		ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('O', PointerState::Modifiers::Super, Open));
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
