// TestStringEnums.cpp : This is a BOOST_TEST_MODULE to test Enum.h
//

#include "pch.h"
#define DEBUG_ENUM // uncomment this to debug.
#include "Enum.h"

#include <iostream>

#define BOOST_TEST_MODULE EnumTesting
#include <boost/test/unit_test.hpp>

MAKE_ENUM(TestEnum,
	TE_A,
	TE_B,
	TE_C = 10,
	TE_D = TE_B,
	TE_E,
	TE_F = TE_B);

enum ControlEnum {
	CE_A,
	CE_B,
	CE_C = 10,
	CE_D = CE_B,
	CE_E,
	CE_F = CE_B
};

MAKE_ENUM(TestEnumWithExplicitZero,
	TEWEZ_A = 0,
	TEWEZ_B = 11,
	TEWEZ_C = 0xf
);

BOOST_AUTO_TEST_CASE(compare_values)
{
	BOOST_CHECK(TE_A == CE_A);
	BOOST_CHECK(TE_B == CE_B);
	BOOST_CHECK(TE_C == CE_C);
	BOOST_CHECK(TE_D == CE_D);
	BOOST_CHECK(TE_E == CE_E);
	BOOST_CHECK(TE_F == CE_F);
}

BOOST_AUTO_TEST_CASE(check_strings)
{
	BOOST_CHECK_MESSAGE(getStringTestEnum(TE_A).find("TE_A") != std::string::npos, getStringTestEnum(TE_A));
	BOOST_CHECK_MESSAGE(getStringTestEnum(TE_B).find("TE_B") != std::string::npos, getStringTestEnum(TE_B));
	BOOST_CHECK_MESSAGE(getStringTestEnum(TE_C).find("TE_C") != std::string::npos, getStringTestEnum(TE_C));
	BOOST_CHECK_MESSAGE(getStringTestEnum(TE_D).find("TE_D") != std::string::npos, getStringTestEnum(TE_D));
	BOOST_CHECK_MESSAGE(getStringTestEnum(TE_E).find("TE_E") != std::string::npos, getStringTestEnum(TE_E));
	BOOST_CHECK_MESSAGE(getStringTestEnum(TE_F).find("TE_F") != std::string::npos, getStringTestEnum(TE_F));
}

MAKE_TYPED_ENUM(TestSizedEnum, char, __T);

BOOST_AUTO_TEST_CASE(check_size)
{
	BOOST_CHECK(sizeof(TestSizedEnum) == sizeof(char));
}

BOOST_AUTO_TEST_CASE(ensure_explicit_zero_works)
{
	BOOST_CHECK(getStringTestEnumWithExplicitZero(TEWEZ_A) == "TEWEZ_A");
	BOOST_CHECK(TEWEZ_A == 0);
	BOOST_CHECK(getStringTestEnumWithExplicitZero(TEWEZ_B) == "TEWEZ_B");
	BOOST_CHECK(TEWEZ_B == 11);
	BOOST_CHECK(getStringTestEnumWithExplicitZero(TEWEZ_B) == "TEWEZ_B");
	BOOST_CHECK(TEWEZ_C == 0xf);
	BOOST_CHECK(getStringTestEnumWithExplicitZero(TEWEZ_C) == "TEWEZ_C");
}
