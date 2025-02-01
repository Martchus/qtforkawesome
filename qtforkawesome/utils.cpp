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
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
Icon iconFromId(QStringView id)
#else
Icon iconFromId(const QString &id)
#endif
{
    static const auto mapping = QHash<QString, Icon>({
#include "private/idmapping.h"
    });
    const auto icon = mapping.find(id);
    return icon != mapping.cend() ? *icon : Icon::Invalid;
}

/*!
 * \brief Returns a QtForkAwesome::Icon for the specified icon \a id.
 * \param id Specifies the icon ID, that's the "name" used on https://forkaweso.me/Fork-Awesome/icons.
 * \returns Returns the QtForkAwesome::Icon which might be QtForkAwesome::Icon::Invalid in case the
 *          specified \a id is unknown.
 */
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
Icon iconFromId(const QString &id)
{
    return iconFromId(QStringView(id));
}
#else
Icon iconFromId(QStringView id)
{
    return iconFromId(id.toString());
}
#endif


} // namespace QtForkAwesome
