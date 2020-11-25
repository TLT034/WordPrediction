#include "WordTree.hpp"
#include "rlutil.h"

#include <algorithm>
#include <fstream>
#include <iostream>

std::shared_ptr<WordTree> readDictionary(std::string filename);

int main()
{
    std::shared_ptr<WordTree> wt = readDictionary("dictionary.txt");
    std::uint8_t consoleRows = static_cast<std::uint8_t>(rlutil::trows());
    std::uint8_t numPred = consoleRows - 5;
    std::string currInput;

    rlutil::cls();
    while (true)
    {
        int cursorX = static_cast<int>(currInput.size()) + 1;
        rlutil::locate(cursorX, 1);
        char key = std::tolower(char(rlutil::getkey()));
        if (key == rlutil::KEY_BACKSPACE && currInput.size() > 0)
        {
            currInput.pop_back();
        }
        else if (std::isalpha(key))
        {
            currInput += key;
        }
        auto predictions = wt->predict(currInput, numPred);

        rlutil::cls();
        rlutil::setString(currInput);
        rlutil::locate(1, 3);
        rlutil::setString("--- Predictions ---");
        rlutil::locate(1, 5);

        int cursorLineNum = 5;
        for (std::size_t i = 0; i < predictions.size(); i++)
        {
            rlutil::setString(predictions[i]);
            cursorLineNum++;
            rlutil::locate(1, cursorLineNum);
        }
    }
    return 0;
}

std::shared_ptr<WordTree> readDictionary(std::string filename)
{
    auto wordTree = std::make_shared<WordTree>();
    std::ifstream inFile = std::ifstream(filename, std::ios::in);

    while (!inFile.eof())
    {
        std::string word;
        std::getline(inFile, word);
        // Need to consume the carriage return character for some systems, if it exists
        if (!word.empty() && word[word.size() - 1] == '\r')
        {
            word.erase(word.end() - 1);
        }
        // Keep only if everything is an alphabetic character -- Have to send isalpha an unsigned char or
        // it will throw exception on negative values; e.g., characters with accent marks.
        if (std::all_of(word.begin(), word.end(), [](unsigned char c) { return std::isalpha(c); }))
        {
            std::transform(word.begin(), word.end(), word.begin(), [](char c) { return static_cast<char>(std::tolower(c)); });
            wordTree->add(word);
        }
    }

    return wordTree;
}
