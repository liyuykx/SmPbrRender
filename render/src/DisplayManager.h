#pragma once
#include"WindowManager.h"
#include"drawableObject.h"
#include "GUI.h"
#include<vector>
struct displayItem {
	const char *itemName;
	drawableObject* item;
};
class DisplayManager
{
public:
	//initializer
	bool startUp();
	void run();
	DisplayManager& add(drawableObject& item, const char* itemName);
	void setGui();
	DisplayManager();
	~DisplayManager();
private:
	WindowManager* window;
	GUI* gui;
	int displayID;
	bool success;
	std::vector<displayItem> displayList;
};