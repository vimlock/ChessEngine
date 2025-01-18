#ifndef __LOG_H__
#define __LOG_H__

#include <string>

namespace vimlock
{

void logInfo(const std::string &msg);
void logError(const std::string &msg);

} // namespace vimlock

#endif // __LOG_H__
