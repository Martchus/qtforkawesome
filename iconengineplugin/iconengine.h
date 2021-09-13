#ifndef QT_FORK_AWESOME_ICON_ENGINE_H
#define QT_FORK_AWESOME_ICON_ENGINE_H

#include <QIconEngine>

#include "./global.h"

#include "../qtforkawesome/iconfwd.h"

namespace QtForkAwesome {

class Renderer;

class QT_FORK_AWESOME_ICON_ENGINE_EXPORT IconEngine : public QIconEngine {
public:
    IconEngine(Renderer &renderer);
    IconEngine(const IconEngine &other);
    ~IconEngine() override;

    void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) override;
    QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) override;
    void addFile(const QString &fileName, const QSize &size, QIcon::Mode mode, QIcon::State state) override;
    QString key() const override;
    QIconEngine *clone() const override;

private:
    Renderer &m_renderer;
    IconBaseType m_char;
    QColor m_color;
};

} // namespace QtForkAwesome

#endif // QT_FORK_AWESOME_ICON_ENGINE_H
