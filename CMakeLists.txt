cmake_minimum_required(VERSION 3.15)

set (CMAKE_DISABLE_SOURCE_CHANGES ON)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 11)

project(
	ChessEngineShowcase
	VERSION 1.0
	LANGUAGES CXX
)

add_compile_options(
       -Wall -Werror -Wno-error=maybe-uninitialized
       $<$<CONFIG:RELEASE>:-Ofast>
       $<$<CONFIG:DEBUG>:-O1>
       $<$<CONFIG:DEBUG>:-ggdb3>
)

add_library(ChessEngineLib STATIC
	Source/Engine/Board.cpp
	Source/Engine/Engine.cpp
	Source/Engine/Eval.cpp
	Source/Engine/Log.cpp
	Source/Engine/Move.cpp
	Source/Engine/Format.cpp
	Source/UCI/Uci.cpp
)

target_include_directories(ChessEngineLib PUBLIC Source)

add_executable(ChessEval
	Source/Main.cpp
)

target_link_libraries(ChessEval PRIVATE ChessEngineLib)

add_executable(RunTests
	Tests/Main.cpp
	Tests/TestBoard.cpp
	Tests/TestBitboard.cpp
	Tests/TestEngine.cpp
	Tests/TestEval.cpp
	Tests/TestMove.cpp
	Tests/TestMoves.cpp
)
target_link_libraries(RunTests PRIVATE ChessEngineLib)
