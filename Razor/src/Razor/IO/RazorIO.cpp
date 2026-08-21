#include "RazorIO.h"

namespace Razor
{

	RazorIO* RazorIO::GRazorIO = nullptr;

	OnMouseButtonPressedDelegate& RazorIO::OnMouseButtonPressed()
	{
		return _mOnMouseButtonPressedDelegate;
	}

	OnMouseButtonReleasedDelegate& RazorIO::OnMouseButtonReleased()
	{
		return _mOnMouseButtonReleasedDelegate;
	}

	OnKeyStateChangedDelegate& RazorIO::OnKeyStateChanged()
	{
		return _mOnKeyStateChangedDelegate;
	}

	OnMousePosChangedDelegate& RazorIO::OnMousePosChanged()
	{
		return _mOnMousePosChangedDelegate;
	}

}