#include "Main.h"

std::string FromFileToString(std::string Path)
{
	try
	{
		std::ifstream t(Path);
		std::string str;

		t.seekg(0, std::ios::end);
		str.reserve(t.tellg());
		t.seekg(0, std::ios::beg);

		str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

		return str;
	}
	catch (...)
	{
		return "";
	}
}

#if defined(__linux)
void glfwSetClipboardString(std::string string) {}
std::string glfwGetClipboardString() { return ""; }
#endif
