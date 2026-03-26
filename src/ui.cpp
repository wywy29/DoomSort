
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

    // the text at the top of the program window
    sf::Text titleText(font);
    titleText.setString("DoomSort");
    titleText.setCharacterSize(45);
    titleText.setFillColor(sf::Color::White);
    auto res = titleText.getLocalBounds();
    titleText.setPosition(sf::Vector2f((window.getSize().x - res.size.x) / 2.f, window.getSize().y / 27.f));


    auto windowSize = window.getSize();

    // info for the left box surrounding where user can input info
    sf::Vector2f userInputBoxSize(windowSize.x * 0.2f, windowSize.y * 0.8f);
    sf::RectangleShape userInputBox(userInputBoxSize);
    userInputBox.setPosition({windowSize.x * .125f - userInputBoxSize.x / 2.f, (windowSize.y - userInputBoxSize.y) / 2.f + window.getSize().y / 36.f});
    userInputBox.setFillColor(sf::Color::Black);
    userInputBox.setOutlineColor(sf::Color::White);
    userInputBox.setOutlineThickness(2);

    // lets the user know what to input (top of left box)
    // split into prompt1 and prompt2 to fit inside the left box
    sf::Text prompt1(font);
    sf::Text prompt2(font);
    prompt1.setString("What is your average daily");
    prompt2.setString("screen time on your phone?");
    prompt1.setCharacterSize(20);
    prompt2.setCharacterSize(20);
    prompt1.setFillColor(sf::Color::White);
    prompt2.setFillColor(sf::Color::White);
    auto leftBoxPos = userInputBox.getPosition();
    auto promptBounds = prompt1.getLocalBounds();
    prompt1.setPosition({leftBoxPos.x + (userInputBoxSize.x - prompt1.getLocalBounds().size.x) / 2.f, leftBoxPos.y + window.getSize().y / 25.f});
    prompt2.setPosition({leftBoxPos.x + (userInputBoxSize.x - prompt2.getLocalBounds().size.x) / 2.f, prompt1.getPosition().y + promptBounds.size.y + 4.f});

    // info for the right box surrounding the blobs
    sf::Vector2f blobBoundsSize(windowSize.x * 0.7f, windowSize.y * 0.8f);
    sf::RectangleShape blobBounds(blobBoundsSize);
    blobBounds.setPosition({windowSize.x * .63f - blobBoundsSize.x / 2.f, (windowSize.y - blobBoundsSize.y) / 2.f + window.getSize().y / 36});
    blobBounds.setOutlineColor(sf::Color::White);
    blobBounds.setFillColor(sf::Color::Black);
    blobBounds.setOutlineThickness(3);

    // the hours and minutes boxes for the user to input their screen time (inside left box)
    sf::RectangleShape userHours(sf::Vector2f(userInputBoxSize.x * .4f, 35.f));
    sf::RectangleShape userMinutes(sf::Vector2f(userInputBoxSize.x * .4f, 35.f));

    // lot of repetition so im making an x and y for the setPosition function
    float inputX = leftBoxPos.x + (userInputBoxSize.x - userHours.getSize().x) / 2.f;
    float inputY = prompt2.getPosition().y + prompt2.getLocalBounds().size.y + 20.f;

    userHours.setPosition({inputX, inputY});
    userMinutes.setPosition({inputX, inputY + window.getSize().y / 20});
    userHours.setFillColor(sf::Color::Black);
    userMinutes.setFillColor(sf::Color::Black);
    userHours.setOutlineColor(sf::Color::White);
    userMinutes.setOutlineColor(sf::Color::White);
    userHours.setOutlineThickness(2);
    userMinutes.setOutlineThickness(2);
    // determines if a box is clicked
    bool hoursClicked = false;
    bool minutesClicked = false;



    // the text that lets user know what each box is for
    sf::Text hoursLabel(font);
    sf::Text minutesLabel(font);
    hoursLabel.setString("hours");
    minutesLabel.setString("minutes");
    hoursLabel.setCharacterSize(14);
    minutesLabel.setCharacterSize(14);
    hoursLabel.setFillColor(sf::Color(128, 128, 128));
    minutesLabel.setFillColor(sf::Color(128, 128, 128));
    hoursLabel.setPosition({inputX + window.getSize().x/ 32, inputY + window.getSize().x/200});
    minutesLabel.setPosition({inputX + window.getSize().x / 35, inputY + window.getSize().y / 17 });

    // the text of the user's input
    sf::Text userHoursText(font);
    sf::Text userMinutesText(font);
    userHoursText.setString(hoursLabel.getString());
    userMinutesText.setString(minutesLabel.getString());
    userHoursText.setCharacterSize(14);
    userMinutesText.setCharacterSize(14);
    userHoursText.setPosition({inputX + window.getSize().x/ 32, inputY + window.getSize().x/200});
    userMinutesText.setPosition({inputX + window.getSize().x / 35, inputY + window.getSize().y / 17 });

    // what the user is typing into
    std::string userHoursInput = "";
    std::string userMinutesInput = "";

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()){
                window.close();
            }

            // change outline color of the user input boxes when clicked
            if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f clickPos(click->position);
                hoursClicked = userHours.getGlobalBounds().contains(clickPos);
                minutesClicked = userMinutes.getGlobalBounds().contains(clickPos);
                if (hoursClicked) {
                    hoursClicked   = true;
                    minutesClicked = false;
                    userHours.setOutlineColor(sf::Color::Red);
                    userMinutes.setOutlineColor(sf::Color::White);
                }
                else if (minutesClicked) {
                    minutesClicked = true;
                    hoursClicked   = false;
                    userMinutes.setOutlineColor(sf::Color::Red);
                    userHours.setOutlineColor(sf::Color::White);
                }
                else {
                    hoursClicked   = false;
                    minutesClicked = false;
                    userHours.setOutlineColor(sf::Color::White);
                    userMinutes.setOutlineColor(sf::Color::White);
                }
            }
            // handles when the user inputs digits into the boxes
            if (const auto* text = event->getIf<sf::Event::TextEntered>()) {
                // 8 is backspace
                if (text->unicode == 8) {
                    if (hoursClicked && !userHoursInput.empty()) {
                        userHoursInput.pop_back();
                    }
                    else if (minutesClicked && !userMinutesInput.empty()) {
                        userMinutesInput.pop_back();
                    }
                }
                else if (text->unicode >= '0' && text->unicode <= '9') {
                    if (hoursClicked) {
                        std::string hrs = userHoursInput + static_cast<char>(text->unicode);
                        if (std::stoi(hrs) <= 24) {
                            userHoursInput = hrs;
                        }
                    }
                    else if (minutesClicked) {
                        std::string mins = userMinutesInput + static_cast<char>(text->unicode);
                        if (std::stoi(mins) <= 59) {
                            userMinutesInput = mins;
                        }
                    }
                }
                if (userHoursInput.empty()) {
                    userHoursText.setString("hours");
                    userHoursText.setFillColor(sf::Color(128,128,128));
                } else {
                    userHoursText.setString(userHoursInput);
                    userHoursText.setFillColor(sf::Color::White);
                }

                if (userMinutesInput.empty()) {
                    userMinutesText.setString("minutes");
                    userMinutesText.setFillColor(sf::Color(128,128,128));
                } else {
                    userMinutesText.setString(userMinutesInput);
                    userMinutesText.setFillColor(sf::Color::White);
                }
            }
        }

        // make the labels disappear when mouse is clicked/hovered over it and changes color of user input boxes when mouse is hovered
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        bool hoursHovered   = userHours.getGlobalBounds().contains(sf::Vector2f(pos));
        bool minutesHovered = userMinutes.getGlobalBounds().contains(sf::Vector2f(pos));

        if (hoursClicked) {
            userHours.setOutlineColor(sf::Color::Red);
        }
        else if (hoursHovered) {
            userHours.setOutlineColor(sf::Color(139,0,0));
        }
        else {
            userHours.setOutlineColor(sf::Color::White);
        }
        if (minutesClicked) {
            userMinutes.setOutlineColor(sf::Color::Red);
        }
        else if (minutesHovered) {
            userMinutes.setOutlineColor(sf::Color(139,0,0));
        }
        else {
            userMinutes.setOutlineColor(sf::Color::White);
        }
        if (userHoursInput.empty()) {
            if (hoursClicked || hoursHovered) {
                userHoursText.setFillColor(sf::Color(0,0,0,0));
            }
            else {
                userHoursText.setFillColor(sf::Color(128,128,128));
            }
        }
        else {
            userHoursText.setFillColor(sf::Color::White);
        }
        if (userMinutesInput.empty()) {
            if (minutesClicked || minutesHovered) {
                userMinutesText.setFillColor(sf::Color(0,0,0,0));
            }
            else {
                userMinutesText.setFillColor(sf::Color(128,128,128));
            }
        }
        else {
            userMinutesText.setFillColor(sf::Color::White);
        }

        window.clear(sf::Color::Black);
        window.draw(titleText);
        window.draw(blobBounds);
        window.draw(userInputBox);
        window.draw(prompt1);
        window.draw(prompt2);
        window.draw(userHours);
        window.draw(userMinutes);
        window.draw(userHoursText);
        window.draw(userMinutesText);
        window.display();

    }
}
