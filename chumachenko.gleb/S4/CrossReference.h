#ifndef CROSS_REFERENCE_H
#define CROSS_REFERENCE_H

#include <cstddef>
#include <map>
#include <set>
#include <string>

class CrossReference {
public:
    using LineSet = std::set<int>;
    using Storage = std::map<std::string, LineSet>;
    using ConstIterator = Storage::const_iterator;

    CrossReference() = default;

    void insert(const std::string& word, int line);
    bool remove(const std::string& word);
    bool contains(const std::string& word) const;
    const LineSet& linesOf(const std::string& word) const;
    void clear();
    std::size_t size() const;
    bool empty() const;

    ConstIterator begin() const;
    ConstIterator end() const;

private:
    Storage entries_;
};

#endif


