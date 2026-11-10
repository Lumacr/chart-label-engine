#include "benchmark_data.hpp"

#include "chart_label_engine/application/placement/placement_scorer.hpp"

#include <benchmark/benchmark.h>

#include <cstdint>

namespace {

    void BM_PlacementScorer(benchmark::State& state) {
        const auto count = static_cast<std::size_t>(state.range(0));
        const auto data = benchmark_support::make_benchmark_data(count);

        const chart_label_engine::application::placement::PlacementScorer
            scorer;

        for (auto _ : state) {
            auto score = scorer.score(
                data.chart,
                data.placements
            );

            benchmark::DoNotOptimize(score);
        }

        state.SetItemsProcessed(
            state.iterations() * static_cast<std::int64_t>(count)
        );
    }

    BENCHMARK(BM_PlacementScorer)
        ->Arg(10)
        ->Arg(50)
        ->Arg(100)
        ->Arg(500);

} // namespace
