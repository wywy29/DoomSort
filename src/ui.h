#pragma once
#include <SFML/Graphics.hpp>

// simple home screen, will just contain a button to go to the actual project
class HomeScreenUI {
    public:
    HomeScreenUI();
    bool show();
};

// actual project, will contain the blobs, buttons, popups, etc.
class ProjectUI {
    public:
    ProjectUI();
    void drawWindow();
};