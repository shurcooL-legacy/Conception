#pragma once
#ifndef __LifeFormWidget_H__
#define __LifeFormWidget_H__

class LifeFormWidget
	: public Widget
{
public:
	LifeFormWidget(Vector2n Position);
	virtual ~LifeFormWidget();

	void Render();

	void ProcessTap() override;

	void ProcessTimePassed(const double TimePassed) override;

private:
	uint8					m_Color;

	Vector2d				m_PositionD;
	Vector2d				m_VelocityD;

	//friend class App;		// DEBUG: For debug printing
};

#endif // __LifeFormWidget_H__
