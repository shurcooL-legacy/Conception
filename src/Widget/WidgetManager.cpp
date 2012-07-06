#include "../Main.h"

#if 0
WidgetManager::WidgetManager()
	: m_Widgets(),
	  m_HoverWidget(nullptr),
	  m_ActiveWidget(nullptr)
{
}

WidgetManager::~WidgetManager()
{
	// TODO: Probably not the right way of doing this... Figure out the ownership/deletion issue
	// Delete all widgets
	while (!m_Widgets.empty())
	{
		delete m_Widgets.back();
		m_Widgets.pop_back();
	}
}

void WidgetManager::Add(Widget * Widget)
{
	m_Widgets.push_back(Widget);
}

void WidgetManager::Render(Canvas & Canvas)
{
	for (Widget * Widget : m_Widgets)
	{
		if (&Widget->GetOwnerCanvas() == &Canvas)
		{
			Widget->Render(*this);
		}
	}
}

bool WidgetManager::HitTest(const Vector2d Position) const
{
	for (Widget * Widget : m_Widgets)
	{
		if (Widget->GetOwnerCanvas().GlobalIsInsideCanvas(Position) && Widget->HitTest(Widget->GetOwnerCanvas().GlobalToCanvas(Position)))
		{
			return true;
		}
	}

	return false;
}

void WidgetManager::ProcessPress(const Vector2d Position)
{
	for (Widget * Widget : m_Widgets)
	{
		if (Widget->GetOwnerCanvas().GlobalIsInsideCanvas(Position) && Widget->HitTest(Widget->GetOwnerCanvas().GlobalToCanvas(Position)))
		{
			m_HoverWidget = Widget;
			m_ActiveWidget = Widget;

			return;
		}
	}
}

void WidgetManager::ProcessMove(const Vector2d Position)
{
	for (Widget * Widget : m_Widgets)
	{
		if (Widget->GetOwnerCanvas().GlobalIsInsideCanvas(Position) && Widget->HitTest(Widget->GetOwnerCanvas().GlobalToCanvas(Position)))
		{
			m_HoverWidget = Widget;

			return;
		}
	}

	m_HoverWidget = nullptr;
}

void WidgetManager::ProcessRelease(const Vector2d Position)
{
	for (Widget * Widget : m_Widgets)
	{
		if (Widget->GetOwnerCanvas().GlobalIsInsideCanvas(Position) && Widget->HitTest(Widget->GetOwnerCanvas().GlobalToCanvas(Position)))
		{
			if (Widget == m_ActiveWidget)
			{
				printf("Pressed Widget %p.\n", Widget);
				Widget->Clicked();
			}

			break;
		}
	}

	m_ActiveWidget = nullptr;
}

void WidgetManager::ProcessDeactivation()
{
	m_ActiveWidget = nullptr;
	m_HoverWidget = nullptr;
}
#endif
