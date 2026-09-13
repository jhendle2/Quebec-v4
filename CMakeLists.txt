# ================================================
cmake_minimum_required(VERSION 3.20)
project(app
    LANGUAGES C CXX # Both standards so we can use Catch2 for Tests
)

# ================================================
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
# set(CMAKE_C_EXTENSIONS OFF) # Disables gnu add-ons

# ================================================
set(BUILD_DIR   ${PROJECT_SOURCE_DIR}/build  )
set(BIN_DIR     ${PROJECT_SOURCE_DIR}/bin    )
set(LIB_DIR     ${PROJECT_SOURCE_DIR}/lib    )
set(SRC_DIR     ${PROJECT_SOURCE_DIR}/src    )
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(TESTS_DIR   ${PROJECT_SOURCE_DIR}/tests  )

# ================================================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON        )

# ================================================
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
add_compile_options(-Wall -Wextra)

# ================================================
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET glib-2.0)

find_package(libzip REQUIRED)

# ================================================
include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(microlog
    GIT_REPOSITORY "https://github.com/an-dr/microlog.git"
    GIT_TAG        v7.1.0
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(microlog)

FetchContent_Declare(argparse
    GIT_REPOSITORY "https://github.com/cofyc/argparse"
    GIT_TAG        v1.1.0
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(argparse)

# ================================================
file(GLOB SRCS ${SRC_DIR}/*/*.c)
add_library(common ${SRCS}
)
target_compile_definitions(common PUBLIC ULOG_BUILD_COLOR=1)
include_directories(common
PRIVATE
    ${INCLUDE_DIR}
    "${logc_SOURCE_DIR}/src"
)
target_link_libraries(common
PRIVATE
    PkgConfig::GLIB
    # raylib
    libzip::zip
    # Lua::Lua
    microlog::microlog
)

# ================================================
add_executable(${PROJECT_NAME}
    ${SRC_DIR}/Main.c
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    common
    PkgConfig::GLIB
    # raylib
    # Lua::Lua
    microlog::microlog
    argparse
)

target_include_directories(${PROJECT_NAME}
PRIVATE
    ${INCLUDE_DIR}
)

# ================================================
enable_testing()

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

# These tests can use the Catch2-provided main
file(GLOB TEST_SRCS ${TESTS_DIR}/*/*.cpp)
add_executable(tests ${TEST_SRCS})
target_link_libraries(tests
PRIVATE
    common
    PkgConfig::GLIB
    microlog::microlog
    Catch2::Catch2WithMain
)

target_include_directories(tests
PRIVATE
    ${INCLUDE_DIR}
)
