#include <gtest/gtest.h>
#include "application.hpp"
#include <sstream>
#include <cmath>
#include <numbers>

class TestApplication : public Application {
public:
    TestApplication() : Application("plugins") {}
    
    double testCalc(const std::string& input) {
        return test(std::string(input));
    }
};

class CalculatorTest : public ::testing::Test {
protected:
    void SetUp() override {
    }
    
    void TearDown() override {
    }
    
    TestApplication app;
};

TEST_F(CalculatorTest, BasicAddition) {
    EXPECT_DOUBLE_EQ(app.test("2 + 3"), 5.0);
}

TEST_F(CalculatorTest, BasicSubtraction) {
    EXPECT_DOUBLE_EQ(app.test("5 - 2"), 3.0);
}

TEST_F(CalculatorTest, BasicMultiplication) {
    EXPECT_DOUBLE_EQ(app.test("3 * 4"), 12.0);
}

TEST_F(CalculatorTest, BasicDivision) {
    EXPECT_DOUBLE_EQ(app.test("8 / 2"), 4.0);
}

TEST_F(CalculatorTest, DivisionByZero) {
    EXPECT_THROW(app.test("5 / 0"), std::logic_error);
}

TEST_F(CalculatorTest, FloatingPointOperations) {
    EXPECT_DOUBLE_EQ(app.test("2.5 + 3.5"), 6.0);
    EXPECT_DOUBLE_EQ(app.test("5.5 * 2"), 11.0);
}

TEST_F(CalculatorTest, OperationPriority) {
    EXPECT_DOUBLE_EQ(app.test("2 + 3 * 4"), 14.0);
    EXPECT_DOUBLE_EQ(app.test("(2 + 3) * 4"), 20.0);
}

TEST_F(CalculatorTest, PowerFunction) {
    EXPECT_DOUBLE_EQ(app.test("2 ^ 3"), 8.0);
    EXPECT_DOUBLE_EQ(app.test("5 ^ 2"), 25.0);
}

TEST_F(CalculatorTest, SinFunction) {
    EXPECT_NEAR(app.test("sin(0)"), 0.0, 1e-10);
    EXPECT_NEAR(app.test("sin(1.5708)"), 1.0, 1e-4);
}

TEST_F(CalculatorTest, LogFunction) {
    EXPECT_DOUBLE_EQ(app.test("log(100, 10)"), 2.0);
    EXPECT_DOUBLE_EQ(app.test("log(8, 2)"), 3.0);
}

TEST_F(CalculatorTest, Constants) {
    EXPECT_DOUBLE_EQ(app.test("$pi"), std::numbers::pi);
    EXPECT_DOUBLE_EQ(app.test("$e"), std::numbers::e);
    EXPECT_THROW(app.test("$unknown"), std::runtime_error);
}

TEST_F(CalculatorTest, ComplexExpressions) {
    EXPECT_DOUBLE_EQ(app.test("2 + 3 ^ 2"), 11.0);
    EXPECT_NEAR(app.test("sin($pi / 2)"), 1.0, 1e-10);
    EXPECT_DOUBLE_EQ(app.test("log(8, 2) * 2 + 1"), 7.0);
}

TEST_F(CalculatorTest, InvalidExpressions) {
    EXPECT_THROW(app.test("2 +"), std::logic_error);
    EXPECT_THROW(app.test("(2 + 3"), std::logic_error);
    EXPECT_THROW(app.test("2 + * 3"), std::logic_error);
    EXPECT_THROW(app.test("unknown(5)"), std::runtime_error);
}

TEST_F(CalculatorTest, WhitespaceAndCase) {
    EXPECT_DOUBLE_EQ(app.test("  2   +   3  "), 5.0);
    EXPECT_DOUBLE_EQ(app.test("2 ^ 3"), 8.0);
    EXPECT_NEAR(app.test("Sin(1.5708)"), 1.0, 1e-5);
}