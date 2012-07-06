#include "../../Main.h"

MousePointer::MousePointer(InputResponder & InputResponder)
	: Pointer(InputResponder)
{
}

MousePointer::~MousePointer()
{
}

Pointer::DeviceCategory MousePointer::GetDeviceCategory() const
{
	return DeviceCategory::MOUSE;
}

MousePointer::VirtualCategory MousePointer::GetVirtualCategory() const
{
	return VirtualCategory::POINTING;
}
