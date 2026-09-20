#include "DataStruct.h"

#include <cmath>
#include <cctype>
#include <iomanip>
#include <limits>
#include <string>

bool parseDoubleSci(
    const std::string& str,
    double& value)
{
    if (str.empty())
    {
        return false;
    }

    size_t pos = 0;

    if (str[pos] == '+' || str[pos] == '-')
    {
        ++pos;
    }

    size_t digitsBefore = 0;

    while (pos < str.size() &&
        std::isdigit(
            static_cast<unsigned char>(str[pos])))
    {
        ++digitsBefore;
        ++pos;
    }

    if (digitsBefore == 0)
    {
        return false;
    }

    if (pos >= str.size() || str[pos] != '.')
    {
        return false;
    }

    ++pos;

    size_t digitsAfter = 0;

    while (pos < str.size() &&
        std::isdigit(
            static_cast<unsigned char>(str[pos])))
    {
        ++digitsAfter;
        ++pos;
    }

    if (digitsAfter == 0)
    {
        return false;
    }

    if (pos >= str.size() ||
        (str[pos] != 'e' && str[pos] != 'E'))
    {
        return false;
    }

    ++pos;

    if (pos < str.size() &&
        (str[pos] == '+' || str[pos] == '-'))
    {
        ++pos;
    }

    size_t exponentDigits = 0;

    while (pos < str.size() &&
        std::isdigit(
            static_cast<unsigned char>(str[pos])))
    {
        ++exponentDigits;
        ++pos;
    }

    if (exponentDigits == 0 ||
        pos != str.size())
    {
        return false;
    }

    try
    {
        size_t converted = 0;
        value = std::stod(str, &converted);

        if (converted != str.size())
        {
            return false;
        }

        if (!std::isfinite(value))
        {
            return false;
        }
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool parseSignedLongLong(
    const std::string& str,
    long long& value)
{
    if (str.size() < 3)
    {
        return false;
    }

    size_t pos = 0;

    if (str[pos] == '+' || str[pos] == '-')
    {
        ++pos;
    }

    size_t digits = 0;

    while (pos < str.size() &&
        std::isdigit(
            static_cast<unsigned char>(str[pos])))
    {
        ++digits;
        ++pos;
    }

    if (digits == 0)
    {
        return false;
    }

    if (pos + 2 != str.size())
    {
        return false;
    }

    if (str[pos] != 'l' && str[pos] != 'L')
    {
        return false;
    }

    if (str[pos + 1] != 'l' && str[pos + 1] != 'L')
    {
        return false;
    }

    std::string number =
        str.substr(0, pos);

    try
    {
        size_t converted = 0;
        value = std::stoll(number, &converted);

        if (converted != number.size())
        {
            return false;
        }
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool readFieldName(
    const std::string& str,
    size_t& pos,
    std::string& name)
{
    size_t start = pos;

    while (pos < str.size())
    {
        char c = str[pos];

        if ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') ||
            c == '_')
        {
            ++pos;
        }
        else
        {
            break;
        }
    }

    if (start == pos)
    {
        return false;
    }

    name = str.substr(
        start,
        pos - start
    );

    return true;
}

bool readSimpleValue(
    const std::string& str,
    size_t& pos,
    std::string& value)
{
    size_t start = pos;

    while (pos < str.size() &&
        str[pos] != ':')
    {
        ++pos;
    }

    if (pos == start)
    {
        return false;
    }

    value = str.substr(
        start,
        pos - start
    );

    return true;
}

bool readStringValue(
    const std::string& str,
    size_t& pos,
    std::string& value)
{
    if (pos >= str.size() ||
        str[pos] != '"')
    {
        return false;
    }

    ++pos;

    std::string result;

    while (pos < str.size())
    {
        if (str[pos] == '"')
        {
            ++pos;
            value = result;
            return true;
        }

        result += str[pos];
        ++pos;
    }

    return false;
}

bool parseDataStruct(
    const std::string& input,
    DataStruct& data)
{
    if (input.size() < 4)
    {
        return false;
    }

    if (input[0] != '(' ||
        input[1] != ':')
    {
        return false;
    }

    size_t pos = 2;

    bool hasKey1 = false;
    bool hasKey2 = false;
    bool hasKey3 = false;

    while (pos < input.size())
    {
        if (pos + 1 < input.size() &&
            input[pos] == ':' &&
            input[pos + 1] == ')')
        {
            pos += 2;

            return pos == input.size() &&
                hasKey1 &&
                hasKey2 &&
                hasKey3;
        }

        std::string fieldName;

        if (!readFieldName(
            input,
            pos,
            fieldName))
        {
            return false;
        }

        if (pos >= input.size() ||
            input[pos] != ' ')
        {
            return false;
        }

        ++pos;

        if (pos >= input.size() ||
            input[pos] == ' ')
        {
            return false;
        }

        if (fieldName == "key1")
        {
            if (hasKey1)
            {
                return false;
            }

            std::string valueString;

            if (!readSimpleValue(
                input,
                pos,
                valueString))
            {
                return false;
            }

            double value;

            if (!parseDoubleSci(
                valueString,
                value))
            {
                return false;
            }

            data.key1 = value;
            hasKey1 = true;
        }
        else if (fieldName == "key2")
        {
            if (hasKey2)
            {
                return false;
            }

            std::string valueString;

            if (!readSimpleValue(
                input,
                pos,
                valueString))
            {
                return false;
            }

            long long value;

            if (!parseSignedLongLong(
                valueString,
                value))
            {
                return false;
            }

            data.key2 = value;
            hasKey2 = true;
        }
        else if (fieldName == "key3")
        {
            if (hasKey3)
            {
                return false;
            }

            if (!readStringValue(
                input,
                pos,
                data.key3))
            {
                return false;
            }

            hasKey3 = true;
        }
        else
        {
            return false;
        }

        if (pos >= input.size() ||
            input[pos] != ':')
        {
            return false;
        }

        if (pos + 1 < input.size() &&
            input[pos + 1] == ')')
        {
            continue;
        }

        ++pos;

        if (pos >= input.size())
        {
            return false;
        }
    }

    return false;
}

std::istream& operator>>(
    std::istream& in,
    DataStruct& data)
{
    std::string line;

    while (std::getline(in, line))
    {
        DataStruct temp;

        if (parseDataStruct(
            line,
            temp))
        {
            data = temp;
            return in;
        }
    }

    in.setstate(std::ios::failbit);

    return in;
}

std::ostream& operator<<(
    std::ostream& out,
    const DataStruct& data)
{
    double mantissa = data.key1;
    int exponent = 0;

    if (mantissa != 0.0)
    {
        exponent = static_cast<int>(
            std::floor(
                std::log10(
                    std::fabs(mantissa)
                )
            )
            );

        mantissa /= std::pow(
            10.0,
            exponent
        );

        if (std::fabs(mantissa) >= 10.0)
        {
            mantissa /= 10.0;
            ++exponent;
        }

        if (std::fabs(mantissa) < 1.0)
        {
            mantissa *= 10.0;
            --exponent;
        }
    }

    out << "(:key1 "
        << std::fixed
        << std::setprecision(10)
        << mantissa
        << 'e';

    if (exponent >= 0)
    {
        out << '+';
    }

    out << exponent
        << ":key2 "
        << data.key2
        << "ll:key3 \""
        << data.key3
        << "\":)";

    return out;
}

bool compareDataStruct(
    const DataStruct& lhs,
    const DataStruct& rhs)
{
    if (lhs.key1 < rhs.key1)
    {
        return true;
    }

    if (rhs.key1 < lhs.key1)
    {
        return false;
    }

    if (lhs.key2 < rhs.key2)
    {
        return true;
    }

    if (rhs.key2 < lhs.key2)
    {
        return false;
    }

    return lhs.key3.size() <
        rhs.key3.size();
}