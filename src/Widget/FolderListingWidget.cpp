#include "../Main.h"

FolderListingWidget::FolderListingWidget(Vector2n Position, std::string Path, CompositeWidget & AddTo, TypingModule & TypingModule, Project & Project)
	: FlowLayoutWidget(Position, {}, { /*std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this))*/ })
{
	// If not empty and doesn't end with slash, add the slash
	if (   !Path.empty()
		&& !HasSuffix(Path, "/"))
	{
		Path += "/";
	}

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
		auto ListingWidget = new FolderListingPureWidget(Vector2n::ZERO, List, Path, TypingModule);

		ListingWidget->m_TapAction = [ListingWidget, this, &TypingModule](Vector2n LocalPosition, std::vector<std::string> & m_List)
		{
			auto Entry = TypingModule.TakeString();

			if (!Entry.empty())
			{
				auto Shell = std::unique_ptr<ShellWidget>(new ShellWidget(Vector2n::ZERO, TypingModule));
				Shell->m_WorkingFolder->SetTarget(ListingWidget);

				if (HasSuffix(Entry, "/"))
					//std::cerr << "Should create folder \"" << Entry << "\" inside \"" << ListingWidget->GetPath() << "\"." << endl;
					Shell->m_CommandWidget->SetContent("mkdir \"" + Entry + "\"");
				else {
					Shell->m_CommandWidget->SetContent("touch \"" + Entry + "\"");
				}

				Shell->m_ExecuteWidget->GetAction()();
			}
			else
			{
				g_InputManager->RequestTypingPointer(ListingWidget->ModifyGestureRecognizer());

				ListingWidget->SetSelectedEntryId(LocalPosition);

				// Reset the selection of child folder listing (if any)
				if (   nullptr != this->m_Child
					&& !m_Child->GetWidgets().empty())
				{
					auto FolderListingPure = dynamic_cast<FolderListingPureWidget *>(m_Child->GetWidgets()[0].get());
					if (nullptr != FolderListingPure)
					{
						FolderListingPure->SetSelectedEntryId(-1);
					}
				}
			}
		};

		ListingWidget->m_OnChange = [&AddTo, &TypingModule, &Project, this, ListingWidget, Path]()
		{
			if (nullptr != m_Child) {
				this->RemoveWidget(m_Child);
				m_Child = nullptr;		// TODO: This is dangerous, easy to froget to set pointer to null... think about a better system
			}

			if (nullptr != ListingWidget->GetSelectedEntry())
			{
				//PlayBeep();

				auto NewPath = Path + *ListingWidget->GetSelectedEntry();
				this->AddWidget(m_Child = new FolderListingWidget(Vector2n::ZERO, NewPath, AddTo, TypingModule, Project));
			}
		};

		AddWidget(ListingWidget);

		auto Open = [&AddTo, &TypingModule, &Project, ListingWidget, Path]() {
			std::string FullPath = Path;
			if (nullptr != ListingWidget->GetSelectedEntry())
				FullPath += *ListingWidget->GetSelectedEntry();

			AddWidgetForPath(FullPath, AddTo, TypingModule, Project);
		};
		ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('O', PointerState::Modifiers::Super, Open, "Open File"));

		auto CopyPath = [&AddTo, &TypingModule, ListingWidget, Path]() {
			std::string FullPath = Path;
			if (nullptr != ListingWidget->GetSelectedEntry())
				FullPath += *ListingWidget->GetSelectedEntry();

#if DECISION_USE_CLIPBOARD_INSTEAD_OF_TypingModule
			glfwSetClipboardString(FullPath);
#else
			TypingModule.SetString(FullPath);
#endif
		};
		ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('I', PointerState::Modifiers::Super, CopyPath, "Copy Path"));

		auto OpenExternal = [&TypingModule, ListingWidget, Path]() {
			std::string FullPath = Path;
			if (nullptr != ListingWidget->GetSelectedEntry())
				FullPath += *ListingWidget->GetSelectedEntry();

			auto Shell = std::unique_ptr<ShellWidget>(new ShellWidget(Vector2n::ZERO, TypingModule));
			std::string Command = "open \'" + FullPath + "\'";
			Shell->m_CommandWidget->SetContent(Command);
			Shell->m_ExecuteWidget->GetAction()();

			std::cerr << "Doing: " << Shell->m_CommandWidget->GetContent() << endl;
			std::cerr << Shell->m_OutputWidget->GetContent() << endl;
		};
		ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('E', PointerState::Modifiers::Super, OpenExternal, "Open in External Editor"));

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

void AddWidgetForPath(const std::string & FullPath, CompositeWidget & AddTo, TypingModule & TypingModule, Project & Project)
{
	auto Position = Vector2n::ZERO;

	// Try to get the postion of the mouse cursor
	for (auto & Pointer : AddTo.GetGestureRecognizer().GetConnected())
	{
		if (Pointer::VirtualCategory::POINTING == Pointer->GetVirtualCategory())
		{
			Vector2n GlobalPosition(Pointer->GetPointerState().GetAxisState(0).GetPosition(), Pointer->GetPointerState().GetAxisState(1).GetPosition());
			Vector2n LocalPosition = AddTo.GlobalToLocal(GlobalPosition);
			Position = LocalPosition;
			break;
		}
	}

	AddWidgetForPath(FullPath, AddTo, TypingModule, Project, Position);
}

// TODO: Consider moving this to CompositeWidget or CanvasWidget as a member function
void AddWidgetForPath(const std::string & FullPath, CompositeWidget & AddTo, TypingModule & TypingModule, Project & Project, Vector2n Position)
{
	// TODO: Add better check if the path is a folder or file (rather than relying on suffix forward slash), take it from FodlerListingWidget's constructor
	if (HasSuffix(FullPath, "/"))
	{
		// Folder Listing
		auto FolderListing = new FolderListingWidget(Position, FullPath, AddTo, TypingModule, Project);
		FolderListing->AddBehavior(new DraggablePositionBehavior(*FolderListing));
		AddTo.AddWidget(FolderListing);
	}
	else
	{
		// Open file
		if (HasSuffix(FullPath, ".go"))
			AddTo.AddWidget(new LiveProgramFileWidget(Position, FullPath, TypingModule, Project));
		else
			AddTo.AddWidget(new TextFileWidget(Position, FullPath, TypingModule));
	}
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

							static_cast<FolderListingPureWidget *>(GetWidgets()[0].get())->SetSelectedEntryId(-1);		// Clear selection
						}
					}
					break;
				case GLFW_KEY_RIGHT:
					{
						if (   nullptr != m_Child
							&& !m_Child->GetWidgets().empty())		// Just in case its old widget is to be removed and new one is to be added at end of frame
						{
							auto FolderListingPure = dynamic_cast<FolderListingPureWidget *>(m_Child->GetWidgets()[0].get());
							if (nullptr != FolderListingPure)
							{
								if (nullptr == FolderListingPure->GetSelectedEntry())
									FolderListingPure->SetSelectedEntryId(0);

								g_InputManager->RequestTypingPointer(m_Child->GetWidgets()[0]->ModifyGestureRecognizer());
							}
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
