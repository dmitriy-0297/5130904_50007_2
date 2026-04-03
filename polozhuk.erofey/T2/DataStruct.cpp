#include "DataStruct.h"
#include <iomanip>
#include <sstream>
#include <cctype>

std::istream& operator>>(std::istream& in, DelimiterIO&& dest){
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    char c = '0';
    in >> c;
    if (c != dest.exp) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, Double&& dest){
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    in >> dest.ref;
    if (!in) {
        return in;
    }
    char c = '0';
    in >> c;
    if (c != 'd' && c != 'D') {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, SLongLongIO&& dest){
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    in >> dest.ref;
    if (!in) {
        return in;
    }
    char c1 = in.peek();
    if (c1 == 'L' || c1 == 'l'){
        in.get(c1);
        char c2 = in.peek();
        if (c2 == 'L' || c2 == 'l'){
            in.get(c2);
        }
        else{
            in.setstate(std::ios::failbit);
        }
    }
    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest){
    std::istream::sentry sentry(in);
    if (!sentry){
        return in;
    }
    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}
std::istream& operator>>(std::istream& in, DataStruct& dest){
    std::string line;
    while (std::getline(in, line)){
        std::istringstream iss(line);
        char a ='0';
        char b='0';
        if (!(iss >> a >> b) || a != '(' || b != ':') {
            continue;
        }
        DataStruct tmp{};
        bool ok1 = false;
        bool ok2 = false;
        bool ok3 = false;
        for (int i = 0; i < 3 && iss; ++i){
            std::string name;
            if (!(iss >> name)) {
                break;
            }
            if (name == "key1" && !ok1) {
                double v;
                char s = '0';
                if (iss >> v >> s && (s == 'd' || s == 'D')) {
                    tmp.key1 = v; ok1 = true;
                }
            }
            else if (name == "key2" && !ok2) {
                long long v;
                char s1 = '0';
                char s2 = '0';
                if (iss >> v >> s1 >> s2) {
                    s1 = std::tolower(s1); s2 = std::tolower(s2);
                    if (s1 == 'l' && s2 == 'l') {
                        tmp.key2 = v; ok2 = true;
                    }
                }
            }
            else if (name == "key3" && !ok3) {
                char q = '0';
                std::string str;
                if (iss >> q && q == '"' && std::getline(iss, str, '"')) {
                    tmp.key3 = str; ok3 = true;
                }
            }
            if (!iss) {
                break;
            }
        }
        if (iss && ok1 && ok2 && ok3) {
            dest = tmp;
            return in;
        }
    }
    in.setstate(std::ios::failbit);
    return in;
}
std::ostream& operator<<(std::ostream& out, const DataStruct& src){
    std::ostream::sentry sentry(out);
    if (!sentry) {
        return out;
    }
    iofmtguard g(out);
    out << "(:key1 ";
    out << std::fixed << std::setprecision(1) << src.key1 << "d:";
    out << "key2 " << src.key2 << "LL:";
    out << "key3 \"" << src.key3 << "\":)";
    return out;
}

iofmtguard::iofmtguard(std::basic_ios<char>& s): s_(s), width_(s.width()), fill_(s.fill()),
precision_(s.precision()), fmt_(s.flags()){}
iofmtguard::~iofmtguard(){
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
}