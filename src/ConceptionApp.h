#pragma once
#ifndef __ConceptionApp_H__
#define __ConceptionApp_H__

extern TextFieldWidget * volatile g_OutputWidget;

class ConceptionApp
	: public App
{
public:
	ConceptionApp(InputManager & InputManager);
	~ConceptionApp();

	void UpdateWindowDimensions(Vector2n WindowDimensions) override;

	void Render() override;

	void ProcessEvent(InputEvent & InputEvent) override;

	bool ShouldRedrawRegardless() override;

private:
	ConceptionApp(const ConceptionApp &);
	ConceptionApp & operator = (const ConceptionApp &);

	Project		m_CurrentProject;

	TypingModule	m_TypingModule;

	TextFieldWidget * m_SourceWidget;
	TextFieldWidget * m_OutputWidget;
};

#endif // __ConceptionApp_H__
