#pragma once
#ifndef __Utilities_H__
#define __Utilities_H__

// Clipboard (from GLFW 3.0)
void glfwSetClipboardString(std::string string);
std::string glfwGetClipboardString();

template <typename T> class reverse_adapter
{
public:
	reverse_adapter(T & c)
		: c(c)
	{}

	typename T::const_reverse_iterator begin() const
	{
		return c.rbegin();
	}

	typename T::const_reverse_iterator end() const
	{
		return c.rend();
	}

private:
	T & c;
};

template <typename T> reverse_adapter<T> reverse(T & c)
{
	return reverse_adapter<T>(c);
}

#endif // __Utilities_H__
