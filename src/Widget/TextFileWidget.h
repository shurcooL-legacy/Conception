#pragma once
#ifndef __TextFileWidget_H__
#define __TextFileWidget_H__

class TextFileWidget
	: public CompositeWidget
{
public:
	TextFileWidget(Vector2n Position, std::string Path, TypingModule & TypingModule);
	~TextFileWidget();

	TextFieldWidget * m_TextFieldWidget;

private:
	TextFileWidget(const TextFileWidget &) = delete;
	TextFileWidget & operator = (const TextFileWidget &) = delete;
};

#endif // __TextFileWidget_H__
