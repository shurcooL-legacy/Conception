#pragma once
#ifndef __RenderTools_H__
#define __RenderTools_H__

class Color
{
public:
	Color(double Red, double Green, double Blue)
	{
		// TODO: Once VC11 gets initializer lists, I can use them...
		m_Components[0] = Red;
		m_Components[1] = Green;
		m_Components[2] = Blue;
	}

	double * GetComponents()
	{
		return m_Components.data();
	}

	double & operator [] (const std::array<double, 3>::size_type & Index)
	{
		return m_Components[Index];
	}

	bool operator == (const Color & Other) const
	{
		return (m_Components == Other.m_Components);
	}

	bool operator != (const Color & Other) const
	{
		return (m_Components != Other.m_Components);
	}

private:
	std::array<double, 3>		m_Components;
};

void InitializeOpenGL();

void DrawBox(Vector2n Position, Vector2n Size);
void DrawBox(Vector2n Position, Vector2n Size, Color BackgroundColor);
void DrawBox(Vector2n Position, Vector2n Size, Color BackgroundColor, Color BorderColor);

void DrawAroundBox(Vector2n Position, Vector2n Size);
void DrawAroundBox(Vector2n Position, Vector2n Size, Color BackgroundColor);
void DrawAroundBox(Vector2n Position, Vector2n Size, Color BackgroundColor, Color BorderColor);

void DrawInnerBox(Vector2n Position, Vector2n Size, Color BackgroundColor, Color BorderColor);

#endif // __RenderTools_H__
