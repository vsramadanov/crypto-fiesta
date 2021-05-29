CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

SET(CMAKE_C_COMPILER "gcc-10")
SET(CMAKE_CXX_COMPILER "g++-10")

PROJECT("matcher")

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED True)
SET(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Wpedantic -Werror -fsanitize=address")

SET(INCLUDE_DIRS "utils")

# ==================================
# =========== UNIT TESTS ===========
ENABLE_TESTING()
FIND_PACKAGE(GTest REQUIRED)

ADD_EXECUTABLE(utests gtest_main.cpp)
FILE(GLOB_RECURSE TEST_SRC *ut.cpp)
MESSAGE("Found unit test sources: " ${TEST_SRC})
TARGET_SOURCES(utests PRIVATE ${TEST_SRC})

TARGET_INCLUDE_DIRECTORIES(utests PUBLIC ${GTEST_INCLUDE_DIR})
TARGET_INCLUDE_DIRECTORIES(utests PUBLIC ${INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(utests ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY})
TARGET_LINK_LIBRARIES(utests pthread)


# ==================================
# =========== MATCHER ==============
ADD_EXECUTABLE(matcher matcher.cpp)
