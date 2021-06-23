#ifndef APPLICATIONHELPER_H
#define APPLICATIONHELPER_H

#include <QtCore/QDebug>
#include <TGlobal>

class T_HELPER_EXPORT ApplicationHelper
{
};

namespace abq
{
enum class MIN_FREQ : short int { ONE = 0x01, FIVE = 0x05, FIFTEEN = 0x0F, THIRTY = 0x1E, SIXTY = 0x3C };

inline QDebug operator<<(QDebug debug, MIN_FREQ freq)
{
    switch (freq) {
    case MIN_FREQ::ONE:
        debug << "MIN_FREQ::ONE";
        break;
    case MIN_FREQ::FIVE:
        debug << "MIN_FREQ::FIVE";
        break;
    case MIN_FREQ::FIFTEEN:
        debug << "MIN_FREQ::FIFTEEN";
        break;
    case MIN_FREQ::THIRTY:
        debug << "MIN_FREQ::THIRTY";
        break;
    case MIN_FREQ::SIXTY:
        debug << "MIN_FREQ::SIXTY";
        break;
    }
    return debug;
}
} // namespace abq

#endif // APPLICATIONHELPER_H
