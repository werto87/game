#ifndef POINTWITHHISTORY_H
#define POINTWITHHISTORY_H
#include "header_only_libs/XoShiro/XoshiroCpp.hpp"
#include <iostream>
#include <random>
#include <src/gameConstant.hxx>
#include <vector>
namespace game {
enum class Direction {
  leftUp = 7,
  up = 8,
  rightUp = 9,
  left = 4,
  doNothing = 5,
  right = 6,
  leftDown = 1,
  down = 2,
  rightDown = 3
};

template <typename T>
auto shouldMoveInDirection(std::pair<T, T> const &position, std::pair<T, T> const &targetPosition) -> Direction {
  if (position.first > targetPosition.first && position.second < targetPosition.second)
    return Direction::leftUp;
  if (position.first == targetPosition.first && position.second < targetPosition.second)
    return Direction::up;
  if (position.first < targetPosition.first && position.second < targetPosition.second)
    return Direction::rightUp;
  if (position.first > targetPosition.first && position.second == targetPosition.second)
    return Direction::left;
  if (position.first == targetPosition.first && position.second == targetPosition.second)
    return Direction::doNothing;
  if (position.first < targetPosition.first && position.second == targetPosition.second)
    return Direction::right;
  if (position.first > targetPosition.first && position.second > targetPosition.second)
    return Direction::leftDown;
  if (position.first == targetPosition.first && position.second > targetPosition.second)
    return Direction::down;
  if (position.first < targetPosition.first && position.second > targetPosition.second)
    return Direction::rightDown;
}
template <typename T>
// T should be a number
class PointWithHistory {
public:
  PointWithHistory() = default;

  PointWithHistory(std::pair<T, T> const &startPoint) { _positions.push_back(startPoint); };

  auto getPositions() const -> std::vector<std::pair<T, T>> const & { return _positions; }
  auto getTouchedFittest() -> bool { return touchedFittest; }
  auto movePointInDirection(Direction direction, T const &stepSize) -> void;

private:
  std::vector<Direction> directionsToMove{};
  bool touchedFittest = false;
  std::vector<std::pair<T, T>> _positions{};
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

template <typename T> auto PointWithHistory<T>::movePointInDirection(Direction direction, T const &stepSize) -> void {
  auto newPosition = _positions.back();
  switch (direction) {
  case Direction::leftUp: {
    newPosition.first -= stepSize;
    newPosition.second -= stepSize;
    break;
  }
  case Direction::up: {
    newPosition.second -= stepSize;
    break;
  }
  case Direction::rightUp: {
    newPosition.first += stepSize;
    newPosition.second -= stepSize;
    break;
  }
  case Direction::left: {
    newPosition.first -= stepSize;
    break;
  }
  case Direction::doNothing: {

    break;
  }
  case Direction::right: {
    newPosition.first += stepSize;
    break;
  }
  case Direction::leftDown: {
    newPosition.first -= stepSize;
    newPosition.second += stepSize;
    break;
  }
  case Direction::down: {

    newPosition.second += stepSize;
    break;
  }
  case Direction::rightDown: {
    newPosition.first += stepSize;
    newPosition.second += stepSize;
    break;
  }
  }
  _positions.push_back(newPosition);
}

} // namespace game
#endif // POINTWITHHISTORY_H
