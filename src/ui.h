#pragma once
#include <SFML/Graphics.hpp>

// simple home screen, will just contain a button to go to the actual project
class HomeScreenUI {
    public:
    HomeScreenUI() = default;
    bool show(sf::RenderWindow& window);
};

// actual project, will contain the blobs, buttons, popups, etc.
class ProjectUI {
    public:
    ProjectUI() = default;
    void drawWindow(sf::RenderWindow& window, std::vector<float> screenTimes);
};