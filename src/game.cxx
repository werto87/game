#include "game.hxx"
#ifdef CONSOLE_OUTPUT
#include <spdlog/spdlog.h>
#endif


namespace game {
auto parseConsoleArguments(int argc, char **argv) -> cxxopts::ParseResult {
  cxxopts::Options options("test", "A brief description");
  options.add_options()
  ("windowWidth", "", cxxopts::value<int>()->default_value("1000"))
  ("windowHeight", "", cxxopts::value<int>()->default_value("1000"))
  ("winningPositionX", "", cxxopts::value<int>()->default_value("500"))
  ("winningPositionY", "", cxxopts::value<int>()->default_value("10"))
  ("minValue", "", cxxopts::value<int>()->default_value("0"))
  ("maxValue", "", cxxopts::value<int>()->default_value("1000"))
  ("pointCount", "", cxxopts::value<int>()->default_value("500"))
  ("startPointX", "", cxxopts::value<int>()->default_value("500"))
  ("startPointY", "", cxxopts::value<int>()->default_value("500"))
  ("maxExecutions", "", cxxopts::value<int>()->default_value("1000"))
//   ("d,debug", "Enable debugging", cxxopts::value<bool>()->default_value("false"))
//   ("f,foo", "Param foo", cxxopts::value<int>()->default_value("10"))
//   ("h,help", "Print usage")
  ;
  auto result = options.parse(argc, argv);
  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    exit(0);
  }
  return result;
}

auto checkGameFinished(std::vector<int> const &field, std::vector<int> const &winningCombination,
                       unsigned int losingNumber, bool &isGameWon) -> bool {
  auto isGameFinished = false;
  if (std::find(std::begin(field), std::end(field), losingNumber) != field.end()) {
    isGameFinished = true;
    isGameWon = false;
#ifdef CONSOLE_OUTPUT
    spdlog::info("player lost");
#endif
  }
  if (field == winningCombination) {
#ifdef CONSOLE_OUTPUT
    spdlog::info("player won");
#endif
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
#ifdef CONSOLE_OUTPUT
      spdlog::info("{}", print_container(field, " "));
#endif
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
#ifdef CONSOLE_OUTPUT
  spdlog::info("please press a key: ");
#endif
  std::cin >> keyPressed;
  while (!std::cin) {
    std::cin.clear();
    std::cin >> keyPressed;
  }
#ifdef CONSOLE_OUTPUT
  spdlog::info("keyPressed: {}", keyPressed);
#endif
  return keyPressed;
}

constexpr auto OFFSET_TO_TRANSFORM_CHAR_INTO_SFML_NUMBER = 22;

auto charNumberToSfEvent(char number) -> sf::Event {
  return sf::Event{.type = sf::Event::KeyPressed,
                   .key = sf::Event::KeyEvent{
                       .code = static_cast<sf::Keyboard::Key>(number - OFFSET_TO_TRANSFORM_CHAR_INTO_SFML_NUMBER),.alt=false,.control=false,.shift=false,.system=false}};
}

} // namespace game
