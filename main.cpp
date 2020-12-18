#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <thread>
#include "lexer.h"

// Change if needed
#define WIDTH 200
#define HEIGHT 512
#define REC_SIZE 40

using namespace sf;

// Position text to display
void setText(sf::Text &text, std::string str, int y_pos) {
    text.setString(str);
    text.setCharacterSize(24); // in pixels
    
    //center text
    sf::FloatRect textRect = text.getLocalBounds(); // get bounds
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f); // use bounds to center text
    text.setPosition(WIDTH/2, y_pos); // set position up for stack
    text.setFillColor(sf::Color(160, 160, 160, 255));
    text.setStyle(sf::Text::Bold);
}

// Generate element with color and size
sf::RectangleShape createElem(int y_pos) {
    // Generate RGB values
    int R = rand() % 256;
    int G = rand() % 256;
    int B = rand() % 256;

    // Generate element, set position, color and thickness
    sf::RectangleShape elem(Vector2f(200, REC_SIZE));
    elem.setPosition(Vector2f(WIDTH/2, y_pos));
    elem.setOrigin(elem.getSize().x/2, elem.getSize().y/2); // center
    elem.setFillColor(sf::Color::White);
    elem.setOutlineColor(sf::Color(R,G,B));
    elem.setOutlineThickness(2.0f);
    
    return elem;
}

// Pop element of stack at given position/index
template <typename T> void vec_pop_at(std::vector<T> &vec, int n) { 
    for(int i = n; i < vec.size()-1; i++)
        vec.at(i) = vec.at(i+1);
    vec.pop_back();
}

// Align vector when an element has been deleted / shrink stack
template <typename T>  void vec_align(std::vector<T> &Vec, sf::RenderWindow &wind) {
    int pos = HEIGHT-(REC_SIZE/2)+1;
    for(auto &it : Vec) { // Loop through stack/vector
        it.setPosition(WIDTH/2, pos);
        pos -= REC_SIZE;    // allign every element by vector size
    }
}

int main() {
    sf::RenderWindow wind(sf::VideoMode(WIDTH, HEIGHT), "Stack Visualizer", Style::Titlebar | Style::Close);
    sf::Event evnt;

    while(wind.isOpen()) { // Window loop
        while(wind.pollEvent(evnt)) {
            switch (evnt.type) {
            case Event::Closed: // ESC
                wind.close();
                break;
            case Event::TextEntered:
                if(evnt.text.unicode == 27)
                    wind.close();
                break;
            }
        }

        /// Main
        std::cout << "Type an equation in RPN: ";
        std::string str;
        std::getline(std::cin, str);                // Get input in RPN

        std::string str2 = str;                     // Used after stack has been initially represented
        std::string token;                          // Lexical analysis buffer
        std::vector<int> nrVec;                     // Used for calculations
        std::vector<sf::RectangleShape> elem_Vec;   // Used for all rectangle objects
        std::vector<sf::Text> text_Vec;             // Used for all labels
        int y_pos = HEIGHT-(REC_SIZE/2)+1;          // Setting start position
        // text setup
        sf::Text text;
        sf::Font font;
        if (!font.loadFromFile("arial.ttf"))        // Load font
            std::cout << "Font not available" << std::endl;
        text.setFont(font); // font is a sf::Font


        // lexer and draw loop
        while(str.length()) {
            token = tokenizeStr(str, ' ');           // Tokenize string

            setText(text, token, y_pos);            // set text = token
            text_Vec.push_back(text);               // push element onto stack

            elem_Vec.push_back(createElem(y_pos));  // create element of rectangleshape and push it on to vector
            y_pos -= REC_SIZE;                      // grow upwards
        }

        wind.clear();
        
        // drawing loops
        for(auto elem_it : elem_Vec)
            wind.draw(elem_it);
        for(auto txt_it : text_Vec)
            wind.draw(txt_it);
        wind.display();
        sf::sleep(sf::milliseconds(1000));

        /// Parsing Stack
        sf::Text text2;
        text2.setFont(font); // font is a sf::Font
        int row = 0;

        // reverse lexer
        while(str2.length()) {
            token = tokenizeStr(str2, ' ');

            if(checkNr(token)) // number found -> push
                nrVec.push_back(std::stoi(token));

            else if(checkOP(token) && nrVec.size() > 1) { // operator found
                int result = OPArithmetic(token, nrVec); // arithmetic operation
                
                // remove last two stack elements
                nrVec.pop_back();
                nrVec.pop_back();
                
                // remove last two rectangles
                vec_pop_at(elem_Vec, row-1);
                vec_pop_at(elem_Vec, row-1);

                for(auto txt_it : text_Vec) // print out all elements on text_Vec
                    std::cout << txt_it.getString().toAnsiString() << " ";
                std::cout << "<-" << std::endl;

                vec_pop_at(text_Vec, row-2); // pop first operand
                vec_pop_at(text_Vec, row-2); // pop second operand
                vec_pop_at(text_Vec, row-2); // pop operator

                setText(text2, std::to_string(result), elem_Vec.at(row-2).getPosition().y); //set text to result and put it on position of first popped element (-2 <= first elem + sec elem + op)
                text_Vec.insert(text_Vec.begin()+row-2, text2); // insert before next element after found operator

                elem_Vec.push_back(createElem(elem_Vec.at(row-2).getPosition().y)); // create element of rectangleshape and push it on to vector
                nrVec.push_back(result); // push result on to stack
                vec_pop_at(elem_Vec, row-1);

                row -= 2; // reduce row as two elements have been popped

                // allign all vectors of the stack
                vec_align(elem_Vec, wind);
                vec_align(text_Vec, wind);
                sf::sleep(sf::milliseconds(1000));
            }
            row++;

            wind.clear();
            // drawing loops
            for(auto elem_it : elem_Vec)
                wind.draw(elem_it);
            for(auto txt_it : text_Vec)
                wind.draw(txt_it);
            wind.display();
        }
    }
    return 0;
}
