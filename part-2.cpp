#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <cctype>

std::map<std::string, std::map<std::string, std::string>> predictiveTable = {
    {"P", {{"program", "program I ; var L begin C end"}}},
    {"I", {{"a", "ZJ"}, {"b", "ZJ"}, {"r", "ZJ"}, {"s", "ZJ"}}},
    {"J", {{"a", "ZJ"}, {"b", "ZJ"}, {"r", "ZJ"}, {"s", "ZJ"}, {")", ""}, {"+", ""}, {"-", ""}, {"*", ""}, {"/", ""}, {";", ""}, {":", ""}, {",", ""}, {"=", ""}, {"0", "YJ"}, {"1", "YJ"}, {"2", "YJ"}, {"3", "YJ"}, {"4", "YJ"}, {"5", "YJ"}, {"6", "YJ"}, {"7", "YJ"}, {"8", "YJ"}, {"9", "YJ"}}},
    {"L", {{"a", "D : B ;"}, {"b", "D : B ;"}, {"r", "D : B ;"}, {"s", "D : B ;"}}},
    {"D", {{"a", "I G"}, {"b", "I G"}, {"r", "I G"}, {"s", "I G"}}},
    {"G", {{":", ""}, {",", ", I G"}}},
    {"B", {{"integer", "integer"}}},
    {"C", {{"a", "S H"}, {"b", "S H"}, {"r", "S H"}, {"s", "S H"}, {"show", "S H"}}},
    {"H", {{"show", "S H"}, {"a", "S H"}, {"b", "S H"}, {"r", "S H"}, {"s", "S H"}}},
    {"S", {{"show", "W"}, {"a", "A"}, {"b", "A"}, {"r", "A"}, {"s", "A"}}},
    {"W", {{"show", "show ( R I ) ;"}}},
    {"R", {{"\"value=\"", "\"value=\""}, {"a", ""}, {"b", ""}, {"r", ""}, {"s", ""}}},
    {"A", {{"a", "I = E ;"}, {"b", "I = E ;"}, {"r", "I = E ;"}, {"s", "I = E ;"}}},
    {"E", {{"(", "T E'"}, {"a", "T E'"}, {"b", "T E'"}, {"r", "T E'"}, {"s", "T E'"}}},
    {"E'", {{")", ""}, {";", ""}, {"+", "+ T E'"}, {"-", "- T E'"}}},
    {"T", {{"(", "F T'"}, {"a", "F T'"}, {"b", "F T'"}, {"r", "F T'"}, {"s", "F T'"}}},
    {"T'", {{")", ""}, {"+", ""}, {"-", ""}, {";", ""}, {"*", "* F T'"}, {"/", "/ F T'"}}},
    {"F", {{"a", "I"}, {"b", "I"}, {"r", "I"}, {"s", "I"}, {"(", "( E )"}, {"+", "N"}, {"-", "N"}}},
    {"N", {{")", ""}, {"+", ""}, {"-", ""}, {"*", ""}, {"/", ""}, {";", ""}}},
    {"M", {{")", ""}, {"+", ""}, {"-", ""}, {"*", ""}, {"/", ""}, {";", ""}, {"0", "Y M"}, {"1", "Y M"}, {"2", "Y M"}, {"3", "Y M"}, {"4", "Y M"}, {"5", "Y M"}, {"6", "Y M"}, {"7", "Y M"}, {"8", "Y M"}, {"9", "Y M"}}},
    {"X", {{")", ""}, {"*", ""}, {"/", ""}, {";", ""}, {"0", ""}, {"1", ""}, {"2", ""}, {"3", ""}, {"4", ""}, {"5", ""}, {"6", ""}, {"7", ""}, {"8", ""}, {"9", ""}, {"+", "+"}, {"-", "-"}}},
    {"Y", {{"0", "0"}, {"1", "1"}, {"2", "2"}, {"3", "3"}, {"4", "4"}, {"5", "5"}, {"6", "6"}, {"7", "7"}, {"8", "8"}, {"9", "9"}}},
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
            } else if (current == "end.") {
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
            } else if (top == "end.") {
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
    std::ifstream file("final.txt");
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
