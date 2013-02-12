#pragma once
#ifndef __ConceptionTestApp_H__
#define __ConceptionTestApp_H__

class ConceptionTestApp
	: public App
{
public:
	ConceptionTestApp(InputManager & InputManager);
	~ConceptionTestApp();

	void UpdateWindowDimensions(Vector2n WindowDimensions) override;

private:
	ConceptionTestApp(const ConceptionTestApp &);
	ConceptionTestApp & operator = (const ConceptionTestApp &);

	public:Project		m_CurrentProject;private:

	TypingModule	m_TypingModule;
};

#endif // __ConceptionTestApp_H__
