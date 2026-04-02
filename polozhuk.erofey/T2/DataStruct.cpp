#include "DataStruct.h"
#include <iomanip>
std::istream& operator>>(std::istream& in, DelimiterIO&& dest){
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    char c ='0';
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
    if (c != 'd' && c!= 'D') {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, SLongLongIO&& dest) {
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
std::istream& operator>>(std::istream& in, StringIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry){
        return in;
    }
    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}
std::istream& operator>>(std::istream& in, DataStruct& dest){
    std::istream::sentry sentry(in);
    if (!sentry){
        return in;
    }
    std::streampos pos = in.tellg();
    DataStruct tmp{};
    bool key1 = false;
    bool key2 = false;
    bool key3 = false;
    char c1 = '0';
    char c2 = '0';
    in >> c1 >> c2;
    if (!in || c1 != '(' || c2 != ':'){
        in.clear();
        in.seekg(pos);
        std::string skip;
        std::getline(in, skip);
        in.setstate(std::ios::failbit);
        return in;
    }
    while (in){
        in >> std::ws;
        if (in.peek() == ':'){
            in.get();
            if (in.peek() == ')'){
                in.get();
                break;
            }
            continue;
        }
        std::string name;
        in >> name;
        if (!in){
            break;
        }
        if (name == "key1" && !key1){
            in >> Double{ tmp.key1 };
            if (in) {
                key1 = true;
            }
        }
        else if (name == "key2" && !key2) {
            in >> SLongLongIO{ tmp.key2 };
            if (in){
                key2 = true;
            }
        }
        else if (name == "key3" && !key3){
            in >> StringIO{ tmp.key3 };
            if (in) {
                key3 = true;
            }
        }
        else{
            in.setstate(std::ios::failbit);
            break;
        }
    }
    if (in && key1 && key2 && key3){
        dest = std::move(tmp);
        return in;
    }
    in.clear();
    in.seekg(pos);
    std::string skip;
    std::getline(in, skip);
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
