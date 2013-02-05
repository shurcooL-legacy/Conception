#include "Main.h"

Canvas::Canvas(Vector2n Position, bool Centered, bool HasBackground, BehaviourScrolling BehaviourScrolling)
	: CompositeWidget(Position, {}, {}),
	  m_BlackBackgroundTEST(false),
	  Camera(0, 0),
	  CameraZ(1),
	  m_Centered(Centered),
	  m_HasBackground(HasBackground),
	  m_ScissorBox(),
	  m_BehaviourScrolling(BehaviourScrolling)
{
	ModifyGestureRecognizer().m_RecognizeTap = true;
	if (   Canvas::BehaviourScrolling::Zooming == m_BehaviourScrolling
		|| Canvas::BehaviourScrolling::Freeform == m_BehaviourScrolling)
		ModifyGestureRecognizer().m_RecognizeManipulationTranslate = true;
	else if (Canvas::BehaviourScrolling::VerticalOnly == m_BehaviourScrolling)
		ModifyGestureRecognizer().m_RecognizeManipulationTranslate = false;
	ModifyGestureRecognizer().m_RecognizeScroll = true;

	if (m_HasBackground)
	{
		g_InputManager->RequestTypingPointer(ModifyGestureRecognizer());
	}
}

Canvas::~Canvas()
{
}

void Canvas::Render()
{
	if (m_HasBackground)
	{
		RenderBackground();
	}

	SetupTransform();
	{
		for (auto & Widget : GetWidgets())
		{
			Widget->Render();
		}
	}
	EndTransform();
}

bool Canvas::HitTest(Vector2n ParentPosition, std::list<Widget *> * Hits) const
{
	bool InsideCanvas;

	if (m_HasBackground)
	{
		InsideCanvas = Widget::HitTest(ParentPosition, nullptr);
	}
	else
	{
		InsideCanvas = Widget::HitTest(ParentPosition, nullptr);
	}

	bool HitInside = false;

	if (InsideCanvas)
	{
		/*Vector2d PositionDouble = GlobalToCanvas(Position);
		Vector2n PositionInt(std::lround(PositionDouble.X()), std::lround(PositionDouble.Y()));		// TODO: Loss of accuracy? Fix it if needed.*/
		auto LocalPosition = ParentToLocal(ParentPosition);

		for (auto & Widget : reverse(GetWidgets()))
		{
			if (Widget->HitTest(LocalPosition, Hits))
			{
				HitInside = true;
#if DECISION_POINTER_MAPPING_CONTAINS_SINGLE_TOPMOST_WIDGET
				break;		// HACK: Also a hack, need to do proper query if widget wants to block any futher hit testing for hover/interest checks
#endif
			}
		}

		if (m_HasBackground)
		{
#if DECISION_POINTER_MAPPING_CONTAINS_SINGLE_TOPMOST_WIDGET
			if (!HitInside)
#endif
				Widget::HitTest(ParentPosition, Hits);
		}
	}

	if (m_HasBackground)
	{
		return InsideCanvas;
	}
	else
	{
		return HitInside;
	}
}

const Vector2n Canvas::GetDimensions() const
{
	return Widget::GetDimensions();
}

// Override CompositeWidget's behavior of including inner widgets
bool Canvas::IsHit(const Vector2n ParentPosition) const
{
	if (m_HasBackground)
	{
		return Widget::IsHit(ParentPosition);
	}
	else
	{
		for (auto & Widget : reverse(GetWidgets()))
		{
			if (Widget->IsHit(ParentToLocal(ParentPosition)))
				return true;
		}

		return false;
	}
}

#if 0
void Canvas::ProcessButton(Pointer * Pointer, Input::InputId ButtonId, bool Pressed)
{
	if (0 == ButtonId)
	{
		if (Pressed)
		{
			//Pointer->GetWidgetMapping().RequestActivation(this);
		}
		else
		{
			//Pointer->GetWidgetMapping().RequestDeactivation(this);
		}
	}
}

void Canvas::ProcessSlider(Pointer * Pointer, Input::InputId SliderId, double MovedAmount)
{
	// TODO: Make it based on this widget being active/having captured the pointer, rather than pointer being active (i.e. not exclusive)
	//if (Pointer->IsActive())
	//if (IsActiveExternally())
	//if (this == Pointer->GetWidgetMapping().GetActiveWidget())
	{
		if (   0 == SliderId
			|| 1 == SliderId)
		{
			MoveView(static_cast<uint8>(SliderId), MovedAmount);
		}
	}

	if (2 == SliderId)
	{
		/*double A[2] = { (GetAxisState(0).GetPosition() - 0.5 * GetAxisState(0).GetLength()),
						(GetAxisState(1).GetPosition() - 0.5 * GetAxisState(1).GetLength()) };*/
		double A[2] = { 0, 0 };
		MoveView(static_cast<uint8>(SliderId), MovedAmount, A);
	}
}
#endif

