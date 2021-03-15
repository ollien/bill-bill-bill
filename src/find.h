#ifndef STRINGFINDER_H
#define STRINGFINDER_H

#include <optional>
#include <string>

// Specifies a range of indexes within a string, with the first position being inclusive and the last exclusive
using StringIndexRange = std::pair<int, int>;
std::optional<StringIndexRange> find_longest_common_substring(const std::string &a, const std::string &b);

#endif
