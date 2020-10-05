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

auto checkGameFinished(std::vector<int> const &field, bool &isGameWon) -> bool {
  auto isGameFinished = false;
  if (std::find(std::begin(field), std::end(field), 10) != field.end()) {
    isGameFinished = true;
    isGameWon = false;
    spdlog::info("player lost");
  }
  if (field == std::vector<int>{1, 1, 1, 1, 1, 1, 1, 1, 1}) {
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
      spdlog::info("1 2 3 4 5 6 7 8 9");
      spdlog::info("{}", print_container(field, " "));
      break;
    }
    default: {
      break;
    }
    }
  }
}
} // namespace game
