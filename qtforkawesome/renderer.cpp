#include "./renderer.h"

#include "resources/config.h"

#include <QFontDatabase>
#include <QGuiApplication>
#include <QHash>
#include <QIcon>
#include <QPainter>

/// \brief Contains classes provided by the QtForkAwesome library.
namespace QtForkAwesome {

/// \cond

struct Renderer::InternalData {
    explicit InternalData(int id);
    static constexpr int invalidId = -1;

    int id;
    QStringList fontFamilies;
    QHash<QChar, QString> themeOverrides;
};

Renderer::InternalData::InternalData(int id)
    : id(id)
    , fontFamilies(id != invalidId ? QFontDatabase::applicationFontFamilies(id) : QStringList())
{
}

/// \endcond

/*!
 *
 * \class QtForkAwesome::Renderer
 * \brief Allows rendering a QtForkAwesome::Icon (or an arbitrary QChar using an arbitrary font file).
 */

/*!
 * \brief Constructs a new renderer with the given \a fontFileName.
 * \remarks If \a fontFileName is empty, the bundled font file will be used.
 */
Renderer::Renderer(const QString &fontFileName)
    : m_d(std::make_unique<InternalData>(
        QFontDatabase::addApplicationFont(fontFileName.isEmpty() ? QStringLiteral(":/" META_FONT_FILE_NAME) : fontFileName)))
{
}

/*!
 * \brief Constructs a new renderer with the given \a fontData.
 */
Renderer::Renderer(const QByteArray &fontData)
    : m_d(std::make_unique<InternalData>(QFontDatabase::addApplicationFont(fontData)))
{
}

/*!
 * \brief Destructs the renderer.
 */
Renderer::~Renderer()
{
    if (QCoreApplication::instance() && m_d->id != InternalData::invalidId) {
        QFontDatabase::removeApplicationFont(m_d->id);
    }
}

/*!
 * \brief Returns whether the render could be initialized correctly.
 */
Renderer::operator bool() const
{
    return !m_d->fontFamilies.empty();
}

/// \cond
static void renderInternally(QChar character, QPainter *painter, QFont &&font, const QRect &rect, const QColor &color)
{
    font.setPixelSize(rect.height());
    painter->save();
    painter->setFont(font);
    painter->setPen(color);
    painter->drawText(rect, QString(character), QTextOption(Qt::AlignCenter));
    painter->restore();
}
/// \endcond

/*!
 * \brief Renders the specified \a icon using the specified \a painter.
 */
void QtForkAwesome::Renderer::render(QChar character, QPainter *painter, const QRect &rect, const QColor &color) const
{
    auto themeOverride = m_d->themeOverrides.find(character);
    if (themeOverride != m_d->themeOverrides.end()) {
        const auto icon = QIcon::fromTheme(*themeOverride);
        if (!icon.isNull()) {
            painter->drawPixmap(rect, icon.pixmap(rect.size(), QIcon::Normal, QIcon::On));
            return;
        }
    }
    if (*this) {
        renderInternally(character, painter, QFont(m_d->fontFamilies.front()), rect, color);
    }
}

/*!
 * \brief Renders the specified \a character as pixmap of the specified \a size.
 */
QPixmap QtForkAwesome::Renderer::pixmap(QChar icon, const QSize &size, const QColor &color) const
{
    auto themeOverride = m_d->themeOverrides.find(icon);
    if (themeOverride != m_d->themeOverrides.end()) {
        const auto icon = QIcon::fromTheme(*themeOverride);
        if (!icon.isNull()) {
            return icon.pixmap(size, QIcon::Normal, QIcon::On);
        }
    }

    const auto scaleFactor =
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
        !QCoreApplication::testAttribute(Qt::AA_UseHighDpiPixmaps) ? 1.0 :
#endif
                                                                   qGuiApp->devicePixelRatio();
    const auto scaledSize = QSize(size * scaleFactor);
    auto pm = QPixmap(scaledSize);
    pm.fill(QColor(Qt::transparent));
    if (*this) {
        auto painter = QPainter(&pm);
        renderInternally(icon, &painter, QFont(m_d->fontFamilies.front()), QRect(QPoint(), scaledSize), color);
    }
    pm.setDevicePixelRatio(scaleFactor);
    return pm;
}

/*!
 * \brief Renders the specified \a icon as pixmap of the specified \a size.
 */
QPixmap Renderer::pixmap(Icon icon, const QSize &size, const QColor &color) const
{
    return pixmap(QChar(static_cast<IconBaseType>(icon)), size, color);
}

/*!
 * \brief Uses the icon from the current icon theme obtained via QIcon::fromTheme() for \a character if it exists.
 */
void Renderer::addThemeOverride(QChar character, const QString &iconNameInTheme)
{
    m_d->themeOverrides.insert(character, iconNameInTheme);
}

/*!
 * \brief Returns the global instance (which is so far only used by the icon engine plugin).
 */
Renderer &Renderer::global()
{
    static auto globalRenderer = Renderer();
    return globalRenderer;
}

} // namespace QtForkAwesome
