#ifndef INFIX_TO_RPN_HPP
#define INFIX_TO_RPN_HPP

#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <map>
#include <algorithm>

#include "tokenizer.hpp"

class Infix_To_RPN {
    private:

        typedef enum {
            LEFT,
            RIGHT
        } ASSOCIATIVITY;

        std::stack<char>                stk;
        std::vector<std::string>        v;
        std::queue<std::string>         q;
        std::map<std::string, long double>   const_map;
        std::vector<std::string>        tokens;

        bool is_opperator(const char c) {
            return is_opperator(std::string(1, c));
        }

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


        int build(std::string input) {
#ifdef DEBUG
            std::cout << "BULDING" << std::endl;
#endif
            Tokenizer t;

            t.set_tokens(tokens);
            this->v = t.tokenize(input);
            if (std::find(v.begin(), v.end(), "FAIL") != v.end()) {
                return -1;
            } else {
                return 1;
            }
        }

        std::string add_spaces(std::string s) {
            std::string return_str;
            for (const char& c : s) {
                if ((isdigit(c)) || (c == '.')) {
                    return_str += c;
                } else {
                    return_str += ' ';
                    return_str += c;
                    return_str += ' ';
                }
            }
            return return_str;
        }

        bool is_const(std::string s) {
            // check if s is in map
            return const_map.find(s) != const_map.end();
        }

        long double as_prim(std::string s) {
            // return value at key s
            return const_map.find(s)->second;
        }

        ASSOCIATIVITY get_associativity(char opperator) {
            if (opperator == '^') {
                return RIGHT;
            } else {
                return LEFT;
            }
        }

        bool set_lets() {
            for (size_t i = 0; i < v.size(); ++i) {
                if (v.at(i) == "let") {
                    if ((i + 3 < v.size()) && (v.at(i + 2) == "=")) {
                        std::string tok = v.at(i + 1);
                        long double val = std::stod(v.at(i + 3));

                        const_map[tok] = val;
                        return true;
                    } else {
                        std::cerr << "Invalid syntax" << std::endl;
                    }
                }
            }
            return false;
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

        Infix_To_RPN() {
            const_map["e"]  = 2.718281828459045;
            const_map["pi"] = 3.141592653589793; 

            std::vector<std::string> tmp {"+", "-", "*", "/", "^", "%", "(", 
                ")", "=", "e", "pi", "let"
            };

            tokens = tmp;
        }

        std::string to_rpn(std::string input) {
            bool error_state = false;

            if (build(input) == -1) {
                return "FAIL";
            }

            if (set_lets()) {
                return "FAIL"; 
            }

#ifdef DEBUG
            std::cout << "Printing Tokens" << std::endl;
            for (auto item : v) {
                std::cout << "'" << item << "'" << std::endl;
            }
#endif

            for (const std::string& token : v) {
#ifdef DEBUG
                std::cout << "token: '" << token << "'" << std::endl;
#endif

                if (is_num(token)) {
#ifdef DEBUG
                    std::cout << "NUMBER" << std::endl;
                    std::cout << "pushing: '" << token << "'" << std::endl;
#endif
                    q.push(token);

                } else if (is_const(token)) {
                    q.push(std::to_string(as_prim(token)));

                } else if (is_opperator(token)) {
#ifdef DEBUG
                    std::cout << "OPPERATOR" << std::endl;
#endif
                    char tok_char = token[0];
                    while (not stk.empty()) {
#ifdef DEBUG
                        std::cout << "stk not empty" << std::endl;
#endif
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
                    v.clear();
                    std::cerr << "Unsuported option: '" << token << "'" << std::endl;
                    error_state = true;
                }

#ifdef DEBUG
                std::cout << std::endl;
#endif

            } // end for

#ifdef DEBUG
            std::cout << "END OF STREAM" << std::endl;
#endif
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
            if (error_state) {
                return "FAIL";
            } else {
                return result;
            }
        }
};

#endif
