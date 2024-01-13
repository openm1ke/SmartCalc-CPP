#include <gtest/gtest.h>

#include "../model/s21_model.h"

TEST(s21_model, exponentional_test_1) {
  s21::model model;
  model.setExpression("2.3");
  EXPECT_EQ(model.calculate(), "2.3");
}

TEST(s21_model, exponentional_test_2) {
  s21::model model;
  model.setExpression("2.3e1");
  EXPECT_EQ(model.calculate(), "23");
}

TEST(s21_model, exponentional_test_3) {
  s21::model model;
  model.setExpression("2.3e-1");
  EXPECT_EQ(model.calculate(), "0.23");
}

TEST(s21_model, exponentional_test_4) {
  s21::model model;
  model.setExpression("2.3e+1");
  EXPECT_EQ(model.calculate(), "23");
}

TEST(s21_model, exponentional_test_5) {
  s21::model model;
  model.setExpression("2.3e+10");
  EXPECT_EQ(model.calculate(), "23000000000");
}

TEST(s21_model, exponentional_test_6) {
  s21::model model;
  model.setExpression("2.3e-8");
  EXPECT_EQ(model.calculate(), "0.000000023");
}

TEST(s21_model, exponentional_test_7) {
  s21::model model;
  model.setExpression("2.3e+20");
  EXPECT_EQ(model.calculate(), "230000000000000000000");
}

TEST(s21_model, Pow1) {
  s21::model model;
  model.setExpression("2^3^2");
  EXPECT_EQ(model.calculate(), "512");
}

TEST(s21_model, Triginometric1) {
  s21::model model;
  model.setExpression(
      "acos(cos(sin(atan(ln(sqrt((8 * ((1 + 2 + 4 * 3 - 3 * 7) / 4)) + "
      "133))))))");
  EXPECT_EQ(model.calculate(), "0.922957039");
}

TEST(s21_model, Triginometric2) {
  s21::model model;
  model.setExpression("tan(40) + asin(0.5)");
  EXPECT_EQ(model.calculate(), "-0.593616155");
}

TEST(s21_model, Triginometric3) {
  s21::model model;
  model.setExpression("+8 % 3");
  EXPECT_EQ(model.calculate(), "2");
}

TEST(s21_model, Triginometric4) {
  s21::model model;
  model.setExpression("log(3) + log(4)");
  EXPECT_EQ(model.calculate(), "1.079181246");
}

TEST(s21_model, SetVarX1) {
  s21::model model;
  model.setVarX(-2);
  model.setExpression("x-x");
  EXPECT_EQ(model.calculate(), "0");
}

TEST(s21_model, SetVarX2) {
  s21::model model;
  model.setVarX(-2);
  model.setExpression("-x+x");
  EXPECT_EQ(model.calculate(), "0");
}

TEST(s21_model, SetStep1) {
  s21::model s21_model;
  s21_model.setPlotRange(-10, 10, 0, 100);
  std::vector<double> x, y;
  std::vector<double> test_x = {-10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0,
                                1,   2,  3,  4,  5,  6,  7,  8,  9,  10};
  std::vector<double> text_y = {100, 81, 64, 49, 36, 25, 16, 9,  4,  1,  0,
                                1,   4,  9,  16, 25, 36, 49, 64, 81, 100};
  s21_model.setExpression("x^2");
  s21_model.setStep(1);
  std::tie(x, y) = s21_model.plot();
  for (auto it = x.begin(), it2 = y.begin(), it3 = test_x.begin(),
            it4 = text_y.begin();
       it != x.end(); it++, it2++, it3++, it4++) {
    EXPECT_EQ(*it, *it3);
    EXPECT_EQ(*it2, *it4);
  }
}

TEST(s21_model, ErrorExtraE1) {
  s21::model model;
  model.setExpression("1.2e2e + 2");
  EXPECT_EQ(model.calculate(), model.view_error(10));
}

TEST(s21_model, ErrorExtraE2) {
  s21::model model;
  model.setExpression("e2 + 1");
  EXPECT_EQ(model.calculate(), model.view_error(11));
}

TEST(s21_model, ErrorEmptyString) {
  s21::model model;
  model.setExpression("");
  EXPECT_EQ(model.calculate(), model.view_error(1));
}

TEST(s21_model, ErrorDotWOPart) {
  s21::model model;
  model.setExpression("2..3 + 1");
  EXPECT_EQ(model.calculate(), model.view_error(5));
}

TEST(s21_model, ErrorExtraDot) {
  s21::model model;
  model.setExpression("2.3. + 1");
  EXPECT_EQ(model.calculate(), model.view_error(2));
}

TEST(s21_model, ErrorParseOperator) {
  s21::model model;
  model.setExpression("siin(30)");
  EXPECT_EQ(model.calculate(), model.view_error(3));
}

TEST(s21_model, ErrorUnknownSymbol) {
  s21::model model;
  model.setExpression("sin(30,)");
  EXPECT_EQ(model.calculate(), model.view_error(4));
}

TEST(s21_model, ErrorTwoOrMoreBinary) {
  s21::model model;
  model.setExpression("5^*2");
  EXPECT_EQ(model.calculate(), model.view_error(9));
}

TEST(s21_model, ErrorUnaryBinaryWOOperand) {
  s21::model model;
  model.setExpression("3 mod 2 ^");
  EXPECT_EQ(model.calculate(), model.view_error(7));
}

TEST(s21_model, ErrorMissedExtraBracket) {
  s21::model model;
  model.setExpression("(2+3");
  EXPECT_EQ(model.calculate(), model.view_error(6));
}

TEST(s21_model, ErrorEmptyBrackets) {
  s21::model model;
  model.setExpression("(())");
  EXPECT_EQ(model.calculate(), model.view_error(8));
}

TEST(s21_model, ErrorUnknownError) {
  s21::model model;
  model.setExpression("0/0");
#ifdef MACOS
  EXPECT_EQ(model.calculate(), "nan");
#else
  EXPECT_EQ(model.calculate(), "-nan");
#endif
}

TEST(s21_model, ErrorUnknownError2) {
  s21::model model;
  model.setExpression("1/-0");
  EXPECT_EQ(model.calculate(), "-inf");
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
