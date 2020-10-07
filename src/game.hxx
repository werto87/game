#ifndef GAME_H
#define GAME_H
#include <SFML/Window/Event.hpp>
#include <cxxopts.hpp>

namespace game {
auto parseConsoleArguments(int argc, char **argv) -> cxxopts::ParseResult;

auto checkGameFinished(std::vector<int> const &field,std::vector<int> const &winningCombination,unsigned int losingNumber, bool &isGameWon) -> bool;

auto changeFieldEvent(std::vector<sf::Event> const &events, std::vector<int> &field) -> void;

auto readKeyFromCin() -> char;

auto charNumberToSfEvent(char number) -> sf::Event; 

template <typename T> auto print_container(const T &container, const std::string &delimiter) -> std::string {
  auto result = std::string{};
  std::for_each(std::begin(container), std::end(container), [&result, &delimiter](auto const &element) {
    result.append(std::to_string(element));
    result.append(delimiter);
  });
  if (!result.empty()) {
      result.pop_back();
  }
  return result;
}
} // namespace game

#endif // GAME_H
