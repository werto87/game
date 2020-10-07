#include "game.hxx"
#include <spdlog/spdlog.h>
namespace game {
auto parseConsoleArguments(int argc, char **argv) -> cxxopts::ParseResult {
  cxxopts::Options options("test", "A brief description");
  options.add_options()("b,bar", "Param bar", cxxopts::value<std::string>())(
      "d,debug", "Enable debugging", cxxopts::value<bool>()->default_value("false"))(
      "f,foo", "Param foo", cxxopts::value<int>()->default_value("10"))("h,help", "Print usage");
  auto result = options.parse(argc, argv);
  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    exit(0);
  }
  return result;
}

auto checkGameFinished(std::vector<int> const &field, std::vector<int> const &winningCombination,unsigned int losingNumber, bool &isGameWon)
    -> bool {
  auto isGameFinished = false;
  if (std::find(std::begin(field), std::end(field), losingNumber) != field.end()) {
    isGameFinished = true;
    isGameWon = false;
    spdlog::info("player lost");
  }
  if (field == winningCombination) {
    spdlog::info("player won");
    isGameFinished = true;
    isGameWon = true;
  }
  return isGameFinished;
}

auto changeFieldEvent(std::vector<sf::Event> const &events, std::vector<int> &field) -> void {
  for (auto const &event : events) {
    using namespace sf;
    switch (event.type) {
    case Event::KeyPressed: {
      if (event.key.code >= Keyboard::Num1 && event.key.code <= Keyboard::Num9) {
        field.at(static_cast<size_t>(event.key.code - Keyboard::Num1)) += 1;
      }
      spdlog::info("{}", print_container(field, " "));
      break;
    }
    default: {
      break;
    }
    }
  }
}
auto readKeyFromCin() -> char {
  auto keyPressed = char{};
  spdlog::info("please press a key: ");
  std::cin >> keyPressed;
  while (!std::cin) {
    std::cin.clear();
    std::cin >> keyPressed;
  }
  spdlog::info("keyPressed: {}", keyPressed);
  return keyPressed;
}

constexpr auto OFFSET_TO_TRANSFORM_CHAR_INTO_SFML_NUMBER = 22;

auto charNumberToSfEvent(char number) -> sf::Event {
  return sf::Event{.type = sf::Event::KeyPressed,
                   .key = sf::Event::KeyEvent{
                       .code = static_cast<sf::Keyboard::Key>(number - OFFSET_TO_TRANSFORM_CHAR_INTO_SFML_NUMBER)}};
}

} // namespace game
