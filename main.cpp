#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>
#include <QLocale>

#include "qquickpolygon.h"
#include "qquickline.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QLocale locale;
    QString trPath;

    if(locale.language() == QLocale::Chinese)
        trPath = ":/content/tr_zh.qm";
    else
        trPath = ":/content/tr_en.qm";

    QTranslator translator;
    translator.load(trPath);
    app.installTranslator(&translator);

    qmlRegisterType<QQuickPolygon>("LL.BPolygon",1,0,"BPolygon");
    qmlRegisterType<QQuickLine>("LL.BLine", 1, 0, "BLine");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    engine.rootContext()->setContextProperty("qGuiApp",&app);

    return app.exec();
}