void Canvas::ProcessTap(const InputEvent & InputEvent, Vector2n Position)
{
	//printf("Canvas %p::ProcessTap()\n", this);
	g_InputManager->RequestTypingPointer(ModifyGestureRecognizer());
}

/*void Canvas::ProcessDrag(Vector2d DragAmount)
{
	double A[2] = { 0, 0 };
	MoveView(0, DragAmount[0], A);
	MoveView(1, DragAmount[1], A);
}*/

void Canvas::ProcessScroll(InputEvent & InputEvent, Vector2n ScrollAmount)
{
	if (BehaviourScrolling::Zooming == m_BehaviourScrolling) {
		auto WidgetLocalPosition = Widget::ParentToLocal(GlobalToParent(Vector2n(InputEvent.m_Pointer->GetPointerState().GetAxisState(0).GetPosition(), InputEvent.m_Pointer->GetPointerState().GetAxisState(1).GetPosition())));
		double A[2] = { WidgetLocalPosition.X() - m_Centered * 0.5 * GetDimensions().X(),
						WidgetLocalPosition.Y() - m_Centered * 0.5 * GetDimensions().Y() };

		auto ParentLocalPosition = GlobalToParent(Vector2n(InputEvent.m_Pointer->GetPointerState().GetAxisState(0).GetPosition(), InputEvent.m_Pointer->GetPointerState().GetAxisState(1).GetPosition()));

		MoveView(2, ScrollAmount[0], A, ParentLocalPosition);
	} else if (BehaviourScrolling::Freeform == m_BehaviourScrolling) {
		MoveView(0, ScrollAmount[1]);
		MoveView(1, ScrollAmount[0]);
	} else if (BehaviourScrolling::VerticalOnly == m_BehaviourScrolling) {
		MoveView(1, ScrollAmount[0]);
	}
}

void Canvas::ProcessManipulationBegin(const PointerState & PointerState)
{
	//printf("MultitouchTestBoxWidget::ProcessManipulationBegin()\n");

	/*Vector2d PositionDouble = GetParent()->GlobalToCanvas(Vector2n(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition()));
	Vector2n PositionInt(std::lround(PositionDouble.X()), std::lround(PositionDouble.Y()));		// TODO: Loss of accuracy? Fix it if needed.*/
	auto ParentLocalPosition = GlobalToParent(Vector2n(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition()));

	// TODO: Loss of accuracy? Fix it if needed.
	ModifyGestureRecognizer().m_ManipulationOffset = Vector2n(Camera.X() * CameraZ, Camera.Y() * CameraZ) + ParentLocalPosition;
}

void Canvas::ProcessManipulationUpdate(const PointerState & PointerState)
{
	//printf("MultitouchTestBoxWidget::ProcessManipulationUpdate()\n");

	/*Vector2d PositionDouble = GetParent()->GlobalToCanvas(Vector2n(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition()));
	Vector2n PositionInt(std::lround(PositionDouble.X()), std::lround(PositionDouble.Y()));		// TODO: Loss of accuracy? Fix it if needed.*/
	auto ParentLocalPosition = GlobalToParent(Vector2n(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition()));

	// TODO: Loss of accuracy? Fix it if needed.
	Camera.X() = (GetGestureRecognizer().m_ManipulationOffset - ParentLocalPosition).X() / CameraZ;
	Camera.Y() = (GetGestureRecognizer().m_ManipulationOffset - ParentLocalPosition).Y() / CameraZ;
}

void Canvas::ProcessManipulationEnd(const PointerState & PointerState)
{
	//printf("MultitouchTestBoxWidget::ProcessManipulationEnd()\n");
}

