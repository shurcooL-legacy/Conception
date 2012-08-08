#pragma once
#ifndef __PointerMapping_H__
#define __PointerMapping_H__

class PointerMapping
	: public InputResponder
{
public:
	PointerMapping(Pointer & Owner);
	~PointerMapping();

	void AddMapping(GestureRecognizer & GestureRecognizer);
	void DoneAdding();
	void RemoveAllMappings();

	/*void ProcessPointerCreation(Pointer * Pointer);
	void ProcessPointerDeletion(Pointer * Pointer);
	void ProcessPointerActivation(Pointer * Pointer);
	void ProcessPointerDeactivation(Pointer * Pointer);

	void ProcessButton(Pointer * Pointer, Input::InputId ButtonId, bool Pressed);
	void ProcessSlider(Pointer * Pointer, Input::InputId SliderId, double MovedAmount);
	void ProcessAxis(Pointer * Pointer, Input::InputId AxisId, Input::AxisState AxisState);
	void Process2Axes(Pointer * Pointer, Input::InputId FirstAxisId, Input::AxisState AxisState[2]);

	void ProcessCharacter(Pointer * Pointer, int Character);*/

	bool ProcessEvent(InputEvent & InputEvent) override;

	void ProcessTimePassed(const double TimePassed) override;

	void RequestPointerCapture(GestureRecognizer * Candidate);
	void RequestPointerRelease(GestureRecognizer * Candidate);

	GestureRecognizer * GetCapturer() const;
	GestureRecognizer * GetHoverer() const;

	//void RemoveHoverPointer(Pointer * Pointer);

private:
	PointerMapping(const PointerMapping &) = delete;
	PointerMapping & operator =(const PointerMapping &) = delete;

	void ChangeCapturer(GestureRecognizer * Capturer);

	//GestureRecognizer *		m_Hoverer;
	GestureRecognizer *		m_Capturer;

	std::vector<GestureRecognizer *>		m_Entries;

	Pointer &				m_Owner;

	friend class App;		// DEBUG: For debug printing
};

#endif // __PointerMapping_H__
