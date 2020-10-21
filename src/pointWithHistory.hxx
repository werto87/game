#ifndef POINTWITHHISTORY_H
#define POINTWITHHISTORY_H
#include "header_only_libs/XoShiro/XoshiroCpp.hpp"
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

  PointWithHistory(std::pair<T, T> position) : _position{std::move(position)} {}

  auto moveInDirection(T stepSize, std::pair<Direction, Direction> const &direction) -> void {
    _position = movePointInDirection(_position, direction.first, direction.second, stepSize);
  }

  //   auto movePointInDirectionToTarget(T stepSize, std::pair<T, T> const &target) -> void {
  //     auto const &lastPosition = _positions.back();
  //     touchedFittest = _positions.back() == target;
  //     Direction directionX = shouldIncreaseDecreaseOrDoNothing(lastPosition.first, target.first);
  //     Direction directionY = shouldIncreaseDecreaseOrDoNothing(lastPosition.second, target.second);
  //     _positions.push_back(movePointInDirection(lastPosition, directionX, directionY, stepSize));
  //   }
  // auto getDirections() const -> std::array<std::pair<Direction, Direction>,pointCount> const & { return _directions;
  // }
  auto getPosition() const -> std::pair<T, T> const & { return _position; }
  auto getTouchedFittest() -> bool { return touchedFittest; }

private:
  bool touchedFittest = false;
  std::pair<T, T> _position{};
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
auto movePointInDirection(std::pair<T, T> const &oldPosition, Direction direction, T stepSize) -> std::pair<T, T> {
  auto newPosition = std::pair<T, T>{};
  switch (direction) {
  case Direction::leftUp: {
    newPosition.first = oldPosition.first - stepSize;
    newPosition.second = oldPosition.second + stepSize;
    break;
  }
  case Direction::up: {
    newPosition.second = oldPosition.second + stepSize;
    break;
  }
  case Direction::rightUp: {
    newPosition.first = oldPosition.first + stepSize;
    newPosition.second = oldPosition.second + stepSize;
    break;
  }
  case Direction::left: {
    newPosition.first = oldPosition.first - stepSize;
    break;
  }
  case Direction::doNothing: {

    break;
  }
  case Direction::right: {
    newPosition.first = oldPosition.first + stepSize;
    break;
  }
  case Direction::leftDown: {
    newPosition.first = oldPosition.first - stepSize;
    newPosition.second = oldPosition.second - stepSize;
    break;
  }
  case Direction::down: {

    newPosition.second = oldPosition.second - stepSize;
    break;
  }
  case Direction::rightDown: {
    newPosition.first = oldPosition.first + stepSize;
    newPosition.second = oldPosition.second - stepSize;
    break;
  }
  }
  return newPosition;
}

} // namespace game
#endif // POINTWITHHISTORY_H
