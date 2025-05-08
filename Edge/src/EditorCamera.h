#pragma once

#include "Razor.h"

namespace EdgeEditor
{
	/**
	* Class to hold Camera object to allow control by the user of the editor
	*/
	class EditorCamera
	{
	public:
		/**
		* Processes all input for the Camera
		* 
		* @param dt - A float representing DeltaTime
		*/
		void ProcessInput(float dt);
		/**
		* Getter function for the Camera member
		* 
		* @return A reference to the Camera member
		*/
		Razor::Camera& GetCamera();
	private:
		Razor::Camera Camera; /** Camera to be used for the editor rendering of the scene*/

		float MouseLastX = 400; /** Last recorded x-coordinates of the mouse */
		float MouseLastY = 300; /** Last recorded y-coordinates of the mouse */
		float Yaw = -90.0f; /** Yaw value for the camera */
		float Pitch = 0.0f; /** Pitch value for the camera*/
		bool bIsFirstFrameDown = true; /** Whether this is the first frame that we have clicked */
	};
}

