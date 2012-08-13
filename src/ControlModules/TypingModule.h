#pragma once
#ifndef __TypingModule_H__
#define __TypingModule_H__

class TypingModule
	: public GestureListener
{
public:
	TypingModule();
	~TypingModule();

	void Render();

	std::string GetString();
	void Clear();

	void ProcessCharacter(const uint32 Character) override;

	void ProcessEvent(InputEvent & InputEvent) override;

private:
	TypingModule(const TypingModule &);
	TypingModule & operator = (const TypingModule &);

	std::string m_Typed;
};

#endif // __TypingModule_H__