void Canvas::ProcessTimePassed(const double TimePassed)
{
	for (auto & Pointer : GetGestureRecognizer().GetConnected())
	{
		if (   Pointer::VirtualCategory::TYPING == Pointer->GetVirtualCategory()
			&& &GetGestureRecognizer() == Pointer->GetPointerMapping().GetHoverer())
		{
			const PointerState & PointerState = Pointer->GetPointerState();

			const double SpeedMultiplier = 250;

			if (BehaviourScrolling::VerticalOnly != m_BehaviourScrolling)
			{
				if (PointerState.GetButtonState(GLFW_KEY_LEFT) && !PointerState.GetButtonState(GLFW_KEY_RIGHT))
				{
					MoveView(0, -SpeedMultiplier * TimePassed);
				}
				else if (PointerState.GetButtonState(GLFW_KEY_RIGHT) && !PointerState.GetButtonState(GLFW_KEY_LEFT))
				{
					MoveView(0, SpeedMultiplier * TimePassed);
				}
			}

			if (PointerState.GetButtonState(GLFW_KEY_UP) && !PointerState.GetButtonState(GLFW_KEY_DOWN))
			{
				MoveView(1, -SpeedMultiplier * TimePassed);
			}
			else if (PointerState.GetButtonState(GLFW_KEY_DOWN) && !PointerState.GetButtonState(GLFW_KEY_UP))
			{
				MoveView(1, SpeedMultiplier * TimePassed);
			}
		}
	}

	CompositeWidget::ProcessTimePassed(TimePassed);
}

void Canvas::ProcessCanvasUpdated()
{
	Widget::ProcessCanvasUpdated();

	for (auto & Widget : GetWidgets())
	{
		Widget->ProcessCanvasUpdated();
	}
}

void Canvas::MoveView(uint8 Degree, double MoveAmount, double A[2], Vector2n ParentLocalPosition)
{
	switch (Degree)
	{
	case 0:
		Camera.X() -= MoveAmount / CameraZ;
		break;
	case 1:
		Camera.Y() -= MoveAmount / CameraZ;
		break;
	case 2:
		{
			if (MoveAmount > 0)
			{
				CameraZ *= 1.5;
				Camera.X() += A[0] * 0.5 / CameraZ;
				Camera.Y() += A[1] * 0.5 / CameraZ;
			}
			else if (MoveAmount < 0)
			{
				Camera.X() -= A[0] * 0.5 / CameraZ;
				Camera.Y() -= A[1] * 0.5 / CameraZ;
				CameraZ /= 1.5;
			}

			// TODO: Loss of accuracy? Fix it if needed.
			ModifyGestureRecognizer().m_ManipulationOffset = Vector2n(Camera.X() * CameraZ, Camera.Y() * CameraZ) + ParentLocalPosition;
		}
		break;
	default:
		break;
	}

	if (0 == Degree || 1 == Degree)
	{
		for (auto & Widget : GetWidgets())
		{
			Widget->ProcessCanvasUpdated();
		}
	}
}

void Canvas::RenderBackground()
{
	// HACK: Black background
	if (m_BlackBackgroundTEST/*dynamic_cast<MultitouchTestApp *>(pMainApp)*/ && m_HasBackground)
	{
		DrawBox(GetPosition(), GetDimensions(), Color(0.0, 0.0, 0.0), Color(0.0, 0.0, 0.0));

		return;
	}

	glBindTexture(GL_TEXTURE_2D, 2);

	/*glMatrixMode(GL_TEXTURE_MATRIX);
	glLoadIdentity();
	glScaled(CameraZ, CameraZ, CameraZ);
	glTranslated(-CameraX, -CameraY, 0);
	glMatrixMode(GL_MODELVIEW);*/

	glColor3d(1, 1, 1);
	glBegin(GL_QUADS);
		/*glTexCoord2d(0 / 256.0, 0 / 256.0); glVertex2d(-0.5 * m_CanvasSize[0], -0.5 * m_CanvasSize[1]);
		glTexCoord2d(0 / 256.0, m_CanvasSize[1] / 256.0); glVertex2d(-0.5 * m_CanvasSize[0], 0.5 * m_CanvasSize[1]);
		glTexCoord2d(m_CanvasSize[0] / 256.0, m_CanvasSize[1] / 256.0); glVertex2d(0.5 * m_CanvasSize[0], 0.5 * m_CanvasSize[1]);
		glTexCoord2d(m_CanvasSize[0] / 256.0, 0 / 256.0); glVertex2d(0.5 * m_CanvasSize[0], -0.5 * m_CanvasSize[1]);*/
		/*glTexCoord2d(-0.5 * m_CanvasSize[0] / 256.0 - 0.5, -0.5 * m_CanvasSize[1] / 256.0 - 0.5); glVertex2d(-0.5 * m_CanvasSize[0], -0.5 * m_CanvasSize[1]);
		glTexCoord2d(-0.5 * m_CanvasSize[0] / 256.0 - 0.5, 0.5 * m_CanvasSize[1] / 256.0 - 0.5); glVertex2d(-0.5 * m_CanvasSize[0], 0.5 * m_CanvasSize[1]);
		glTexCoord2d(0.5 * m_CanvasSize[0] / 256.0 - 0.5, 0.5 * m_CanvasSize[1] / 256.0 - 0.5); glVertex2d(0.5 * m_CanvasSize[0], 0.5 * m_CanvasSize[1]);
		glTexCoord2d(0.5 * m_CanvasSize[0] / 256.0 - 0.5, -0.5 * m_CanvasSize[1] / 256.0 - 0.5); glVertex2d(0.5 * m_CanvasSize[0], -0.5 * m_CanvasSize[1]);*/
		glTexCoord2d((-0.5 * GetDimensions()[0] + Camera.X() * CameraZ) / 256.0 / CameraZ - 0.5, (-0.5 * GetDimensions()[1] + Camera.Y() * CameraZ) / 256.0 / CameraZ - 0.5); glVertex2i(GetPosition()[0], GetPosition()[1]);
		glTexCoord2d((-0.5 * GetDimensions()[0] + Camera.X() * CameraZ) / 256.0 / CameraZ - 0.5, (0.5 * GetDimensions()[1] + Camera.Y() * CameraZ) / 256.0 / CameraZ - 0.5); glVertex2i(GetPosition()[0], GetPosition()[1] + GetDimensions()[1]);
		glTexCoord2d((0.5 * GetDimensions()[0] + Camera.X() * CameraZ) / 256.0 / CameraZ - 0.5, (0.5 * GetDimensions()[1] + Camera.Y() * CameraZ) / 256.0 / CameraZ - 0.5); glVertex2i(GetPosition()[0] + GetDimensions()[0], GetPosition()[1] + GetDimensions()[1]);
		glTexCoord2d((0.5 * GetDimensions()[0] + Camera.X() * CameraZ) / 256.0 / CameraZ - 0.5, (-0.5 * GetDimensions()[1] + Camera.Y() * CameraZ) / 256.0 / CameraZ - 0.5); glVertex2i(GetPosition()[0] + GetDimensions()[0], GetPosition()[1]);
	glEnd();
}

