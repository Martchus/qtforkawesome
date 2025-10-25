#include "./renderer.h"

#include "resources/config.h"

#include <QDebug>
#include <QFile>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QHash>
#include <QIcon>
#include <QPaintDevice>
#include <QPainter>

#include <utility>

/// \brief Contains classes provided by the QtForkAwesome library.
namespace QtForkAwesome {

/// \cond

struct IconOverride {
    void setIcon(const QIcon &icon);
    void addIconName(const QString &iconName);
    const QIcon &locateIcon();

private:
    QStringList iconNames;
    QIcon cachedIcon;
};

void IconOverride::setIcon(const QIcon &icon)
{
    iconNames.clear();
    cachedIcon = icon;
}

void IconOverride::addIconName(const QString &iconName)
{
    iconNames.append(iconName);
    if (!cachedIcon.isNull()) {
        cachedIcon = QIcon();
    }
}

const QIcon &IconOverride::locateIcon()
{
    if (!cachedIcon.isNull()) {
        return cachedIcon;
    }
    for (const auto &iconName : std::as_const(iconNames)) {
        cachedIcon = QIcon::fromTheme(iconName);
        if (!cachedIcon.isNull()) {
            return cachedIcon;
        }
    }
    return cachedIcon;
}

struct Renderer::InternalData {
    explicit InternalData(int id);
    static constexpr int invalidId = -1;

    int id;
    QString fontFilePath;
    QStringList fontFamilies;
    QHash<QChar, IconOverride> overrides;
    QPaintDevice *paintDevice;
};

Renderer::InternalData::InternalData(int id)
    : id(id)
    , fontFamilies(id != invalidId ? QFontDatabase::applicationFontFamilies(id) : QStringList())
    , paintDevice(nullptr)
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
 * \brief Returns the path of the font file.
 * \remarks Returns an empty string when the renderer was constructed from a QByteArray().
 */
const QString &Renderer::fontFilePath() const
{
    return m_d->fontFilePath;
}

/*!
 * \brief Prints a warning using qWarning() if no font could be loaded.
 */
void Renderer::warnIfInvalid() const
{
    if (!*this) {
        const auto &path = m_d->fontFilePath;
        if (!path.isEmpty() && !QFile::exists(path)) {
            qWarning() << "ForkAwesome font file does not exist";
        }
        qWarning() << "Unable to load ForkAwesome font from " << (path.isEmpty() ? QStringLiteral("buffer") : path);
    }
}

/*!
 * \brief Returns whether the renderer could load the font file.
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
    if (auto override = m_d->overrides.find(character); override != m_d->overrides.end()) {
        if (const auto &overrideIcon = override->locateIcon(); !overrideIcon.isNull()) {
            overrideIcon.paint(painter, rect, Qt::AlignCenter, QIcon::Normal, QIcon::On);
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
QPixmap Renderer::pixmap(QChar icon, const QSize &size, const QColor &color, qreal scaleFactor) const
{
    if (auto override = m_d->overrides.find(icon); override != m_d->overrides.end()) {
        if (const auto &overrideIcon = override->locateIcon(); !overrideIcon.isNull()) {
            return overrideIcon.pixmap(size, QIcon::Normal, QIcon::On);
        }
    }

    if (!static_cast<bool>(scaleFactor)) {
        scaleFactor =
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
            !QCoreApplication::testAttribute(Qt::AA_UseHighDpiPixmaps)
            ? 1.0
            :
#endif
            (m_d->paintDevice ? m_d->paintDevice->devicePixelRatioF() : qGuiApp->devicePixelRatio());
    }

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
QPixmap Renderer::pixmap(Icon icon, const QSize &size, const QColor &color, qreal scaleFactor) const
{
    return pixmap(QChar(static_cast<IconBaseType>(icon)), size, color, scaleFactor);
}

/*!
 * \brief Renders the specified \a character as pixmap of the specified \a size.
 * \remarks
 * - The pixmap will be scaled for the associated paint device or the global device-dixel-ratio if no paint
 *   device has been associated.
 * - When rendering a QPixmap for a QIcon, better use the other overloads with the actual size (and a scale factor
 *   of one).
 */
QPixmap QtForkAwesome::Renderer::pixmap(QChar icon, const QSize &size, const QColor &color) const
{
    return pixmap(icon, size, color, 0.0);
}

/*!
 * \brief Renders the specified \a icon as pixmap of the specified \a size.
 * \remarks
 * - The pixmap will be scaled for the associated paint device or the global device-dixel-ratio if no paint
 *   device has been associated.
 * - When rendering a QPixmap for a QIcon, better use the other overloads with the actual size (and a scale factor
 *   of one).
 */
QPixmap Renderer::pixmap(Icon icon, const QSize &size, const QColor &color) const
{
    return pixmap(QChar(static_cast<IconBaseType>(icon)), size, color, 0.0);
}

/*!
 * \brief Uses the icon from the current icon theme obtained via QIcon::fromTheme() for \a character if it exists.
 */
void Renderer::addThemeOverride(QChar character, const QString &iconNameInTheme)
{
    m_d->overrides[character].addIconName(iconNameInTheme);
}

/*!
 * \brief Uses the specified \a override icon for \a character if it is not null.
 */
void Renderer::addOverride(QChar character, const QIcon &override)
{
    m_d->overrides[character].setIcon(override);
}

/*!
 * \brief Clears all overrides added via addThemeOverride() or addOverride().
 */
void Renderer::clearOverrides()
{
    m_d->overrides.clear();
}

/*!
 * \brief Sets the associated \a paintDevice.
 * \remarks
 * The device-pixel-ratio of the specified device will be used when rendering pixmaps using the overloads that
 * do *not* take a scale factor.
 */
void Renderer::setAssociatedPaintDevice(QPaintDevice *paintDevice)
{
    m_d->paintDevice = paintDevice;
}

/*!
 * \brief Returns the global instance (which is currently only used by the icon engine plugin).
 */
Renderer &Renderer::global()
{
    static auto globalRenderer = Renderer();
    return globalRenderer;
}

} // namespace QtForkAwesome
