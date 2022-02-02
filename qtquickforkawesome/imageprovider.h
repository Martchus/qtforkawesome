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
    Q_PROPERTY(QColor defaultColor defaultColor name WRITE setDefaultColor)
    Q_PROPERTY(QSize defaultColor defaultSize name WRITE setDefaultSize)

public:
    QuickImageProvider(const Renderer &renderer, const QColor &defaultColor = QColor(), const QSize &defaultSize = QSize(64, 64),
        QQuickImageProvider::ImageType type = QQuickImageProvider::Pixmap);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize, const QQuickImageProviderOptions &options);
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize, const QQuickImageProviderOptions &options);
#else
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
#endif

    QColor defaultColor() const;
    QSize defaultSize() const;

public Q_SLOTS:
    void setDefaultColor(const QColor &color);
    void setDefaultSize(const QSize &size);

private:
    const Renderer &m_renderer;
    QColor m_defaultColor;
    QSize m_defaultSize;
};

inline QColor QuickImageProvider::defaultColor() const
{
    return m_defaultColor;
}

inline QSize QuickImageProvider::defaultSize() const
{
    return m_defaultSize;
}

inline void QuickImageProvider::setDefaultColor(const QColor &color)
{
    m_defaultColor = color;
}

inline void QuickImageProvider::setDefaultSize(const QSize &size)
{
    m_defaultSize = size;
}

} // namespace QtForkAwesome

#endif // QT_FORK_AWESOME_QUICK_IMAGE_PROVIDER
