#ifndef QT_FORK_AWESOME_ICON
#define QT_FORK_AWESOME_ICON

#include "./iconfwd.h"

namespace QtForkAwesome {

enum class Icon : IconBaseType {
    Invalid, /**< invalid, used by QtForkAwesome::iconFromId() to indicate failure */
#include "private/icons.h"
};

} // namespace QtForkAwesome

#endif // QT_FORK_AWESOME_ICON
