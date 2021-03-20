#include <bits/ranges_algo.h>
#include <pstl/glue_execution_defs.h>
#ifdef GUI_MODE
#include "src/view.hxx"
#endif
#include "src/game.hxx"
#include "src/gameConstant.hxx"
#include "src/pointWithHistory.hxx"
#include "src/scopeDuration.hxx"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <algorithm>
#include <array>
#include <boost/foreach.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/range.hpp>
#include <boost/range/combine.hpp>
#include <execution>
#include <imgui-SFML.h>
#include <imgui.h>
#include <magic_enum.hpp>
#include <ranges>
#include <spdlog/spdlog.h>

// See https://stackoverflow.com/a/8513803/2706707
template <typename... Containers>
auto zip(Containers &&...containers)
    -> boost::iterator_range<boost::zip_iterator<decltype(boost::make_tuple(std::begin(containers)...))>> {
  auto zip_begin = boost::make_zip_iterator(boost::make_tuple(std::begin(containers)...));
  auto zip_end = boost::make_zip_iterator(boost::make_tuple(std::end(containers)...));
  return boost::make_iterator_range(zip_begin, zip_end);
}
int main(int argc, char **argv) {
  auto consoleArguments = game::parseConsoleArguments(argc, argv);
#ifdef GUI_MODE
  auto const windowWidth = consoleArguments["windowWidth"].as<int>();
  auto const windowHeight = consoleArguments["windowHeight"].as<int>();
#endif
  auto const winningPositionX = consoleArguments["winningPositionX"].as<int>();
  auto const winningPositionY = consoleArguments["winningPositionY"].as<int>();
  auto const minValue = consoleArguments["minValue"].as<int>();
  auto const maxValue = consoleArguments["maxValue"].as<int>();
  auto const pointCount = consoleArguments["pointCount"].as<int>();
  auto const startPointX = consoleArguments["startPointX"].as<int>();
  auto const startPointY = consoleArguments["startPointY"].as<int>();
  auto const winningPosition =
      std::pair{static_cast<unsigned short int>(winningPositionX), static_cast<unsigned short int>(winningPositionY)};
  auto const startPoint =
      std::pair{static_cast<unsigned short int>(startPointX), static_cast<unsigned short int>(startPointY)};
  auto points = std::vector<game::PointWithHistory<unsigned short int>>(static_cast<size_t>(pointCount));
  std::generate(points.begin(), points.end(), [&startPoint]() { return startPoint; });
  auto limit = std::pair<unsigned short int, unsigned short int>(minValue, maxValue);
  auto fittest = std::optional<game::PointWithHistory<unsigned short int>>{};
#ifdef GUI_MODE
  sf::RenderWindow window(sf::VideoMode{static_cast<unsigned>(windowWidth), static_cast<unsigned>(windowHeight)},
                          "ImGui + SFML = <3");
  // window.setFramerateLimit(10);
  ImGui::SFML::Init(window);
  sf::Clock deltaClock;
#endif
  auto executions = 0;
  auto generation = 1;
  while (
#ifdef GUI_MODE
      window.isOpen() &&
#endif
      true) {
    spdlog::info("generation {}", generation);
    auto executionDurations = std::vector<long long>{};
    executionDurations.reserve(static_cast<size_t>(game::maxExecutions));
    while (
#ifdef GUI_MODE
        window.isOpen() &&
#endif
        executions < game::maxExecutions) {
      auto executionDuration = game::ScopeDuration{};
#ifdef GUI_MODE
      auto eventsToProcess = view::extractEvent(window);
      view::changeWindowEvent(eventsToProcess, window);
#endif
      auto directions = std::vector<game::Direction>(pointCount);
      std::for_each(std::execution::par, directions.begin(), directions.end(), [](auto &direction) {
        thread_local std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<unsigned int> distribution(1, 9);
        direction = static_cast<game::Direction>(distribution(generator));
      });
      std::ranges::for_each(zip(points, directions), [](auto value) {
        auto [point, direction] = value;
        point.movePointInDirection(direction, 1);
      });
      ++executions;
#ifdef GUI_MODE
      view::drawBoard(window, points, winningPosition, deltaClock);
#endif
      executionDurations.push_back(executionDuration.duration());
    }
    ++generation;
    executions = 0;
    spdlog::info("executionDurations total duration in generation: {}",
                 std::accumulate(executionDurations.begin(), executionDurations.end(), 0ul));
    spdlog::info("executionDurations meanDuration: {}",
                 std::accumulate(executionDurations.begin(), executionDurations.end(), 0ul) /
                     executionDurations.size());
    points = std::vector<game::PointWithHistory<unsigned short int>>(static_cast<size_t>(pointCount));
    std::generate(points.begin(), points.end(), [&startPoint]() { return startPoint; });
  }
#ifdef GUI_MODE
  window.close();
  ImGui::SFML::Shutdown();
#endif
  return 0;
}
