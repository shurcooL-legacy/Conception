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

	void ProcessTap(const InputEvent & InputEvent, Vector2n Position) override;

	void ProcessTimePassed(const double TimePassed) override;

protected:
	virtual bool IsHit(const Vector2n ParentPosition) const override;

private:
	struct State
	{
		sint32				Health		= 100;		// [0, 200]
		sint32				Pain		= 0;		// [0, 100]
		sint32				Energy		= 1000;		// [0, 2000]
	};
	
	struct Output
	{
		sint32				Action		= -1;		// [-1, 8], move direction, where -1 means stop
	};

	uint8					m_Color;

	Vector2d				m_PositionD;
	Vector2d				m_VelocityD;

	State					m_CurrentState;

	Output GenerateOutput();
};

#endif // __LifeFormWidget_H__
