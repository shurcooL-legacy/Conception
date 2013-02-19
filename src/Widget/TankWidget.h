#pragma once
#ifndef __TankWidget_H__
#define __TankWidget_H__

class TankWidget
	: public Widget
{
public:
	TankWidget(Vector2n Position);
	~TankWidget();

	void Render() override;

	void ProcessEvent(InputEvent & InputEvent) override;

	void ProcessTimePassed(const double TimePassed) override;

private:
	Vector2n m_TargetPosition;

	Vector2d m_PositionD;
	Vector2d m_VelocityD = Vector2d(0, 0);
};

#endif // __TankWidget_H__
