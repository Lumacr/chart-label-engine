#include "benchmark_data.hpp"

#include "chart_label_engine/application/placement/greedy_placement_optimizer.hpp"

#include <benchmark/benchmark.h>

#include <cstdint>

namespace {

    void BM_GreedyPlacementOptimizer(benchmark::State& state) {
        const auto count = static_cast<std::size_t>(state.range(0));
        const auto data = benchmark_support::make_benchmark_data(count);

        const chart_label_engine::application::placement::GreedyPlacementOptimizer
            optimizer;

        for (auto _ : state) {
            auto placements = optimizer.optimize(data.chart);

            benchmark::DoNotOptimize(placements);
        }

        state.SetItemsProcessed(
            state.iterations() * static_cast<std::int64_t>(count)
        );
    }

    BENCHMARK(BM_GreedyPlacementOptimizer)
        ->Arg(10)
        ->Arg(25)
        ->Arg(50)
        ->Arg(100);

} // namespace
