#ifndef SCOPEDURATION_H
#define SCOPEDURATION_H
#include <chrono>
#include <spdlog/spdlog.h>
namespace game {
struct ScopeDuration {
  auto duration() -> long long { return (std::chrono::high_resolution_clock::now() - start).count(); }
  std::chrono::time_point<std::chrono::high_resolution_clock> start{std::chrono::high_resolution_clock::now()};
};
} // namespace game

#endif // SCOPEDURATION_H
