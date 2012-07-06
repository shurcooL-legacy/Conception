#include "../Main.h"

ListWidget::ListWidget(Vector2n Position, std::vector<ConceptId> & List)
	: Widget(Position, Vector2n::ZERO),
	  m_List(List)
{
	ModifyGestureRecognizer().m_RecognizeTap = true;

	UpdateDimensions();
}

ListWidget::~ListWidget()
{
}

void ListWidget::Render()
{
	Color BackgroundColor(1, 1, 1);
	Color BorderColor(0.3, 0.3, 0.3);

	/*if (CheckHover(WidgetManager) && CheckActive(WidgetManager))
	{
	}
	else if ((CheckHover(WidgetManager) && !CheckAnyActive(WidgetManager)) || (!CheckHover(WidgetManager) && CheckActive(WidgetManager)))
	{
		BorderColor[0] = 0.898;
		BorderColor[1] = 0.765;
		BorderColor[2] = 0.396;
	}
	else
	{
	}*/

	// TODO: Think if I should outsource the entire rendering code for more generality
	// Draw list
	{
		UpdateDimensions();		// LATER: Optimize by not repeating this calculation each time, only when something changes?
		
		if (m_List.empty())
		{
			BackgroundColor[0] = 234 / 255.0;
			BackgroundColor[1] = 233 / 255.0;
			BackgroundColor[2] = 190 / 255.0;
		}

		DrawAroundBox(m_Position, m_Dimensions, BackgroundColor, BorderColor);

		// TODO: This part is not general
		std::string Description[2] = { "#include <", ">" };
		glColor3d(0, 0, 0); OglUtilsPrint(m_Position.X(), m_Position.Y(), 0, RIGHT, Description[0].c_str());
		glColor3d(0, 0, 0); OglUtilsPrint(m_Position.X() + m_Dimensions.X(), m_Position.Y(), 0, LEFT, Description[1].c_str());

		OpenGLStream OpenGLStream(m_Position);
		for (auto & Entry : m_List)
		{
			OpenGLStream << Concepts[Entry] << endl;
		}
	}
}

void ListWidget::ProcessTap()
{
	// TEST: This is specific stuff for quick testing
	{
		//auto Entry = g_TypingModuleTEST->GetString();
		//g_TypingModuleTEST->Clear();
		std::string Entry = "TypingModule not yet re-enabled";

		if (Entry.length() > 0)
		{
			auto ConceptId = FindConcept(Entry);
			if (0 == ConceptId)
			{
				Concepts.push_back(Concept("", Entry));
				ConceptId = LastConceptId();
			}

			m_List.push_back(ConceptId);
		}
	}
}

void ListWidget::UpdateDimensions()
{
	Vector2n MinDimensions(3 * charWidth, lineHeight);

	Vector2n MaxDimensions(MinDimensions);
	for (auto & Entry : m_List)
	{
		MaxDimensions.X() = std::max<sint32>(MaxDimensions.X(), Concepts[Entry].GetDimensions().X());
	}
	MaxDimensions.Y() = std::max<sint32>(MaxDimensions.Y(), (m_List.size() + 1) * lineHeight);

	Vector2n Dimensions = MaxDimensions;

	m_Dimensions = Dimensions;
}
