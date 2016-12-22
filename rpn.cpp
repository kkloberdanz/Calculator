#include <iostream>
#include <stack>
#include <vector>
#include <cmath>

#include "infix_to_rpn.hpp"

class RPN {
    private:
        std::stack<std::string> stk;
        std::vector<std::string> v;

        bool is_opperator(std::string token) {
            return ((token == "+") ||
                    (token == "-") ||
                    (token == "*") ||
                    (token == "/") ||
                    (token == "^") ||
                    (token == "%") );
        }

        double calculate(char opperator, double a, double b) {
            switch (opperator) {
                case '+':
                    return a + b;
                    break;

                case '-':
                    return b - a;
                    break;

                case '*':
                    return a * b;
                    break;

                case '/':
                    return b / a;
                    break;

                case '^':
                    return pow(a, b);
                    break;

                default:
                    std::cerr << "error: '" << 
                        opperator << "' is not a valid opperator" 
                    << std::endl;
                    exit(EXIT_FAILURE);
                    break;
            }
        }

    public:
        RPN() {
            ;
        }

        RPN(std::string expr) {
            this->build(expr);
        }

        void build(std::string expr) {
            std::string word;
            for (const char& c : expr) { 
                if (c != ' ') {
                    word += c;
                } else if (not word.empty()){
                    v.push_back(word);
                    word = "";
                }
            }
            if (not word.empty()) {
                v.push_back(word);
            }
        }

        std::string pop() {
            std::string s = stk.top();
            stk.pop();
            return s;
        }

        std::string top() {
            return stk.top();
        }

        bool empty() {
            return stk.empty();
        }

        void dump() {
            while (not this->empty()) {
                std::cout << this->pop() << std::endl;
            }
        }

        std::string run() {
            for (const std::string& token : v) {
                if (not is_opperator(token)) { 
                    stk.push(token);
                } else {
                    double arg_a = atof(top().c_str());
                    stk.pop();
                    double arg_b = atof(top().c_str());
                    stk.pop();

                    double result = calculate(token[0], arg_a, arg_b);
                    stk.push(std::to_string(result));
                }
            }
            std::string result = stk.top();
            stk.pop();

            v.clear();

            if (stk.empty()) {
                return result;
            } else {
                std::cerr << "error: too many values given" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
};

int interactive() {
    Infix_To_RPN itr;
    std::string line;
    std::string rpn_string;
    RPN r;
    while (1) {
        std::cout << "> ";
        getline(std::cin, line);
        if ((line == "q") || (line == "quit")) {
            return 1;
        }
        rpn_string = itr.to_rpn(line);
        //std::cout << "RPN: " << rpn_string << std::endl;
        r.build(rpn_string);
        std::cout << r.run() << std::endl;
    }
}

int main() {
    interactive();
    /*
    RPN r;
    r.build("1 2 3 + 2 * 4 2 - /");
    std::cout << r.run() << std::endl;
    */
    return 0;
}