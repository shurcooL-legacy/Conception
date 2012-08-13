#pragma once
#ifndef __TextFieldWidget_H__
#define __TextFieldWidget_H__

class TextFieldWidget
	: public Widget
{
public:
	TextFieldWidget(Vector2n Position);
	virtual ~TextFieldWidget();

	void Render() override;

	void ProcessTap() override;
	void ProcessCharacter(const uint32 Character) override;

	void ProcessManipulationStarted(const PointerState & PointerState) override;
	void ProcessManipulationUpdated(const PointerState & PointerState) override;
	void ProcessManipulationCompleted(const PointerState & PointerState) override;

	void ProcessEvent(InputEvent & InputEvent) override;

private:
	struct ContentLine
	{
		std::string::size_type			m_StartPosition;
		decltype(m_StartPosition)		m_Length;

		ContentLine(decltype(m_StartPosition) StartPosition, decltype(m_Length) Length)
			: m_StartPosition(StartPosition),
			  m_Length(Length)
		{}
	};

	std::string							m_Content;
	std::vector<ContentLine>			m_ContentLines;
	decltype(ContentLine::m_Length)		m_MaxLineLength;

	std::string::size_type				m_CaretPosition;
	uint32								m_TargetCaretColumnX;
	decltype(m_CaretPosition)			m_SelectionPosition;

	void SetCaretPosition(decltype(m_CaretPosition) CaretPosition, bool ResetSelection, bool UpdateTargetCaretColumn = true);
	void MoveCaret(sint32 MoveAmount, bool ResetSelection);
	void MoveCaretTry(sint32 MoveAmount, bool ResetSelection);
	void MoveCaretVerticallyTry(sint32 MoveAmount, bool ResetSelection);
	std::string GetSelectionContent();
	bool EraseSelectionIfAny();
	void UpdateContentLines();
	uint32 GetCaretPositionX(std::vector<class ContentLine>::size_type LineNumber, std::vector<class ContentLine>::size_type ColumnNumber);
	decltype(m_CaretPosition) GetNearestCaretPosition(Vector2n LocalPosition);
	decltype(m_CaretPosition) GetNearestCaretPosition(std::vector<class ContentLine>::size_type LineNumber, uint32 LocalPositionX);
};

#endif // __TextFieldWidget_H__
