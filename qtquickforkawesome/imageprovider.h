#ifndef QT_FORK_AWESOME_QUICK_IMAGE_PROVIDER
#define QT_FORK_AWESOME_QUICK_IMAGE_PROVIDER

#include "./global.h"

#include <QColor>
#include <QQuickImageProvider>
#include <QSize>

QT_FORWARD_DECLARE_CLASS(QPixmap)
QT_FORWARD_DECLARE_CLASS(QSize)

namespace QtForkAwesome {

class Renderer;

class QT_QUICK_FORK_AWESOME_EXPORT QuickImageProvider : public QQuickImageProvider {
public:
    QuickImageProvider(Renderer &renderer, const QColor &defaultColor = QColor(), const QSize &defaultSize = QSize(64, 64),
        QQuickImageProvider::ImageType type = QQuickImageProvider::Pixmap);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize, const QQuickImageProviderOptions &options);
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize, const QQuickImageProviderOptions &options);
#else
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
#endif

private:
    Renderer &m_renderer;
    QColor m_defaultColor;
    QSize m_defaultSize;
};

} // namespace QtForkAwesome

#endif // QT_FORK_AWESOME_QUICK_IMAGE_PROVIDER
