#pragma once
#ifndef __LiveEditorApp_H__
#define __LiveEditorApp_H__

class LiveEditorApp
	: public App
{
public:
	LiveEditorApp(InputManager & InputManager);
	~LiveEditorApp();

	void UpdateWindowDimensions(Vector2n WindowDimensions) override;

	void Render() override;

	void ProcessEvent(InputEvent & InputEvent) override;

	bool ShouldRedrawRegardless() override;

	std::string GetTitle() override;

private:
	LiveEditorApp(const LiveEditorApp &);
	LiveEditorApp & operator = (const LiveEditorApp &);

	Project		m_CurrentProject;

	TypingModule	m_TypingModule;

	TextFieldWidget * m_SourceWidget = nullptr;
	TextFieldWidget * m_OutputWidget = nullptr;
	ToggleWidget * m_LiveToggle = nullptr;
};

#endif // __LiveEditorApp_H__
