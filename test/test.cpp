#include <fmt/core.h>
#include <gtest/gtest.h>

// Example test suite
TEST(ExampleTest, BasicAssertions) {
    // Expect equality
    EXPECT_EQ(7 * 6, 42);
    EXPECT_EQ(1 + 1, 2);
}

// Example test with string formatting
TEST(ExampleTest, StringFormatting) {
    std::string result = fmt::format("The answer is {}", 42);
    EXPECT_EQ(result, "The answer is 42");
}
