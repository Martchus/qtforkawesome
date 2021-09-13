#include "./utils.h"
#include "./icon.h"

#include <QHash>
#include <QString>

namespace QtForkAwesome {

/*!
 * \brief Returns a QtForkAwesome::Icon for the specified icon \a id.
 * \param id Specifies the icon ID, that's the "name" used on https://forkaweso.me/Fork-Awesome/icons.
 * \returns Returns the QtForkAwesome::Icon which might be QtForkAwesome::Icon::Invalid in case the
 *          specified \a id is unknown.
 */
Icon iconFromId(const QString &id)
{
    static const auto mapping = QHash<QString, Icon>({
#include "private/idmapping.h"
    });
    const auto icon = mapping.find(id);
    return icon != mapping.cend() ? *icon : Icon::Invalid;
}

} // namespace QtForkAwesome
