#include "Move.h"

#include <cctype>

namespace vimlock
{

Move::Move():
	src(A1),
	dst(A1),
	promote(PAWN)
{
}

Move::Move(Square src_, Square dst_, Piece promote_):
	src(src_),
	dst(dst_),
	promote(promote_)
{
}

Square Move::getSource() const
{
	return src;
}

Square Move::getDestination() const
{
	return dst;
}

/// TODO: we could easily reject promotions not on first or last rank
bool Move::parseLan(const std::string &str)
{
	if (str.size() != 4 && str.size() != 5)
		return false;

	char srcFile = tolower(str[0]);
	char srcRank = str[1];

	char dstFile = tolower(str[2]);
	char dstRank = str[3];

	if (srcFile < 'a' || srcFile > 'h')
		return false;
	if (srcRank < '1' || srcRank > '9')
		return false;

	if (dstFile < 'a' || dstFile > 'h')
		return false;
	if (dstRank < '1' || dstRank > '9')
		return false;

	// Promotion?
	if (str.size() == 5) {
		switch (tolower(str[4])) {
			case 'q': promote = QUEEN;  break;
			case 'n': promote = KNIGHT; break;
			case 'b': promote = BISHOP; break;
			case 'r': promote = ROOK;   break;
			default:
				return false;
		}
	}

	src = Square(static_cast<uint64_t>((srcFile - 'a') + (srcRank - '1') * 8));
	dst = Square(static_cast<uint64_t>((dstFile - 'a') + (dstRank - '1') * 8));

	return true;
}

std::string Move::toLan() const
{
	std::string ret;

	ret += 'a' + src.getFile();
	ret += '1' + src.getRank();

	ret += 'a' + dst.getFile();
	ret += '1' + dst.getRank();

	switch (promote) {
		case PAWN:   break;
		case ROOK:   ret += 'r'; break;
		case KNIGHT: ret += 'n'; break;
		case BISHOP: ret += 'b'; break;
		case QUEEN:  ret += 'q'; break;
		case KING:   break;
	}

	return ret;
}

Piece Move::getPromotion() const
{
	return promote;
}

bool Move::hasPromotion() const
{
	return promote != PAWN;
}

bool Move::operator == (Move rhs) const
{
	return !(*this != rhs);
}

bool Move::operator != (Move rhs) const
{
	if (src != rhs.src)
		return true;

	if (dst != rhs.dst)
		return true;

	if (promote != rhs.promote)
		return true;

	return false;
}

} // namespace vimlock
