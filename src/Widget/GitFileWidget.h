#pragma once
#ifndef __GitFileWidget_H__
#define __GitFileWidget_H__

class GitFileWidget
	: public CompositeWidget,
	  public MutuallyConnectable<GitFileWidget, ConnectionWidget<GitFileWidget>>
{
public:
	GitFileWidget(Vector2n Position, std::string Path, TypingModule & TypingModule);
	~GitFileWidget();

	// TODO: Generalize this to all automate-able widgets?
	void NotifyChange(bool OverrideLiveToggle = false);				// Called when this widget changes, to notify its connected widgets
	void NotifyExternalChange(bool OverrideLiveToggle = false);		// Called when an external dependency changes, to notify its connected widgets

	std::function<void()> m_OnChange = nullptr;

private:
	GitFileWidget(const GitFileWidget &) = delete;
	GitFileWidget & operator = (const GitFileWidget &) = delete;

	public:TextFileWidget * m_TextFileWidget;private:		// HACK?
	GitDiffWidget * m_GitDiffWidget;
};

#endif // __GitFileWidget_H__
