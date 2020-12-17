#include <benchmark/benchmark.h>
#include <random>
#include <unordered_set>

struct quaternion {
  unsigned a, b, c, d;
};

struct quat_equal {
  bool operator()(const quaternion &lhs, const quaternion &rhs) const {
    return std::tie(lhs.a, lhs.b, lhs.c, lhs.d) ==
           std::tie(rhs.a, rhs.b, rhs.c, rhs.d);
  }
};

namespace helpers {
constexpr unsigned cantor_pairing(unsigned x, unsigned y) {
  return x + (x + y) * (x + y + 1) / 2;
}
constexpr unsigned fueter_poyla_xyz(unsigned x, unsigned y, unsigned z) {
  return cantor_pairing(x, y) +
         (x + y + z + 2) * (x + y + z + 1) * (x + y + z) / 6;
}
constexpr unsigned fueter_poyla_xyzw(unsigned x, unsigned y, unsigned z,
                                     unsigned w) {
  return fueter_poyla_xyz(x, y, z) + (x + y + z + w + 3) * (x + y + z + w + 2) *
                                         (x + y + z + w + 1) * (x + y + z + w) /
                                         24;
}
} // namespace helpers

// uses the cantor pairing function 3 times - to hash a with b, c with d and
// then combine the results
struct triple_cantor {
  constexpr unsigned operator()(const quaternion &q) const {
    using helpers::cantor_pairing;
    return cantor_pairing(cantor_pairing(q.a, q.b), cantor_pairing(q.c, q.d));
  }
};

// extension of cantor pairing for higher dimensions
// i am using the 3d variant and then mixing up the result with cantor pairing
struct cantor_fp {
  constexpr unsigned operator()(const quaternion &q) const {
    using namespace helpers;
    return cantor_pairing(q.a, fueter_poyla_xyz(q.b, q.c, q.d));
  }
};

struct fp_cantor {
  constexpr unsigned operator()(const quaternion &q) const {
    using namespace helpers;
    return fueter_poyla_xyz(cantor_pairing(q.a, q.b), q.c, q.d);
  }
};

struct fp_4d {
  constexpr unsigned operator()(const quaternion &q) const {
    using namespace helpers;
    return fueter_poyla_xyzw(q.a, q.b, q.c, q.d);
  }
};

struct primes_8digits {
  constexpr unsigned operator()(const quaternion &q) const {
    return q.a * 10000019 + q.b * 38941207 + q.c * 60908101 + q.d * 91528739;
  }
};

struct primes_4digits {
  constexpr unsigned operator()(const quaternion &q) const {
    return q.a * 1031 + q.b * 3673 + q.c * 5591 + q.d * 9349;
  }
};

template <typename Hash> static void bench_quat(benchmark::State &state) {
  static constexpr unsigned iterations = 100000;
  std::random_device rd;
  std::uniform_int_distribution<unsigned> dist(0, 987);
  for (auto _ : state) {
    std::unordered_set<quaternion, Hash, quat_equal> quats;
    for (unsigned i = 0; i < iterations; ++i)
      quats.insert({dist(rd), dist(rd), dist(rd), dist(rd)});
  }
}

BENCHMARK_TEMPLATE(bench_quat, triple_cantor);
BENCHMARK_TEMPLATE(bench_quat, cantor_fp);
BENCHMARK_TEMPLATE(bench_quat, fp_cantor);
BENCHMARK_TEMPLATE(bench_quat, fp_4d);
BENCHMARK_TEMPLATE(bench_quat, primes_8digits);
BENCHMARK_TEMPLATE(bench_quat, primes_4digits);
