#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <cctype>
// for this case E' and T' are Q and K respectfully
std::map<std::string, std::map<std::string, std::string>> predictiveTable = {
    {"P", {{"program", "program I ; var L begin C end ."}}},
    {"I", {{"a", "Z J"}, {"b", "Z J"}, {"r", "Z J"}, {"s", "Z J"}}},
    {"J", {
        {"a", "Z J"}, {"b", "Z J"}, {"r", "Z J"}, {"s", "Z J"},
        {")", ""}, {"+", ""}, {"-", ""}, {"*", ""}, {"/", ""}, {";", ""}, {":", ""}, {",", ""}, {"=", ""},
        {"0", "Y J"}, {"1", "Y J"}, {"2", "Y J"}, {"3", "Y J"}, {"4", "Y J"},
        {"5", "Y J"}, {"6", "Y J"}, {"7", "Y J"}, {"8", "Y J"}, {"9", "Y J"}
    }},
    {"L", {{"a", "D : B ;"}, {"b", "D : B ;"}, {"r", "D : B ;"}, {"s", "D : B ;"}}},
    {"D", {{"a", "I G"}, {"b", "I G"}, {"r", "I G"}, {"s", "I G"}}},
    {"G", {{":", ""}, {",", ", I G"}}},
    {"B", {{"integer", "integer"}}},
    {"C", {{"a", "S H"}, {"b", "S H"}, {"r", "S H"}, {"s", "S H"}, {"show", "S H"}}},
    {"H", {{"show", "S H"}, {"a", "S H"}, {"b", "S H"}, {"r", "S H"}, {"s", "S H"}, {"end", ""}}},
    {"S", {{"show", "W"}, {"a", "A"}, {"b", "A"}, {"r", "A"}, {"s", "A"}}},
    {"W", {{"show", "show ( R I ) ;"}}},
    {"R", {{"\"value=\"", "\"value=\""}, {"a", ""}, {"b", ""}, {"r", ""}, {"s", ""}}},
    {"A", {{"a", "I = E ;"}, {"b", "I = E ;"}, {"r", "I = E ;"}, {"s", "I = E ;"}}},
    {"E", {{"(", "T Q"}, {"a", "T Q"}, {"b", "T Q"}, {"r", "T Q"}, {"s", "T Q"}}},
    {"Q", {{")", ""}, {";", ""}, {"+", "+ T Q"}, {"-", "- T Q"}}},
    {"T", {{"(", "F K"}, {"a", "F K"}, {"b", "F K"}, {"r", "F K"}, {"s", "F K"}}},
    {"K", {{")", ""}, {"+", ""}, {"-", ""}, {";", ""}, {"*", "* F K"}, {"/", "/ F K"}}},
    {"F", {
        {"a", "I"}, {"b", "I"}, {"r", "I"}, {"s", "I"},
        {"(", "( E )"}, {"+", "N"}, {"-", "N"}, {"0", "N"}, {"1", "N"}, {"2", "N"}, {"3", "N"}, {"4", "N"},
        {"5", "N"}, {"6", "N"}, {"7", "N"}, {"8", "N"}, {"9", "N"}
    }},
    {"N", {
        {"+", "X Y M"}, {"-", "X Y M"}, {"0", "X Y M"}, {"1", "X Y M"}, {"2", "X Y M"},
        {"3", "X Y M"}, {"4", "X Y M"}, {"5", "X Y M"}, {"6", "X Y M"}, {"7", "X Y M"}, {"8", "X Y M"}, {"9", "X Y M"}
    }},
    {"M", {
        {")", ""}, {"+", ""}, {"-", ""}, {"*", ""}, {"/", ""}, {";", ""},
        {"0", "Y M"}, {"1", "Y M"}, {"2", "Y M"}, {"3", "Y M"}, {"4", "Y M"},
        {"5", "Y M"}, {"6", "Y M"}, {"7", "Y M"}, {"8", "Y M"}, {"9", "Y M"}
    }},
    {"X", {{"+", "+"}, {"-", "-"}}},
    {"Y", {
        {"0", "0"}, {"1", "1"}, {"2", "2"}, {"3", "3"}, {"4", "4"},
        {"5", "5"}, {"6", "6"}, {"7", "7"}, {"8", "8"}, {"9", "9"}
    }},
    {"Z", {{"a", "a"}, {"b", "b"}, {"r", "r"}, {"s", "s"}}}
};


std::map<std::string, bool> declaredVars;

bool isLetter(char c) {
    return c == 'a' || c == 'b' || c == 'r' || c == 's';
}

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

