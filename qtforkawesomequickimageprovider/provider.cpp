#include "./provider.h"

#include <qtforkawesome/renderer.h>
#include <qtforkawesome/utils.h>

#include <qtutilities/misc/compat.h>

#include <QGuiApplication>
#include <QPalette>

/// \brief Contains classes provided by the QtForkAwesome library.
namespace QtForkAwesome {

QuickImageProvider::QuickImageProvider(Renderer &renderer, const QColor &defaultColor, const QSize &defaultSize, QQuickImageProvider::ImageType type)
    : QQuickImageProvider(type)
    , m_renderer(renderer)
    , m_defaultColor(defaultColor)
    , m_defaultSize(defaultSize)
{
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
QPixmap QuickImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize, const QQuickImageProviderOptions &options)
#else
QPixmap QuickImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
#endif
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    Q_UNUSED(options)
#endif
    const auto parts = id.splitRef(QChar(':'));
    if (parts.empty()) {
        return QPixmap();
    }
    const auto icon = iconFromId(parts.front().toString());
    if (!isIconValid(icon)) {
        return QPixmap();
    }
    auto color = parts.size() > 1 ? QColor(parts.at(1).toString()) : m_defaultColor;
    if (!color.isValid()) {
        color = QGuiApplication::palette().color(QPalette::Normal, QPalette::Text);
    }
    const auto renderSize = requestedSize.isValid() ? requestedSize : m_defaultSize;
    if (size) {
        *size = renderSize;
    }
    return m_renderer.pixmap(icon, renderSize, color);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
QImage QuickImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize, const QQuickImageProviderOptions &options)
{
    return requestPixmap(id, size, requestedSize, options).toImage();
}
#else
QImage QuickImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    return requestPixmap(id, size, requestedSize).toImage();
}
#endif

} // namespace QtForkAwesome
