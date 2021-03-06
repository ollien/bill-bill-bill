#include <gtest/gtest.h>

#include <utility>

#include "../src/find.hpp"

TEST(StringFinder, FindsAtStartOfStirng) {
	auto result = find_longest_common_substring("Hello", "HelloWorld");
	auto expected = std::make_pair(0, 5);

	ASSERT_EQ(result, expected);
}

TEST(StringFinder, FindsEqualStrings) {
	auto result = find_longest_common_substring("Hello", "Hello");
	auto expected = std::make_pair(0, 5);

	ASSERT_EQ(result, expected);
}

TEST(StringFinder, FindsInMiddle) {
	auto result = find_longest_common_substring("AAABBAAA", "ABBA");
	auto expected = std::make_pair(2, 6);

	ASSERT_EQ(result, expected);
}

TEST(StringFinder, MatchDoesNotExist) {
	auto result = find_longest_common_substring("hello", ":(");
	auto expected = std::nullopt;

	ASSERT_EQ(result, expected);
}

TEST(StringFinder, MatchOneStringEmpty) {
	auto result = find_longest_common_substring("hello", "");
	auto expected = std::nullopt;

	ASSERT_EQ(result, expected);
}
