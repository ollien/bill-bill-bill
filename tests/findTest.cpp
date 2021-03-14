#include <gmock/gmock.h>

#include <utility>

#include "../src/find.h"

TEST(StringFinder, FindsAtStartOfStirng) {
	auto result = find_longest_common_substring("Hello", "HelloWorld");
	auto expected = std::pair<int, int>(0, 5);

	ASSERT_EQ(result, expected);
}

TEST(StringFinder, FindsEqualStrings) {
	auto result = find_longest_common_substring("Hello", "Hello");
	auto expected = std::pair<int, int>(0, 5);

	ASSERT_EQ(result, expected);
}

TEST(StringFinder, FindsInMiddle) {
	auto result = find_longest_common_substring("AAABBAAA", "ABBA");
	auto expected = std::pair<int, int>(2, 6);

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
