#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>

struct DataStruct {
    double key1;
    unsigned long long key2;
    std::string key3;
};

static std::string trim(const std::string str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

static bool parseBinary(const std::string str, unsigned long long& out) {
    if (str.length() < 3) return false;
    if (str[0] != '0' || (str[1] != 'b' && str[1] != 'B')) return false;
    out = 0;
    for (size_t i = 2; i < str.length(); ++i) {
        char c = str[i];
        if (c != '0' && c != '1') return false;
        out = out * 2 + (c - '0');
    }
    return true;
}

static bool parseDoubleSci(const std::string& str, double& out) {
    try {
        size_t pos;
        out = std::stod(str, &pos);
        return pos == str.length();
    } catch (...) {
        return false;
    }
}

std::istream& operator>>(std::istream& in, DataStruct& data) {
    std::string line;
    if (!std::getline(in, line)) {
        return in;
    }
    line = trim(line);
    if (line.length() < 4 || line.substr(0, 2) != "(:" || line.substr(line.length() - 2) != ":)") {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::string content = line.substr(2, line.length() - 4);
    size_t key1_pos = content.find("key1");
    size_t key2_pos = content.find("key2");
    size_t key3_pos = content.find("key3");
    if (key1_pos == std::string::npos || key2_pos == std::string::npos || key3_pos == std::string::npos) {
        in.setstate(std::ios::failbit);
        return in;
    }
    size_t start = content.find(" ", key1_pos);
    if (start == std::string::npos) {
        in.setstate(std::ios::failbit);
        return in;
    }
    start = content.find_first_not_of(" ", start);
    size_t end = content.find(":", start);
    if (end == std::string::npos) {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::string key1_str = content.substr(start, end - start);
    if (!parseDoubleSci(key1_str, data.key1)) {
        in.setstate(std::ios::failbit);
        return in;
    }
    start = content.find(' ', key2_pos);
    if (start == std::string::npos) {
        in.setstate(std::ios::failbit);
        return in;
    }
    start = content.find_first_not_of(" ", start);
    end = content.find(":", start);
    if (end == std::string::npos) {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::string key2_str = content.substr(start, end - start);
    if (!parseBinary(key2_str, data.key2)) {
        in.setstate(std::ios::failbit);
        return in;
    }
    start = content.find(' ', key3_pos);
    if (start == std::string::npos) {
        in.setstate(std::ios::failbit);
        return in;
    }
    start = content.find_first_not_of(' ', start);
    if (start == std::string::npos || content[start] != '"') {
        in.setstate(std::ios::failbit);
        return in;
    }
    start++;
    end = content.find('"', start);
    if (end == std::string::npos) {
        in.setstate(std::ios::failbit);
        return in;
    }
    data.key3 = content.substr(start, end - start);
    return in;
}

std::string formatDoubleSci(double value) {
    if (value == 0) {
        return "0.0e+0";
    }
    std::stringstream ss;
    ss << std::scientific << std::setprecision(15) << value;
    std::string result = ss.str();
    size_t e_pos = result.find('e');
    if (e_pos != std::string::npos) {
        std::string mantissa = result.substr(0, e_pos);
        std::string exponent = result.substr(e_pos);
        size_t dot_pos = mantissa.find('.');
        if (dot_pos != std::string::npos) {
            while (mantissa.back() == '0') {
                mantissa.pop_back();
            }
            if (mantissa.back() == '.') {
                mantissa.pop_back();
            }
        }
        if (exponent.length() > 2 && exponent[0] == 'e') {
            char sign = exponent[1];
            std::string exp_num = exponent.substr(2);
            size_t non_zero = 0;
            while (non_zero < exp_num.length() - 1 && exp_num[non_zero] == '0') {
                non_zero++;
            }
            exp_num = exp_num.substr(non_zero);
            exponent = "e" + std::string(1, sign) + exp_num;
        }
        result = mantissa + exponent;
    }
    return result;
}

static std::string formatBinary(unsigned long long value) {
    if (value == 0) return "0b0";
    std::string result;
    while (value > 0) {
        result.push_back((value & 1) ? '1' : '0');
        value >>= 1;
    }
    std::reverse(result.begin(), result.end());
    return "0b" + result;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
    out << "(:key1 " << formatDoubleSci(data.key1)
        << ":key2 " << formatBinary(data.key2)
        << ":key3 \"" << data.key3 << "\":)";
    return out;
}

bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) {
        return a.key1 < b.key1;
    }
    if (a.key2 != b.key2) {
        return a.key2 < b.key2;
    }
    return a.key3.length() < b.key3.length();
}

int main() {
    std::vector<DataStruct> data;
    std::copy(std::istream_iterator<DataStruct>(std::cin),
              std::istream_iterator<DataStruct>(),
              std::back_inserter(data));
    std::sort(data.begin(), data.end(), compareDataStruct);
    std::copy(data.begin(), data.end(),
              std::ostream_iterator<DataStruct>(std::cout, "\n"));
    return 0;
}
