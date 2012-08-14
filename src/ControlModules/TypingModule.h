#pragma once
#ifndef __TypingModule_H__
#define __TypingModule_H__

class TypingModule
	: public GestureHandler
{
public:
	TypingModule();
	~TypingModule();

	void Render(const InputManager & InputManager);

	std::string GetString();
	void SetString(std::string String);
	void Clear();

	void ProcessCharacter(InputEvent & InputEvent, const uint32 Character) override;

	void ProcessEvent(InputEvent & InputEvent) override;

private:
	TypingModule(const TypingModule &);
	TypingModule & operator = (const TypingModule &);

	std::string m_Typed;
};

#endif // __TypingModule_H__
