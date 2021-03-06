#include <iostream>

#include "infix_to_rpn.hpp"
#include "rpn.hpp"

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
        if (not line.empty()) {
            rpn_string = itr.to_rpn(line);
            if (rpn_string != "FAIL") {
                r.build(rpn_string);
                std::cout << r.run() << std::endl;
            }
        }
    }
}

int main() {
    interactive();
    return 0;
}
