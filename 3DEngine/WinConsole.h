#ifndef __WINCONSOLE_H__
#define __WINCONSOLE_H__

#include "Imgui/imgui.h"
#include "WinBase.h"

class WinConsole :
	public WinBase
{
public:

	WinConsole(Application* parent, bool start_enabled = true);
	~WinConsole();

	bool Update();

	void Draw();

	void Clear();
	void AddLog(const char* entry);
	void AddNumLog(int number);

private:

	ImGuiTextBuffer Buffer;
	bool ScrollToBottom = false;
	int width;
	int height;
	int posx;
	int posy;
	bool active = true;
};

#endif  // __WINCONSOLE_H__