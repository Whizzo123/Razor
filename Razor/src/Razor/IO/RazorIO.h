#pragma once

#include "../Utils/Delegates.h"
#include <unordered_map>

enum class RazorKey
{
	A,
	D,
	S,
	W,
	Escape
};

enum RazorKeyState
{
	KEY_RELEASED,
	KEY_PRESSED
};

enum RazorMouseButton
{
	LEFT,
	RIGHT
};

enum RazorMouseState
{
	MOUSE_UP, 
	MOUSE_DOWN
};

using OnMouseButtonPressedDelegate = MulticastDelegate<RazorMouseButton, RazorMouseState>;
using OnMouseButtonReleasedDelegate = MulticastDelegate<int>;
using OnKeyStateChangedDelegate = MulticastDelegate<RazorKey, RazorKeyState>;
using OnMousePosChangedDelegate = MulticastDelegate<double, double>;

struct Vector2D
{
	double X;
	double Y;
};

class RazorIO
{
public:
	static RazorIO& Get()
	{
		if (GRazorIO == nullptr)
		{
			GRazorIO = new RazorIO();
		}
		return *GRazorIO;
	}

	RazorKeyState GetStateForKey(RazorKey Key) { return Keyboard[Key]; }
	RazorMouseState GetStateForMouseButton(RazorMouseButton Button) { return MouseButtons[Button]; }

	// Alter to be multi-cast in here multiple things might want to know if mouse button is pressed
	OnMouseButtonPressedDelegate& OnMouseButtonPressed();
	OnMouseButtonReleasedDelegate& OnMouseButtonReleased();
	OnKeyStateChangedDelegate& OnKeyStateChanged();
	OnMousePosChangedDelegate& OnMousePosChanged();
	Vector2D LastMousePos;
	Vector2D CurrentMousePos = {400, 300};
protected:
	RazorIO() 
	{
		OnMousePosChanged().AddRaw(this, &RazorIO::UpdateMousePositions);
		OnKeyStateChanged().AddRaw(this, &RazorIO::UpdateKeyStates);
		OnMouseButtonPressed().AddRaw(this, &RazorIO::UpdateMouseStates);
	};
	~RazorIO() {};
	std::unordered_map<RazorKey, RazorKeyState> Keyboard;
	std::unordered_map<RazorMouseButton, RazorMouseState> MouseButtons;
	
private:
	static RazorIO* GRazorIO;
	
	//DELEGATES
	OnMouseButtonPressedDelegate OnMouseButtonPressedDelegate;
	OnMouseButtonReleasedDelegate OnMouseButtonReleasedDelegate;
	OnKeyStateChangedDelegate OnKeyStateChangedDelegate;
	OnMousePosChangedDelegate OnMousePosChangedDelegate;

	// May want to move the LastMousePos calculation to inndividual systems as not really an IO thing to do
	void UpdateMousePositions(double Xpos, double Ypos)
	{
		CurrentMousePos.X = Xpos;
		CurrentMousePos.Y = Ypos;
	}

	void UpdateKeyStates(RazorKey Key, RazorKeyState State)
	{
		Keyboard[Key] = State;
	}

	void UpdateMouseStates(RazorMouseButton MouseButton, RazorMouseState State)
	{
		MouseButtons[MouseButton] = State;
	}
	bool bIsFirstMouse = true;
};

