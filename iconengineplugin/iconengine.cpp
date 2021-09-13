#include "./iconengine.h"

#include "../qtforkawesome/renderer.h"
#include "../qtforkawesome/utils.h"

#include <qtutilities/misc/compat.h>

#include <QCoreApplication>
#include <QPainter>
#include <QString>

namespace QtForkAwesome {

IconEngine::IconEngine(Renderer &renderer)
    : m_renderer(renderer)
    , m_char(0)
    , m_color(Qt::black)
{
}

IconEngine::IconEngine(const IconEngine &other)
    : QIconEngine(other)
    , m_renderer(other.m_renderer)
    , m_char(0)
    , m_color(Qt::black)
{
}

IconEngine::~IconEngine()
{
}

QPixmap IconEngine::pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state)
{
    Q_UNUSED(mode)
    Q_UNUSED(state)
    return m_renderer.pixmap(QChar(m_char), size, m_color);
}

void IconEngine::paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state)
{
    Q_UNUSED(mode)
    Q_UNUSED(state)
    m_renderer.render(QChar(m_char), painter, rect, m_color);
}

void IconEngine::addFile(const QString &fileName, const QSize &, QIcon::Mode mode, QIcon::State state)
{
    Q_UNUSED(mode)
    Q_UNUSED(state)
    if (!fileName.endsWith(QLatin1String(".fa"), Qt::CaseInsensitive)) {
        return;
    }
    const auto parts = QtUtilities::midRef(fileName, 0, fileName.size() - 3).split(QChar(':'));
    if (parts.empty()) {
        return;
    }
    m_char = static_cast<IconBaseType>(iconFromId(parts.at(0).toString()));
    m_color = parts.size() > 1 ? QColor(parts.at(1)) : Qt::black;
}

QString IconEngine::key() const
{
    return QStringLiteral("fa");
}

QIconEngine *IconEngine::clone() const
{
    return new IconEngine(*this);
}

} // namespace QtForkAwesome
