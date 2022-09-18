#include "./iconengine.h"

#include "../qtforkawesome/renderer.h"

#include "resources/qtconfig.h"

#include <QIcon>
#include <QIconEnginePlugin>
#include <QSize>

// ensure we don't miss the icon font when linking against qtforkawesome statically but don't build
// a static plugin
#ifndef QT_FORK_AWESOME_ICON_ENGINE_STATIC
ENABLE_QT_RESOURCES_OF_STATIC_DEPENDENCIES
#endif

namespace QtForkAwesome {

class QT_FORK_AWESOME_ICON_ENGINE_EXPORT ForkAwesomeIconEnginePlugin : public QIconEnginePlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QIconEngineFactoryInterface" FILE "qtforkawesomeiconengine.json")

public:
    QIconEngine *create(const QString &filename = QString()) override;
};

QIconEngine *ForkAwesomeIconEnginePlugin::create(const QString &file)
{
    auto *const engine = new IconEngine(Renderer::global());
    if (!file.isNull()) {
        engine->addFile(file, QSize(), QIcon::Normal, QIcon::Off);
    }
    return engine;
}

} // namespace QtForkAwesome

#include "main.moc"
