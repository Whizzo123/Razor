#include "RazorIO.h"

RazorIO* RazorIO::GRazorIO = nullptr;

OnMouseButtonPressedDelegate& RazorIO::OnMouseButtonPressed()
{
	return OnMouseButtonPressedDelegate;
}

OnMouseButtonReleasedDelegate& RazorIO::OnMouseButtonReleased()
{
	return OnMouseButtonReleasedDelegate;
}

OnKeyStateChangedDelegate& RazorIO::OnKeyStateChanged()
{
	return OnKeyStateChangedDelegate;
}

OnMousePosChangedDelegate& RazorIO::OnMousePosChanged()
{
	return OnMousePosChangedDelegate;
}