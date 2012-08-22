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

	Vector2n GetInsertionPosition(Vector2n PointerPosition) const;

	std::string GetString();
	std::string TakeString();
	void SetString(std::string String);

	void ProcessCharacter(InputEvent & InputEvent, const uint32 Character) override;

	void ProcessEvent(InputEvent & InputEvent) override;

private:
	TypingModule(const TypingModule &);
	TypingModule & operator = (const TypingModule &);

	std::string m_Typed;
};

#endif // __TypingModule_H__
