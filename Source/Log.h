#pragma once
#include <string>

namespace vimlock
{

class Bitboard;
class Board;

void logInfo(const std::string &msg);
void logInfo(const std::string &msg, const Board &board);
void logInfo(const std::string &msg, const Board &board, const Bitboard &bitboard);

void logError(const std::string &msg);

} // namespace vimlock
