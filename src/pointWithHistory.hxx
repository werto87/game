#ifndef POINTWITHHISTORY_H
#define POINTWITHHISTORY_H
#include "header_only_libs/XoShiro/XoshiroCpp.hpp"
#include <random>
#include <vector>
namespace game {
enum class Direction { decrease = -1, doNothing = 0, increase = 1 };

template <typename T> auto shouldIncreaseDecreaseOrDoNothing(T const &position, T const &target) -> Direction {
  return position < target ? Direction::increase : (position == target) ? Direction::doNothing : Direction::decrease;
}
template <typename T>
// T should be a number
class PointWithHistory {
public:
  PointWithHistory() = default;
  PointWithHistory(std::pair<T, T> position, T capacity) : _positions{std::move(position)} {
    _positions.reserve(static_cast<size_t>(capacity));
  }
  auto movePointOneStepIntoRndDirection(T stepSize) -> void {
    using namespace XoshiroCpp;
    static const std::uint64_t seed = 12345;
    static Xoshiro128Plus rng(seed);
    static std::uniform_int_distribution<int> dist(-1, 1);
    Direction directionX = static_cast<Direction>(dist(rng));
    Direction directionY = static_cast<Direction>(dist(rng));
    _positions.push_back(movePointInDirection(_positions.back(), directionX, directionY, stepSize));
  }
  auto movePointInDirectionToTarget(T stepSize, std::pair<T, T> const &target) -> void {
    auto const &lastPosition = _positions.back();
    touchedFittest = _positions.back() == target;
    Direction directionX = shouldIncreaseDecreaseOrDoNothing(lastPosition.first, target.first);
    Direction directionY = shouldIncreaseDecreaseOrDoNothing(lastPosition.second, target.second);
    _positions.push_back(movePointInDirection(lastPosition, directionX, directionY, stepSize));
  }
  auto getPositions() const -> std::vector<std::pair<T, T>> const & { return _positions; }

  auto getTouchedFittest() -> bool { return touchedFittest; }

private:
  std::vector<std::pair<T, T>> _positions{};
  bool touchedFittest = false;
};

template <typename T>
auto didLastPositionFinish(PointWithHistory<T> const &pointWithHistory, std::pair<T, T> const &winningPosition)
    -> bool {
  return pointWithHistory.getPositions().back() == winningPosition;
}

template <typename T> auto isPointInLimit(std::pair<T, T> const &point, std::pair<T, T> const &limit) -> bool {
  return (point.first >= limit.first && point.first <= limit.second) &&
         (point.second >= limit.first && point.second <= limit.second);
}

template <typename T> auto isDead(PointWithHistory<T> const &pointWithHistory, std::pair<T, T> const &limit) -> bool {
  return !isPointInLimit(pointWithHistory.getPositions().back(), limit);
}

template <typename T>
auto allowedToMove(PointWithHistory<T> const &pointWithHistory, std::pair<T, T> const &limit,
                   std::pair<T, T> const &winningPosition) -> bool {
  return (!isDead(pointWithHistory, limit) && !didLastPositionFinish(pointWithHistory, winningPosition));
}

auto trueInPercent(unsigned int percent) -> bool;

template <typename T>
auto movePointInDirection(std::pair<T, T> const &oldPosition, Direction directionX, Direction directionY, T stepSize)
    -> std::pair<T, T> {
  auto newPosition = std::pair<T, T>{};
  switch (directionX) {

  case Direction::decrease: {
    newPosition.first = oldPosition.first - stepSize;
    break;
  }
  case Direction::doNothing: {
    newPosition.first = oldPosition.first;
    break;
  }
  case Direction::increase: {
    newPosition.first = oldPosition.first + stepSize;
    break;
  }
  }
  switch (directionY) {
  case Direction::decrease: {
    newPosition.second = oldPosition.second - stepSize;
    break;
  }
  case Direction::doNothing: {
    newPosition.second = oldPosition.second;
    break;
  }
  case Direction::increase: {
    newPosition.second = oldPosition.second + stepSize;
    break;
  }
  }
  return newPosition;
}

} // namespace game
#endif // POINTWITHHISTORY_H
