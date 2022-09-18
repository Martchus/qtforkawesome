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
    void render(QChar character, QPainter *painter, const QRect &rect, const QColor &color) const;
    void render(Icon icon, QPainter *painter, const QRect &rect, const QColor &color) const;

    QPixmap pixmap(QChar icon, const QSize &size, const QColor &color) const;
    QPixmap pixmap(Icon icon, const QSize &size, const QColor &color) const;

    static Renderer &global();

private:
    std::unique_ptr<Renderer::InternalData> m_d;
};

/*!
 * \brief Renders the specified \a icon using the specified \a painter.
 */
inline void Renderer::render(Icon icon, QPainter *painter, const QRect &rect, const QColor &color) const
{
    render(QChar(static_cast<IconBaseType>(icon)), painter, rect, color);
}

} // namespace QtForkAwesome

#endif // QT_FORK_AWESOME_RENDERER
