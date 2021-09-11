#ifndef QT_FORK_AWESOME_UTILS
#define QT_FORK_AWESOME_UTILS

#include "./global.h"
#include "./iconfwd.h"

QT_FORWARD_DECLARE_CLASS(QString)

namespace QtForkAwesome {

constexpr bool isIconValid(Icon icon)
{
    return static_cast<IconBaseType>(icon);
}

QT_FORK_AWESOME_EXPORT Icon iconFromId(const QString &id);

} // namespace QtForkAwesome

#endif // QT_FORK_AWESOME_UTILS
