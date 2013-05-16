#include "Main.h"

// Write all of COUNT bytes from BUFFER to file descriptor FD
// Returns -1 on error, or the number of bytes written
ssize_t write_all(int fd, const void * buffer, size_t count)
{
	size_t left_to_write = count;
	while (left_to_write > 0) {
		size_t written = write(fd, buffer, count);
		if (written == -1)
			// An error occurred; bail
			return -1;
		else
			// Keep count of how much more we need to write
			left_to_write -= written;
	}
	// We should have written no more than COUNT bytes!
	assert (left_to_write == 0);
	// The number of bytes written is exactly COUNT
	return count;
}

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