std::vector<std::string> symbolize(std::string input) {
    std::vector<std::string> symbols;
    std::string current;
    bool inComment = false;
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '*' && i + 1 < input.size() && input[i + 1] == '*') {
            inComment = !inComment;
            i++;
            continue;
        }
        if (inComment || std::isspace(input[i])) {
            continue;
        }
        current.clear();
        if (input[i] == ';' || input[i] == ',' || input[i] == ':' || input[i] == '=' ||
            input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/' ||
            input[i] == '(' || input[i] == ')') {
            current += input[i];
            symbols.push_back(current);
        } else if (input[i] == '"') {
            current += input[i];
            i++;
            while (i < input.size() && input[i] != '"') {
                current += input[i];
                i++;
            }
            if (i < input.size()) {
                current += input[i];
            }
            if (current == "\"value=\"") {
                symbols.push_back(current);
            }
        } else if (std::isalnum(input[i])) {
            current += input[i];
            size_t j = i + 1;
            while (j < input.size() && std::isalnum(input[j])) {
                current += input[j];
                j++;
            }
            i = j - 1;
            if (current == "program" || current == "var" || current == "begin" ||
                current == "end" || current == "integer" || current == "show") {
                symbols.push_back(current);
            } else if (current == "end") {
                symbols.push_back(current);
            } else if (isLetter(current[0])) {
                symbols.push_back(current);
            } else if (isDigit(current[0])) {
                symbols.push_back(current);
            }
        }
    }
    symbols.push_back("$");
    return symbols;
}

bool parse(std::vector<std::string>& symbols, size_t& index) {
    std::stack<std::string> stack;
    stack.push("$");
    stack.push("P");
    bool error = false;
    while (!stack.empty() && index < symbols.size()) {
        std::string top = stack.top();
        std::string current = symbols[index];
        if (top == current || (top == "Z" && isLetter(current[0])) ||
            (top == "Y" && isDigit(current[0])) || (top == "I" && declaredVars.find(current) != declaredVars.end())) {
            stack.pop();
            if (top == "I" && declaredVars.find(current) == declaredVars.end()) {
                std::cout << "Some Errors: unknown identifier\n";
                return false;
            }
            index++;
        } else if (predictiveTable.find(top) != predictiveTable.end() &&
                   predictiveTable[top].find(current) != predictiveTable[top].end()) {
            std::string production = predictiveTable[top][current];
            stack.pop();
            if (production.empty()) {
                continue;
            }
            std::vector<std::string> prodSymbols;
            std::string temp;
            for (char c : production) {
                if (std::isspace(c)) {
                    if (!temp.empty()) {
                        prodSymbols.push_back(temp);
                        temp.clear();
                    }
                } else {
                    temp += c;
                }
            }
            if (!temp.empty()) {
                prodSymbols.push_back(temp);
            }
            for (auto it = prodSymbols.rbegin(); it != prodSymbols.rend(); ++it) {
                if (*it == "I") {
                    if (index < symbols.size() && isLetter(symbols[index][0])) {
                        declaredVars[symbols[index]] = true;
                    }
                }
                stack.push(*it);
            }
        } else {
            error = true;
            if (top == "program") {
                std::cout << "Some Errors: program is expected\n";
            } else if (top == "var") {
                std::cout << "Some Errors: var is expected\n";
            } else if (top == "begin") {
                std::cout << "Some Errors: begin is expected\n";
            } else if (top == "end") {
                std::cout << "Some Errors: end is expected\n";
            } else if (top == "integer") {
                std::cout << "Some Errors: integer is expected\n";
            } else if (top == "show") {
                std::cout << "Some Errors: show is expected\n";
            } else if (top == ";") {
                std::cout << "Some Errors: ; is missing\n";
            } else if (top == ",") {
                std::cout << "Some Errors: , is missing\n";
            } else if (top == "(") {
                std::cout << "Some Errors: Left parenthesis is missing\n";
            } else if (top == ")") {
                std::cout << "Some Errors: Right parenthesis is missing\n";
            } else {
                std::cout << "Some Errors: syntax error\n";
            }
            break;
        }
    }
    if (!error && stack.empty() && index == symbols.size()) {
        std::cout << "No Error\n";
        return true;
    }
    return false;
}

int main() {
    std::ifstream file("final25.txt");
    std::string input, line;
    while (std::getline(file, line)) {
        input += line + "\n";
    }
    file.close();
    std::vector<std::string> symbols = symbolize(input);
    size_t index = 0;
    declaredVars.clear();
    parse(symbols, index);
    return 0;
}