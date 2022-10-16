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

    void addThemeOverride(QChar character, const QString &iconNameInTheme);
    void addThemeOverride(Icon icon, const QString &iconNameInTheme);
    void addOverride(QChar character, const QIcon &override);
    void addOverride(Icon icon, const QIcon &override);
    void clearOverrides();

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

/*!
 * \brief Uses the icon from the current icon theme obtained via QIcon::fromTheme() for \a icon if it exists.
 */
inline void Renderer::addThemeOverride(Icon icon, const QString &iconNameInTheme)
{
    addThemeOverride(QChar(static_cast<IconBaseType>(icon)), iconNameInTheme);
}

/*!
 * \brief Uses the specified \a override icon for \a icon if it is not null.
 */
inline void Renderer::addOverride(Icon icon, const QIcon &override)
{
    addOverride(QChar(static_cast<IconBaseType>(icon)), override);
}

} // namespace QtForkAwesome

#endif // QT_FORK_AWESOME_RENDERER
