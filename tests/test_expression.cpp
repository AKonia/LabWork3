#include "gtest.h"
#include "MathExpr.hpp"

TEST(MathExpression, can_calc_correct_expression)
{
    MathExpr mexpr;
    mexpr.loadExpr("1+2+3");
    ASSERT_NO_THROW(mexpr.compute());
}

TEST(MathExpression, can_compute_according_operations_priority)
{
    MathExpr mexpr;
    mexpr.loadExpr("2+2*2");
    EXPECT_EQ((2+2*2), mexpr.compute());
}

TEST(MathExpression, can_compute_according_braces_priority)
{
    MathExpr mexpr;
    mexpr.loadExpr("(2+2)*2");
    EXPECT_EQ(((2+2)*2), mexpr.compute());
}

TEST(MathExpression, can_compute_embedded_function_abs)
{
    MathExpr mexpr;
    mexpr.loadExpr("abs(12 - 24)");
    EXPECT_EQ(12, mexpr.compute());
}

TEST(MathExpression, can_correct_compute_power)
{
    MathExpr mexpr;
    mexpr.loadExpr("2^5");
    EXPECT_EQ(32, mexpr.compute());
}

TEST(MathExpression, can_correct_compute_expr_with_binary_addition)
{
    MathExpr mexpr;
    mexpr.loadExpr("6+4");
    EXPECT_EQ((6+4), mexpr.compute());
}

TEST(MathExpression, can_correct_compute_expr_with_binary_minus)
{
    MathExpr mexpr;
    mexpr.loadExpr("12-5");
    EXPECT_EQ((12-5), mexpr.compute());
}

TEST(MathExpression, can_correct_compute_expr_with_binary_divide)
{
    MathExpr mexpr;
    mexpr.loadExpr("4/2");
    EXPECT_EQ((4/2), mexpr.compute());
}

TEST(MathExpression, can_correct_compute_expr_with_binary_multiply)
{
    MathExpr mexpr;
    mexpr.loadExpr("2*117");
    EXPECT_EQ(2*117, mexpr.compute());
}

TEST(MathExpression, can_correct_compute_expr_with_unary_minus)
{
    MathExpr mexpr;
    mexpr.loadExpr("-5 + 5");
    EXPECT_EQ((-5 + 5), mexpr.compute());
}

TEST(MathExpression, can_compute_with_multiple_braces_)
{
    MathExpr mexpr;
    mexpr.loadExpr("(((3*(2+2))))");
    EXPECT_EQ((3*(2+2)), mexpr.compute());
}

TEST(MathExpression, can_calculate_expression_with_std_constant_pi)
{
    MathExpr mexpr;
    mexpr.loadExpr("sin(pi) - cos(pi)");
    ASSERT_NEAR(1.0, mexpr.compute(), 0.00001);
}

TEST(MathExpression, throws_when_there_wasnt_entered_variable)
{
    MathExpr mexpr;
    mexpr.loadExpr("x^2 + 5");
    ASSERT_ANY_THROW(mexpr.compute());
}

TEST(MathExpression, throws_when_meet_with_unknown_token_type)
{
    MathExpr mexpr;
    mexpr.loadExpr("3 + 3var");
    ASSERT_ANY_THROW(mexpr.compute());
}

TEST(MathExpression, throws_when_braces_followed_by_constant_or_variable)
{
    MathExpr mexpr;
    mexpr.loadExpr("e+6");
    ASSERT_ANY_THROW(mexpr.loadVariable("e", 2.71));
}

TEST(MathExpression, throws_when_too_much_brackets)
{
    MathExpr mexpr;
    mexpr.loadExpr("2+2)");
    ASSERT_ANY_THROW(mexpr.compute());
}

TEST(MathExpression, throws_when_user_tries_to_change_value_of_std_constant)
{
    MathExpr mexpr;
    mexpr.loadExpr("e*2");
    ASSERT_ANY_THROW(mexpr.loadVariable("e", 2.71));
}
