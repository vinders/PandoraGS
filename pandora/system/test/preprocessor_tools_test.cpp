#include <cstdint>
#include <string>
#include <vector>
#include <gtest/gtest.h>
#include <system/force_inline.h> // include to force compiler check
#include <system/preprocessor_tools.h>

class PreprocessorToolsTest : public testing::Test {
public:
protected:
  //static void SetUpTestCase() {}
  //static void TearDownTestCase() {}
  void SetUp() override {}
  void TearDown() override {}

  __forceinline int forceInlineTest() const { return 42; }
};

// -- expand/stringify macros --

TEST_F(PreprocessorToolsTest, expand) {
  int abc = 2;

  EXPECT_EQ(999, _P_EXPAND(999));
  EXPECT_EQ(2.5, _P_EXPAND(2.5));
  EXPECT_STREQ("abc", _P_EXPAND("abc"));
  EXPECT_EQ(abc, _P_EXPAND(abc));
}

TEST_F(PreprocessorToolsTest, stringify) {
  EXPECT_STREQ("999", _P_STRINGIFY(999));
  EXPECT_STREQ("2.5", _P_STRINGIFY(2.5));
  EXPECT_STREQ("abc", _P_STRINGIFY(abc));
}

// -- variadic macros --

TEST_F(PreprocessorToolsTest, getFirstArg) {
  int abc = 2, def = 4;
  EXPECT_EQ(5, _P_GET_FIRST_ARG(5, 7, 9, 11));
  EXPECT_EQ(5, _P_GET_FIRST_ARG(5));
  EXPECT_EQ(2.5, _P_GET_FIRST_ARG(2.5, 2.7, 2.9, 2.11));
  EXPECT_EQ(2.5, _P_GET_FIRST_ARG(2.5));
  EXPECT_STREQ("abc", _P_GET_FIRST_ARG("abc", "def"));
  EXPECT_STREQ("abc", _P_GET_FIRST_ARG("abc"));
  EXPECT_EQ(abc, _P_GET_FIRST_ARG(abc, def));
  EXPECT_EQ(def, _P_GET_FIRST_ARG(def));
  EXPECT_EQ(5, _P_GET_FIRST_ARG(5, 7.5, "abc", def));
}

TEST_F(PreprocessorToolsTest, dropFirstArg) {
  std::vector<int> arrayInt { _P_DROP_FIRST_ARG(5, 7, 9, 11) };
  ASSERT_EQ(3, arrayInt.size());
  EXPECT_EQ(7, arrayInt[0]);
  EXPECT_EQ(9, arrayInt[1]);
  EXPECT_EQ(11, arrayInt[2]);

  std::vector<double> arrayDec { _P_DROP_FIRST_ARG(2.5, 2.7, 2.9, 2.11) };
  ASSERT_EQ(3, arrayDec.size());
  EXPECT_EQ(2.7, arrayDec[0]);
  EXPECT_EQ(2.9, arrayDec[1]);
  EXPECT_EQ(2.11, arrayDec[2]);

  std::vector<std::string> arrayStr { _P_DROP_FIRST_ARG("abc", "def", "ghi") };
  ASSERT_EQ(2, arrayStr.size());
  EXPECT_EQ(std::string("def"), arrayStr[0]);
  EXPECT_EQ(std::string("ghi"), arrayStr[1]);

  std::vector<int> arrayEmpty { _P_DROP_FIRST_ARG(5) };
  EXPECT_EQ(0, arrayEmpty.size());
}

TEST_F(PreprocessorToolsTest, getArgCount) {
  EXPECT_EQ(4, _P_GET_ARG_COUNT(5, 7, 9, 11));
  EXPECT_EQ(4, _P_GET_ARG_COUNT(2.5, 2.7, 2.9, 2.11));
  EXPECT_EQ(3, _P_GET_ARG_COUNT("abc", "def", "ghi"));
  EXPECT_EQ(4, _P_GET_ARG_COUNT(5, 2.7, "def", "ghi"));
}

