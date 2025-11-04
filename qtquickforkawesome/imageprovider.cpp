#include "./imageprovider.h"

#include <qtforkawesome/renderer.h>
#include <qtforkawesome/utils.h>

#include <QGuiApplication>
#include <QPalette>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0) // could be QT_VERSION < QT_VERSION_CHECK(5, 15, 2)
#define USE_COMPAT_SPLIT_REF
#include <qtutilities/misc/compat.h>
#endif

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
#ifndef USE_COMPAT_SPLIT_REF
    const auto parts = QStringView(id).split(QChar(':'));
#else
    const auto parts = QtUtilities::splitRef(id, QChar(':'));
#endif
    if (parts.empty()) {
        return QPixmap();
    }
#ifndef USE_COMPAT_SPLIT_REF
    const auto icon = iconFromId(parts.front());
#else
    const auto icon = iconFromId(parts.front().toString());
#endif
    if (!isIconValid(icon)) {
        return QPixmap();
    }
    auto color = m_defaultColor;
    if (parts.size() > 1) {
#ifndef USE_COMPAT_SPLIT_REF
        if (const auto specifiedColor = QColor(parts.at(1)); specifiedColor.isValid()) {
#else
        if (const auto specifiedColor = QColor(parts.at(1).toString()); specifiedColor.isValid()) {
#endif
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
