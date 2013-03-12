#pragma once
#ifndef __RenderTools_H__
#define __RenderTools_H__

class Color
{
public:
	Color(double Red, double Green, double Blue)
		: m_Components( { Red, Green, Blue } )
	{}

	Color(uint8 Red, uint8 Green, uint8 Blue)
		: Color(Red / 255.0, Green / 255.0, Blue / 255.0)
	{}

	//Color & operator = (const Color & Other)
	Color(const Color & Other)
		: m_Components(Other.m_Components)
	{
	}

	double * GetComponents()
	{
		return m_Components.data();
	}

	Color operator * (const double Value)
	{
		return Color(m_Components[0] * Value, m_Components[1] * Value, m_Components[2] * Value);
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

	static const Color WHITE;
	static const Color BLACK;

private:
	std::array<double, 3>		m_Components;
};

void InitializeOpenGL();

void DrawBoxBorderless(Vector2n Position, Vector2n Size, Color Color);

void DrawBox(Vector2n Position, Vector2n Size);
void DrawBox(Vector2n Position, Vector2n Size, Color BackgroundColor);
void DrawBox(Vector2n Position, Vector2n Size, Color BackgroundColor, Color BorderColor);

void DrawAroundBox(Vector2n Position, Vector2n Size);
void DrawAroundBox(Vector2n Position, Vector2n Size, Color BackgroundColor);
void DrawAroundBox(Vector2n Position, Vector2n Size, Color BackgroundColor, Color BorderColor);

void DrawInnerBox(Vector2n Position, Vector2n Size, Color BackgroundColor);
void DrawInnerBox(Vector2n Position, Vector2n Size, Color BackgroundColor, Color BorderColor);

void DrawInnerRoundedBox(Vector2n Position, Vector2n Size, uint32 Radius, Color BackgroundColor, Color BorderColor);

void DrawCircle(Vector2n Position, Vector2n Size);
void DrawCircle(Vector2n Position, Vector2n Size, Color BackgroundColor);
void DrawCircle(Vector2n Position, Vector2n Size, Color BackgroundColor, Color BorderColor);

#endif // __RenderTools_H__
