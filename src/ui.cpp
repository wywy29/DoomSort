#include "ui.h"
#include "sort.h"
#include <SFML/Graphics.hpp>
#include <iostream>

//Blob constructor
Blob::Blob(float radius, sf::Vector2f pos, sf::Vector2f velocity) {
    this->radius = radius;
    shape.setRadius(radius);
    shape.setPosition(pos);
    this->velocity = velocity;
}

struct Popup {
    sf::Text text;
    bool shown; // popups should appear after sort, so if its sorted, the popups will be shown (true)

    Popup(const sf::Font &font, std::string msg, sf::Vector2f pos) : text(font, msg, 16), shown(false) {
        text.setFillColor(sf::Color::White);
        text.setPosition(pos);
    }
};



sf::Color determineColor(float radius, int opacity) {
    // color blobs (neon-colors) based on their screen time/original radius values
    int r, g, b;
    if (radius >= 0.0 && radius <= 1.0) {
        //Neon-Green: Excellent
        r = 50, g = 255, b = 25;
    } else if (radius > 1.0 && radius <= 2.0) {
        //Neon-Cyan: Great
        r = 0, g = 255, b = 255;
    } else if (radius > 2.0 && radius <= 4.0) {
        //Neon-Blue: Good
        r = 25, g = 100, b = 255;
    } else if (radius > 4.0 && radius <= 6.0) {
        //Neon-Pink: OK
        r = 255, g = 25, b = 150;
    } else if (radius > 6.0 && radius <= 8.0) {
        //Neon-Yellow: Fair
        r = 255, g = 255, b = 0;
    } else if (radius > 8.0 && radius <= 10.0) {
        //Neon-Orange: Bad/Poor
        r = 255, g = 125, b = 0;
    } else if (radius > 10.0) {
        //Neon-Red: Dangerous
        r = 255, g = 25, b = 25;
    }

    return sf::Color(r, g, b, opacity);
}

sf::Vector2f getRandomVelocity() {
    // randomly give them velocity between -2 and 2
    float vx = (rand() % 401 - 200) / 100.f;
    float vy = (rand() % 401 - 200) / 100.f;
    // making sure that no blobs stay stationary or are too slow
    if (abs(vx) < 0.1f) vx = vx < 0 ? -0.25f : 0.25f;
    if (abs(vy) < 0.1f) vy = vy < 0 ? -0.25f : 0.25f;

    return sf::Vector2f({vx, vy});
}

// transition between homescreen and project (only really works for black background to black background for now)
void fadeOutEffect(sf::RenderWindow &window) {
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

// some helper functions to make the key!
sf::CircleShape makeKeyDot(sf::Color color, float x, float y, float radius) {
    sf::CircleShape dot(radius);
    dot.setFillColor(color);
    // the first and second dot looking very out of place on the key compared to the other dots
    if (radius == 1.5f) {
        dot.setPosition({x + 5, y + 5});
    } else if (radius == 2.5f) {
        dot.setPosition({x + 4, y + 4});
    } else {
        dot.setPosition({x, y});
    }
    return dot;
}

sf::Text keyLabel(const sf::Font &font, std::string txt, float x, float y) {
    sf::Text text(font);
    text.setString(txt);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);
    text.setPosition({x + 6.f, y - 3.f});
    return text;
}

