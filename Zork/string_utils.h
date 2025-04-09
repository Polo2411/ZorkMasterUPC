#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <algorithm>
#include <cctype>

inline std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return result;
}

static std::string getIndefiniteArticle(const std::string& name) {
    if (name.empty()) return "a"; // caso raro
    char c = std::tolower(name[0]);
    // Si empieza con vocal, usamos "an"
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
        return "an";
    return "a";
}


#endif // STRING_UTILS_H
