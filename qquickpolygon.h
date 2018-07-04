#ifndef QMLPOLYGON_H
#define QMLPOLYGON_H

#include <QQuickItem>
#include <QVariant>
#include <QColor>
#include <QList>
#include <QPointF>
#include <QPolygonF>
#include <QSGNode>
#include <QSGGeometry>
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>

#include "C2DPolygon.h"
#include "C2DPolygonSet.h"
#include "C2DLineSet.h"

class QQuickPolygon : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY (bool         closed READ getClosed WRITE setClosed NOTIFY closedChanged) // whether last point should connect to first
    Q_PROPERTY (qreal        border READ getBorder WRITE setBorder NOTIFY borderChanged) // border width
    Q_PROPERTY (QColor       color  READ getColor  WRITE setColor  NOTIFY colorChanged)  // back color
    Q_PROPERTY (QColor       stroke READ getStroke WRITE setStroke NOTIFY strokeChanged) // border color
    Q_PROPERTY (QVariantList points READ getPoints WRITE setPoints NOTIFY pointsChanged) // points list

public:
    explicit QQuickPolygon (QQuickItem * parent = NULL);

    Q_INVOKABLE bool         getClosed (void) const;
    Q_INVOKABLE qreal        getBorder (void) const;
    Q_INVOKABLE QColor       getColor  (void) const;
    Q_INVOKABLE QColor       getStroke (void) const;
    Q_INVOKABLE QVariantList getPoints (void) const;
    Q_INVOKABLE bool         isCutPolygon(qreal x1,qreal y1,qreal x2,qreal y2);
    Q_INVOKABLE int calcSlashPoly(qreal w,qreal h,qreal x1,qreal y1,qreal x2,qreal y2,const QVariantList &ballsPos,const QVariantList &ballsRadius);
    Q_INVOKABLE QVariantList getResultPoly(int type);
    Q_INVOKABLE qreal        getProgress(void) const;
    Q_INVOKABLE void         deInit();
    Q_INVOKABLE bool         isShouldDrawLine(qreal x1,qreal y1,qreal x2,qreal y2);
    Q_INVOKABLE QPointF      getLineStart();
    Q_INVOKABLE int          isBallCrossLine(qreal x1,qreal y1,qreal x2,qreal y2,const QVariantList &ballsPos,const QVariantList &ballsRadius);
    Q_INVOKABLE int          isCrossPolygon(qreal x1,qreal y1,qreal x2,qreal y2);

public:
    void createPolygon(const QVector<QPointF> &pts,C2DPolygon &poly);
    bool isCutPolygon(const C2DPolygon &poly,qreal x1,qreal y1,qreal x2,qreal y2);
    //画的线将整个屏幕划分为两个多边形，分别用着两个多边形和游戏区域的多边形计算重叠区域
    void calParts(float sx,float sy,float ex,float ey,QVector<QPointF> &poly1,QVector<QPointF> &poly2);

    //查看pt是否在pts中，如果在则返回index，否则返回-1
    int isPointSetContain(const C2DPointSet &pts,const C2DPoint &pt);
    void combinePolygon(C2DPolygon &poly1,C2DPolygon &poly2,C2DPolygon &comPoly);
    void dealOverlaps(const C2DHoledPolygonSet &holedPolySet,C2DPolygonSet &onePolySet,C2DPolygonSet &multiPolySet);
    void getLastPolys(C2DPolygonSet &onePolySet,C2DPolygonSet &multiPolySet,qreal x1,qreal y1,qreal x2,qreal y2);
    void shownPolyUpdate(int flyIdx,int remainIdx);

    int isPtRigid(QPointF &pt1, QPointF &pt2);
    int isLineRigid(C2DLineBaseSet &lineSet);
    int isPtEqual(QPoint &pt1,QPoint &pt2);

    void debugPolygon(C2DPolygon &poly);

public slots:
    void setClosed (bool closed);
    void setBorder (qreal border);
    void setColor  (const QColor & color);
    void setStroke (const QColor & stroke);
    void setPoints (const QVariantList & points);

signals:
    void colorChanged  (void);
    void pointsChanged (void);
    void borderChanged (void);
    void closedChanged (void);
    void strokeChanged (void);

protected:
    virtual QSGNode * updatePaintNode (QSGNode * oldNode, UpdatePaintNodeData * updatePaintNodeData);

protected slots:
    void processTriangulation (void);

private:
    bool m_closed;
    qreal m_border;
    qreal m_minX;
    qreal m_maxX;
    qreal m_minY;
    qreal m_maxY;
    QColor m_color;
    QColor m_stroke;
    QPolygonF m_points;
    QVector<QPointF> m_triangles;
    QSGNode *              m_node;
    QSGGeometryNode *      m_foreNode;
    QSGGeometryNode *      m_backNode;
    QSGGeometry *          m_foreGeometry;
    QSGGeometry *          m_backGeometry;
    QSGFlatColorMaterial * m_foreMaterial;
    QSGFlatColorMaterial * m_backMaterial;

    C2DPolygon *m_poly;
    C2DPolygonSet m_lastPolySet;
    int m_remainIdx;
    int m_flyIdx;
    qreal m_totalArea;
    qreal m_flyArea;
    qreal m_progress;
    QPointF m_lineStart;
    C2DLine m_interLine;
    QVector<QPoint> m_rigidPt;
};

#endif // QMLPOLYGON_H
