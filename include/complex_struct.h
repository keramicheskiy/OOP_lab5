#pragma once

struct alignas(32) complex_structure {
    int data[8] = {};
    std::string name;

    std::string toString() const {
        std::string res = "{";
        for (int i = 0; i < 8; ++i) {
            res += std::to_string(data[i]);
            if (i < 7) res += ", ";
        }
        res += "}, \"" + name + "\"";
        return res;
    }
};

std::ostream& operator<<(std::ostream& os, const complex_structure& cs) {
    os << cs.toString();
    return os;
}
