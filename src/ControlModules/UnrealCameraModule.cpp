#include "../Main.h"

UnrealCameraModule::UnrealCameraModule(Canvas & Canvas)
	: ControlModule(1, 3, 2),
	  m_Canvas(Canvas)
{
}

UnrealCameraModule::~UnrealCameraModule()
{
}

void UnrealCameraModule::ProcessActivation(ControlModule * Previous)
{
}

void UnrealCameraModule::ProcessDeactivation(ControlModule * Next)
{
}

void UnrealCameraModule::ModuleProcessSlider(InputManager::VirtualInputId SliderId, double MovedAmount)
{
	if (IsButtonPressed(0))
	{
		if (   0 == SliderId
			|| 1 == SliderId)
		{
			m_Canvas.MoveView(static_cast<uint8>(SliderId), MovedAmount);
		}
	}

	// TODO: Figure out this whole thing being outside of IsButtonPressed(0): either make it work properly, or remove it
	if (2 == SliderId)
	{
		double A[2] = { (GetAxisState(0).GetPosition() - 0.5 * GetAxisState(0).GetLength()),
						(GetAxisState(1).GetPosition() - 0.5 * GetAxisState(1).GetLength()) };
		m_Canvas.MoveView(static_cast<uint8>(SliderId), MovedAmount, A);
	}
}
