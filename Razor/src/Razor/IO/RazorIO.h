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

using OnMouseButtonPressedDelegate = Delegate<bool, int>;
using OnMouseButtonReleasedDelegate = Delegate<bool, int>;
using OnKeyStateChangedDelegate = Delegate<void, RazorKey, RazorKeyState>;
using OnMousePosChangedDelegate = Delegate<void, double, double>;

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

	// Alter to be multi-cast in here multiple things might want to know if mouse button is pressed
	OnMouseButtonPressedDelegate& OnMouseButtonPressed();
	OnMouseButtonReleasedDelegate& OnMouseButtonReleased();
	OnKeyStateChangedDelegate& OnKeyStateChanged();
	OnMousePosChangedDelegate& OnMousePosChanged();
	Vector2D LastMousePos;
	Vector2D CurrentMousePos = {400, 300};
protected:
	RazorIO() { OnMousePosChanged().BindRaw(this, &RazorIO::UpdateMousePositions); OnKeyStateChanged().BindRaw(this, &RazorIO::UpdateKeyStates);
	};
	~RazorIO() {};
	std::unordered_map<RazorKey, RazorKeyState> Keyboard;
	
	
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
	bool bIsFirstMouse = true;
};

