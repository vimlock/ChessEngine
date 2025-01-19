#pragma once
#include "Bitboard.h"
#include "Board.h"

namespace vimlock
{

/// The function return bitboard of all positions given piece can be moved to
///
/// NOTE: none of these functions test for check or castling, that must be handled
/// by comparing the new position against opponents `getAvailableCaptures()` bitboard.
Bitboard getAvailableMoves(Color color, Piece piece, Square idx, Bitboard allPieces, Bitboard ownPieces);

/// Return bitboard of positions which given piece can attack to
/// Almost same as `getAvailableCaptures()` but differs in case of pawns
/// which can capture only diagonally.
Bitboard getAvailableCaptures(Color color, Piece piece, Square idx, Bitboard allPieces);

/// For given board, return bitboard of the squares given color can capture
Bitboard getAvailableCaptures(const Board &board, Bitboard allPieces, Bitboard ownPieces);

Bitboard getPawnMoves(Color color, Square idx, Bitboard allPieces);
Bitboard getPawnAttacks(Color color, Square idx);
Bitboard getRookMoves(Square idx, Bitboard allPieces);
Bitboard getKnightMoves(Square idx);
Bitboard getBishopMoves(Square idx, Bitboard allPieces);
Bitboard getQueenMoves(Square idx, Bitboard allPieces);
Bitboard getKingMoves(Square idx);

} // namespace vimlock

#include "Moves.inl"
