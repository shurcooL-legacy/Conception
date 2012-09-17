#include "Main.h"

MotionBlurTestApp::MotionBlurTestApp(InputManager & InputManager)
	: App(InputManager),
	  m_ShaderTools(InputManager),
	  m_MovedTEST(0),
	  m_JustMovedTEST(false)
{
	for (int i = 0; i < 2; ++i)
	{
		auto MainCanvas = new Canvas(Vector2n(0, 0), false, true);
		MainCanvas->m_BlackBackgroundTEST = true;
		/*{
			double A[2] = { 0, 0 };
			MainCanvas->MoveView(2, -1, A);
		}*/

		//MainCanvas->AddWidget(new LabelWidget(Vector2n(0, 0), FromFileToString("src/Input/PointerMapping.cpp")));
		MainCanvas->AddWidget(new LabelWidget(Vector2n(0, 0), FromFileToString("src/Concepts.h")));

		m_Widgets.push_back(std::unique_ptr<Widget>(MainCanvas));
	}
}

MotionBlurTestApp::~MotionBlurTestApp()
{
}

void MotionBlurTestApp::UpdateWindowDimensions(Vector2n WindowDimensions)
{
	// TODO: This is a hack, I should create a WindowResize listener type of thing and take care within Widget itself
	static_cast<Canvas *>(m_Widgets[0].get())->SetDimensions(Vector2n(WindowDimensions.X() / 2, WindowDimensions.Y()));

	static_cast<Canvas *>(m_Widgets[1].get())->SetPosition(Vector2n(WindowDimensions.X() / 2, 0));
	static_cast<Canvas *>(m_Widgets[1].get())->SetDimensions(Vector2n(WindowDimensions.X() / 2, WindowDimensions.Y()));
}

void MotionBlurTestApp::Render()
{
	//App::Render();

	{
		{
			// Prepare viewport
			glViewport(0, 0, GetInputManager().GetWindowDimensions().X(), GetInputManager().GetWindowDimensions().Y());

			// Set target FBO
			glBindFramebuffer(GL_FRAMEBUFFER, m_ShaderTools.m_MotionBlurFboId);
			//glDrawBuffer(GL_COLOR_ATTACHMENT0);

			m_Widgets[0]->Render();

			// Return to normal framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			//glDrawBuffer(GL_BACK);
			glViewport(0, 0, GetInputManager().GetWindowDimensions().X(), GetInputManager().GetWindowDimensions().Y());

			// Display a textured quad with results
			{
				glEnable(GL_TEXTURE_RECTANGLE);
				glBindTexture(GL_TEXTURE_RECTANGLE, m_ShaderTools.m_MotionBlurColorTexId);

				m_ShaderTools.m_MotionBlurShader.bind();
				m_ShaderTools.m_MotionBlurShader.bindTextureRECT("SourceTex", m_ShaderTools.m_MotionBlurColorTexId, 0);
				//GLint Samples = GetInputManager().m_MousePointer->GetPointerState().GetAxisState(1).GetPosition() - 400;
				GLint Samples = m_MovedTEST;
				m_ShaderTools.m_MotionBlurShader.setUniform("Samples", &Samples, 1);

				glPushMatrix();
				glLoadIdentity();
				glBegin(GL_QUADS);
					glColor3d(1, 1, 1);
					glTexCoord2d(0, m_ShaderTools.m_MotionBlurImageHeight); glVertex2d(0, 0);
					glTexCoord2d(0, 0); glVertex2d(0, m_ShaderTools.m_MotionBlurImageHeight);
					glTexCoord2d(m_ShaderTools.m_MotionBlurImageWidth, 0); glVertex2d(m_ShaderTools.m_MotionBlurImageWidth, m_ShaderTools.m_MotionBlurImageHeight);
					glTexCoord2d(m_ShaderTools.m_MotionBlurImageWidth, m_ShaderTools.m_MotionBlurImageHeight); glVertex2d(m_ShaderTools.m_MotionBlurImageWidth, 0);
				glEnd();
				glPopMatrix();

				m_ShaderTools.m_MotionBlurShader.unbind();

				glDisable(GL_TEXTURE_RECTANGLE);
			}
		}

		m_Widgets[1]->Render();
	}
}

void MotionBlurTestApp::ProcessEvent(InputEvent & InputEvent)
{
	//App::ProcessEvent(InputEvent);

	if (   InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::AXIS_EVENT)
		&& 2 == InputEvent.m_InputId)
	{
		m_MovedTEST = InputEvent.m_Sliders[0];
		m_JustMovedTEST = true;
	}

	if (nullptr != InputEvent.m_Pointer)
	{
		InputEvent.m_Pointer->ModifyPointerMapping().RemoveAllMappings();
		for (auto & Hit : m_Widgets)
		{
			InputEvent.m_Pointer->ModifyPointerMapping().AddMapping(Hit->ModifyGestureRecognizer());
		}
		InputEvent.m_Pointer->ModifyPointerMapping().DoneAdding();

		InputEvent.m_Pointer->ModifyPointerMapping().ProcessEvent(InputEvent);
	}

	if (InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::PARENT_SIZE))
	{
		UpdateWindowDimensions(ModifyInputManager().GetWindowDimensions());
	}

	// DEBUG, TEST: System key handling
	if (false == InputEvent.m_Handled)
	{
		if (InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::BUTTON_EVENT))
		{
			if (Pointer::VirtualCategory::TYPING == InputEvent.m_Pointer->GetVirtualCategory())
			{
				auto ButtonId = InputEvent.m_InputId;
				bool Pressed = InputEvent.m_Buttons[0];		// TODO: Check if there are >1 buttons

				if (Pressed)
				{
					switch (ButtonId)
					{
					case GLFW_KEY_ESC:
						{
							glfwCloseWindow();

							InputEvent.m_Handled = true;
						}
						break;
					// TEST
					case 'M':
						{
							//if (glfwGetKey(GLFW_KEY_LCTRL) || glfwGetKey(GLFW_KEY_RCTRL))
							{
								//g_InputManager->SetMouseCursorVisibility(!g_InputManager->IsMouseCursorVisible());
								/*int xpos, ypos;
								glfwGetMousePos(&xpos, &ypos);
								glfwSetMousePos(xpos+1, ypos);*/
							}

							InputEvent.m_Handled = true;
						}
						break;
					default:
						break;
					}
				}
			}
		}
	}
}

void MotionBlurTestApp::ProcessTimePassed(const double TimePassed)
{
	if (m_JustMovedTEST)
		m_JustMovedTEST = false;
	else
		m_MovedTEST = 0;
}
