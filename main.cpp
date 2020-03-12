#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "lexer.h"

#define WIDTH 200
#define HEIGHT 512
#define REC_SIZE 40

using namespace sf;

void setText(sf::Text &text, std::string str, int y_pos) {
    if(str == "+")
        std::cout << "PLUS" << std::endl;
    text.setString(str);
    text.setCharacterSize(24); // in pixels
    //center text
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    text.setPosition(WIDTH/2, y_pos);
    text.setFillColor(sf::Color(160, 160, 160, 255));
    text.setStyle(sf::Text::Bold);
}


sf::RectangleShape createElem(int y_pos) {
    sf::RectangleShape elem(Vector2f(200, REC_SIZE));
    elem.setPosition(Vector2f(WIDTH/2, y_pos));
    elem.setOrigin(elem.getSize().x/2, elem.getSize().y/2); // center
    elem.setFillColor(sf::Color::White);
    elem.setOutlineColor(sf::Color::Black);
    elem.setOutlineThickness(1.0f);
    return elem;
}

template <typename T> void vec_pop_at(std::vector<T> &vec, int n) {
    for(int i = n; i < vec.size()-1; i++)
        vec.at(i) = vec.at(i+1);
    vec.pop_back();
}


int main() {
    sf::RenderWindow wind(sf::VideoMode(WIDTH, HEIGHT), "Stack Visualizer", Style::Titlebar | Style::Close);
    sf::Event evnt;

    while(wind.isOpen()) {
        while(wind.pollEvent(evnt)) {
            switch (evnt.type) {
            case Event::Closed:
                wind.close();
                break;
            case Event::TextEntered:
                if(evnt.text.unicode == 27)
                    wind.close();
                break;
            }
        }

        /// Main
        std::cout << "Type a statement in RPN: ";
        std::string str;
        std::getline(std::cin, str);

        std::string str2 = str;
        std::string token;
        std::vector<int> nrVec;
        std::vector<sf::RectangleShape> elem_Vec;
        std::vector<sf::Text> text_Vec;
        int y_pos = HEIGHT-(REC_SIZE/2)+1; // setting start position
        // text setup
        sf::Text text;
        sf::Font font;
        if (!font.loadFromFile("arial.ttf"))
            std::cout << "Font not available" << std::endl;
        text.setFont(font); // font is a sf::Font


        // lexer and draw loop
        while(str.length()) {
            token = tokenizeStr(str, ' ');

            setText(text, token, y_pos); // set text = token
            text_Vec.push_back(text);

            elem_Vec.push_back(createElem(y_pos)); // create element of rectangleshape and push it on to vector
            y_pos -= REC_SIZE; // grow upwards
        }
        wind.clear();
        // drawing loops
        for(auto elem_it : elem_Vec)
            wind.draw(elem_it);
        for(auto txt_it : text_Vec)
            wind.draw(txt_it);
        wind.display();

        sf::sleep(sf::seconds(1));


        /// Parsing Stack
        sf::Text text2;
        text2.setFont(font); // font is a sf::Font
        int row = 0;

        // reverse lexer
        while(str2.length()) {
            token = tokenizeStr(str2, ' ');

            if(checkNr(token)) // number found -> push
                nrVec.push_back(std::stoi(token));

            else if(checkOP(token)) { // operator found
                int result = OPArithmetic(token, nrVec);
                // remove last two stack elements
                nrVec.pop_back();
                nrVec.pop_back();

                vec_pop_at(elem_Vec, row-1);
                vec_pop_at(elem_Vec, row-1);

                for(auto txt_it : text_Vec)
                    std::cout << txt_it.getString().toAnsiString() << " ";
                std::cout << "<-" << std::endl;


                vec_pop_at(text_Vec, row-2); // pop first operand
                vec_pop_at(text_Vec, row-2); // pop second operand
                vec_pop_at(text_Vec, row-2); // pop operator

                setText(text2, std::to_string(result), elem_Vec.at(row-2).getPosition().y); //set text to result and put it on position of first popped element (-2 <= first elem + sec elem + op)
                text_Vec.insert(text_Vec.begin()+row-2, text2); // insert before next element after found operator

                elem_Vec.push_back(createElem(elem_Vec.at(row-2).getPosition().y)); // create element of rectangleshape and push it on to vector
                nrVec.push_back(result); // push result on to stack
                row -= 2; // reduce row as two elements have been popped
                sf::sleep(sf::milliseconds(500));
            }
            row++;
            std::cout << "Row: " << row << " " << std::endl;

            wind.clear();
            // drawing loops
            for(auto elem_it : elem_Vec)
                wind.draw(elem_it);
            for(auto txt_it : text_Vec)
                wind.draw(txt_it);
            wind.display();
        }

        wind.clear();
        // drawing loops
        for(auto elem_it : elem_Vec)
            wind.draw(elem_it);
        for(auto txt_it : text_Vec)
            wind.draw(txt_it);
        for(auto txt_it2 : text_Vec2)
            wind.draw(txt_it2);
        wind.display();
    }
    return 0;
}
