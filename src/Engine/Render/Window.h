#ifndef ENGINE_WINDOW_H
#define ENGINE_WINDOW_H

#include <windows.h>

class Window
{
public:
	Window();
	~Window();

	void Create(const char* title, int width, int height);
	void Destroy();
	void Update();

private:
};

#endif // !ENGINE_WINDOW_H