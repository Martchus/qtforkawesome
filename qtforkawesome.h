#ifndef QT_FORK_AWESOME_MAIN
#define QT_FORK_AWESOME_MAIN

#include "./global.h"

#include <QString>

#include <memory>

QT_FORWARD_DECLARE_CLASS(QByteArray)
QT_FORWARD_DECLARE_CLASS(QColor)
QT_FORWARD_DECLARE_CLASS(QPainter)
QT_FORWARD_DECLARE_CLASS(QRect)
QT_FORWARD_DECLARE_CLASS(QSize)
QT_FORWARD_DECLARE_CLASS(QPixmap)

namespace QtForkAwesome {

using IconBaseType = std::remove_reference_t<decltype(QChar().unicode())>;

enum class Icon : IconBaseType {
#include "icons.h"
};

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

#endif // QT_FORK_AWESOME_MAIN
