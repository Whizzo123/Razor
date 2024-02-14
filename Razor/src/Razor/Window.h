#pragma once

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <iostream>
#include "Core.h"


class RAZOR_API Window
{
public:
    Window(int width, int height);
    ~Window();
    GLFWwindow* GetWindowPtr() const;

    int GetWidth() const{ return Width; }
    int GetHeight() const{ return Height; }

protected:
    GLFWwindow* window;
    int Width;
    int Height;
};

