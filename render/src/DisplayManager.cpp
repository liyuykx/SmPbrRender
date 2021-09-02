#include "DisplayManager.h"

bool DisplayManager::startUp()
{
    return success;
}

void DisplayManager::run()
{
    while (window->continueLoop())
    {
        window->mouseCapture = ImGui::GetIO().WantCaptureMouse;
        gui->update();
        setGui();
        displayList[displayID].item->display();
        gui->render();
        window->swapBuffersAndPollEvents();
        window->camera->updateCameraPos();
        window->processInput();
       
    }
}

DisplayManager& DisplayManager::add(drawableObject& item, const char *itemName)
{
    displayItem dItem;
    dItem.item = &item;
    dItem.itemName = itemName;
    displayList.push_back(dItem);
    return *this;
}
void DisplayManager::setGui() 
{
  
    ImGui::Begin("displaycontrols");
    ImGui::Text("You can move the view around using the left mouse button ");
    ImGui::Text("and use the WASDQE keys to move left/right/forward/backward/up and down.");
    for (int exampleID = 0; exampleID < displayList.size(); exampleID++) {
        ImGui::RadioButton(displayList[exampleID].itemName, &displayID, exampleID);
    }
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
   

}
// dummy constructor destructor
DisplayManager::DisplayManager()
{
    window = new WindowManager(success, 1200, 900);
    gui = new GUI(*window);
    displayID = 0;
}
DisplayManager::~DisplayManager()
{
    delete gui;
    delete window;
}
