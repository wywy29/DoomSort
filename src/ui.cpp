
#include "ui.h"
#include <SFML/Graphics.hpp>
#include <iostream>
// transition between homescreen and project (only really works for black background to black background for now)
void fadeOutEffect(sf::RenderWindow& window) {
    sf::Texture currentWin(window.getSize());
    currentWin.update(window);

    sf::Sprite screen(currentWin);

    sf::RectangleShape fade(sf::Vector2f(window.getSize()));

    for (int i = 0; i <= 255; i += 15) {
        fade.setFillColor(sf::Color(0, 0, 0, i));
        window.clear();
        window.draw(screen);
        window.draw(fade);
        window.display();
        sf::sleep(sf::milliseconds(30));
    }
}
// second half of the transition, WIP/prob not needed
void fadeInEffect(sf::RenderWindow& window) {
    sf::Texture currentWin(window.getSize());
    currentWin.update(window);

    sf::Sprite screen(currentWin);

    sf::RectangleShape fade(sf::Vector2f(window.getSize()));

    for (int i = 255; i >= 0; i -= 15) {
        fade.setFillColor(sf::Color(0, 0, 0, i));
        window.clear();
        window.draw(screen);
        window.draw(fade);
        window.display();
        sf::sleep(sf::milliseconds(30));
    }
}

bool HomeScreenUI::show(sf::RenderWindow& window) {

    sf::Font boldFont;
    // you guys can more fonts to the resources folder and change this font if you'd like
    if (!boldFont.openFromFile("../resources/Iosevka_Charon/IosevkaCharon-Bold.ttf")) {
        std::cerr << "error" << std::endl;
        return false;
    }
    sf::Font regFont;
    if (!regFont.openFromFile("../resources/Iosevka_Charon/IosevkaCharon-Regular.ttf")) {
        std::cerr << "error" << std::endl;
        return false;
    }

    // homescreen title font details
    sf::Text titleText(boldFont);
    titleText.setString("DoomSort");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);

    sf::Text captionText(regFont);
    // lol
    captionText.setString("How brainrotted are you?");
    captionText.setCharacterSize(40);
    captionText.setFillColor(sf::Color::White);

    sf::Text continueButton(regFont);
    continueButton.setString("FIND OUT");
    continueButton.setCharacterSize(25);
    continueButton.setFillColor(sf::Color::White);

    // positioning title text in correct spot (centered and 1/4 way down from top)
    auto windowSize = window.getSize();
    auto res = titleText.getLocalBounds();
    titleText.setPosition({(windowSize.x - res.size.x) / 2.f, (windowSize.y / 4.f) - (res.size.y / 2.f)});

    // positioning caption text in correct spot (right below title text)
    captionText.setPosition({(windowSize.x - captionText.getLocalBounds().size.x) / 2.f, titleText.getPosition().y + res.size.y + windowSize.y/30});

    // positioning continue button in correct spot (decently below caption text)
    auto buttonBounds = continueButton.getLocalBounds();
    continueButton.setPosition({(windowSize.x - buttonBounds.size.x) / 2.f, titleText.getPosition().y + res.size.y + windowSize.y/4});


    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()){
                window.close();
            }

            sf::Vector2i pos = sf::Mouse::getPosition(window);
            if (continueButton.getGlobalBounds().contains(sf::Vector2f(pos))) {
                continueButton.setFillColor(sf::Color::Red);

                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    fadeOutEffect(window);
                    return true;
                }
            }
            else {
                continueButton.setFillColor(sf::Color::White);
            }

        }
        window.clear(sf::Color::Black);
        window.draw(titleText);
        window.draw(captionText);
        window.draw(continueButton);
        window.display();
    }
    return false;
}

void ProjectUI::drawWindow(sf::RenderWindow& window) {

    sf::Font font;
    if (!font.openFromFile("../resources/Iosevka_Charon/IosevkaCharon-Regular.ttf")) {
        std::cerr << "error" << std::endl;
        return;
    }

    sf::Text titleText(font);
    titleText.setString("DoomSort");
    titleText.setCharacterSize(45);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 1.1));

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
}
