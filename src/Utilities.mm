#include <string>

#if defined(__OBJC__)
	#import <Cocoa/Cocoa.h>
#else
	#include <ApplicationServices/ApplicationServices.h>
	typedef void* id;
#endif

#include "Utilities.h"

// Set the clipboard contents
void glfwSetClipboardString(std::string string)
{
	NSArray* types = [NSArray arrayWithObjects:NSStringPboardType, nil];
	
	NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
	[pasteboard declareTypes:types owner:nil];
	[pasteboard setString:[NSString stringWithUTF8String:string.c_str()]
				  forType:NSStringPboardType];
}

// Return the current clipboard contents
std::string glfwGetClipboardString()
{
	NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];

	if (![[pasteboard types] containsObject:NSStringPboardType])
	{
		return "";
	}

	NSString* object = [pasteboard stringForType:NSStringPboardType];
	if (!object)
	{
		return "";
	}
	
	/*free(_glfwLibrary.NS.clipboardString);
	_glfwLibrary.NS.clipboardString = strdup([object UTF8String]);

	return _glfwLibrary.NS.clipboardString;*/

	return std::string([object UTF8String]);
}