const Rectanglen Canvas::GetScissorBox() const
{
	return m_ScissorBox;
}

void Canvas::SetScissorBox(Rectanglen ScissorBox)
{
	GLdouble ModelMatrix[16]; glGetDoublev(GL_MODELVIEW_MATRIX, ModelMatrix);
	GLdouble ProjectionMatrix[16]; glGetDoublev(GL_PROJECTION_MATRIX, ProjectionMatrix);
	GLint Viewport[4]; glGetIntegerv(GL_VIEWPORT, Viewport);
	GLdouble x0, y0, z0, x1, y1, z1, x2, y2, z2;

	gluProject(ScissorBox.GetPosition().X(), ScissorBox.GetPosition().Y() + ScissorBox.GetDimensions().Y(), 0.0, ModelMatrix, ProjectionMatrix, Viewport, &x0, &y0, &z0);
	gluProject(ScissorBox.GetDimensions().X(), ScissorBox.GetDimensions().Y(), 0.0, ModelMatrix, ProjectionMatrix, Viewport, &x1, &y1, &z1);
	gluProject(0.0, 0.0, 0.0, ModelMatrix, ProjectionMatrix, Viewport, &x2, &y2, &z2);

	m_ScissorBox.SetPosition().X() = static_cast<sint32>(std::lround(x0));
	m_ScissorBox.SetPosition().Y() = static_cast<sint32>(std::lround(y0));
	m_ScissorBox.SetDimensions().X() = static_cast<sint32>(std::lround(x1 - x2));
	m_ScissorBox.SetDimensions().Y() = static_cast<sint32>(std::lround(y2 - y1));

	// Crop the scissor box by the parent scissor box
	auto ParentCanvas = dynamic_cast<const Canvas *>(GetParent());
	if (nullptr != ParentCanvas)
	{
		auto ParentScissorBox = ParentCanvas->GetScissorBox();

		// Squish along two dimensions
		for (uint8 Dimension = 0; Dimension < 2; ++Dimension)
		{
			// Squish the left wall if needed
			if (m_ScissorBox.GetPosition()[Dimension] < ParentScissorBox.GetPosition()[Dimension])
			{
				m_ScissorBox.SetDimensions()[Dimension] = m_ScissorBox.SetDimensions()[Dimension] - (ParentScissorBox.GetPosition()[Dimension] - m_ScissorBox.GetPosition()[Dimension]);
				m_ScissorBox.SetPosition()[Dimension] = ParentScissorBox.GetPosition()[Dimension];
			}

			// Squish the right wall if needed
			if (m_ScissorBox.GetPosition()[Dimension] + m_ScissorBox.GetDimensions()[Dimension] > ParentScissorBox.GetPosition()[Dimension] + ParentScissorBox.GetDimensions()[Dimension])
			{
				m_ScissorBox.SetDimensions()[Dimension] = m_ScissorBox.SetDimensions()[Dimension] - ((m_ScissorBox.GetPosition()[Dimension] + m_ScissorBox.GetDimensions()[Dimension]) - (ParentScissorBox.GetPosition()[Dimension] + ParentScissorBox.GetDimensions()[Dimension]));
			}

			// Make sure the dimensions are non-negative
			m_ScissorBox.SetDimensions()[Dimension] = std::max(0, m_ScissorBox.GetDimensions()[Dimension]);
		}

		// This is needed to fix a bug on Intel 3000 where height of 0 acts as if height is 1 at top of screen
		if (0 == m_ScissorBox.SetDimensions()[0] || 0 == m_ScissorBox.SetDimensions()[1])
		{
			m_ScissorBox.SetDimensions() = Vector2n::ZERO;
		}
	}

	glScissor(m_ScissorBox.GetPosition().X(), m_ScissorBox.GetPosition().Y(), m_ScissorBox.GetDimensions().X(), m_ScissorBox.GetDimensions().Y());
	glEnable(GL_SCISSOR_TEST);
}

