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
#include <execution>
#include <imgui-SFML.h>
#include <imgui.h>
#include <spdlog/spdlog.h>
int main(int argc, char **argv) {

  // todo find out why array<ClassWithArrayAsMemberAndTemplate<SomeType>> does not work -- maybe its because of
  // template?=??

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
  // auto const maxExecutions = consoleArguments["maxExecutions"].as<int>();
  auto const winningPosition =
      std::pair{static_cast<unsigned short int>(winningPositionX), static_cast<unsigned short int>(winningPositionY)};
  auto const startPoint =
      std::pair{static_cast<unsigned short int>(startPointX), static_cast<unsigned short int>(startPointY)};
  auto points = std::vector<game::PointWithHistory<unsigned short int>>(static_cast<size_t>(pointCount));
  std::generate(points.begin(), points.end(), [&startPoint]() { return startPoint; });
  std::for_each(std::execution::par_unseq, points.begin(), points.end(),
                [](game::PointWithHistory<unsigned short int> &point) { point.generateRndDirections(); });
  auto limit = std::pair<unsigned short int, unsigned short int>(minValue, maxValue);
  auto fittest = std::optional<game::PointWithHistory<unsigned short int>>{};
#ifdef GUI_MODE
  sf::RenderWindow window(sf::VideoMode{static_cast<unsigned>(windowWidth), static_cast<unsigned>(windowHeight)},
                          "ImGui + SFML = <3");
  // window.setFramerateLimit(60);

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
    // auto movePointOneStepIntoRndDirectionDurations = std::vector<long long>{};
    // movePointOneStepIntoRndDirectionDurations.reserve(points.size());
    while (
        //std::aray<game::Direction,game::m>
#ifdef GUI_MODE
        window.isOpen() &&
#endif
        executions < game::maxExecutions) {
      auto executionDuration = game::ScopeDuration{};
#ifdef GUI_MODE
      auto eventsToProcess = view::extractEvent(window);
      view::changeWindowEvent(eventsToProcess, window);

#endif

      
      std::for_each(std::execution::par_unseq, points.begin(), points.end(),
                    [](game::PointWithHistory<unsigned short int> &point) { point.generateRndDirections(); });
      //       int i=0;
      //       std::for_each(std::execution::par_unseq, points.begin(), points.end(), [limit, winningPosition,
      //       fittest,i](game::PointWithHistory<unsigned short int> &point)mutable {
      //         if (allowedToMove(point, limit, winningPosition)) {
      //           if (fittest && !point.getTouchedFittest() && (game::trueInPercent(5) || i==0)) {
      //             //point.movePointInDirectionToTarget(1, fittest->getPositions().at(point.getPositions().size()));
      //               point.movePointInDirectionToTarget(1, fittest->getPositions().back());
      //           } else {
      //             // auto movePointOneStepIntoRndDirectionDuration = game::ScopeDuration{};
      //             point.movePointOneStepIntoRndDirection(1);
      //             //
      //             movePointOneStepIntoRndDirectionDurations.push_back(movePointOneStepIntoRndDirectionDuration.duration());
      //           }
      //         }
      //         ++i;
      //       });
      ++executions;
      executionDurations.push_back(executionDuration.duration());
#ifdef GUI_MODE
      view::drawBoard(window, points, deltaClock);
#endif
    }
    ++generation;
    executions = 0;
    //     auto pointCompare = [&winningPosition](auto const &pointWithHistory1, auto const &pointWithHistory2) {
    //       return game::fitnes(pointWithHistory1.getPositions().back(), winningPosition) <
    //              game::fitnes(pointWithHistory2.getPositions().back(), winningPosition);
    //     };
    //     if (fittest) {
    //       fittest = std::max(*std::max_element(points.begin(), points.end(), pointCompare), *fittest, pointCompare);
    //     } else {
    //       fittest = *std::max_element(points.begin(), points.end(), pointCompare);
    //     }
    //     spdlog::info("movePointOneStepIntoRndDirectionDuration total duration in generation: {}",
    //                  std::accumulate(movePointOneStepIntoRndDirectionDurations.begin(),
    //                                  movePointOneStepIntoRndDirectionDurations.end(), 0ul));
    //     spdlog::info("movePointOneStepIntoRndDirectionDurations meanDuration: {}",
    //                  std::accumulate(movePointOneStepIntoRndDirectionDurations.begin(),
    //                                  movePointOneStepIntoRndDirectionDurations.end(), 0ul) /
    //                      movePointOneStepIntoRndDirectionDurations.size());
    spdlog::info("executionDurations total duration in generation: {}",
                 std::accumulate(executionDurations.begin(), executionDurations.end(), 0ul));
    spdlog::info("executionDurations meanDuration: {}",
                 std::accumulate(executionDurations.begin(), executionDurations.end(), 0ul) /
                     executionDurations.size());
    //     spdlog::info("new fittest x: {} y: {}", fittest->getPositions().back().first,
    //                  fittest->getPositions().back().second);
    //     spdlog::info("goal x: {} y: {}", winningPosition.first, winningPosition.second);
    points = std::vector<game::PointWithHistory<unsigned short int>>(static_cast<size_t>(pointCount));
    std::generate(points.begin(), points.end(), [&startPoint]() { return startPoint; });
    std::for_each(std::execution::par_unseq, points.begin(), points.end(),
                  [](game::PointWithHistory<unsigned short int> &point) { point.generateRndDirections(); });
    //     executionDurations.clear();
    //     movePointOneStepIntoRndDirectionDurations.clear();
  }
#ifdef GUI_MODE
  window.close();
  ImGui::SFML::Shutdown();
#endif
  return 0;
}
