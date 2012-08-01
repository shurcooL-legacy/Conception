#pragma once
#ifndef __ConceptionApp_H__
#define __ConceptionApp_H__

class ConceptionApp
	: public App
{
public:
	ConceptionApp(InputManager & InputManager);
	~ConceptionApp();

	void UpdateWindowDimensions(Vector2n WindowDimensions) override;

	bool ProcessEvent(InputEvent & InputEvent) override;

private:
	ConceptionApp(const ConceptionApp &);
	ConceptionApp & operator = (const ConceptionApp &);

	Project		m_CurrentProject;
};

#endif // __ConceptionApp_H__
