
#include "pointWithHistory.hxx"

namespace game {
auto trueInPercent(unsigned int percent) -> bool {
    using namespace XoshiroCpp;
    static const std::uint64_t seed = 12345;
    static Xoshiro128Plus rng(seed);
    static std::uniform_int_distribution<unsigned int> dist1_100(1, 100);
    return (dist1_100(rng) <= percent) ? true : false;
//               using namespace XoshiroCpp;
//    static const std::uint64_t seed = 12345;
//    static Xoshiro256PlusPlus rng(seed);
//   return (static_cast<unsigned>(rng()%100 +1) <= percent) ? true : false;
}
} // namespace game
