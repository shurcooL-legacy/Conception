#pragma once
#ifndef __TextFieldWidget_H__
#define __TextFieldWidget_H__

class TextFieldWidget
  : public CompositeWidget,
	public MutuallyConnectable<TextFieldWidget, ConnectionWidget<TextFieldWidget>>
{
private:
	std::string							m_Content;

	decltype(m_Content)::size_type		m_CaretPosition;
	decltype(m_CaretPosition)			m_SelectionPosition;
	uint32								m_TargetCaretColumnX;

	struct ContentLine
	{
		decltype(m_CaretPosition)		m_StartPosition;
		decltype(m_StartPosition)		m_Length;				// In number of items
		// DEBUG: Disabled because it's just an optimization
		//decltype(m_StartPosition)		m_ExpandedLength;		// In physical space taken up by characters

		ContentLine(decltype(m_StartPosition) StartPosition, decltype(m_Length) Length)
			: m_StartPosition(StartPosition),
			  m_Length(Length)
		{}
	};

	std::vector<ContentLine>			m_ContentLines;
	decltype(ContentLine::m_Length)		m_MaxLineLength;

	TypingModule &						m_TypingModule;

	Color								m_BackgroundColor = Color::WHITE;

public:
	std::function<void()>													m_OnChange = nullptr;
	std::function<std::vector<std::string>(decltype(m_CaretPosition))>		m_GetAutocompletions = nullptr;
	std::function<Color(uint32, const std::string &)>						m_GetLineHighlighting = nullptr;
	std::function<std::string(uint32)>										m_GetLineGutters = nullptr;
	bool																	m_GolangHighlightHighLevel = true;

	const ToggleWidget * m_MinimizeToggle = nullptr;

	TextFieldWidget(Vector2n Position, TypingModule & TypingModule);
	~TextFieldWidget();

	void Render() override;

	void ProcessTap(const InputEvent & InputEvent, Vector2n Position) override;
	void ProcessDoubleTap(const InputEvent & InputEvent, Vector2n Position) override;
	void ProcessCharacter(InputEvent & InputEvent, const uint32 Character) override;

	void ProcessEvent(InputEvent & InputEvent) override;

	void ProcessTimePassed(const double TimePassed) override;

	const Vector2n GetDimensions() const override;

	// TODO: Generalize this to all automate-able widgets?
	void NotifyChange(bool OverrideLiveToggle = false);		// Called when this widget changes, to notify its connected widgets

	std::string GetContent() const;
	void SetContent(std::string Content);
	void AppendContent(std::string ExtraContent);

	decltype(m_CaretPosition) GetCaretPosition() const;

	void SetBackground(Color BackgroundColor);
	Color GetBackground() const;

	Color m_Foreground = Color::BLACK;

	bool m_Private = false;

protected:

private:
	TextFieldWidget(const TextFieldWidget &) = delete;
	TextFieldWidget & operator = (const TextFieldWidget &) = delete;

	void UpdateGestureRecognizer();

	void SetCaretPosition(decltype(m_CaretPosition) CaretPosition, bool ResetSelection, bool UpdateTargetCaretColumn = true);
	void MoveCaret(sint32 MoveAmount, bool ResetSelection);
	void MoveCaretTry(sint32 MoveAmount, bool ResetSelection);
	void MoveCaretVerticallyTry(sint32 MoveAmount, bool ResetSelection);
	void MoveCaretHorizontallyLeft(bool SuperActive, bool AltActive, bool ShiftActive);
	void MoveCaretHorizontallyRight(bool SuperActive, bool AltActive, bool ShiftActive);
	decltype(m_CaretPosition) GetSelectionLength() const;
	std::string GetSelectionContent() const;
	bool EraseSelectionIfAny();
	void UpdateContentLines();
	uint32 GetCaretPositionX(std::vector<ContentLine>::size_type LineNumber, std::vector<ContentLine>::size_type ColumnNumber) const;
	decltype(m_CaretPosition) GetNearestCaretPosition(Vector2n LocalPosition) const;
	decltype(m_CaretPosition) GetNearestCaretPosition(std::vector<ContentLine>::size_type LineNumber, uint32 LocalPositionX) const;
	void GetLineAndColumnNumber(std::vector<ContentLine>::size_type & LineNumber, std::vector<ContentLine>::size_type & ColumnNumber) const;
	std::vector<ContentLine>::size_type GetLineNumber() const;
	const Vector2n GetCaretLocalPosition() const;
	uint32 GetLeadingTabCount() const;
	uint32 FindLineThatStartsWith(const std::string Target, const uint32 StartingPoint = 0);

	bool IsNotMinimized() const;

	static bool IsCoreCharacter(uint8 Character);
};

#endif // __TextFieldWidget_H__
