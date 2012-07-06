#include "../Main.h"

TypingModule::TypingModule()
	: ControlModule(1, 0, 2),
	  m_Typed()
{
}

TypingModule::~TypingModule()
{
}

bool TypingModule::ShouldActivate() const
{
	return true;
}

void TypingModule::ModuleProcessButton(InputManager::VirtualInputId ButtonId, bool Pressed)
{
	if (0 == ButtonId && Pressed)
	{
		// Erase the last character in string
		if (m_Typed.length() > 0)
		{
			m_Typed.erase(m_Typed.end() - 1);
		}
	}
}

void TypingModule::ModuleProcessCharacter(int Character)
{
	if ((Character >= 'a' && Character <= 'z')
		|| (Character >= 'A' && Character <= 'Z')
		|| (Character >= '0' && Character <= '9')
		|| Character == ' '
		|| Character == '-'
		|| Character == '_'
		|| Character == '.')
	{
		m_Typed += static_cast<char>(Character);
	}
}

void TypingModule::Render()
{
	// Draw "m_Typed" string
	if (m_Typed.length() > 0)
	{
		Vector2n Position(static_cast<sint32>(GetAxisState(0).GetPosition()) - m_Typed.length() * charWidth / 2 + charWidth, static_cast<sint32>(GetAxisState(1).GetPosition()) + lineHeight);
		Vector2n Dimensions(m_Typed.length() * charWidth, 1 * lineHeight);

		DrawAroundBox(Position, Dimensions, Color(234 / 255.0, 233 / 255.0, 190 / 255.0));

		glColor3d(0, 0, 0); OglUtilsPrint(Position.X(), Position.Y(), 0, LEFT, m_Typed.c_str());
	}
}

std::string TypingModule::GetString()
{
	return m_Typed;
}

void TypingModule::Clear()
{
	m_Typed.clear();
}
