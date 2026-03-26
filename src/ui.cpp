
#include "ui.h"
#include <SFML/Graphics.hpp>
#include <iostream>

HomeScreenUI::HomeScreenUI() {

}

bool HomeScreenUI::show() {
    sf::RenderWindow window(sf::VideoMode({1067, 600}), "DoomSort HomeScreen");

    sf::Font boldFont;
    // you guys can more fonts to the resources folder and change this font if you'd like
    if (!boldFont.openFromFile("../resources/Iosevka_Charon/IosevkaCharon-Bold.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return false;
    }

    // homescreen title font details
    sf::Text titleText(boldFont);
    titleText.setString("DoomScroller");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::White);

    // positioning title text in correct spot (centered and 1/4 way down from top)
    auto windowSize = window.getSize();
    auto res = titleText.getLocalBounds();
    titleText.setPosition({(windowSize.x - res.size.x) / 2.f, (windowSize.y / 4.f) - (res.size.y / 2.f)});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
        }
        window.clear(sf::Color::Black);
        window.draw(titleText);
        window.display();
    }
    return false;
}

ProjectUI::ProjectUI() {

}

void ProjectUI::drawWindow() {

}
