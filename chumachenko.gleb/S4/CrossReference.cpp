#include "CrossReference.h"

#include <stdexcept>

void CrossReference::insert(const std::string& word, int line) {
    if (word.empty()) {
        throw std::invalid_argument("insert: word must not be empty");
    }
    if (line <= 0) {
        throw std::out_of_range("insert: line number must be positive");
    }
    entries_[word].insert(line);
}

bool CrossReference::remove(const std::string& word) {
    if (word.empty()) {
        throw std::invalid_argument("remove: word must not be empty");
    }
    return entries_.erase(word) > 0;
}

bool CrossReference::contains(const std::string& word) const {
    if (word.empty()) {
        throw std::invalid_argument("contains: word must not be empty");
    }
    return entries_.find(word) != entries_.end();
}

const CrossReference::LineSet& CrossReference::linesOf(const std::string& word) const {
    if (word.empty()) {
        throw std::invalid_argument("linesOf: word must not be empty");
    }
    auto it = entries_.find(word);
    if (it == entries_.end()) {
        throw std::out_of_range("linesOf: word not found: " + word);
    }
    return it->second;
}

void CrossReference::clear() {
    entries_.clear();
}

std::size_t CrossReference::size() const {
    return entries_.size();
}

bool CrossReference::empty() const {
    return entries_.empty();
}

CrossReference::ConstIterator CrossReference::begin() const {
    return entries_.begin();
}

CrossReference::ConstIterator CrossReference::end() const {
    return entries_.end();
}

