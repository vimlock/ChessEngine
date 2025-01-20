ChessEngine
===========

Silly little chess engine written in C++ over a weekend.

Uses Alpha-Beta pruning for decreasing the search space for the best move.

Currently hardcoded to search up to 6 plies.

Supports Universal Chess Interface (UCI) and can be used with any compatible GUI, for example https://github.com/fsmosca/Python-Easy-Chess-GUI


Better at chess than me :)

Building
=============

On Linux
--------------

Install build requirements

```shell
sudo apt get install cmake3
sudo apt get install catch2 # for testing
```

Build

```shell
mkdir build
cd build

# Release build recommended, otherwise evaluation is really slow
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

TODO
============

- [ ] Add multithreaded traversal, at least from the root position
- [ ] Consider castling when evaluating possible moves
- [ ] Consider en passant when evaluating possible moves
