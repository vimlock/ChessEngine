#include "Move.h"
#include "Eval.h"

#include <cctype>

namespace vimlock
{

using namespace RankAndFile;

Move::Move():
	src(A1),
	dst(A1)
{
}

Move::Move(RankAndFile::Enum src_, RankAndFile::Enum dst_):
	src(src_),
	dst(dst_)
{
}

RankAndFile::Enum Move::getSource() const
{
	return src;
}

RankAndFile::Enum Move::getDestination() const
{
	return dst;
}

bool Move::parseLan(const std::string &str)
{
	if (str.size() != 4)
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

	src = static_cast<RankAndFile::Enum>((srcFile - 'a') + (srcRank - '1') * 8);
	dst = static_cast<RankAndFile::Enum>((dstFile - 'a') + (dstRank - '1') * 8);

	return true;
}

std::string Move::toLan() const
{
	std::string ret;

	ret += 'a' + BoardUtils::getFile(src);
	ret += '1' + BoardUtils::getRank(src);

	ret += 'a' + BoardUtils::getFile(dst);
	ret += '1' + BoardUtils::getRank(dst);

	return ret;
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

	return false;
}

} // namespace vimlock
