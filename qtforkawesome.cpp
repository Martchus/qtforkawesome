#include "./qtforkawesome.h"

#include "resources/config.h"

#include <QFontDatabase>
#include <QGuiApplication>
#include <QPainter>

/// \brief Contains classes provided by the QtForkAwesome library.
namespace QtForkAwesome {

/// \cond

struct Renderer::InternalData {
    explicit InternalData(int id);
    static constexpr int invalidId = -1;

    int id;
    QStringList fontFamilies;
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
 * \brief Allows rendering a QtForkAwesome::Icon.
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
    if (m_d->id != InternalData::invalidId) {
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

/*!
 * \brief Renders the specified \a icon using the specified \a painter.
 */
void Renderer::render(Icon icon, QPainter *painter, const QRect &rect, const QColor &color)
{
    if (!*this) {
        return;
    }
    auto font = QFont(m_d->fontFamilies.front());
    font.setPixelSize(rect.height());
    painter->save();
    painter->setFont(font);
    painter->setPen(color);
    painter->drawText(rect, QString(QChar(static_cast<IconBaseType>(icon))), QTextOption(Qt::AlignCenter));
    painter->restore();
}

/*!
 * \brief Renders the specified \a icon as pixmap of the specified \a size.
 */
QPixmap QtForkAwesome::Renderer::pixmap(Icon icon, const QSize &size, const QColor &color)
{
    const auto scaleFactor =
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
        !QCoreApplication::testAttribute(Qt::AA_UseHighDpiPixmaps) ? 1.0 :
#endif
                                                                   qGuiApp->devicePixelRatio();
    const auto scaledSize = QSize(size * scaleFactor);
    auto pm = QPixmap(scaledSize);
    pm.fill(QColor(Qt::transparent));
    auto painter = QPainter(&pm);
    render(icon, &painter, QRect(QPoint(), scaledSize), color);
    pm.setDevicePixelRatio(scaleFactor);
    return pm;
}

} // namespace QtForkAwesome
