#pragma once
#ifndef __GestureRecognizer_H__
#define __GestureRecognizer_H__

class GestureRecognizer
	: public InputListener,
	  public MutuallyConnectable<GestureRecognizer, Pointer>
{
public:
	GestureRecognizer(GestureListener & Owner);
	~GestureRecognizer();

	void ProcessEvent(InputEvent & InputEvent) override;

	virtual void ProcessCanvasUpdated();

	GestureListener & GetOwner();

	bool				m_RecognizeTap;
	bool				m_RecognizeManipulationTranslate;

	Vector2n			m_ManipulationOffset;

private:
	GestureRecognizer(const GestureRecognizer &);
	GestureRecognizer & operator =(const GestureRecognizer &);

	PointerState		m_LastTapStateTEST;

	bool				m_InManipulation;

	GestureListener &			m_Owner;
};

#endif // __GestureRecognizer_H__
