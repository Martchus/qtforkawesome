#ifndef QT_FORK_AWESOME_ICON
#define QT_FORK_AWESOME_ICON

#include "./iconfwd.h"

namespace QtForkAwesome {

/*!
 * \brief The Icon enum specifies a ForkAwesome icon for calling QtForkAwesome::Renderer::render().
 */
enum class Icon : IconBaseType {
    Invalid, /**< invalid, used by QtForkAwesome::iconFromId() to indicate failure */
#ifndef Q_MOC_RUN // workaround moc parse errors on Fedora 33 and 34 builds
#include "private/icons.h"
#endif
};

} // namespace QtForkAwesome

#endif // QT_FORK_AWESOME_ICON
