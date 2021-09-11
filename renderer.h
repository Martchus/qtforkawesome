#ifndef QT_FORK_AWESOME_RENDERER
#define QT_FORK_AWESOME_RENDERER

#include "./global.h"
#include "./iconfwd.h"

#include <QString>

#include <memory>

QT_FORWARD_DECLARE_CLASS(QByteArray)
QT_FORWARD_DECLARE_CLASS(QColor)
QT_FORWARD_DECLARE_CLASS(QPainter)
QT_FORWARD_DECLARE_CLASS(QRect)
QT_FORWARD_DECLARE_CLASS(QSize)
QT_FORWARD_DECLARE_CLASS(QPixmap)
QT_FORWARD_DECLARE_CLASS(QIcon)

namespace QtForkAwesome {

class QT_FORK_AWESOME_EXPORT Renderer {
    struct InternalData;

public:
    explicit Renderer(const QString &fontFileName = QString());
    explicit Renderer(const QByteArray &fontData);
    ~Renderer();

    operator bool() const;
    void render(Icon icon, QPainter *painter, const QRect &rect, const QColor &color);
    QPixmap pixmap(Icon icon, const QSize &size, const QColor &color);

private:
    std::unique_ptr<Renderer::InternalData> m_d;
};

} // namespace QtForkAwesome

#endif // QT_FORK_AWESOME_RENDERER
