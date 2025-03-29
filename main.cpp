#include<iostream>
#include <SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>

void draw_text(bool i_black, unsigned short i_x, unsigned short i_y, const std::string& i_text, sf::RenderWindow& i_window)
{
    short character_x = i_x;
    short character_y = i_y;
    unsigned char character_width;
    sf::Sprite character_sprite;
    sf::Texture font_texture;
    font_texture.loadFromFile("images/Font.png");
    character_width = font_texture.getSize().x / 96;
    character_sprite.setTexture(font_texture);
    if (1 == i_black)
    {
        character_sprite.setColor(sf::Color(0, 0, 0));
    }
    for (const char a : i_text)
    {
        if ('\n' == a)
        {
            character_x = i_x;
            character_y += font_texture.getSize().y;
            continue;
        }
        character_sprite.setPosition(character_x, character_y);
        character_sprite.setTextureRect(sf::IntRect(character_width * (a - 32), 0, character_width, font_texture.getSize().y));
        character_x += character_width;
        i_window.draw(character_sprite);
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(700, 700), "Tic Tac Toe", sf::Style::Default);
    sf::Event ev;

    sf::Sprite background;
    sf::Texture backgroundimage;
    backgroundimage.loadFromFile("images/background.png");
    background.setTexture(backgroundimage);
    background.setScale(window.getSize().x / backgroundimage.getSize().x, window.getSize().y / backgroundimage.getSize().y);

    const int GRID_SIZE = 3;
    const float CELL_SIZE = window.getSize().x / GRID_SIZE;

    sf::Texture xTexture, oTexture;
    xTexture.loadFromFile("images/x.png");
    oTexture.loadFromFile("images/o.png");

    sf::Sprite selector;
    sf::Texture selectorTexture;
    selectorTexture.loadFromFile("images/select.png");
    selector.setTexture(selectorTexture);
    selector.setScale(CELL_SIZE / selectorTexture.getSize().x, CELL_SIZE / selectorTexture.getSize().y);

    int board[GRID_SIZE][GRID_SIZE] = { 0 };
    int turn = 1;
    int scoreX = 0, scoreO = 0;
    bool gameActive = true;
    bool showResults = false;
    bool isDraw = false;

    std::vector<sf::Sprite> xSprites, oSprites;

    while (window.isOpen()) {
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (!showResults) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            int cellX = mousePos.x / CELL_SIZE;
            int cellY = mousePos.y / CELL_SIZE;
            selector.setPosition(cellX * CELL_SIZE, cellY * CELL_SIZE);

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && gameActive) {
                if (cellX >= 0 && cellX < GRID_SIZE && cellY >= 0 && cellY < GRID_SIZE && board[cellX][cellY] == 0) {
                    sf::Sprite mark;
                    mark.setScale(CELL_SIZE / xTexture.getSize().x, CELL_SIZE / xTexture.getSize().y);
                    mark.setPosition(cellX * CELL_SIZE, cellY * CELL_SIZE);

                    if (turn % 2 == 1) {
                        mark.setTexture(xTexture);
                        board[cellX][cellY] = 1;
                        xSprites.push_back(mark);
                    }
                    else {
                        mark.setTexture(oTexture);
                        board[cellX][cellY] = 2;
                        oSprites.push_back(mark);
                    }

                    int winner = 0;
                    for (int i = 0; i < GRID_SIZE; i++) {
                        if ((board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != 0)) {
                            winner = board[i][0];
                        }
                        if ((board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != 0)) {
                            winner = board[0][i];
                        }
                    }
                    if ((board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != 0)) {
                        winner = board[0][0];
                    }
                    if ((board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != 0)) {
                        winner = board[0][2];
                    }

                    if (winner != 0) {
                        gameActive = false;
                        showResults = true;
                        if (winner == 1) {
                            scoreX++;
                        }
                        else {
                            scoreO++;
                        }
                    }
                    else {
                        bool fullBoard = true;
                        for (int i = 0; i < GRID_SIZE; i++) {
                            for (int j = 0; j < GRID_SIZE; j++) {
                                if (board[i][j] == 0) {
                                    fullBoard = false;
                                    break;
                                }
                            }
                            if (!fullBoard) break;
                        }
                        if (fullBoard) {
                            gameActive = false;
                            showResults = true;
                            isDraw = true;
                        }
                    }
                    turn++;
                }
            }
        }
        else {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                showResults = false;
                for (int i = 0; i < GRID_SIZE; i++) {
                    for (int j = 0; j < GRID_SIZE; j++) {
                        board[i][j] = 0;
                    }
                }
                xSprites.clear();
                oSprites.clear();
                turn = 1;
                gameActive = true;
                isDraw = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                showResults = false;
                for (int i = 0; i < GRID_SIZE; i++) {
                    for (int j = 0; j < GRID_SIZE; j++) {
                        board[i][j] = 0;
                    }
                }
                xSprites.clear();
                oSprites.clear();
                turn = 1;
                gameActive = true;
                isDraw = false;
                scoreX = 0;
                scoreO = 0;
            }
        }

        window.clear();
        if (showResults) {
            window.clear(sf::Color::Black);
            draw_text(0, 250, 200, isDraw ? "Game Draw!" : "X VS O", window);
            draw_text(0, 250, 230, "X: " + std::to_string(scoreX) + " O: " + std::to_string(scoreO), window);
            draw_text(0, 250, 260, "Press Enter To Continue", window);
            draw_text(0, 250, 290, "Press R To Reset The Scores", window);
        }
        else {
            window.draw(background);
            for (const auto& sprite : xSprites) {
                window.draw(sprite);
            }
            for (const auto& sprite : oSprites) {
                window.draw(sprite);
            }
            window.draw(selector);
        }
        window.display();
    }
    return 0;
}
