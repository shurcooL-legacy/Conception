#pragma once
#ifndef __TextFieldWidget_H__
#define __TextFieldWidget_H__

class TextFieldWidget
	: public Widget
{
public:
	TextFieldWidget(Vector2n Position, TypingModule & TypingModule);
	virtual ~TextFieldWidget();

	void Render() override;

	void ProcessTap(InputEvent & InputEvent, Vector2n Position) override;
	void ProcessDoubleTap(InputEvent & InputEvent, Vector2n Position) override;
	void ProcessCharacter(InputEvent & InputEvent, const uint32 Character) override;

	void ProcessManipulationStarted(const PointerState & PointerState) override;
	void ProcessManipulationUpdated(const PointerState & PointerState) override;
	void ProcessManipulationCompleted(const PointerState & PointerState) override;

	void ProcessEvent(InputEvent & InputEvent) override;

	std::string GetContent() const;
	void SetContent(std::string Content);
	void AppendContent(std::string ExtraContent);

	void SetBackground(Color BackgroundColor);

	std::function<void()>				m_OnChange;

private:
	std::string							m_Content;

	decltype(m_Content)::size_type		m_CaretPosition;
	decltype(m_CaretPosition)			m_SelectionPosition;
	uint32								m_TargetCaretColumnX;

	struct ContentLine
	{
		decltype(m_CaretPosition)		m_StartPosition;
		decltype(m_StartPosition)		m_Length;

		ContentLine(decltype(m_StartPosition) StartPosition, decltype(m_Length) Length)
			: m_StartPosition(StartPosition),
			  m_Length(Length)
		{}
	};

	std::vector<ContentLine>			m_ContentLines;
	decltype(ContentLine::m_Length)		m_MaxLineLength;

	TypingModule &						m_TypingModule;

	Color								m_BackgroundColor;

	void SetCaretPosition(decltype(m_CaretPosition) CaretPosition, bool ResetSelection, bool UpdateTargetCaretColumn = true);
	void MoveCaret(sint32 MoveAmount, bool ResetSelection);
	void MoveCaretTry(sint32 MoveAmount, bool ResetSelection);
	void MoveCaretVerticallyTry(sint32 MoveAmount, bool ResetSelection);
	std::string GetSelectionContent() const;
	bool EraseSelectionIfAny();
	void UpdateContentLines();
	uint32 GetCaretPositionX(std::vector<ContentLine>::size_type LineNumber, std::vector<ContentLine>::size_type ColumnNumber);
	decltype(m_CaretPosition) GetNearestCaretPosition(Vector2n LocalPosition);
	decltype(m_CaretPosition) GetNearestCaretPosition(std::vector<ContentLine>::size_type LineNumber, uint32 LocalPositionX);
	void GetLineAndColumnNumber(std::vector<ContentLine>::size_type & LineNumber, std::vector<ContentLine>::size_type & ColumnNumber) const;
	std::vector<ContentLine>::size_type GetLineNumber() const;
	uint32 GetLeadingTabCount() const;

	static bool IsCoreCharacter(uint8 Character);
};

#endif // __TextFieldWidget_H__
