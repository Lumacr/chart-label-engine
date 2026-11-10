#include "benchmark_data.hpp"

#include "chart_label_engine/application/placement/candidate_generator.hpp"

#include <benchmark/benchmark.h>

#include <cstdint>
#include <vector>

namespace {

    void BM_CandidateGenerator(benchmark::State& state) {
        const auto count = static_cast<std::size_t>(state.range(0));
        const auto data = benchmark_support::make_benchmark_data(count);

        const chart_label_engine::application::placement::CandidateGenerator
            generator;

        for (auto _ : state) {
            std::vector<
                std::vector<chart_label_engine::domain::placement::Placement>
            > results;

            results.reserve(count);

            for (const auto& target : data.chart.label_targets()) {
                results.push_back(generator.generate(target));
            }

            benchmark::DoNotOptimize(results);
            benchmark::ClobberMemory();
        }

        state.SetItemsProcessed(
            state.iterations() * static_cast<std::int64_t>(count)
        );
    }

    BENCHMARK(BM_CandidateGenerator)
        ->Arg(10)
        ->Arg(50)
        ->Arg(100)
        ->Arg(500);

} // namespace
