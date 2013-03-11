#pragma once
#ifndef __TextFileWidget_H__
#define __TextFileWidget_H__

class TextFileWidget
	: public CompositeWidget
{
public:
	TextFileWidget(Vector2n Position, std::string Path, TypingModule & TypingModule);
	~TextFileWidget();

	void ProcessTimePassed(const double TimePassed) override;

	std::string GetPath() const;

	std::function<void()> m_OnChange = nullptr;

private:
	TextFileWidget(const TextFileWidget &) = delete;
	TextFileWidget & operator = (const TextFileWidget &) = delete;

	// HACK, TODO: Keep this private by generalizing some interface so ConnectionWidget<TextFieldWidget> can accept TextFileWidget too
	public:TextFieldWidget * m_TextFieldWidget;private:

	std::string m_Path;
};

#endif // __TextFileWidget_H__
