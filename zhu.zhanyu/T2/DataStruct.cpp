#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cctype>

struct DataStruct
{
    double key1;
    unsigned long long key2;
    std::string key3;
};

static std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

static bool parseBinary(const std::string& str, unsigned long long& out)
{
    if (str.length() < 3) return false;
    if (str[0] != '0' || (str[1] != 'b' && str[1] != 'B')) return false;
    out = 0;
    for (size_t i = 2; i < str.length(); ++i)
    {
        char c = str[i];
        if (c != '0' && c != '1') return false;
        out = out * 2 + (c - '0');
    }
    return true;
}

static bool parseDoubleSci(const std::string& str, double& out)
{
    try
    {
        size_t pos;
        out = std::stod(str, &pos);
        return pos == str.length();
    }
    catch (...) { return false; }
}

static std::string formatDoubleSci(double value)
{
    if (value == 0.0) return "0.0e+0";
    std::stringstream ss;
    ss << std::scientific << std::setprecision(15) << value;
    std::string result = ss.str();
    size_t e_pos = result.find('e');
    if (e_pos != std::string::npos)
    {
        std::string mantissa = result.substr(0, e_pos);
        std::string exponent = result.substr(e_pos);
        size_t dot_pos = mantissa.find('.');
        if (dot_pos != std::string::npos)
        {
            while (mantissa.back() == '0') mantissa.pop_back();
            if (mantissa.back() == '.') mantissa.pop_back();
        }
        if (mantissa.find('.') == std::string::npos) mantissa += ".0";
        if (exponent.length() > 2 && exponent[0] == 'e')
        {
            char sign = exponent[1];
            std::string exp_num = exponent.substr(2);
            size_t non_zero = 0;
            while (non_zero < exp_num.length() - 1 && exp_num[non_zero] == '0') ++non_zero;
            exp_num = exp_num.substr(non_zero);
            exponent = "e" + std::string(1, sign) + exp_num;
        }
        result = mantissa + exponent;
    }
    return result;
}

static std::string formatBinary(unsigned long long value)
{
    if (value == 0) return "0b0";
    std::string result;
    while (value > 0)
    {
        result.push_back((value & 1) ? '1' : '0');
        value >>= 1;
    }
    std::reverse(result.begin(), result.end());
    if (result.length() == 1) result = "0" + result;
    return "0b" + result;
}

static bool readQuotedString(std::istream& in, std::string& out)
{
    char c;
    if (!(in >> std::ws) || in.get(c), c != '"') return false;
    out.clear();
    while (in.get(c) && c != '"') out += c;
    return c == '"';
}

static bool readFieldValue(std::istream& in, const std::string& key, DataStruct& temp, bool& hasKey1, bool& hasKey2, bool& hasKey3)
{
    if (key == "key1")
    {
        std::string val;
        char c;
        while (in.get(c) && c != ':' && c != ')') val += c;
        if (c == ':') in.unget();
        if (parseDoubleSci(trim(val), temp.key1))
        {
            hasKey1 = true;
            return true;
        }
        return false;
    }
    else if (key == "key2")
    {
        std::string val;
        char c;
        while (in.get(c) && c != ':' && c != ')') val += c;
        if (c == ':') in.unget();
        if (parseBinary(trim(val), temp.key2))
        {
            hasKey2 = true;
            return true;
        }
        return false;
    }
    else if (key == "key3")
    {
        std::string val;
        if (readQuotedString(in, val))
        {
            temp.key3 = val;
            hasKey3 = true;
            return true;
        }
        return false;
    }
    return false;
}

std::istream& operator>>(std::istream& in, DataStruct& data)
{
    std::string line;
    while (std::getline(in, line))
    {
        line = trim(line);
        if (line.length() < 4 || line.substr(0, 2) != "(:" || line.substr(line.length() - 2) != ":)")
            continue;

        std::istringstream iss(line.substr(2, line.length() - 4));
        DataStruct temp;
        bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
        bool ok = true;

        for (int i = 0; i < 3 && ok; ++i)
        {
            std::string key;
            char c;
            while (iss.get(c) && c != ' ') key += c;
            if (key.empty()) { ok = false; break; }

            if (!readFieldValue(iss, key, temp, hasKey1, hasKey2, hasKey3))
            {
                ok = false;
                break;
            }

            char sep;
            if (iss.get(sep) && sep != ':')
            {
                if (sep == ')') break;
                ok = false;
                break;
            }
        }

        if (ok && hasKey1 && hasKey2 && hasKey3)
        {
            data = temp;
            return in;
        }
    }
    in.setstate(std::ios::failbit);
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data)
{
    out << "(:key1 " << formatDoubleSci(data.key1)
        << ":key2 " << formatBinary(data.key2)
        << ":key3 \"" << data.key3 << "\":)";
    return out;
}

bool compareDataStruct(const DataStruct& a, const DataStruct& b)
{
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.length() < b.key3.length();
}

int main()
{
    std::vector<DataStruct> data;
    std::copy(std::istream_iterator<DataStruct>(std::cin),
              std::istream_iterator<DataStruct>(),
              std::back_inserter(data));
    std::sort(data.begin(), data.end(), compareDataStruct);
    std::copy(data.begin(), data.end(),
              std::ostream_iterator<DataStruct>(std::cout, "\n"));
    return 0;
}
