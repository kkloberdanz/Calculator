#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>
#include <vector>
#include <cmath>

class RPN {
    private:
        std::stack<std::string> stk;
        std::vector<std::string> v;

        bool is_operator(std::string token) {
            return ((token == "+") ||
                    (token == "-") ||
                    (token == "*") ||
                    (token == "/") ||
                    (token == "^") ||
                    (token == "%") );
        }

        long double calculate(char op, long double a, long double b) {
            switch (op) {
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
                    return pow(b, a);
                    break;

                case '%':
                    return (long long)b % (long long)a;
                    break;

                default:
                    std::cerr << "error: '" << 
                        op << "' is not a valid operator" 
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
                if (not is_operator(token)) { 
                    stk.push(token);
                } else {
                    long double arg_a = std::stold(top());
                    stk.pop();
                    long double arg_b = std::stold(top());
                    stk.pop();

                    long double result = calculate(token[0], arg_a, arg_b);
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
                return "ERROR";
            }
        }
};

#endif
