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