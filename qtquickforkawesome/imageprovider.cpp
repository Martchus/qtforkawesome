#include "./imageprovider.h"

#include <qtforkawesome/renderer.h>
#include <qtforkawesome/utils.h>

#include <QGuiApplication>
#include <QPalette>

/// \brief Contains classes provided by the QtForkAwesome library.
namespace QtForkAwesome {

QuickImageProvider::QuickImageProvider(
    const Renderer &renderer, const QColor &defaultColor, const QSize &defaultSize, QQuickImageProvider::ImageType type)
    : QQuickImageProvider(type)
    , m_renderer(renderer)
    , m_defaultColor(defaultColor)
    , m_defaultSize(defaultSize)
{
}

QPixmap QuickImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    const auto parts = QStringView(id).split(QChar(':'));
    if (parts.empty()) {
        return QPixmap();
    }
    const auto icon = iconFromId(parts.front());
    if (!isIconValid(icon)) {
        return QPixmap();
    }
    auto color = m_defaultColor;
    if (parts.size() > 1) {
        if (const auto specifiedColor = QColor(parts.at(1)); specifiedColor.isValid()) {
            color = specifiedColor;
        }
    }
    if (!color.isValid()) {
        color = QGuiApplication::palette().color(QPalette::Normal, QPalette::Text);
    }
    const auto renderSize = requestedSize.isValid() ? requestedSize : m_defaultSize;
    const auto scaleFactor = requestedSize.isValid() ? 1.0 : 0.0;
    if (size) {
        *size = renderSize;
    }
    return m_renderer.pixmap(icon, renderSize, color, scaleFactor);
}

QImage QuickImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    return requestPixmap(id, size, requestedSize).toImage();
}

} // namespace QtForkAwesome
