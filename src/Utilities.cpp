#include "Main.h"

bool HasSuffix(const std::string & s, const std::string & suffix)
{
	if (s.length() >= suffix.length()) {
		return (0 == s.compare(s.length() - suffix.length(), suffix.length(), suffix));
	} else {
		return false;
	}
}

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
