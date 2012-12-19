#pragma once
#ifndef __Main_H__
#define __Main_H__

#if defined(_MSC_VER)
#	pragma warning( disable : 4351 )
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include <list>
#include <forward_list>
#include <cmath>
#include <string>
#include <vector>
#include <cassert>
#include <stdio.h>
#include <string.h>
#include <array>
#include <memory>
#include <cstdlib>

#ifdef WIN32
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>		// Needed for creating a process to compile projects
#endif // WIN32

#if defined(__APPLE__) && defined(__MACH__)
#	include <sys/types.h>
#	include <unistd.h>
#	include <signal.h>
#	include <fcntl.h>
#endif

#include <GL/glfw.h>

#ifdef WIN32
	typedef signed __int8		sint8;
	typedef signed __int16		sint16;
	typedef signed __int32		sint32;
	typedef signed __int64		sint64;
	typedef unsigned __int8		uint8;
	typedef unsigned __int16	uint16;
	typedef unsigned __int32	uint32;
	typedef unsigned __int64	uint64;
//#elif defined(__APPLE__) && defined(__MACH__)
#else		// For __APPLE__ and Linux
	typedef int8_t				sint8;
	typedef int16_t				sint16;
	typedef int32_t				sint32;
	typedef int64_t				sint64;
	typedef uint8_t				uint8;
	typedef uint16_t			uint16;
	typedef uint32_t			uint32;
	typedef uint64_t			uint64;
#endif // WIN32

using std::endl;

#ifdef WIN32
namespace std
{
	long lround(double arg);
}
#endif // WIN32

// Decisions
#define DECISION_POINTER_MAPPING_CONTAINS_SINGLE_TOPMOST_WIDGET 0
#define DECISION_USE_CLIPBOARD_INSTEAD_OF_TypingModule 1
#define DECISION_CONCEPTS_DISPLAYED_SMALL 1
#define DECISION_USE_CPP_INSTEAD_OF_GO 0

class Concept;
class ConceptBasic;
class ConceptCompound;
class ConceptParameterized;
class ConceptInstance;
class Function;
class Project;
class InputEvent;
class InputEventQueue;
class InputManager;
class InputHandler;
class InputListener;
class PointerInputListener;
class ControlModuleMapping;
class ControlModule;
class UnrealCameraModule;
class TypingModule;
class WidgetModule;
class WidgetManager;
class PointerMapping;
class Widget;
class CompositeWidget;
class FlowLayoutWidget;
class ButtonWidget;
class MultitouchTestBoxWidget;
class TextFieldWidget;
class ConceptStringBoxWidget;
class FunctionWidget;
class LifeFormWidget;
class OpenGLStream;
class Canvas;
class App;
class ConceptionApp;
class ConceptionTestApp;
class MultitouchTestApp;
class SentienceApp;
class Pointer;
class MousePointer;
class TouchPointer;
class TypingPointer;
class GestureRecognizer;
class GestureHandler;
class PointerState;
class Color;

typedef std::vector<Concept>::size_type ConceptId;

namespace Input
{
	typedef uint16 InputId;

	class AxisState
	{
	public:
		AxisState(sint32 Position, sint32 Length)
			: m_Position(Position),
			  m_Length(Length)
		{
		}
		AxisState()
			: AxisState(0, 0)
		{
		}

		sint32 GetPosition() const { return m_Position; }
		sint32 GetLength() const { return m_Length; }

	private:
		sint32 m_Position;
		sint32 m_Length;
	};
};

#include "Vector2.h"

template <typename T> class Rectangle
{
private:
	Vector2<T> m_Position;
	Vector2<T> m_Dimensions;

public:
	Rectangle(Vector2<T> Position, Vector2<T> Dimensions)
		: m_Position(Position),
		  m_Dimensions(Dimensions)
	{
	}
	Rectangle()
		: Rectangle(Vector2<T>::ZERO, Vector2<T>::ZERO)
	{
	}

	const decltype(m_Position) GetPosition() const { return m_Position; }
	const decltype(m_Dimensions) GetDimensions() const { return m_Dimensions; }
	decltype((m_Position)) SetPosition() { return m_Position; }
	decltype((m_Dimensions)) SetDimensions() { return m_Dimensions; }
};

typedef Rectangle<sint32> Rectanglen;
typedef Rectangle<double> Rectangled;

extern InputManager * g_InputManager;

#include "Utilities.h"
#include "MutuallyConnectable.h"
#include "ogl_utils.h"
#include "RenderTools.h"
#include "OpenGLStream.h"
#include "Concept.h"
#include "Conception/Function.h"
#include "Thread.h"
#include "Project.h"
#include "Input/PointerInputListener.h"
#include "Input/InputHandler.h"
#include "Input/PointerMapping.h"
#include "Input/InputListener.h"
#include "Input/GestureHandler.h"
#include "Input/PointerState.h"
#include "Input/InputEvent.h"
#include "Input/InputEventQueue.h"
#include "Input/Pointers/Pointer.h"
#include "Input/Pointers/MousePointer.h"
#include "Input/Pointers/TouchPointer.h"
#include "Input/Pointers/TypingPointer.h"
#include "Input/InputManager.h"
#include "Input/GestureRecognizer.h"
#include "ControlModules/TypingModule.h"
#include "Behavior/Behavior.h"
#include "Behavior/DraggablePositionBehavior.h"
#include "Behavior/NonDraggablePositionBehavior.h"
#include "Widget/Widget.h"
#include "Widget/CompositeWidget.h"
#include "Widget/FlowLayoutWidget.h"
#include "Widget/LabelWidget.h"
#include "Widget/ButtonWidget.h"
#include "Widget/MultitouchTestBoxWidget.h"
#include "Widget/TextFieldWidget.h"
#include "Widget/ConceptStringBoxWidget.h"
#include "Widget/ListWidget.h"
#include "Widget/ContextMenuWidget.h"
#include "Widget/FunctionWidget.h"
#include "Widget/LifeFormWidget.h"
#include "Widget/LiveProgramWidget.h"
#include "Widget/LiveFunctionWidget.h"
#include "Canvas.h"
#include "App.h"
#include "ConceptionApp.h"
#include "LiveEditorApp.h"
#include "ConceptionTestApp.h"
#include "MultitouchTestApp.h"
#include "SentienceApp.h"

#ifdef WIN32
#elif defined(__APPLE__) && defined(__MACH__)
#endif

#endif // __Main_H__
