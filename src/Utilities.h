#pragma once
#ifndef __Utilities_H__
#define __Utilities_H__

bool HasEnding(const std::string & fullString, const std::string & ending);

// Clipboard (from GLFW 3.0)
void glfwSetClipboardString(std::string string);
std::string glfwGetClipboardString();

template <typename T> class reverse_adapter
{
public:
	reverse_adapter(T & c)
		: m_C(c)
	{}

	typename T::const_reverse_iterator begin() const
	{
		return m_C.rbegin();
	}

	typename T::const_reverse_iterator end() const
	{
		return m_C.rend();
	}

private:
	T & m_C;
};

template <typename T> reverse_adapter<T> reverse(T & c)
{
	return reverse_adapter<T>(c);
}

std::string FromFileToString(std::string Path);

void Beep();

#endif // __Utilities_H__
