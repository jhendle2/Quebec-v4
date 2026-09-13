extern "C" {
    #include "Lexer/TokenType.h"
}

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Possible Identifier", "[tokenType]") {
    REQUIRE(isPossibleIdentifier("Hello"));
    REQUIRE(isPossibleIdentifier("____"));
    REQUIRE(isPossibleIdentifier("test"));
    REQUIRE(isPossibleIdentifier("test1"));

    REQUIRE_FALSE(isPossibleIdentifier("21345"));
    REQUIRE_FALSE(isPossibleIdentifier("Hello World!"));
    REQUIRE_FALSE(isPossibleIdentifier("1test"));
}

TEST_CASE("Const Int", "[tokenType]") {
    REQUIRE(isConstInt("1234"));
    REQUIRE(isConstInt("10101"));
    REQUIRE(isConstInt("238123"));

    REQUIRE_FALSE(isConstInt("a2b1c3"));
    REQUIRE_FALSE(isConstInt("0x1234"));
    REQUIRE_FALSE(isConstInt("Hello"));
    REQUIRE_FALSE(isConstInt("1.234"));
}

TEST_CASE("Const Oct", "[tokenType]") {
    REQUIRE(isConstOct("01234"));
    REQUIRE(isConstOct("0"));
    
    REQUIRE_FALSE(isConstOct("123985182"));
    REQUIRE_FALSE(isConstOct("1234"));
    REQUIRE_FALSE(isConstOct("a2b1c3"));
    REQUIRE_FALSE(isConstOct("0x1234"));
    REQUIRE_FALSE(isConstOct("Hello"));
    REQUIRE_FALSE(isConstOct("1.234"));
}

TEST_CASE("Const Hex", "[tokenType]") {
    REQUIRE(isConstHex("0x1234"));
    REQUIRE(isConstHex("0x10101"));
    REQUIRE(isConstHex("0xDEADBEEF"));
    REQUIRE(isConstHex("0xc0ffee"));

    REQUIRE_FALSE(isConstHex("0"));
    REQUIRE_FALSE(isConstHex("a2b1c3"));
    REQUIRE_FALSE(isConstHex("1234ff"));
    REQUIRE_FALSE(isConstHex("Hello"));
    REQUIRE_FALSE(isConstHex("1.234"));
}

TEST_CASE("Const Float", "[tokenType]") {
    REQUIRE(isConstFloat("0.0"));
    REQUIRE(isConstFloat("1.2345"));
    REQUIRE(isConstFloat("3.14159"));
    REQUIRE(isConstFloat(".234"));

    REQUIRE_FALSE(isConstHex("0"));
    REQUIRE_FALSE(isConstHex("a2b1c3"));
    REQUIRE_FALSE(isConstHex("1234ff"));
    REQUIRE_FALSE(isConstHex("Hello"));
    REQUIRE_FALSE(isConstHex("0.1.2"));
}