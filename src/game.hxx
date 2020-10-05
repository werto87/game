#ifndef GAME_H
#define GAME_H
#include <SFML/Window/Event.hpp>
#include <cxxopts.hpp>

namespace game {
auto parseConsoleArguments(int argc, char **argv) -> cxxopts::ParseResult;

auto checkGameFinished(std::vector<int> const &field, bool &isGameWon) -> bool;

auto changeFieldEvent(std::vector<sf::Event> const &events, std::vector<int> &field) -> void;

template <typename T> auto print_container(const T &container, const std::string &delimiter) -> std::string {
  auto result = std::string{};
  std::for_each(std::begin(container), std::end(container), [&result, &delimiter](auto const &element) {
    result.append(std::to_string(element));
    result.append(delimiter);
  });
  return result;
}
} // namespace game

#endif // GAME_H
