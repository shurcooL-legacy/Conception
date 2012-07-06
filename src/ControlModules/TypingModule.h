#pragma once
#ifndef __TypingModule_H__
#define __TypingModule_H__

class TypingModule : public ControlModule
{
public:
	TypingModule();
	virtual ~TypingModule();
	
	virtual bool ShouldActivate() const;
	
	//virtual bool IsActiveInternally() { return true; }
	//virtual bool ShouldMouseCursorVisible() { return false; }

	void Render();
	
	std::string GetString();
	void Clear();

protected:
	virtual void ModuleProcessButton(InputManager::VirtualInputId ButtonId, bool Pressed);

	virtual void ModuleProcessCharacter(int Character);

private:
	TypingModule(const TypingModule &);
	TypingModule & operator =(const TypingModule &);
	
	std::string m_Typed;
};

#endif // __TypingModule_H__
