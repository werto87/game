#include <catch2/catch.hpp>
#include "game.hxx"

unsigned int Factorial(unsigned int number) {
   return number <= 1 ? number : Factorial(number - 1) * number;
}

TEST_CASE("Factorials are computed1") {
REQUIRE(game::print_container(std::vector<int>{1,2,3,4,5},",") == "1,2,3,4,5");
}


