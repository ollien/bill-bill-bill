#include "../src/find.h"
#include <gmock/gmock.h>

TEST(StringFinder, FindsAtStartOfStirng) {
	auto result = find_longest_common_substring("Hello", "HelloWorld");
	ASSERT_EQ(result, "Hello");
}

TEST(StringFinder, FindsEqualStrings) {
	auto result = find_longest_common_substring("Hello", "Hello");
	ASSERT_EQ(result, "Hello");
}

TEST(StringFinder, FindsInMiddle) {
	auto result = find_longest_common_substring("AAABBAAA", "ABBA");
	ASSERT_EQ(result, "ABBA");
}

TEST(StringFinder, MatchDoesNotExist) {
	auto result = find_longest_common_substring("hello", ":(");
	ASSERT_EQ(result, "");
}

TEST(StringFinder, MatchOneStringEmpty) {
	auto result = find_longest_common_substring("hello", "");
	ASSERT_EQ(result, "");
}