#define _FOREACH_TEST(x) |x
TEST_F(PreprocessorToolsTest, foreach) {
  int abc = 5 _P_FOREACH(_FOREACH_TEST, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
  EXPECT_EQ((5|0|1|2|3|4|5|6|7|8|9), abc);
}
#undef _FOREACH_TEST

#define _FOREACH_TEST(x) +x
TEST_F(PreprocessorToolsTest, foreachString) {
  std::string abc = std::string("a") _P_FOREACH(_FOREACH_TEST, "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                                  "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                                  "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                                  "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                                  "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                                  "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                                  "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                                  "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                                  "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                                  "b", "c", "d", "e", "f", "g", "h", "i", "j", "k");
  ASSERT_EQ(101, abc.size());
  EXPECT_EQ(std::string("abcdefghijkbcdefghijkbcdefghijkbcdefghijkbcdefghijkbcdefghijkbcdefghijkbcdefghijkbcdefghijkbcdefghijk"), abc);
}
#undef _FOREACH_TEST

TEST_F(PreprocessorToolsTest, foreachComma) {
  std::vector<int> array{ _P_FOREACH_COMMA(_P_EXPAND, 1, 2, 3, 4, 5, 6, 7, 8) };
  ASSERT_EQ(8, array.size());
  for (int i = 0; static_cast<size_t>(i) < array.size(); ++i)
    EXPECT_EQ(i+1, array[i]);
}

TEST_F(PreprocessorToolsTest, foreachCommaString) {
  std::vector<std::string> array{ _P_FOREACH_COMMA(_P_EXPAND, "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                                   "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                                   "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                                   "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                                   "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                                   "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                                   "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                                   "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                                   "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                                   "b", "c", "d", "e", "f", "g", "h", "i", "j", "k") };
  ASSERT_EQ(100, array.size());
  EXPECT_EQ("b", array[0]);
  EXPECT_EQ("c", array[1]);
  EXPECT_EQ("k", array[99]);
}

#define _FOREACH_TEST(x) counter+=x
TEST_F(PreprocessorToolsTest, foreachSemicolon) {
  int counter = 0;
  _P_FOREACH_SEMICOLON(_FOREACH_TEST, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
  EXPECT_EQ(100, counter);
}
#undef _FOREACH_TEST

#define _FOREACH_TEST(val, x) +(val << x)
TEST_F(PreprocessorToolsTest, paramForeach) {
  int abc = 5 _P_PARAM_FOREACH(_FOREACH_TEST, 1, 0, 1, 2, 3, 4, 0, 1, 2, 3, 4,
                               0, 1, 2, 3, 4, 0, 1, 2, 3, 4,
                               0, 1, 2, 3, 4, 0, 1, 2, 3, 4,
                               0, 1, 2, 3, 4, 0, 1, 2, 3, 4,
                               0, 1, 2, 3, 4, 0, 1, 2, 3, 4,
                               0, 1, 2, 3, 4, 0, 1, 2, 3, 4,
                               0, 1, 2, 3, 4, 0, 1, 2, 3, 4,
                               0, 1, 2, 3, 4, 0, 1, 2, 3, 4,
                               0, 1, 2, 3, 4, 0, 1, 2, 3, 4,
                               0, 1, 2, 3, 4, 0, 1, 2, 3, 4);
  EXPECT_EQ(5+20*0x1F, abc);
}
#undef _FOREACH_TEST

// -- enumerations --

enum class DummyEnum: uint32_t {
  a,
  b,
  c,
  d,
  e,
  f,
  g
};
_P_LIST_ENUM_VALUES(DummyEnum, all, a, b, c, d, e, f, g);
_P_LIST_ENUM_VALUES(DummyEnum, repeat, a, a, b, b, a, b);
_P_LIST_ENUM_VALUES(DummyEnum, odd, g, e, c, a);
_P_SERIALIZABLE_ENUM(DummyEnum, a, b, c, d, e, f, g);

TEST_F(PreprocessorToolsTest, listEnumValues) {
  ASSERT_EQ(7, DummyEnum_all().size());
  EXPECT_EQ(DummyEnum::a, DummyEnum_all()[0]);
  EXPECT_EQ(DummyEnum::b, DummyEnum_all()[1]);
  EXPECT_EQ(DummyEnum::c, DummyEnum_all()[2]);
  EXPECT_EQ(DummyEnum::d, DummyEnum_all()[3]);
  EXPECT_EQ(DummyEnum::e, DummyEnum_all()[4]);
  EXPECT_EQ(DummyEnum::f, DummyEnum_all()[5]);
  EXPECT_EQ(DummyEnum::g, DummyEnum_all()[6]);

  ASSERT_EQ(6, DummyEnum_repeat().size());
  EXPECT_EQ(DummyEnum::a, DummyEnum_repeat()[0]);
  EXPECT_EQ(DummyEnum::a, DummyEnum_repeat()[1]);
  EXPECT_EQ(DummyEnum::b, DummyEnum_repeat()[2]);
  EXPECT_EQ(DummyEnum::b, DummyEnum_repeat()[3]);
  EXPECT_EQ(DummyEnum::a, DummyEnum_repeat()[4]);
  EXPECT_EQ(DummyEnum::b, DummyEnum_repeat()[5]);

  ASSERT_EQ(4, DummyEnum_odd().size());
  EXPECT_EQ(DummyEnum::g, DummyEnum_odd()[0]);
  EXPECT_EQ(DummyEnum::e, DummyEnum_odd()[1]);
  EXPECT_EQ(DummyEnum::c, DummyEnum_odd()[2]);
  EXPECT_EQ(DummyEnum::a, DummyEnum_odd()[3]);
}

TEST_F(PreprocessorToolsTest, fromSerializableEnum) {
  EXPECT_EQ(std::string("a"), toString(DummyEnum::a));
  EXPECT_EQ(std::string("b"), toString(DummyEnum::b));
  EXPECT_EQ(std::string("c"), toString(DummyEnum::c));
  EXPECT_EQ(std::string("d"), toString(DummyEnum::d));
  EXPECT_EQ(std::string("e"), toString(DummyEnum::e));
  EXPECT_EQ(std::string("f"), toString(DummyEnum::f));
  EXPECT_EQ(std::string("g"), toString(DummyEnum::g));
}

TEST_F(PreprocessorToolsTest, toSerializableEnum) {
  DummyEnum result = DummyEnum::a;
  EXPECT_TRUE(fromString("a", result));
  EXPECT_EQ(DummyEnum::a, result);
  EXPECT_TRUE(fromString("b", result));
  EXPECT_EQ(DummyEnum::b, result);
  EXPECT_TRUE(fromString("c", result));
  EXPECT_EQ(DummyEnum::c, result);
  EXPECT_TRUE(fromString("d", result));
  EXPECT_EQ(DummyEnum::d, result);
  EXPECT_TRUE(fromString("e", result));
  EXPECT_EQ(DummyEnum::e, result);
  EXPECT_TRUE(fromString("f", result));
  EXPECT_EQ(DummyEnum::f, result);
  EXPECT_TRUE(fromString("g", result));
  EXPECT_EQ(DummyEnum::g, result);
}

TEST_F(PreprocessorToolsTest, toFromSerializableListedEnum) {
  DummyEnum result = DummyEnum::a;
  for (auto it : DummyEnum_all()) {
    EXPECT_TRUE(fromString(toString(it), result));
    EXPECT_EQ(it, result);
  }
}

// -- flags --

enum class FlagEnum: uint32_t {
  none = 0,
  b = 1,
  c = 2,
  d = 4,
  e = 8
};
_P_FLAGS_OPERATORS(FlagEnum, uint32_t);

TEST_F(PreprocessorToolsTest, flagCompare) {
  EXPECT_TRUE(FlagEnum::none != true);
  EXPECT_TRUE(FlagEnum::b == true);
  EXPECT_TRUE(FlagEnum::c == true);
  EXPECT_TRUE(FlagEnum::d == true);
  EXPECT_TRUE(FlagEnum::none == false);
  EXPECT_TRUE(FlagEnum::b != false);
  EXPECT_TRUE(FlagEnum::c != false);
  EXPECT_TRUE(FlagEnum::d != false);

  EXPECT_FALSE(FlagEnum::none < FlagEnum::none);
  EXPECT_TRUE(FlagEnum::none <= FlagEnum::none);
  EXPECT_FALSE(FlagEnum::none > FlagEnum::none);
  EXPECT_TRUE(FlagEnum::none >= FlagEnum::none);

  EXPECT_TRUE(FlagEnum::none < FlagEnum::b);
  EXPECT_TRUE(FlagEnum::none <= FlagEnum::b);
  EXPECT_FALSE(FlagEnum::none > FlagEnum::b);
  EXPECT_FALSE(FlagEnum::none >= FlagEnum::b);

  EXPECT_TRUE(FlagEnum::b < FlagEnum::d);
  EXPECT_TRUE(FlagEnum::b <= FlagEnum::d);
  EXPECT_FALSE(FlagEnum::b > FlagEnum::d);
  EXPECT_FALSE(FlagEnum::b >= FlagEnum::d);
}

TEST_F(PreprocessorToolsTest, flagOperation) {
  EXPECT_EQ(FlagEnum::none, FlagEnum::none&FlagEnum::b);
  EXPECT_EQ(FlagEnum::b, FlagEnum::none|FlagEnum::b);
  EXPECT_EQ(FlagEnum::b, FlagEnum::none^FlagEnum::b);
  EXPECT_EQ(((uint32_t)-1) ^ 1, (uint32_t)~FlagEnum::b);

  EXPECT_EQ(FlagEnum::none, FlagEnum::b&FlagEnum::c);
  EXPECT_EQ(3, (int)(FlagEnum::b|FlagEnum::c));
  EXPECT_EQ(3, (int)(FlagEnum::b^FlagEnum::c));

  EXPECT_EQ(FlagEnum::c, (FlagEnum)(3)&FlagEnum::c);
  EXPECT_EQ(3, (int)((FlagEnum)3|FlagEnum::c));
  EXPECT_EQ(FlagEnum::b, (FlagEnum)(3)^FlagEnum::c);

  FlagEnum abc = FlagEnum::b;
  abc &= FlagEnum::b;
  EXPECT_EQ(FlagEnum::b, abc);
  abc |= FlagEnum::c;
  EXPECT_EQ(3, (int)abc);
  abc ^= FlagEnum::c;
  EXPECT_EQ(FlagEnum::b, abc);
  abc &= FlagEnum::d;
  EXPECT_EQ(FlagEnum::none, abc);
}

TEST_F(PreprocessorToolsTest, flagAddRemove) {
  FlagEnum abc = FlagEnum::none;
  addFlag(abc, FlagEnum::b);
  EXPECT_EQ(FlagEnum::b, abc);
  addFlag(abc, FlagEnum::c);
  EXPECT_EQ(3, (int)abc);
  removeFlag(abc, FlagEnum::c);
  EXPECT_EQ(FlagEnum::b, abc);
}

// -- duplication --

TEST_F(PreprocessorToolsTest, duplicate) {
  int abc = 0 _P_DUPLICATE_64X(+1);
  EXPECT_EQ(64, abc);

  abc = 0 _P_DUPLICATE_48X(+1);
  EXPECT_EQ(48, abc);
  abc = 0 _P_DUPLICATE_24X(+1);
  EXPECT_EQ(24, abc);
  abc = 0 _P_DUPLICATE_12X(+1);
  EXPECT_EQ(12, abc);
  abc = 0 _P_DUPLICATE_10X(+1);
  EXPECT_EQ(10, abc);
  abc = 0 _P_DUPLICATE_9X(+1);
  EXPECT_EQ(9, abc);
  abc = 0 _P_DUPLICATE_7X(+1);
  EXPECT_EQ(7, abc);
}

TEST_F(PreprocessorToolsTest, duplicateComma) {
  std::vector<int> array{ _P_DUPLICATE_64X_COMMA(42) };
  EXPECT_EQ(64, array.size());
  for (auto it : array) {
    EXPECT_EQ(42, it);
  }

  array = { _P_DUPLICATE_48X_COMMA(42) };
  EXPECT_EQ(48, array.size());
  array = { _P_DUPLICATE_24X_COMMA(42) };
  EXPECT_EQ(24, array.size());
  array = { _P_DUPLICATE_12X_COMMA(42) };
  EXPECT_EQ(12, array.size());
  array = { _P_DUPLICATE_10X_COMMA(42) };
  EXPECT_EQ(10, array.size());
  array = { _P_DUPLICATE_9X_COMMA(42) };
  EXPECT_EQ(9, array.size());
  array = { _P_DUPLICATE_7X_COMMA(42) };
  EXPECT_EQ(7, array.size());
}

TEST_F(PreprocessorToolsTest, duplicateSemicolon) {
  int counter = 0;
  _P_DUPLICATE_64X_SEMICOLON(++counter);
  EXPECT_EQ(64, counter);

  counter = 0;
  _P_DUPLICATE_48X_SEMICOLON(++counter);
  EXPECT_EQ(48, counter);
  counter = 0;
  _P_DUPLICATE_24X_SEMICOLON(++counter);
  EXPECT_EQ(24, counter);
  counter = 0;
  _P_DUPLICATE_12X_SEMICOLON(++counter);
  EXPECT_EQ(12, counter);
  counter = 0;
  _P_DUPLICATE_10X_SEMICOLON(++counter);
  EXPECT_EQ(10, counter);
  counter = 0;
  _P_DUPLICATE_9X_SEMICOLON(++counter);
  EXPECT_EQ(9, counter);
  counter = 0;
  _P_DUPLICATE_7X_SEMICOLON(++counter);
  EXPECT_EQ(7, counter);
}

// -- force inline --

TEST_F(PreprocessorToolsTest, forceInlineMethod) {
  EXPECT_EQ(42, forceInlineTest());
}
