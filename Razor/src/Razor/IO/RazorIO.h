#pragma once

#include "../Utils/Delegates.h"

using OnMouseButtonPressedDelegate = Delegate<bool, int>;
using OnMouseButtonReleasedDelegate = Delegate<bool, int>;

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

	// Alter to be multi-cast in here multiple things might want to know if mouse button is pressed
	OnMouseButtonPressedDelegate& OnMouseButtonPressed();
	OnMouseButtonReleasedDelegate& OnMouseButtonReleased();

	// TODO  Now you must do the other half which is creating the OpenGL side which will capture these events and pass them to RazorIO so that the events can be called
	// and passed onto their subscribers
protected:
	RazorIO() {};
	~RazorIO() {};

private:
	static RazorIO* GRazorIO;
	
	//DELEGATES
	OnMouseButtonPressedDelegate OnMouseButtonPressedDelegate;
	OnMouseButtonReleasedDelegate OnMouseButtonReleasedDelegate;
};