void Canvas::SetupTransform()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();

	SetScissorBox(Rectanglen(GetPosition(), GetDimensions()));

	glTranslated(GetPosition().X(), GetPosition().Y(), 0);
	if (m_Centered)
	{
		glTranslated(0.5 * GetDimensions().X(), 0.5 * GetDimensions().Y(), 0);
	}
	glScaled(CameraZ, CameraZ, CameraZ);
	glTranslated(-Camera.X(), -Camera.Y(), 0);
}

void Canvas::EndTransform()
{
	glPopMatrix();
	glPopAttrib();
}

/*
// DELETED: Because this functionality is there in HitTest()
const bool Canvas::GlobalIsInsideCanvas(const Vector2n Position) const
{
	return (   Position.X() >= m_Position.X()
			&& Position.Y() >= m_Position.Y()
			&& Position.X() <= m_Position.X() + m_Dimensions.X()
			&& Position.Y() <= m_Position.Y() + m_Dimensions.Y());
}*/

/*
// TO BE DELETED: Because moved this specific functionality to general GlobalToLocal override
// Precondition: GlobalIsInsideCanvas(Position) is true
const Vector2d Canvas::GlobalToCanvas(const Vector2n Position) const
{
	Vector2d PositionDouble(Position.X(), Position.Y());
	
	if (nullptr == this)
	{
		return PositionDouble;
	}

	if (!m_Centered)
	{
		return Vector2d((PositionDouble - Vector2d(m_Position.X(), m_Position.Y())) / CameraZ + Camera);
	}
	else
	{
		return Vector2d((PositionDouble - Vector2d(m_Position.X(), m_Position.Y()) - Vector2d(m_Dimensions.X(), m_Dimensions.Y()) * 0.5) / CameraZ + Camera);
	}
}*/

const Vector2n Canvas::ParentToLocal(const Vector2n ParentPosition) const
{
	auto WidgetLocalPosition = Widget::ParentToLocal(ParentPosition);

	Vector2d PositionDouble(WidgetLocalPosition.X(), WidgetLocalPosition.Y());

	if (!m_Centered)
	{
		PositionDouble = Vector2d(PositionDouble / CameraZ + Camera);
	}
	else
	{
		PositionDouble = Vector2d((PositionDouble - Vector2d(GetDimensions().X(), GetDimensions().Y()) * 0.5) / CameraZ + Camera);
	}

	Vector2n PositionInt(static_cast<sint32>(std::lround(std::floor(PositionDouble.X()))), static_cast<sint32>(std::lround(std::floor(PositionDouble.Y()))));		// TODO: Loss of accuracy? Fix it if needed.
	return PositionInt;
}

/*const Vector2n Canvas::GlobalToLocal(const Vector2n GlobalPosition) const
{
	auto WidgetLocalPosition = Widget::GlobalToLocal(GlobalPosition);

	Vector2d PositionDouble(WidgetLocalPosition.X(), WidgetLocalPosition.Y());

	if (!m_Centered)
	{
		PositionDouble = Vector2d(PositionDouble / CameraZ + Camera);
	}
	else
	{
		PositionDouble = Vector2d((PositionDouble - Vector2d(m_Dimensions.X(), m_Dimensions.Y()) * 0.5) / CameraZ + Camera);
	}

	Vector2n PositionInt(std::lround(std::floor(PositionDouble.X())), std::lround(std::floor(PositionDouble.Y())));		// TODO: Loss of accuracy? Fix it if needed.
	return PositionInt;
}*/
