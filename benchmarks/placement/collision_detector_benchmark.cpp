#include "benchmark_data.hpp"

#include "chart_label_engine/application/placement/collision_detector.hpp"

#include <benchmark/benchmark.h>

#include <cstdint>

namespace {

    void BM_CollisionDetector(benchmark::State& state) {
        const auto count = static_cast<std::size_t>(state.range(0));
        const auto data = benchmark_support::make_benchmark_data(count);

        const chart_label_engine::application::placement::CollisionDetector
            detector;

        for (auto _ : state) {
            auto collisions = detector.detect(data.placements);

            benchmark::DoNotOptimize(collisions);
        }

        state.SetItemsProcessed(
            state.iterations() * static_cast<std::int64_t>(count)
        );
    }

    BENCHMARK(BM_CollisionDetector)
        ->Arg(10)
        ->Arg(50)
        ->Arg(100)
        ->Arg(500);

} // namespace
