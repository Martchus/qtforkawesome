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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    Q_OBJECT
    Q_PROPERTY(QColor defaultColor READ defaultColor WRITE setDefaultColor)
    Q_PROPERTY(QSize defaultSize READ defaultSize WRITE setDefaultSize)
#endif

public:
    QuickImageProvider(const Renderer &renderer, const QColor &defaultColor = QColor(), const QSize &defaultSize = QSize(64, 64),
        QQuickImageProvider::ImageType type = QQuickImageProvider::Pixmap);
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;

    QColor defaultColor() const;
    QSize defaultSize() const;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
public Q_SLOTS:
#else
public:
#endif
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
