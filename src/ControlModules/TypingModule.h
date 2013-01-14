#pragma once
#ifndef __TypingModule_H__
#define __TypingModule_H__

class TypingModule
	: public Widget
{
public:
	TypingModule();
	~TypingModule();

	void Render() override;
	void Render(const InputManager & InputManager);

	Vector2n GetInsertionPosition(Vector2n PointerPosition) const;

	std::string GetString();
	std::string TakeString();
	void SetString(std::string String);

	//bool IsHit(const Vector2n ParentPosition) const override;

	void ProcessEvent(InputEvent & InputEvent) override;
	void ProcessCharacter(InputEvent & InputEvent, const uint32 Character) override;

private:
	TypingModule(const TypingModule &);
	TypingModule & operator = (const TypingModule &);

	std::string m_Typed;
};

#endif // __TypingModule_H__
