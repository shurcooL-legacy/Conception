#pragma once
#ifndef __MultitouchTestApp_H__
#define __MultitouchTestApp_H__

class MultitouchTestApp
	: public App
{
public:
	MultitouchTestApp(InputManager & InputManager);
	~MultitouchTestApp();

	void UpdateWindowDimensions(Vector2n WindowDimensions) override;

private:
	MultitouchTestApp(const MultitouchTestApp &);
	MultitouchTestApp & operator =(const MultitouchTestApp &);

	//std::unique_ptr<GestureRecognizer> m_GestureRecognizerTEST;
};

#endif // __MultitouchTestApp_H__
