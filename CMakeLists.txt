cmake_minimum_required(VERSION 3.15)

set (CMAKE_DISABLE_SOURCE_CHANGES ON)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 11)

project(
	ChessEngineShowcase
	VERSION 1.0
	LANGUAGES CXX
)

add_library(ChessEngineLib
	Source/Engine/Bitboard.cpp
	Source/Engine/Board.cpp
	Source/Engine/Eval.cpp
	Source/Engine/Format.cpp
)

target_include_directories(ChessEngineLib PUBLIC Source)

add_executable(ChessEval
	Source/Main.cpp
)

target_link_libraries(ChessEval PRIVATE ChessEngineLib)

add_executable(RunTests
	Tests/Main.cpp
	Tests/TestBitboard.cpp
	Tests/TestEval.cpp
)
target_link_libraries(RunTests PRIVATE ChessEngineLib)
