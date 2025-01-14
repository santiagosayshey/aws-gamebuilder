#include "HelpState.hpp"
#include <iostream>

HelpState::HelpState(sf::RenderWindow &window)
    : State(window)
{
    loadResources();
    initializeText();
    initializeButtons();
}

void HelpState::loadResources()
{
    std::vector<std::string> contentFontPaths = {
        "src/assets/font/Casino.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "arial.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "/System/Library/Fonts/Helvetica.ttf"};

    bool contentFontLoaded = false;
    for (const auto &path : contentFontPaths)
    {
        if (contentFont.loadFromFile(path))
        {
            contentFontLoaded = true;
            break;
        }
    }

    if (!contentFontLoaded)
    {
        std::cerr << "Error: Could not load content font file." << std::endl;
        return;
    }

    // Load button font
    if (!buttonFont.loadFromFile("src/assets/font/Troska.ttf"))
    {
        std::cerr << "Error: Could not load button font file." << std::endl;
        return;
    }

    // Setup title
    titleText.setFont(contentFont);
    titleText.setString("How to Play");
    titleText.setCharacterSize(64); // Larger title
    titleText.setFillColor(sf::Color(220, 220, 220));
    titleText.setOutlineThickness(2.0f); // Add outline for better visibility
    titleText.setOutlineColor(sf::Color(0, 40, 0));
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.width / 2.f, titleBounds.height / 2.f);
    titleText.setPosition(
        window.getSize().x / 2.f,
        50.f // Moved up slightly
    );
}

void HelpState::initializeText()
{
    // Define sections with their content
    sections = {
        {"Basic Rules", {"1. Try to get closer to 21 than the other plauers without going over", "2. Aces are worth 1 or 11", "3. Face cards are worth 10", "4. Place your bet before each hand", "5. Hit to take another card, Stand to keep your hand", "6. Double down doubles your debt and hits once"}},
        {"Betting", {"1. Minimum bet varies based on settings", "2. Betting pays 1:1 for all players that win", "3. Blackjack (21) with first two cards pays 3:2", "4. Push (tie) returns your bet"}},
        {"Wildcards", {"1. Special abilities can be enabled in settings", "2. Card Swap: Exchange one of your cards with a new one", "3. Foresight: Peek at the next card in the deck by pressing hit", "4. Extra bet: Doubles bet for free", "5. Tycoon: Doubles your total money", "6. Duplicate: Copies a random player's wildcard", "7. Balance Shift: Balances your card values with a random player", "8. Safe Hit: Passive wildcard that activates to save you from busting", "9. Instant Stand: Automatically sets cards to 20", "10. Each wildcard can only be used once per game"}},
        {"Winning", {"1. Get the highest total without busting", "2. Everyone else busts", "3. Get a blackjack (Ace + 10-value card)", "4. Your bet is returned in case of a tie"}}};

    // Calculate positions for two columns
    const float columnWidth = 600.f;                              // Fixed width for columns
    const float columnSpacing = 200.f;                            // Increased spacing between columns
    const float totalWidth = columnWidth * 2 + columnSpacing;     // Total width including spacing
    const float startX = (window.getSize().x - totalWidth) / 2.f; // Center everything
    const float column1X = startX;
    const float column2X = startX + columnWidth + columnSpacing;
    float column1Y = 180.f; // Increased top padding
    float column2Y = 180.f;
    const float sectionSpacing = 40.f;
    const float lineSpacing = 30.f;

    // Create text objects for each section, alternating between columns
    for (size_t i = 0; i < sections.size(); i++)
    {
        const auto &section = sections[i];
        float &currentY = (i % 2 == 0) ? column1Y : column2Y;
        float currentX = (i % 2 == 0) ? column1X : column2X;

        // Section title
        sf::Text sectionTitle;
        sectionTitle.setFont(contentFont);
        sectionTitle.setString(section.title);
        sectionTitle.setCharacterSize(36);
        sectionTitle.setFillColor(sf::Color(255, 215, 0)); // Gold color
        sectionTitle.setPosition(currentX, currentY);
        helpTexts.push_back(sectionTitle);

        currentY += 50.f; // Space after title

        // Section content
        for (const auto &line : section.lines)
        {
            sf::Text lineText;
            lineText.setFont(contentFont);
            lineText.setString(line);
            lineText.setCharacterSize(24);
            lineText.setFillColor(sf::Color(220, 220, 220));
            lineText.setPosition(currentX + 20.f, currentY);
            helpTexts.push_back(lineText);

            currentY += lineSpacing;
        }

        currentY += sectionSpacing; // Space between sections
    }
}

void HelpState::initializeButtons()
{
    // Back button
    buttons.emplace_back(
        sf::Vector2f(50.f, window.getSize().y - 70.f),
        sf::Vector2f(120.f, 50.f),
        "Back",
        buttonFont, // Use buttonFont here
        sf::Color(220, 220, 220),
        sf::Color(255, 215, 0));
}

void HelpState::handleInput()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        if (event.type == sf::Event::MouseMoved)
        {
            sf::Vector2f mousePos = getMousePosition();
            for (auto &button : buttons)
            {
                button.setHighlight(button.isMouseOver(mousePos));
            }
        }

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePos = getMousePosition();

            if (buttons[0].isMouseOver(mousePos))
            {
                requestStateChange(StateChange::Menu);
                return;
            }
        }
    }
}

void HelpState::update()
{
    for (auto &button : buttons)
    {
        button.update(0.016f);
    }
}

void HelpState::render()
{
    window.clear(sf::Color(0, 60, 0)); // Dark green background

    // Draw help texts
    window.draw(titleText);
    for (const auto &text : helpTexts)
    {
        window.draw(text);
    }

    // Draw buttons
    for (auto &button : buttons)
    {
        button.draw(window);
    }

    window.display();
}

sf::Vector2f HelpState::getMousePosition() const
{
    return window.mapPixelToCoords(sf::Mouse::getPosition(window));
}