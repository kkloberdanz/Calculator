#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

class Tokenizer {
    private:
        std::vector<std::string> output_v;
        std::vector<std::string> tokens;
        std::vector<std::string> reserved_symbols {
            "(", ")", "*", "^", "+", "=", "-", "%"
        };
        
        bool is_token(char c) {
            return is_token(std::string(1, c));
        }

        bool is_token(std::string s) {
            for (const std::string& str : tokens) { 
                if (str == s) {
                    return true;
                }
            }
            return false;
        }

        bool is_reserved(char c) {
            return is_reserved(std::string(1, c));
        }

        bool is_reserved(std::string s) {
            for (const std::string& str : reserved_symbols) { 
                if (str == s) {
                    return true;
                }
            }
            return false;
        }

    public:

        void set_tokens(std::string tok) { 
            tokens.push_back(tok);
        }

        void set_tokens(std::vector<std::string> toks) {
            for (const std::string& tok : toks) {
                tokens.push_back(tok);
            }
        }

        std::vector<std::string> get_tokens() {
            return tokens;
        }

        std::vector<std::string> tokenize(std::string input) {
#ifdef DEBUG
            std::cout << "Tokens:::" << std::endl;
            for (const auto& tok : tokens) {
                std::cout << "'" << tok << "'" << std::endl;
            }
#endif
            bool error_state = false;
            std::string token;
            for (const char& c : input) {

                if (is_reserved(c)) {
                    if (not token.empty()) {
                        output_v.push_back(token);
                    }
                    output_v.push_back(std::string(1, c));
                    token = "";

                    /*
                } else if ( ((c == ' ') || (is_reserved(c))) && (not isdigit(c)) ) { 
                    if (is_reserved(token)) {
                        output_v.push_back(token);
                        token = "";

                    } else if (c != ' ') {
                        std::cerr << "Unknown token: '" << token << "'" << std::endl;
                        error_state = true;
                    } 

                    */
                } else if (c == ' ') {
                    if (not token.empty()) {
                        output_v.push_back(token);
                        token = "";
                    }

                } else {
#ifdef DEBUG
                    std::cout << "c = " << c << std::endl;
#endif
                    token += c;
                }
            }

            output_v.push_back(token);

#ifdef DEBUG
            std::cout << "Tokenizer: Tokens:" << std::endl;
            for (const std::string s : output_v) {
                std::cout << "'" << s << "'" << std::endl;
            }
#endif
            if (error_state) {
                std::vector<std::string> error_v;
                error_v.push_back("FAIL");
                return error_v;
            } else {
                return output_v;
            }
        }
};

#endif // TOKENIZER_HPP
