#pragma once
#include <SFML/Graphics.hpp>

//Simple home screen, which will just contain a button to go to the actual project
class HomeScreenUI {
    public:
    HomeScreenUI() = default;
    bool show(sf::RenderWindow& window);
};

//The actual project, which will contain the blobs, buttons, popups, etc.
class ProjectUI {
    public:
    ProjectUI() = default;
    void drawWindow(sf::RenderWindow& window, std::vector<float> screenTimes);
};

//Blob class declaration
struct Blob {
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float radius;

    Blob() : radius(0.f) {
    }

    Blob(float radius, sf::Vector2f pos, sf::Vector2f velocity);
};
