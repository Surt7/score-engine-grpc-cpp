cmake_minimum_required(VERSION 4.2)
project(ScoreEngine)

find_package(nlohmann_json REQUIRED)
find_package(protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(sol2 REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)

set(PROTO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/protos")
set(PROTO_FILE "${PROTO_DIR}/score_engine.proto")
set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")

file(MAKE_DIRECTORY "${GENERATED_DIR}")

add_custom_command(
        OUTPUT
        "${GENERATED_DIR}/score_engine.pb.cc"
        "${GENERATED_DIR}/score_engine.pb.h"
        "${GENERATED_DIR}/score_engine.grpc.pb.cc"
        "${GENERATED_DIR}/score_engine.grpc.pb.h"
        COMMAND protobuf::protoc
        ARGS
        --proto_path=${PROTO_DIR}
        --cpp_out=${GENERATED_DIR}
        --grpc_out=${GENERATED_DIR}
        --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
        ${PROTO_FILE}
        DEPENDS ${PROTO_FILE}
        VERBATIM
)

add_library(score_proto
        "${GENERATED_DIR}/score_engine.pb.cc"
        "${GENERATED_DIR}/score_engine.grpc.pb.cc"
)
target_include_directories(score_proto PUBLIC "${GENERATED_DIR}")
target_link_libraries(score_proto PUBLIC protobuf::protobuf gRPC::grpc++)

add_executable(ScoreEngine main.cpp
        include/core/Event.h
        include/core/ScoreEngine.h
        src/core/ScoreEngine.cpp
        include/rules/IRule.h
        include/rules/Rules.h
        include/parser/EventParser.h
        src/parse/EventParser.cpp
        include/display/ConsoleDisplay.h
        src/display/ConsoleDisplay.cpp
        include/core/GameSession.h
        src/core/GameSession.cpp
        include/core/Score.h
        include/grpc/ScoreServiceImpl.h
        src/grpc/ScoreServiceImpl.cpp
)

if (MSVC)
    target_compile_options(ScoreEngine PRIVATE /W4)
else ()
    target_compile_options(ScoreEngine PRIVATE -Wall -Wextra -Wpedantic)
endif ()
target_include_directories(ScoreEngine PRIVATE include)

target_link_libraries(ScoreEngine PRIVATE
        nlohmann_json::nlohmann_json
        score_proto
        sol2::sol2
)

add_dependencies(ScoreEngine score_proto)
