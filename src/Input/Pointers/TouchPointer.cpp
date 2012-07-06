#include "../../Main.h"

TouchPointer::TouchPointer(InputResponder & InputResponder)
	: Pointer(InputResponder)
{
}

TouchPointer::~TouchPointer()
{
}

TouchPointer::DeviceCategory TouchPointer::GetDeviceCategory() const
{
	return DeviceCategory::TOUCH;
}

TouchPointer::VirtualCategory TouchPointer::GetVirtualCategory() const
{
	return VirtualCategory::POINTING;
}
