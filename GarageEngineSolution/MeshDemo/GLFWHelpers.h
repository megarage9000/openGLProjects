
#pragma once

class GLFWwindow;

class GLFWHelper
{
public:
	GLFWHelper(const GLFWwindow* window);
private:
	GLFWwindow* windowRef;

};