bool HomeScreenUI::show(sf::RenderWindow &window) {
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
    captionText.setPosition({
        (windowSize.x - captionText.getLocalBounds().size.x) / 2.f,
        titleText.getPosition().y + res.size.y + windowSize.y / 30
    });

    // positioning continue button in correct spot (decently below caption text)
    auto buttonBounds = continueButton.getLocalBounds();
    continueButton.setPosition({
        (windowSize.x - buttonBounds.size.x) / 2.f, titleText.getPosition().y + res.size.y + windowSize.y / 4
    });

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            sf::Vector2i pos = sf::Mouse::getPosition(window);
            if (continueButton.getGlobalBounds().contains(sf::Vector2f(pos))) {
                continueButton.setFillColor(sf::Color::Red);

                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    fadeOutEffect(window);
                    return true;
                }
            } else {
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

void ProjectUI::drawWindow(sf::RenderWindow &window, std::vector<float> screenTimes) {
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
    userInputBox.setPosition({
        windowSize.x * .125f - userInputBoxSize.x / 2.f,
        (windowSize.y - userInputBoxSize.y) / 2.f + window.getSize().y / 36.f
    });
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
    prompt1.setPosition({
        leftBoxPos.x + (userInputBoxSize.x - prompt1.getLocalBounds().size.x) / 2.f,
        leftBoxPos.y + window.getSize().y / 25.f
    });
    prompt2.setPosition({
        leftBoxPos.x + (userInputBoxSize.x - prompt2.getLocalBounds().size.x) / 2.f,
        prompt1.getPosition().y + promptBounds.size.y + 4.f
    });

    // info for the right box surrounding the blobs
    sf::Vector2f blobBoundsSize(windowSize.x * 0.7f, windowSize.y * 0.8f);
    sf::RectangleShape blobBounds(blobBoundsSize);
    blobBounds.setPosition({
        windowSize.x * .63f - blobBoundsSize.x / 2.f, (windowSize.y - blobBoundsSize.y) / 2.f + window.getSize().y / 36
    });
    blobBounds.setOutlineColor(sf::Color::White);
    blobBounds.setFillColor(sf::Color::Black);
    blobBounds.setOutlineThickness(3);

    sf::Vector2f boxSize(userInputBoxSize.x * 0.4f, 35.f); // size of the hours and minutes boxes
    // the hours and minutes boxes for the user to input their screen time (inside left box)
    sf::RectangleShape userHours(boxSize);
    sf::RectangleShape userMinutes(boxSize);

    static Popup popup1(font, "+0% chance of myopia based on your screentime", {blobBounds.getPosition().x + 10.f, blobBounds.getPosition().y + 10.f});
    static Popup popup2(font, "You have more screentime than 0% of the population", {blobBounds.getPosition().x + 10.f, blobBounds.getPosition().y + 10.f});
    static Popup popup3(font, "smth sleep quality", {blobBounds.getPosition().x + 10.f, blobBounds.getPosition().y + 10.f});


    // lot of repetition so i'm making an x and y for the setPosition function
    float inputX = leftBoxPos.x + (userInputBoxSize.x - userHours.getSize().x) / 2.f;
    float inputY = prompt2.getPosition().y + prompt2.getLocalBounds().size.y + 20.f;

    userHours.setPosition({inputX, inputY});
    userMinutes.setPosition({inputX, inputY + boxSize.y + 2.5f});
    // the floating point value added at the end is the vertical gap between the two boxes
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
    hoursLabel.setString("h");
    minutesLabel.setString("min");
    hoursLabel.setCharacterSize(17);
    minutesLabel.setCharacterSize(17);
    hoursLabel.setFillColor(sf::Color(128, 128, 128));
    minutesLabel.setFillColor(sf::Color(128, 128, 128));

    // DA MERGE SORT, QUICK SORT, AND RESET BUTTONS!
    sf::Vector2f sortBoxSizes(userInputBoxSize.x * 0.6f, 35.f);

    sf::RectangleShape quickSortBox(sortBoxSizes);
    sf::RectangleShape mergeSortBox(sortBoxSizes);
    quickSortBox.setPosition({inputX - 22.f, inputY + boxSize.y + window.getSize().y / 7.5f});
    mergeSortBox.setPosition({inputX - 22.f, inputY + boxSize.y + window.getSize().y / 4.6f});
    quickSortBox.setFillColor(sf::Color::Black);
    quickSortBox.setOutlineColor(sf::Color::White);
    mergeSortBox.setFillColor(sf::Color::Black);
    mergeSortBox.setOutlineColor(sf::Color::White);
    quickSortBox.setOutlineThickness(2);
    mergeSortBox.setOutlineThickness(2);

    sf::Text quickSortText(font);
    quickSortText.setString("QUICK SORT");
    quickSortText.setCharacterSize(17);
    quickSortText.setFillColor(sf::Color::White);
    quickSortText.setPosition({inputX + 7.f, inputY + boxSize.y + window.getSize().y / 6.8f});

    sf::Text mergeSortText(font);
    mergeSortText.setString("MERGE SORT");
    mergeSortText.setCharacterSize(17);
    mergeSortText.setFillColor(sf::Color::White);
    mergeSortText.setPosition({inputX + 3.f, inputY + boxSize.y + window.getSize().y / 4.35f});

    // RESET BUTTON (should reset any inputs, outline color of the boxes, remove the user blob, and reset all blobs to float again)

    sf::RectangleShape resetBox(sortBoxSizes);
    resetBox.setPosition({inputX - 22.f, inputY + boxSize.y + window.getSize().y / 3.3f});
    resetBox.setFillColor(sf::Color::Black);
    resetBox.setOutlineColor(sf::Color::White);
    resetBox.setOutlineThickness(2);

    sf::Text resetText(font);
    resetText.setString("RESET");
    resetText.setCharacterSize(17);
    resetText.setFillColor(sf::Color::White);
    resetText.setPosition({inputX + 22.f, inputY + boxSize.y + window.getSize().y / 3.18f});
    bool resetClicked = false;

    // legend for what each color represents
    float keyX = resetBox.getPosition().x - 15.f;
    float keyY = resetBox.getPosition().y;
    float keyYOffset = 25.f;
    float keyXOffset = 20.f;
    float keyDotsRadius = 6.f;

    std::vector<sf::CircleShape> keyDots;
    std::vector<sf::Text> keyDotLabels;

    float keyStartingY = resetBox.getPosition().y + 60.f;
    std::vector<std::pair<sf::Color, std::string> > keyItems = {
        {sf::Color(50, 255, 25, 155), "0-1 hr"},
        {sf::Color(0, 255, 255, 155), "1-2 hrs"},
        {sf::Color(25, 100, 255, 155), "2-4 hrs"},
        {sf::Color(255, 25, 150, 155), "4-6 hrs"},
        {sf::Color(255, 255, 0, 155), "6-8 hrs"},
        {sf::Color(255, 125, 0, 155), "8-10 hrs"},
        {sf::Color(255, 25, 25, 155), "10+ hrs"}
    };

    std::vector<float> dotsRadii = {1.5f, 2.5f, 5.f, 6.f, 7.f, 8.f, 9.f};

    for (int i = 0; i < 4; i++) {
        float dotY = keyStartingY + i * keyYOffset;
        keyDots.push_back(makeKeyDot(keyItems[i].first, keyX, dotY, dotsRadii[i]));
        keyDotLabels.push_back(keyLabel(font, keyItems[i].second, keyX + keyXOffset, dotY));
    }
    for (int i = 4; i < (int) keyItems.size(); i++) {
        float secondColumnX = keyX + sortBoxSizes.x / 1.5f;
        float dotY = keyStartingY + (i - 4) * keyYOffset;
        keyDots.push_back(makeKeyDot(keyItems[i].first, secondColumnX, dotY, dotsRadii[i]));
        keyDotLabels.push_back(keyLabel(font, keyItems[i].second, secondColumnX + keyXOffset, dotY));
    }

    float hoursCenterY = userHours.getPosition().y + (userHours.getSize().y / 2.f);
    float minutesCenterY = userMinutes.getPosition().y + (userMinutes.getSize().y / 2.f);

    auto hrBounds = hoursLabel.getLocalBounds();
    hoursLabel.setOrigin({0.f, hrBounds.size.y / 2.f + hrBounds.position.y});
    auto minBounds = minutesLabel.getLocalBounds();
    minutesLabel.setOrigin({0, minBounds.size.y / 2.f + minBounds.position.y});

    hoursLabel.setPosition({userHours.getPosition().x + 40.f, hoursCenterY});
    minutesLabel.setPosition({userMinutes.getPosition().x + 40.f, minutesCenterY});

    // the text of the user's input
    sf::Text userHoursText(font);
    sf::Text userMinutesText(font);
    userHoursText.setString("");
    userMinutesText.setString("");
    userHoursText.setCharacterSize(17);
    userMinutesText.setCharacterSize(17);

    userHoursText.setPosition({userHours.getPosition().x + 10.f, hoursCenterY});
    userMinutesText.setPosition({userMinutes.getPosition().x + 10.f, minutesCenterY});

    // lets us know if quick sort or merge sort box is clicked, so we can have it highlighted
    bool quickSortClicked = false;
    bool mergeSortClicked = false;
    bool hasInput = false;
    // determines if the user input boxes have inputs, to determine if the sort boxes can be clicked or not

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
        float x = minX + (float) (rand()) / float(RAND_MAX / (maxX - minX));
        float y = minY + (float) (rand()) / float(RAND_MAX / (maxY - minY));

        //Add blob to vector
        blobs.push_back(Blob(radius + 3.5, sf::Vector2f(x, y), getRandomVelocity()));
        blobs.back().shape.setFillColor(determineColor(radius, 150));
    }
    bool userBlobExists = false;

    // submit button
    bool submitClicked = false;
    sf::Vector2f submitSize(userInputBoxSize.x * 0.3f, 20.f);
    sf::RectangleShape submitBox(submitSize);
    submitBox.setPosition({
        inputX + (boxSize.x - submitSize.x) / 2.f, userMinutes.getPosition().y + userMinutes.getSize().y / 2.f + 27.5f
    });
    submitBox.setFillColor(sf::Color::Black);
    submitBox.setOutlineColor(sf::Color::White);
    submitBox.setOutlineThickness(2);

    sf::Text submitText(font);
    submitText.setString("SUBMIT");
    submitText.setCharacterSize(14);
    submitText.setFillColor(sf::Color::White);

    auto subBounds = submitText.getLocalBounds();
    submitText.setOrigin({subBounds.size.x / 2.f, subBounds.size.y / 2.f + subBounds.position.y});
    submitText.setPosition({
        submitBox.getPosition().x + submitBox.getSize().x / 2.f,
        submitBox.getPosition().y + submitBox.getSize().y / 2.f
    });

    bool submitted = false; // to check if user submits in order to determine if they can click one of the sort boxes

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // change outline color of the user input boxes when clicked
            if (const auto *click = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f clickPos(click->position);
                submitClicked = submitBox.getGlobalBounds().contains(clickPos);
                hoursClicked = userHours.getGlobalBounds().contains(clickPos);
                minutesClicked = userMinutes.getGlobalBounds().contains(clickPos);
                quickSortClicked = quickSortBox.getGlobalBounds().contains(clickPos);
                mergeSortClicked = mergeSortBox.getGlobalBounds().contains(clickPos);
                resetClicked = resetBox.getGlobalBounds().contains(clickPos);

                if (!userHoursInput.empty() || !userMinutesInput.empty()) {
                    hasInput = true;
                } else {
                    hasInput = false;
                }

                if (minutesClicked && userHoursInput == "24") // the minutes box won't turn red if 24 hours is input
                    minutesClicked = false;

                if (hoursClicked) {
                    hoursClicked = true;
                    minutesClicked = false;
                    quickSortClicked = false;
                    mergeSortClicked = false;
                    resetClicked = false;
                    submitClicked = false;
                    resetBox.setOutlineColor(sf::Color::White);
                    userHours.setOutlineColor(sf::Color::Red);
                    userMinutes.setOutlineColor(sf::Color::White);
                    quickSortBox.setOutlineColor(sf::Color::White);
                    mergeSortBox.setOutlineColor(sf::Color::White);
                } else if (minutesClicked) {
                    minutesClicked = true;
                    hoursClicked = false;
                    quickSortClicked = false;
                    mergeSortClicked = false;
                    resetClicked = false;
                    submitClicked = false;
                    resetBox.setOutlineColor(sf::Color::White);
                    userMinutes.setOutlineColor(sf::Color::Red);
                    userHours.setOutlineColor(sf::Color::White);
                    quickSortBox.setOutlineColor(sf::Color::White);
                    mergeSortBox.setOutlineColor(sf::Color::White);
                }
                // users must have an input to be able to click sort boxes
                else if (quickSortClicked && submitted) {
                    quickSortClicked = true;
                    hoursClicked = false;
                    minutesClicked = false;
                    mergeSortClicked = false;
                    resetClicked = false;
                    resetBox.setOutlineColor(sf::Color::White);
                    userMinutes.setOutlineColor(sf::Color::White);
                    userHours.setOutlineColor(sf::Color::White);
                    quickSortBox.setOutlineColor(sf::Color::Red);
                    mergeSortBox.setOutlineColor(sf::Color::White);

                    //popups
                    bool shown = true;

                    float h = userHoursInput.empty() ? 0.f : std::stof(userHoursInput);
                    float m = userMinutesInput.empty() ? 0.f : std::stof(userMinutesInput);
                    float totalTime = h + (m / 60.f);

                    // myopia increases by 17 pct per additonal hr
                    int myopiaRisk = int(totalTime * 17);
                    int percentage = 0; // added after sort is completed

                    // update popup text
                    popup1.text.setString("+" + to_string(myopiaRisk) + "% chance of myopia based on your screentime");
                    popup2.text.setString("You have more screen time than " + std::to_string(percentage) + "% of the population");
                    popup3.text.setString("qweweqw"); //undecided for now


                    sf::FloatRect bounds = popup1.text.getGlobalBounds();
                    popup1.text.setPosition({blobBounds.getPosition().x-240.f, blobBounds.getPosition().y-40.f});
                    popup2.text.setPosition({blobBounds.getPosition().x + 370.f, blobBounds.getPosition().y-40.f});
                    popup3.text.setPosition({blobBounds.getPosition().x + 140.f, blobBounds.getPosition().y + 490.f});

                    popup1.shown = true;
                    popup2.shown = true;
                    popup3.shown = true;
                } else if (mergeSortClicked && submitted) {
                    mergeSortClicked = true;
                    hoursClicked = false;
                    minutesClicked = false;
                    quickSortClicked = false;
                    resetClicked = false;
                    resetBox.setOutlineColor(sf::Color::White);
                    userMinutes.setOutlineColor(sf::Color::White);
                    userHours.setOutlineColor(sf::Color::White);
                    quickSortBox.setOutlineColor(sf::Color::White);
                    mergeSortBox.setOutlineColor(sf::Color::Red);

                    //popups
                    bool shown = true;
                    float h = userHoursInput.empty() ? 0.f : std::stof(userHoursInput);
                    float m = userMinutesInput.empty() ? 0.f : std::stof(userMinutesInput);
                    float totalTime = h + (m / 60.f);

                    // myopia increases by 17 pct per additonal hr
                    int myopiaRisk = int(totalTime * 17);
                    int percentage = 0; // added after sort is completed

                    // update popup text
                    sf::FloatRect bounds = popup1.text.getGlobalBounds();
                    popup1.text.setPosition({blobBounds.getPosition().x-240.f, blobBounds.getPosition().y-40.f});
                    popup2.text.setPosition({blobBounds.getPosition().x + 370.f, blobBounds.getPosition().y-40.f});
                    popup3.text.setPosition({blobBounds.getPosition().x + 140.f, blobBounds.getPosition().y + 490.f}); //

                    popup1.shown = true;
                    popup2.shown = true;
                    popup3.shown = true;
                } else if (resetClicked && hasInput) {
                    mergeSortClicked = false;
                    hoursClicked = false;
                    minutesClicked = false;
                    quickSortClicked = false;
                    resetClicked = true;
                    submitted = false;
                    resetBox.setOutlineColor(sf::Color::Red);
                    userMinutes.setOutlineColor(sf::Color::White);
                    userHours.setOutlineColor(sf::Color::White);
                    quickSortBox.setOutlineColor(sf::Color::White);
                    mergeSortBox.setOutlineColor(sf::Color::White);
                    blobs.pop_back(); // remove user blob
                    popup1.shown = false;
                    popup2.shown = false;
                    popup3.shown = false;

                    userHoursInput = "";
                    userMinutesInput = "";
                    userHoursText.setString("");
                    userMinutesText.setString("");
                } else if (submitClicked && hasInput) {
                    submitted = true;
                    if (userBlobExists && !blobs.empty())
                        blobs.pop_back(); // if the user tries to enter a new blob, remove their previous blob

                    hoursClicked = false;
                    minutesClicked = false;
                    quickSortClicked = false;
                    mergeSortClicked = false;
                    resetClicked = false;

                    submitBox.setOutlineColor(sf::Color::Red);
                    resetBox.setOutlineColor(sf::Color::Red);
                    userMinutes.setOutlineColor(sf::Color::White);
                    userHours.setOutlineColor(sf::Color::White);
                    quickSortBox.setOutlineColor(sf::Color::White);
                    mergeSortBox.setOutlineColor(sf::Color::White);



                    // create new blob for user
                    float h = userHoursInput.empty() ? 0.f : std::stof(userHoursInput);
                    float m = userMinutesInput.empty() ? 0.f : std::stof(userMinutesInput);
                    float totalTime = h + (m / 60.f);


                    sf::Vector2f spawnPos = {
                        blobBounds.getPosition().x + blobBoundsSize.x / 2.f,
                        blobBounds.getPosition().y + blobBoundsSize.y / 2.f
                    };
                    sf::Color userColor = determineColor(totalTime, 150);

                    blobs.push_back(Blob(totalTime + 3.5f, spawnPos, getRandomVelocity()));
                    userBlobExists = true;


                    blobs.back().shape.setFillColor(userColor);
                    blobs.back().shape.setOutlineColor(sf::Color::White);
                    blobs.back().shape.setOutlineThickness(2);

                    submitClicked = true;
                    hoursClicked = false;
                    minutesClicked = false;
                } else {
                    hoursClicked = false;
                    minutesClicked = false;
                    mergeSortClicked = false;
                    quickSortClicked = false;
                    resetClicked = false;
                    resetBox.setOutlineColor(sf::Color::White);
                    userHours.setOutlineColor(sf::Color::White);
                    userMinutes.setOutlineColor(sf::Color::White);
                    quickSortBox.setOutlineColor(sf::Color::White);
                    mergeSortBox.setOutlineColor(sf::Color::White);
                }
            }
            // handles when the user inputs digits into the boxes
            if (const auto *text = event->getIf<sf::Event::TextEntered>()) {
                // 8 is backspace
                if (text->unicode == 8) {
                    if (hoursClicked && !userHoursInput.empty()) {
                        userHoursInput.pop_back();
                    } else if (minutesClicked && !userMinutesInput.empty()) {
                        userMinutesInput.pop_back();
                    }
                } else if (text->unicode >= '0' && text->unicode <= '9') {
                    if (hoursClicked) {
                        std::string hrs = userHoursInput + static_cast<char>(text->unicode);
                        // prevents being able to put a num above 24 in hrs and prevents 0 from being a digit
                        if (std::stoi(hrs) <= 24 && hrs.size() <= 2 && !(hrs.size() == 1 && hrs[0] == '0')) {
                            userHoursInput = hrs;

                            if (userHoursInput == "24") {
                                // if user enters 24 in the hours section, their minutes automatically becomes 0
                                userMinutesInput = "";
                                userMinutesText.setString("");
                                // immediately after typing 24 hours, wipe the minutes section and make it unclickable
                                minutesClicked = false;
                            }
                        }
                    } else if (minutesClicked) {
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
                    userHoursText.setFillColor(sf::Color(128, 128, 128));
                } else {
                    userHoursText.setString(userHoursInput);
                    userHoursText.setFillColor(sf::Color::White);
                }

                if (userMinutesInput.empty()) {
                    userMinutesText.setString("");
                    userMinutesText.setFillColor(sf::Color(128, 128, 128));
                } else {
                    userMinutesText.setString(userMinutesInput);
                    userMinutesText.setFillColor(sf::Color::White);
                }

                auto hBounds = userHoursText.getLocalBounds();
                userHoursText.setOrigin({0.f, hBounds.size.y / 2.f + hBounds.position.y});
                auto mBounds = userMinutesText.getLocalBounds();
                userMinutesText.setOrigin({0.f, mBounds.size.y / 2.f + mBounds.position.y});

                if (userHoursInput.empty() && userMinutesInput.empty()) {
                    quickSortClicked = false;
                    mergeSortClicked = false;
                }
            }
        }

        // make the labels disappear when mouse is clicked/hovered over it and changes color of user input boxes when mouse is hovered
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        bool hoursHovered = userHours.getGlobalBounds().contains(sf::Vector2f(pos));
        bool minutesHovered = userMinutes.getGlobalBounds().contains(sf::Vector2f(pos));
        bool quickSortHovered = quickSortBox.getGlobalBounds().contains(sf::Vector2f(pos));
        bool mergeSortHovered = mergeSortBox.getGlobalBounds().contains(sf::Vector2f(pos));
        bool resetHovered = resetBox.getGlobalBounds().contains(sf::Vector2f(pos));
        bool submitHovered = submitBox.getGlobalBounds().contains(sf::Vector2f(pos));

        if (submitClicked) {
            submitBox.setOutlineColor(sf::Color::Red);
        } else if (submitHovered) {
            submitBox.setOutlineColor((sf::Color(139, 0, 0)));
        } else {
            submitBox.setOutlineColor(sf::Color::White);
        }

        if (hoursClicked) {
            userHours.setOutlineColor(sf::Color::Red);
        } else if (hoursHovered) {
            userHours.setOutlineColor(sf::Color(139, 0, 0));
        } else {
            userHours.setOutlineColor(sf::Color::White);
        }

        if (userHoursInput == "24") {
            // make the minutes box looked locked when hours entered is 24
            userMinutes.setFillColor(sf::Color(60, 60, 60));
            userMinutes.setOutlineColor(sf::Color(100, 100, 100));
            minutesLabel.setFillColor(sf::Color(80, 80, 80));
        } else {
            // go back to normal if the minutes box is not locked
            userMinutes.setFillColor(sf::Color::Black);
            minutesLabel.setFillColor(sf::Color(128, 128, 128));

            if (minutesClicked) {
                userMinutes.setOutlineColor(sf::Color::Red);
            } else if (minutesHovered) {
                userMinutes.setOutlineColor(sf::Color(139, 0, 0));
            } else {
                userMinutes.setOutlineColor(sf::Color::White);
            }
        }
        if (quickSortClicked) {
            quickSortBox.setOutlineColor(sf::Color::Red);
        } else if (quickSortHovered) {
            quickSortBox.setOutlineColor(sf::Color(139, 0, 0));
        } else {
            quickSortBox.setOutlineColor(sf::Color::White);
        }

        if (mergeSortClicked) {
            mergeSortBox.setOutlineColor(sf::Color::Red);
        } else if (mergeSortHovered) {
            mergeSortBox.setOutlineColor(sf::Color(139, 0, 0));
        } else {
            mergeSortBox.setOutlineColor(sf::Color::White);
        }

        if (resetClicked) {
            resetBox.setOutlineColor(sf::Color::Red);
            if (hasInput) {
                submitClicked = false;
                submitBox.setOutlineColor(sf::Color::White);
            }
        } else if (resetHovered) {
            resetBox.setOutlineColor(sf::Color(139, 0, 0));
        } else {
            resetBox.setOutlineColor(sf::Color::White);
        }

        for (Blob &blob: blobs) {
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

        for (Blob &blob: blobs)
            window.draw(blob.shape);

        if (hoursHovered || hoursClicked) {
            // when you hover over the boxes, one of the boxes had its red-outlined edge get overlapped by the
            window.draw(userMinutes); // other box's white outline
            window.draw(userHours);
            // doing this make it so that the box that is "active" or being interacted with is draw later so its red outlines are not overlapped by white lines
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
        window.draw(submitBox);
        window.draw(submitText);
        window.draw(quickSortBox);
        window.draw(mergeSortBox);
        window.draw(quickSortText);
        window.draw(mergeSortText);
        window.draw(resetBox);
        window.draw(resetText);

        for (auto &dot: keyDots) {
            window.draw(dot);
        }
        for (auto &label: keyDotLabels) {
            window.draw(label);
        }
        if (popup1.shown) {
            window.draw(popup1.text);
        }

        if (popup2.shown) {
            window.draw(popup2.text);
        }
        if (popup2.shown) {
            window.draw(popup3.text);
        }

        window.display();
    }
}