#include "DataStruct.h"
#include <iomanip>
#include <sstream>
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

std::istream& operator>>(std::istream& in, Double&& key){
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    in >> key.ref;
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

std::istream& operator>>(std::istream& in, SLongLongIO&& key){
    std::istream::sentry sentry(in);
    if (!sentry){
        return in;
    }
    in >> key.ref;
    if (in)
    {
        char next = in.peek();
        if (next == 'L' || next == 'l'){
            in.get();
            char next2 = in.peek();
            if (next2 == 'L' || next2 == 'l'){
                in.get();
            }
            else{
                in.setstate(std::ios::failbit);
            }
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
std::istream& operator>>(std::istream& in, DataStruct& data){
    std::istream::sentry sentry(in);
    if (!sentry){
        return in;
    }
    size_t currKey = 0;
    DataStruct tmp = { 0, 0, "" };
    constexpr size_t MAX_KEYS = 3;
    in >> DelimiterIO{ '(' };
    for (size_t i = 0; i < MAX_KEYS && in; ++i){
        in >> DelimiterIO{ ':' } >> DelimiterIO{ 'k' } >> DelimiterIO{ 'e' } >> DelimiterIO{ 'y' };
        in >> currKey;
        if (currKey == 1){
            in >> Double{ tmp.key1 };
        }
        else if (currKey == 2){
            in >> SLongLongIO{ tmp.key2 };
        }
        else if (currKey == 3){
            in >> StringIO{ tmp.key3 };
        }
        else{
            in.setstate(std::ios::failbit);
        }
    }
    in >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };
    if (in){
        data = tmp;
    }
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

