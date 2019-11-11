//
// json.h
// Created by TheFatNinja 
// 08-11-2019
//

#ifndef REDIS_VIEWER_JSON_H
#define REDIS_VIEWER_JSON_H

#include <string>

namespace json {

    std::string pretty_string(const std::string &original) {
        size_t pos = original.find_first_not_of(" \t\n");
        char current = original.at(pos);
        char last = '\0';
        std::string formatted;
        int scope_level = 0;
        std::stack<char> scope_stack;
        char ignore_until_char = '\0';
        while (pos < original.length()) {
            current = original.at(pos);
            pos++;
            if (ignore_until_char != '\0') {
                if (current == ignore_until_char) {
                    ignore_until_char = '\0';  // reset ignore character
                }
                formatted += current;
            }
            else {
                switch (current) {
                    case '{':
                    case '[':
                        scope_level++;
                        scope_stack.push(current);
                        formatted += current;
                        formatted.append("\n");
                        formatted.append(4 * scope_level, ' '); //indent for next part
                        break;
                    case '}':
                    case ']':
                        if (scope_level == 0) {
                            throw std::runtime_error("Mismatched scope");
                        }
                        if (scope_stack.top() == '{' && current != '}') {
                            throw std::runtime_error("Expected } but got ]");
                        }
                        else if (scope_stack.top() == '[' && current != ']') {
                            throw std::runtime_error("Expected ] but got }");
                        }
                        scope_level--;
                        scope_stack.pop();
                        formatted.append("\n");
                        formatted.append(4 * scope_level, ' '); //indent for next part
                        formatted += current;
                        break;
                    case '"':
                        formatted += current;
                        ignore_until_char = '"';
                        break;
                    case ',':
                        formatted += current;
                        formatted += "\n";
                        formatted.append(4 * scope_level, ' '); //indent for next part
                        break;
                    case '\n':
                    case '\t':
                        // Ignore these completely and skip straight to next character
                        continue;
                    case ' ':
                        if (last == ':') {
                            formatted += current;
                        }
                        break;
                    default:
                        formatted += current;
                }
            }
            last = current;
        }
        if (scope_level > 0 || ignore_until_char != '\0') {
            throw std::runtime_error("Unexpected End");
        }
        return formatted;
    }

}

#endif //REDIS_VIEWER_JSON_H
