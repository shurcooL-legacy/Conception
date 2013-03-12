#pragma once
#ifndef __TextFileWidget_H__
#define __TextFileWidget_H__

class TextFileWidget
	: public CompositeWidget,
	  public MutuallyConnectable<TextFileWidget, ConnectionWidget<TextFileWidget>>
{
public:
	TextFileWidget(Vector2n Position, std::string Path, TypingModule & TypingModule);
	~TextFileWidget();

	void ProcessTimePassed(const double TimePassed) override;

	std::string GetPath() const;

	// TODO: Generalize this to all automate-able widgets?
	void NotifyChange(bool OverrideLiveToggle = false);		// Called when this widget changes, to notify its connected widgets

	std::function<void()> m_OnChange = nullptr;

private:
	TextFileWidget(const TextFileWidget &) = delete;
	TextFileWidget & operator = (const TextFileWidget &) = delete;

	// HACK, TODO: Keep this private by generalizing some interface so ConnectionWidget<TextFieldWidget> can accept TextFileWidget too
	public:TextFieldWidget * m_TextFieldWidget;private:

	std::string m_Path;
};

#endif // __TextFileWidget_H__
