#pragma once
#ifndef __SentienceApp_H__
#define __SentienceApp_H__

class SentienceApp
	: public App
{
public:
	SentienceApp(InputManager & InputManager);
	~SentienceApp();

	void UpdateWindowDimensions(Vector2n WindowDimensions) override;

	void ProcessEvent(InputEvent & InputEvent) override;

private:
	SentienceApp(const SentienceApp &);
	SentienceApp & operator = (const SentienceApp &);
};

#endif // __SentienceApp_H__
