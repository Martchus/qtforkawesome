#include "../renderer.h"
#include "../icon.h"

#include <c++utilities/tests/testutils.h>

#include <qtutilities/misc/disablewarningsmoc.h>

#include <QDir>
#include <QPixmap>
#include <QtTest/QtTest>

using namespace CppUtilities;

class RendererTests : public QObject {
    Q_OBJECT

private Q_SLOTS:
    void testRender();
};

void RendererTests::testRender()
{
    // render two random icons
    auto renderer = QtForkAwesome::Renderer();
    const auto globe = renderer.pixmap(QtForkAwesome::Icon::Globe, QSize(80, 64), Qt::green);
    QCOMPARE(globe.width(), 80 * globe.devicePixelRatio());
    QCOMPARE(globe.height(), 64 * globe.devicePixelRatio());
    const auto hashtag = renderer.pixmap(QtForkAwesome::Icon::Hashtag, QSize(50, 64), Qt::blue);
    QCOMPARE(hashtag.width(), 50 * hashtag.devicePixelRatio());
    QCOMPARE(hashtag.height(), 64 * hashtag.devicePixelRatio());

    // save the icons as PNGs to check manually whether they look as expected
    const auto workingDirectory = QString::fromStdString(TestApplication().workingDirectory());
    QDir().mkpath(workingDirectory);
    globe.save(workingDirectory + QStringLiteral("/globe.png"));
    hashtag.save(workingDirectory + QStringLiteral("/hashtag.png"));
}

QT_UTILITIES_DISABLE_WARNINGS_FOR_MOC_INCLUDE
QTEST_MAIN(RendererTests)
#include "renderer.moc"
