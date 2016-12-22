#ifndef INFIX_TO_RPN_HPP
#define INFIX_TO_RPN_HPP

#include <iostream>
#include <stack>
#include <queue>
#include <vector>

class Infix_To_RPN {
    private:

        typedef enum {
            LEFT,
            RIGHT
        } ASSOCIATIVITY;

        std::stack<char> stk;
        std::vector<std::string> v;
        std::queue<std::string> q;

        bool is_opperator(std::string token) {
            return ((token == "+") ||
                    (token == "-") ||
                    (token == "*") ||
                    (token == "/") ||
                    (token == "^") ||
                    (token == "%") );
        }

        bool is_num(std::string s) { 
            for (const char& c : s) {
                if (((c < '0') || (c > '9')) && (c != '.')) { 
                    return false;
                }
            }
            return true;
        }

        void build(std::string input) {
            std::string word;
            for (const char& c : input) {
                if (c == ' ') {
                    v.push_back(word);
                    word = "";
                } else {
                    word += c;
                }
            }
            v.push_back(word);
        }

        ASSOCIATIVITY get_associativity(char opperator) {
            if (opperator == '^') {
                return RIGHT;
            } else {
                return LEFT;
            }
        }

        int get_precedence(char opperator) {
            switch (opperator) {
                case '^':
                    return 4;
                    break;

                case '*':
                    return 3;
                    break;

                case '/':
                    return 3;
                    break;

                case '+':
                    return 2;
                    break;

                case '-':
                    return 2;
                    break;

                default:
                    std::cerr << "error: No such opperator: '" 
                        << opperator << "'" << std::endl;
                    exit(EXIT_FAILURE);
                    break;
            }
        }

    public:

        std::string to_rpn(std::string input) {
            this->build(input);
            /*
            for (auto item : v) {
                std::cout << "'" << item << "'" << std::endl;
            }
            */

            for (const std::string& token : v) {
                //std::cout << "token: '" << token << "'" << std::endl;

                if (is_num(token)) {
                    //std::cout << "NUMBER" << std::endl;
                    //std::cout << "pushing: '" << token << "'" << std::endl;
                    q.push(token);

                } else if (is_opperator(token)) {
                    //std::cout << "OPPERATOR" << std::endl;
                    char tok_char = token[0];
                    while (not stk.empty()) {
                        //std::cout << "stk not empty" << std::endl;
                        if (stk.top() == '(') {
                            break;
                        }
                        if ((get_associativity(tok_char) == LEFT) && 
                            (get_precedence(tok_char) <= get_precedence(stk.top())) ) {

                            char op2 = stk.top();
                            stk.pop(); 
                            q.push(std::string(1, op2));
                        } else if ((get_associativity(tok_char) == RIGHT) && 
                                    (get_precedence(tok_char)) < stk.top()) {

                            char op2 = stk.top();
                            stk.pop(); 
                            q.push(std::string(1, op2));

                        } else {
                            break;
                        }
                    }
                    stk.push(tok_char);

                } else if (token == "(") {
                    stk.push('(');

                } else if (token == ")") {
                    while (stk.top() != '(') {
                        q.push(std::string(1, stk.top()));
                        stk.pop();
                    }
                    stk.pop();

                } else {
                    std::cerr << "Unsuported option: '" << token << "'" << std::endl;
                }

                //std::cout << std::endl;

            } // end for

            //std::cout << "END OF STREAM" << std::endl;
            while (not stk.empty()) {
                q.push(std::string(1, stk.top()));
                stk.pop();
            }
            std::string result;
            while (not q.empty()) {
                result += q.front() + " ";
                q.pop();
            }

            v.clear();
            return result;
        }
};

/*
int main() {
    Infix_To_RPN inf;

    std::cout << inf.to_rpn("1 + ( 4 * 7 )") << std::endl;
    return 0;
}
*/

#endif
