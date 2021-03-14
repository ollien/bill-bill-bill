#ifndef STRINGFINDER_H
#define STRINGFINDER_H

#include <optional>
#include <string>

std::optional<std::pair<int, int>> find_longest_common_substring(const std::string &a, const std::string &b);

#endif
