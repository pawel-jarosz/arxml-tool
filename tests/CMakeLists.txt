add_executable(foo_test foo_test.cpp)
target_link_libraries(foo_test PRIVATE
        gmock
        gtest
        gtest_main
        pthread
        arxml
)