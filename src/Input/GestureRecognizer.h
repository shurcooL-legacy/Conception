#pragma once
#ifndef __GestureRecognizer_H__
#define __GestureRecognizer_H__

class GestureRecognizer
	: public InputHandler,
	  public MutuallyConnectable<GestureRecognizer, Pointer>
{
public:
	GestureRecognizer(GestureHandler & Owner);
	~GestureRecognizer();

	void ProcessEvent(InputEvent & InputEvent) override;

	virtual void ProcessCanvasUpdated();

	GestureHandler & GetOwner();

	bool				m_RecognizeTap;
	bool				m_RecognizeDoubleTap;
	bool				m_RecognizeManipulationTranslate;

	Vector2n			m_ManipulationOffset;

private:
	GestureRecognizer(const GestureRecognizer &);
	GestureRecognizer & operator = (const GestureRecognizer &);

	PointerState		m_LastTapStateTEST;
	PointerState		m_LastTapCompletedStateTEST;

	bool				m_InManipulation;

	GestureHandler &			m_Owner;
};

#endif // __GestureRecognizer_H__
