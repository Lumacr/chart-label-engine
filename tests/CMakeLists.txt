include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest)

add_executable(chart_label_engine_tests
        test_main.cpp
        domain/geometry/point2d_test.cpp
        domain/geometry/rectangle_test.cpp
        domain/label/label_test.cpp
        domain/placement/placement_test.cpp
        domain/chart/label_target_test.cpp
        domain/chart/chart_test.cpp

        application/placement/candidate_generator_test.cpp
)

target_link_libraries(chart_label_engine_tests
        PRIVATE
        chart_label_engine
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(chart_label_engine_tests)

