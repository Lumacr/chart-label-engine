
include(FetchContent)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        googlebenchmark
        URL https://github.com/google/benchmark/archive/refs/tags/v1.9.1.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googlebenchmark)

add_executable(chart_label_engine_benchmarks
        placement/candidate_generator_benchmark.cpp
        placement/collision_detector_benchmark.cpp
        placement/placement_scorer_benchmark.cpp
        placement/greedy_placement_optimizer_benchmark.cpp
)

target_link_libraries(chart_label_engine_benchmarks
        PRIVATE
        chart_label_engine
        benchmark::benchmark_main
)
