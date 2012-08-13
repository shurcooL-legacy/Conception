#include "../../Main.h"

TouchPointer::TouchPointer(InputHandler & InputHandler)
	: Pointer(InputHandler)
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
