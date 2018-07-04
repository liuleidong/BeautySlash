QT += quick
CONFIG += c++11 resources_big

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS _STATIC

TRANSLATIONS += content/tr_zh.ts \
               content/tr_en.ts

lupdate_only{
    SOURCES+ = GameCanvas.qml \
                GameLevel.qml \
                GameToolBar.qml \
                NewGameScreen.qml \
                main.qml
}

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += GeoLib

SOURCES += main.cpp \
    qquickpolygon.cpp \
    GeoLib/C2DArc.cpp \
    GeoLib/C2DBaseSet.cpp \
    GeoLib/C2DCircle.cpp \
    GeoLib/C2DHoledPolyArc.cpp \
    GeoLib/C2DHoledPolyArcSet.cpp \
    GeoLib/C2DHoledPolyBase.cpp \
    GeoLib/C2DHoledPolyBaseSet.cpp \
    GeoLib/C2DHoledPolygon.cpp \
    GeoLib/C2DHoledPolygonSet.cpp \
    GeoLib/C2DLine.cpp \
    GeoLib/C2DLineBase.cpp \
    GeoLib/C2DLineBaseSet.cpp \
    GeoLib/C2DLineBaseSetSet.cpp \
    GeoLib/C2DLineSet.cpp \
    GeoLib/C2DPoint.cpp \
    GeoLib/C2DPointSet.cpp \
    GeoLib/C2DPolyArc.cpp \
    GeoLib/C2DPolyArcSet.cpp \
    GeoLib/C2DPolyBase.cpp \
    GeoLib/C2DPolyBaseSet.cpp \
    GeoLib/C2DPolygon.cpp \
    GeoLib/C2DPolygonSet.cpp \
    GeoLib/C2DRect.cpp \
    GeoLib/C2DRectSet.cpp \
    GeoLib/C2DRoute.cpp \
    GeoLib/C2DSegment.cpp \
    GeoLib/C2DTriangle.cpp \
    GeoLib/C2DVector.cpp \
    GeoLib/C3DPoint.cpp \
    GeoLib/Grid.cpp \
    GeoLib/IndexSet.cpp \
    GeoLib/Interval.cpp \
    GeoLib/RandomNumber.cpp \
    GeoLib/TravellingSalesman.cpp \
    qquickline.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    qquickpolygon.h \
    GeoLib/C2DArc.h \
    GeoLib/C2DBase.h \
    GeoLib/C2DBaseSet.h \
    GeoLib/C2DCircle.h \
    GeoLib/C2DHoledPolyArc.h \
    GeoLib/C2DHoledPolyArcSet.h \
    GeoLib/C2DHoledPolyBase.h \
    GeoLib/C2DHoledPolyBaseSet.h \
    GeoLib/C2DHoledPolygon.h \
    GeoLib/C2DHoledPolygonSet.h \
    GeoLib/C2DLine.h \
    GeoLib/C2DLineBase.h \
    GeoLib/C2DLineBaseSet.h \
    GeoLib/C2DLineBaseSetSet.h \
    GeoLib/C2DLineSet.h \
    GeoLib/C2DPoint.h \
    GeoLib/C2DPointSet.h \
    GeoLib/C2DPolyArc.h \
    GeoLib/C2DPolyArcSet.h \
    GeoLib/C2DPolyBase.h \
    GeoLib/C2DPolyBaseSet.h \
    GeoLib/C2DPolygon.h \
    GeoLib/C2DPolygonSet.h \
    GeoLib/C2DRect.h \
    GeoLib/C2DRectSet.h \
    GeoLib/C2DRoute.h \
    GeoLib/C2DSegment.h \
    GeoLib/C2DTriangle.h \
    GeoLib/C2DVector.h \
    GeoLib/C3DPoint.h \
    GeoLib/Constants.h \
    GeoLib/GeoLib.h \
    GeoLib/Grid.h \
    GeoLib/IndexSet.h \
    GeoLib/Interval.h \
    GeoLib/MemoryPool.h \
    GeoLib/RandomNumber.h \
    GeoLib/resource.h \
    GeoLib/Sort.h \
    GeoLib/StdAfx.h \
    GeoLib/Transformation.h \
    GeoLib/TravellingSalesman.h \
    qquickline.h

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
