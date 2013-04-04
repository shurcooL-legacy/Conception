#include "../Main.h"

FolderListingWidget::FolderListingWidget(Vector2n Position, std::string Path, CompositeWidget & AddTo, TypingModule & TypingModule)
	: FlowLayoutWidget(Position, {}, { /*std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this))*/ })
{
	//printf("FolderListingWidget(Path = `%s`) created.\n", Path.c_str());
	// If not empty and doesn't end with slash, add the slash
	if (   !Path.empty()
		&& '/' != Path.at(Path.length() - 1))
	{
		Path += "/";
	}
	//printf("FolderListingWidget(Path = `%s`) became.\n", Path.c_str());

	auto List = Ls(Path);

	if (   1 == List.size()
		&& List.front() == "ls: " + Path + ": Not a directory")
	{
		// TEST: Add a file preview pane
		//if (0)
		{
			auto PathString = Path.substr(0, Path.length() - 1);
#if 0
			AddWidget(new TextFileWidget(Vector2n::ZERO, PathString, *m_TypingModule));
#else
			auto Content = [PathString]() -> std::string {
				return FromFileToString(PathString);
			};
			auto LabelWidget = new class LabelWidget(Vector2n::ZERO, Content, LabelWidget::Background::Normal);
			AddWidget(LabelWidget);
#endif
		}
	}
	else if (!(   false//List.empty()		// Try to allow empty folders? See what happens
			   || (   1 == List.size()
				   && List.front() == "ls: " + Path + ": Not a directory")))
	{
		auto ListingWidget = new FolderListingPureWidget(Vector2n::ZERO, List, Path);

		ListingWidget->m_TapAction = [=](Vector2n LocalPosition, std::vector<std::string> & m_List)
		{
			g_InputManager->RequestTypingPointer(ListingWidget->ModifyGestureRecognizer());

			ListingWidget->SetSelectedEntryId(-1);		// Reset the selection regardless of what it was before
			ListingWidget->SetSelectedEntryId(LocalPosition);
		};

		ListingWidget->m_OnChange = [&AddTo, &TypingModule, this, ListingWidget, Path]()
		{
			if (nullptr != m_Child) {
				this->RemoveWidget(m_Child);
				m_Child = nullptr;		// TODO: This is dangerous, easy to froget to set pointer to null... think about a better system
			}

			if (nullptr != ListingWidget->GetSelectedEntry())
			{
				//PlayBeep();

				auto NewPath = Path + *ListingWidget->GetSelectedEntry();
				this->AddWidget(m_Child = new FolderListingWidget(Vector2n::ZERO, NewPath, AddTo, TypingModule));
			}
		};

		AddWidget(ListingWidget);

		auto Open = [&AddTo, &TypingModule, ListingWidget, Path]() {
			if (   nullptr != ListingWidget->GetSelectedEntry()
				&& '/' != *ListingWidget->GetSelectedEntry()->rbegin())		// Make sure it's not a folder, i.e. doesn't end with a slash
			{
				std::string FullPath = Path + *ListingWidget->GetSelectedEntry();

				std::cout << "Open sesame '" << FullPath << "'.\n";
				AddTo.AddWidget(new TextFileWidget(Vector2n(240, -230), FullPath, TypingModule));
			}
		};
		ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('O', PointerState::Modifiers::Super, Open, "Open File"));

		auto CopyPath = [&AddTo, &TypingModule, ListingWidget, Path]() {
			if (nullptr != ListingWidget->GetSelectedEntry())
			{
				std::string FullPath = Path + *ListingWidget->GetSelectedEntry();

#if DECISION_USE_CLIPBOARD_INSTEAD_OF_TypingModule
				glfwSetClipboardString(FullPath);
#else
				TypingModule.SetString(FullPath);
#endif
			}
		};
		ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('I', PointerState::Modifiers::Super, CopyPath, "Copy Path"));

		// Double tap to select and open file
		ListingWidget->m_DoubleTapAction = [ListingWidget, Open](Vector2n LocalPosition, std::vector<std::string> &) {
			ListingWidget->SetSelectedEntryId(LocalPosition);

			Open();
		};
		ListingWidget->ModifyGestureRecognizer().m_RecognizeDoubleTap = true;
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

							static_cast<MenuWidget<std::string> *>(GetWidgets()[0].get())->SetSelectedEntryId(-1);		// Clear selection
						}
					}
					break;
				case GLFW_KEY_RIGHT:
					{
						if (   nullptr != m_Child
							&& !m_Child->GetWidgets().empty()		// Just in case its old widget is to be removed and new one is to be added at end of frame
							&& nullptr != dynamic_cast<MenuWidget<std::string> *>(m_Child->GetWidgets()[0].get()))
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
