#include "./iconengine.h"

#include "../qtforkawesome/renderer.h"

#include <QIcon>
#include <QIconEnginePlugin>
#include <QSize>

namespace QtForkAwesome {

class QT_FORK_AWESOME_ICON_ENGINE_EXPORT ForkAwesomeIconEnginePlugin : public QIconEnginePlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QIconEngineFactoryInterface" FILE "qtforkawesomeiconengine.json")

public:
    QIconEngine *create(const QString &filename = QString()) override;

private:
    Renderer m_renderer;
};

QIconEngine *ForkAwesomeIconEnginePlugin::create(const QString &file)
{
    auto *const engine = new IconEngine(m_renderer);
    if (!file.isNull()) {
        engine->addFile(file, QSize(), QIcon::Normal, QIcon::Off);
    }
    return engine;
}

} // namespace QtForkAwesome

#include "main.moc"
