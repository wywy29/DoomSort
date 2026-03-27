#include "ui.h"
#include <SFML/Graphics.hpp>
#include <iostream>

struct Blob {
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float radius;

    Blob(float radius, sf::Vector2f pos, sf::Vector2f velocity) {
        this->radius = radius;
        shape.setRadius(radius);
        shape.setPosition(pos);
        this->velocity = velocity; // its color will be added randomly later
    }
};

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

void ProjectUI::drawWindow(sf::RenderWindow& window, std::vector<float> screenTimes) {
    window.setFramerateLimit(60); // frames per second fixed so the blobs move at the same velocity for everyone

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

    sf::Vector2f boxSize(userInputBoxSize.x * 0.4f, 35.f); // size of the hours and minutes boxes
    // the hours and minutes boxes for the user to input their screen time (inside left box)
    sf::RectangleShape userHours(boxSize);
    sf::RectangleShape userMinutes(boxSize);


    // lot of repetition so i'm making an x and y for the setPosition function
    float inputX = leftBoxPos.x + (userInputBoxSize.x - userHours.getSize().x) / 2.f;
    float inputY = prompt2.getPosition().y + prompt2.getLocalBounds().size.y + 20.f;

    userHours.setPosition({inputX, inputY});
    userMinutes.setPosition({inputX, inputY + boxSize.y  + 2.5f}); // the floating point value added at the end is the vertical gap between the two boxes
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
    hoursLabel.setCharacterSize(17);
    minutesLabel.setCharacterSize(17);
    hoursLabel.setFillColor(sf::Color(128, 128, 128));
    minutesLabel.setFillColor(sf::Color(128, 128, 128));

    auto hrBounds = hoursLabel.getLocalBounds();
    hoursLabel.setOrigin({0, hrBounds.size.y / 2.f});
    auto minBounds = minutesLabel.getLocalBounds();
    minutesLabel.setOrigin({0, minBounds.size.y / 2.f});

    hoursLabel.setPosition({userHours.getPosition().x + userHours.getSize().x * 0.5f,
                               userHours.getPosition().y + userHours.getSize().y / 2.f});

    minutesLabel.setPosition({userMinutes.getPosition().x + userMinutes.getSize().x * 0.5f,
                              userMinutes.getPosition().y + userMinutes.getSize().y / 2.f});

    // the text of the user's input
    sf::Text userHoursText(font);
    sf::Text userMinutesText(font);
    userHoursText.setString("");
    userMinutesText.setString("");
    userHoursText.setCharacterSize(17);
    userMinutesText.setCharacterSize(17);
    userHoursText.setPosition({inputX + window.getSize().x/ 170, inputY + window.getSize().x/200});
    userMinutesText.setPosition({inputX + window.getSize().x / 170, inputY + window.getSize().y / 17 });

    // what the user is typing into
    std::string userHoursInput = "";
    std::string userMinutesInput = "";

    // blobs
    std::vector<Blob> blobs;

    for (int i = 0; i < screenTimes.size(); i++) {
        float radius = screenTimes[i];
        // calculate max and min x- and y-coordinate values
        float minX = blobBounds.getPosition().x + 5.f;
        float maxX = blobBounds.getPosition().x + blobBoundsSize.x - (radius * 2.f) - 5.f;
        float minY = blobBounds.getPosition().y + 5.f;
        float maxY = blobBounds.getPosition().y + blobBoundsSize.y - (radius * 2.f) - 5.f;
        // randomly generate x- and y-coordinate values
        float x = minX + (float)(rand()) / float(RAND_MAX / (maxX - minX));
        float y = minY + (float)(rand()) / float(RAND_MAX / (maxY - minY));
        // randomly give them velocity between -1 and 1
        float vx = (rand() % 401 - 200) / 100.f;
        float vy = (rand() % 401 - 200) / 100.f;
        // making sure that no blobs stay stationary or are too slow
        if (abs(vx) < 0.1f) vx = vx < 0 ? -0.25f : 0.25f;
        if (abs(vy) < 0.1f) vy = vy < 0 ? -0.25f : 0.25f;

        int r = rand() % 256;
        int g = rand() % 256;
        int b = rand() % 256;

        blobs.push_back(Blob(radius, sf::Vector2f(x, y), sf::Vector2f(vx, vy)));
        blobs.back().shape.setFillColor(sf::Color(r, g, b, 150));
    }

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

                if (minutesClicked && userHoursInput == "24") // the minutes box won't turn red if 24 hours is input
                    minutesClicked = false;

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
                        // prevents being able to put a num above 24 in hrs and prevents 0 from being a digit
                        if (std::stoi(hrs) <= 24 && hrs.size() <= 2 && !(hrs.size() == 1 && hrs[0] == '0')) {
                            userHoursInput = hrs;

                            if (userHoursInput == "24") {
                                // if user enters 24 in the hours section, their minutes automatically becomes 0
                                userMinutesInput = "";
                                userMinutesText.setString(""); // immediately after typing 24 hours, wipe the minutes section and make it unclickable
                                minutesClicked = false;
                            }
                        }
                    }
                    else if (minutesClicked) {
                        // only allow typing in minutes if the number of hours is not 24
                        if (userHoursInput != "24") {
                            std::string mins = userMinutesInput + static_cast<char>(text->unicode);
                            // same thing as hrs, except for mins its maxed at 59
                            if (std::stoi(mins) <= 59 && mins.size() <= 2 && !(mins.size() == 1 && mins[0] == '0')) {
                                userMinutesInput = mins;
                            }
                        }
                    }
                }
                if (userHoursInput.empty()) {
                    userHoursText.setString("");
                    userHoursText.setFillColor(sf::Color(128,128,128));
                } else {
                    userHoursText.setString(userHoursInput);
                    userHoursText.setFillColor(sf::Color::White);
                }

                if (userMinutesInput.empty()) {
                    userMinutesText.setString("");
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

        if (userHoursInput == "24") { // make the minutes box looked locked when hours entered is 24
            userMinutes.setFillColor(sf::Color(60, 60, 60));
            userMinutes.setOutlineColor(sf::Color(100, 100, 100));
            minutesLabel.setFillColor(sf::Color(80, 80, 80));
        } else {
            // go back to normal if the minutes box is not locked
            userMinutes.setFillColor(sf::Color::Black);
            minutesLabel.setFillColor(sf::Color(128, 128, 128));

            if (minutesClicked) {
                userMinutes.setOutlineColor(sf::Color::Red);
            }
            else if (minutesHovered) {
                userMinutes.setOutlineColor(sf::Color(139,0,0));
            }
            else {
                userMinutes.setOutlineColor(sf::Color::White);
            }
        }

        for (Blob& blob : blobs) {
            blob.shape.move(blob.velocity);
            sf::Vector2f pos = blob.shape.getPosition();
            sf::Vector2f boxPos = blobBounds.getPosition();

            if (pos.x <= boxPos.x || pos.x + 2 * blob.radius >= boxPos.x + blobBoundsSize.x)
                blob.velocity.x *= -1; // horizontal bounce back

            if (pos.y <= boxPos.y || pos.y + 2 * blob.radius >= boxPos.y + blobBoundsSize.y)
                blob.velocity.y *= -1; // vertical bounce back
        }

        window.clear(sf::Color::Black);
        window.draw(titleText);
        window.draw(blobBounds);
        window.draw(userInputBox);

        for (Blob& blob : blobs)
            window.draw(blob.shape);

        if (hoursHovered || hoursClicked) { // when you hover over the boxes, one of the boxes had its red-outlined edge get overlapped by the
            window.draw(userMinutes); // other box's white outline
            window.draw(userHours); // doing this make it so that the box that is "active" or being interacted with is draw later so its red outlines are not overlapped by white lines
        } else {
            window.draw(userHours);
            window.draw(userMinutes);
        }

        window.draw(prompt1);
        window.draw(prompt2);
        window.draw(userHoursText);
        window.draw(userMinutesText);
        window.draw(hoursLabel);
        window.draw(minutesLabel);

        window.display();
    }
}
