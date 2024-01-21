#pragma once

#include "../Utils/Delegates.h"

using OnMouseButtonPressedDelegate = Delegate<bool, int>;

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


protected:
	RazorIO() {};
	~RazorIO() {};

private:
	static RazorIO* GRazorIO;
	
	//DELEGATES
	OnMouseButtonPressedDelegate OnMouseButtonPressedDelegate;
};

