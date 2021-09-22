#include "./iconengine.h"

#include "../qtforkawesome/renderer.h"
#include "../qtforkawesome/utils.h"

#include "resources/config.h"

#include <qtutilities/misc/compat.h>

#include <QGuiApplication>
#include <QPainter>
#include <QPalette>
#include <QString>
#ifdef QT_FORK_AWESOME_ICON_ENGINE_ENABLE_STYLE_SUPPORT
#include <QApplication>
#include <QStyle>
#include <QStyleOption>
#endif

namespace QtForkAwesome {

IconEngine::IconEngine(Renderer &renderer)
    : m_renderer(renderer)
    , m_char(0)
    , m_color()
{
}

IconEngine::IconEngine(const IconEngine &other)
    : QIconEngine(other)
    , m_renderer(other.m_renderer)
    , m_char(0)
    , m_color()
{
}

IconEngine::~IconEngine()
{
}

QPixmap IconEngine::pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state)
{
    Q_UNUSED(state)

    auto color = m_color;
    if (!m_color.isValid()) {
        auto group = QPalette::Normal;
        auto role = QPalette::Text;
#ifndef QT_FORK_AWESOME_ICON_ENGINE_ENABLE_STYLE_SUPPORT
        switch (mode) {
        case QIcon::Disabled:
            group = QPalette::Disabled;
            break;
        case QIcon::Active:
            group = QPalette::Active;
            break;
        case QIcon::Selected:
            role = QPalette::HighlightedText;
            break;
        default:;
        }
#endif
        color = QGuiApplication::palette().color(group, role);
    }

    auto pixmap = m_renderer.pixmap(QChar(m_char), size, color);
#ifdef QT_FORK_AWESOME_ICON_ENGINE_ENABLE_STYLE_SUPPORT
    auto *const app = qobject_cast<QApplication *>(QApplication::instance());
    if (auto *const style = app ? app->style() : nullptr) {
        auto opt = QStyleOption();
        opt.palette = QGuiApplication::palette();
        if (const auto generated = style->generatedIconPixmap(mode, pixmap, &opt); !generated.isNull()) {
            pixmap = generated;
        }
    }
#endif

    return pixmap;
}

void IconEngine::paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state)
{
    painter->drawPixmap(rect, pixmap(rect.size(), mode, state));
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
    m_color = parts.size() > 1 ? QColor(parts.at(1)) : QColor();
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
