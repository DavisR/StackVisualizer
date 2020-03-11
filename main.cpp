#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#define WIDTH 200
#define HEIGHT 512
#define REC_SIZE 40

using namespace sf;

int findNextChar(std::string str, int i) {
    for(; str.at(i) == ' ' && i < str.length(); i++) {}
    return i;
}

std::string tokenizeStr(std::string &str, char atChar) {
    std::string buffer;

    int i = 0; // indexing variable
    if(str.at(0) == ' ')
        i = findNextChar(str, 0);

    for(; str[i] != atChar && i < str.length(); i++)
        buffer += str.at(i);

    str.erase(0, i+1);    // erase string from position 0 up to length i+1(1='spacebar')
    return buffer;
}

bool checkNr(std::string str) {
    for(int i = 0; i<str.length(); i++) {
        if(!(static_cast<int>(str.at(i)) < 58 && static_cast<int>(str.at(i)) > 47))
            return false;
    }
    return true;
}

bool checkOP(std::string str) {
    int operators[] = {static_cast<int>('+'), static_cast<int>('-'), static_cast<int>('*'), static_cast<int>('/')};
    for(int i = 0; i<str.length(); i++) {
        for(int g = 0; g < 4; g++) {
            if(static_cast<int>(str.at(i)) == operators[g])
                return true;
        }
    }
    return false;
}

int OPArithmetic(std::string &tok, std::vector<int> &Stack) {
    if(tok == "+")
        return Stack.at(Stack.size()-2) + Stack.at(Stack.size()-1);
    if(tok == "-")
        return Stack.at(Stack.size()-2) - Stack.at(Stack.size()-1);
    if(tok == "*")
        return Stack.at(Stack.size()-2) * Stack.at(Stack.size()-1);
    if(tok == "/")
        return Stack.at(Stack.size()-2) / Stack.at(Stack.size()-1);
}

sf::Text setText(sf::Text &text, std::string str, int y_pos) {
    text.setString(str);
    //center text
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    text.setPosition(WIDTH/2, y_pos);
    text.setFillColor(sf::Color(160, 160, 160, 255));
    text.setStyle(sf::Text::Bold);

    return text;
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

int main() {
    sf::RenderWindow wind(sf::VideoMode(WIDTH, HEIGHT), "Stack Visualizer", Style::Titlebar | Style::Close);
    sf::Event evnt;

    sf::Vector2i MousePos;

    while(wind.isOpen()) {
        while(wind.pollEvent(evnt)) {
            switch (evnt.type) {
            case Event::Closed:
                wind.close();
                break;
            case Event::TextEntered:
                if(evnt.text.unicode < 128) {
                    if(evnt.text.unicode == 27)
                        wind.close();
                    break;
                }
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

        sf::Text text;
        sf::Font font;
        if (!font.loadFromFile("arial.ttf"))
            std::cout << "Font not available" << std::endl;
        text.setFont(font); // font is a sf::Font
        text.setCharacterSize(24); // in pixels, not points!
        text.setFillColor(sf::Color(160, 160, 160, 255));
        text.setStyle(sf::Text::Bold);


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


        sf::sleep(sf::seconds(2));


        sf::Text text2;
        text2.setFont(font); // font is a sf::Font
        text2.setCharacterSize(24); // in pixels, not points!
        text2.setFillColor(sf::Color(160, 160, 160, 255));
        text2.setStyle(sf::Text::Bold);

        int row = 0;
        std::vector<sf::Text> text_Vec2;
        // reverse lexer
        while(str2.length()) {
            token = tokenizeStr(str2, ' ');

            if(checkNr(token)) // number found -> push
                nrVec.push_back(std::stoi(token));

            if(checkOP(token)) { // operator found
                int result = OPArithmetic(token, nrVec);
                // remove last two stack elements
                nrVec.pop_back();
                nrVec.pop_back();
                setText(text2, std::to_string(result), elem_Vec.at(row-2).getPosition().y); //set text to result and put it on position of first popped element (-2 <= first elem + sec elem + op)

//
                elem_Vec.erase(elem_Vec.begin() + row-1);
//                elem_Vec.erase(elem_Vec.begin() + row-1);
//                elem_Vec.pop_back();
//
//                text_Vec.erase(text_Vec2.begin() + row);
//                text_Vec.erase(text_Vec2.begin() + row-1);
//                text_Vec.erase(text_Vec2.begin() + row-2);

                text_Vec2.push_back(text2);

                elem_Vec.push_back(createElem(elem_Vec.at(row-2).getPosition().y)); // create element of rectangleshape and push it on to vector
                nrVec.push_back(result); // push result on to stack
            }
            row++;
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
