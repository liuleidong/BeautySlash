
#include "qquickpolygon.h"

#include "C2DPoint.h"
#include "C2DPointSet.h"
#include "C2DLine.h"
#include "C2DHoledPolygonSet.h"
#include "Grid.h"
#include "C2DPolyBase.h"

#include <QGuiApplication>
#include <QDebug>
#include <QWindow>
#include <qmath.h>


QQuickPolygon::QQuickPolygon (QQuickItem * parent)
    : QQuickItem (parent)
    , m_closed (true)
    , m_border (8.0)
    , m_minX   (0.0)
    , m_maxX   (0.0)
    , m_minY   (0.0)
    , m_maxY   (0.0)
    , m_color  (QColor(13,91,43))
    , m_stroke (QColor(255,218,143))
    , m_node         (Q_NULLPTR)
    , m_foreNode     (Q_NULLPTR)
    , m_backNode     (Q_NULLPTR)
    , m_foreGeometry (Q_NULLPTR)
    , m_backGeometry (Q_NULLPTR)
    , m_foreMaterial (Q_NULLPTR)
    , m_backMaterial (Q_NULLPTR)
    , m_poly(NULL)
    , m_remainIdx(-1)
    , m_flyIdx(-1)
    , m_totalArea(0.0)
    , m_progress(0.0)
    , m_flyArea(0.0)
{
    setFlag (QQuickItem::ItemHasContents);
}

qreal QQuickPolygon::getBorder (void) const {
    return m_border;
}

bool QQuickPolygon::getClosed (void) const {
    return m_closed;
}

QColor QQuickPolygon::getColor (void) const {
    return m_color;
}

QColor QQuickPolygon::getStroke (void) const {
    return m_stroke;
}

QVariantList QQuickPolygon::getPoints (void) const {
    QVariantList ret;
    foreach (QPointF point, m_points) {
        ret.append (point);
    }
    return ret;
}

bool QQuickPolygon::isCutPolygon(qreal x1, qreal y1, qreal x2, qreal y2)
{
    C2DPointSet interSet;
    C2DPoint pt1(x1,y1);
    C2DPoint pt2(x2,y2);
    C2DLine line(pt1,pt2);
    m_poly->Crosses(line,&interSet);
    if(interSet.size() > 1)
        return true;
    else
        return false;
}

void QQuickPolygon::createPolygon(const QVector<QPointF> &pts,C2DPolygon &poly)
{
    C2DPointSet polySet;
    for(int i = 0;i < pts.size();++i)
    {
        C2DPoint pt(pts[i].x(),pts[i].y());
        polySet.AddCopy(pt);
    }
    poly.Create(polySet,true);
}

bool QQuickPolygon::isCutPolygon(const C2DPolygon &poly, qreal x1, qreal y1, qreal x2, qreal y2)
{
    C2DPointSet interSet;
    C2DPoint pt1(x1,y1);
    C2DPoint pt2(x2,y2);
    C2DLine line(pt1,pt2);
    poly.Crosses(line,&interSet);

    if((interSet.size() > 0 && interSet.size() % 2 == 0))
        return true;
    else
        return false;
}

//返回1-失败，重新开局
//2-画在了两球之间，画线闪烁
int QQuickPolygon::calcSlashPoly(qreal w,qreal h,qreal x1, qreal y1, qreal x2, qreal y2,const QVariantList &ballsPos,const QVariantList &ballsRadius)
{
    m_lastPolySet.DeleteAll();
    m_remainIdx = -1;
    m_flyIdx = -1;

    //1.计算画的线和大矩形框交割成的两个多边形
    m_maxX = w;
    m_maxY = h;
    QVector<QPointF> polyPt1;
    QVector<QPointF> polyPt2;
    calParts(x1,y1,x2,y2,polyPt1,polyPt2);
    //2.计算两个多边形和图中矩形重叠的多边形
    C2DPolygon poly1;
    C2DPolygon poly2;
    createPolygon(polyPt1,poly1);
    createPolygon(polyPt2,poly2);

    C2DHoledPolygonSet overPolySet1;
    C2DHoledPolygonSet overPolySet2;
    poly1.GetOverlaps(*m_poly,overPolySet1,CGrid::RandomPerturbation);
    poly2.GetOverlaps(*m_poly,overPolySet2,CGrid::RandomPerturbation);

    //3.切割区域判断合并
    C2DPolygonSet onePolySet;
    C2DPolygonSet multiPolySet;
    dealOverlaps(overPolySet1,onePolySet,multiPolySet);
    dealOverlaps(overPolySet2,onePolySet,multiPolySet);

    getLastPolys(onePolySet,multiPolySet,x1,y1,x2,y2);
#if 1
    //4.判断切割线和球距离
    C2DLine slashLine(C2DPoint(x1,y1),C2DPoint(x2,y2));
    for (int idx = 0; idx < ballsPos.size(); idx++)
    {
        QPointF pt = ballsPos.at (idx).value<QPointF> ();
        double dst = slashLine.Distance(C2DPoint(pt.x(),pt.y()));
        double radius = ballsRadius.at(idx).value<double>();
        if(dst < radius)
        {
            qDebug() << "line cross ball failed";
            //TODO 失败，重新开始
            return 1;
        }
    }
#endif
    m_flyIdx = m_remainIdx = -1;
    for(size_t i = 0;i < m_lastPolySet.size();++i)
    {
        int kk = 0;
        for (int idx = 0; idx < ballsPos.size(); idx++)
        {
            QPointF pt = ballsPos.at (idx).value<QPointF> ();
            if(m_lastPolySet[i].Contains(C2DPoint(pt.x(),pt.y())))
                kk++;
        }
        if(kk == 0)
        {
            qDebug() << "I will fly away";
            m_flyIdx = i;
            break;
        }
    }
    if(m_flyIdx != -1)
    {
        m_remainIdx = 0;
        if(m_flyIdx == 0)
            m_remainIdx = 1;
        //更新剩余面积信息
        shownPolyUpdate(m_flyIdx,m_remainIdx);
        return 0;
    }

    return 2;
}

//type 0-剩余的poly 1-切掉的poly
QVariantList QQuickPolygon::getResultPoly(int type)
{
    QVariantList list;
    int idx = -1;
    if(type == 0)
        idx = m_remainIdx;
    else
        idx = m_flyIdx;
    if(idx < 0 || idx > m_lastPolySet.size())
        return list;
    C2DPolygon poly = m_lastPolySet[idx];
    C2DPointSet pts;
    poly.GetPointsCopy(pts);
    for(size_t i = 0;i < pts.size();++i)
    {
        list.append(pts[i].x);
        list.append(pts[i].y);
    }
    return list;
}

qreal QQuickPolygon::getProgress() const
{
    return m_progress;
}

void QQuickPolygon::deInit()
{
    if(m_poly)
        delete m_poly;
    m_lastPolySet.DeleteAll();
    m_remainIdx = m_flyIdx = -1;
    m_totalArea = m_progress = 0.0;
    m_flyArea = 0.0;
    m_points.clear();
}

bool QQuickPolygon::isShouldDrawLine(qreal x1, qreal y1, qreal x2, qreal y2)
{
    C2DPointSet interSet;
    C2DLineBaseSet lineSet;
    C2DPoint pt1(x1,y1);
    C2DPoint pt2(x2,y2);
    C2DLine line(pt1,pt2);
    m_poly->Crosses(line,&interSet,&lineSet);
    if(interSet.size() == 1)
    {
        m_lineStart = QPointF(interSet[0].x,interSet[0].y);
        return true;
    }

    return false;
}

QPointF QQuickPolygon::getLineStart()
{
    return m_lineStart;
}

int QQuickPolygon::isBallCrossLine(qreal x1,qreal y1,qreal x2,qreal y2,const QVariantList &ballsPos,const QVariantList &ballsRadius)
{
    C2DLine slashLine(C2DPoint(x1,y1),C2DPoint(x2,y2));
    for (int idx = 0; idx < ballsPos.size(); idx++)
    {
        QPointF pt = ballsPos.at (idx).value<QPointF> ();
        double dst = slashLine.Distance(C2DPoint(pt.x(),pt.y()));
        double radius = ballsRadius.at(idx).value<double>();
        if(dst < radius)
        {
            qDebug() << "line cross ball failed";
            return 1;
        }
    }
    return 0;
}

int QQuickPolygon::isCrossPolygon(qreal x1, qreal y1, qreal x2, qreal y2)
{
    C2DPointSet interSet;
    C2DLineBaseSet lineSet;
    C2DPoint pt1(x1,y1);
    C2DPoint pt2(x2,y2);
    C2DLine line(pt1,pt2);
    m_poly->Crosses(line,&interSet,&lineSet);
    if(interSet.size() == 1)
    {
        m_lineStart = QPointF(interSet[0].x,interSet[0].y);
    }
    else if(interSet.size() == 2)
    {
        m_interLine = line;
    }

    if(interSet.size() > 0 && isLineRigid(lineSet))
        return 9999;

    return interSet.size();
}

void QQuickPolygon::calParts(float sx, float sy, float ex, float ey,QVector<QPointF> &poly1,QVector<QPointF> &poly2)
{
    // 0[xmin,ymin]----------3[xmax,ymin]
    // |                              |
    // |                              |
    // 1[xmin,ymax]----------2[xmax,ymax]

    int currIndex = 0;
    QVector<QPointF> a1;
    a1.push_back(QPointF(m_minX,m_minY));
    currIndex++;
    int jd1Index = -1;
    int jd2Index = -1;

    float t = (m_minX - sx)/(ex - sx);
    float y = (ey - sy) * t + sy;
    if(y > m_minY && y < m_maxY)
    {
        jd1Index = currIndex;
        a1.push_back(QPointF(m_minX,y));
        currIndex++;
    }

    a1.push_back(QPointF(m_minX,m_maxY));
    currIndex++;
    t = (m_maxY - sy) / (ey - sy);
    float x = (ex - sx) * t + sx;
    if(x > m_minX && x < m_maxX)
    {
        if(jd1Index == -1)
        {
            jd1Index = currIndex;
        }
        else
        {
            jd2Index = currIndex;
        }
        a1.push_back(QPointF(x,m_maxY));
        currIndex++;
    }

    a1.push_back(QPointF(m_maxX,m_maxY));
    currIndex++;
    t=(m_maxX-sx)/(ex-sx);
    y=(ey-sy)*t+sy;
    if(y>m_minY&&y<m_maxY)
    {
        if(jd1Index==-1)
        {
            jd1Index=currIndex;
        }
        else
        {
            jd2Index=currIndex;
        }
        a1.push_back(QPointF(m_maxX,y));
        currIndex++;
    }

    a1.push_back(QPointF(m_maxX,m_minY));
    currIndex++;

    //求3--0线段传入切割线的交点 y=ymin
    t=(m_minY-sy)/(ey-sy);
    x=(ex-sx)*t+sx;
    if(x>m_minX&&x<m_maxX)
    {
        if(jd1Index==-1)
        {
            jd1Index=currIndex;
        }
        else
        {
            jd2Index=currIndex;
        }
        a1.push_back(QPointF(x,m_minY));
        currIndex++;
    }

    //卷绕第一个多边形
    int startIndex=jd1Index;
    while(true)
    {
        poly1.push_back(a1[startIndex]);
        if(startIndex==jd2Index)
        {
            break;
        }
        startIndex=(startIndex+1)%a1.size();
    }

    startIndex=jd2Index;
    while(true)
    {
        poly2.push_back(a1[startIndex]);
        if(startIndex==jd1Index)
        {
            break;
        }
        startIndex=(startIndex+1)%a1.size();
    }
}

int QQuickPolygon::isPointSetContain(const C2DPointSet &pts, const C2DPoint &pt)
{
    int index = -1;
    for(size_t i = 0;i < pts.size();++i)
    {
        if(qAbs(pts[i].x - pt.x) < 0.001 && qAbs(pts[i].y - pt.y) < 0.001 )
        {
            index = i;
            break;
        }
    }
    return index;
}

void QQuickPolygon::combinePolygon(C2DPolygon &poly1, C2DPolygon &poly2, C2DPolygon &comPoly)
{
#if 1
    C2DPointSet pts1;
    C2DPointSet pts2;
    poly1.GetPointsCopy(pts1);
    poly2.GetPointsCopy(pts2);

    QVector<int> repeatIdx;
    for(size_t i = 0;i < pts1.size();++i)
    {
        const C2DPoint pt = pts1[i];
        int index = isPointSetContain(pts2,pt);
        if(index == -1)
        {
            pts2.AddCopy(pt);
        }
        else
        {
            repeatIdx.push_back(index);
            //pts2.DeleteAndSet(index,pts2.GetAt((index+1)%pts2.size()));
        }
    }

    C2DPointSet finalSet;
    for(int i = 0;i < pts2.size();i++)
    {
        if(repeatIdx.indexOf(i) == -1)
            finalSet.AddCopy(pts2[i]);
        //pts2.DeleteAndSet(index,pts2.GetAt((index+1)%pts2.size()));
    }

    comPoly.Create(finalSet,true);
    debugPolygon(comPoly);
#endif
}

void QQuickPolygon::dealOverlaps(const C2DHoledPolygonSet &holedPolySet, C2DPolygonSet &onePolySet, C2DPolygonSet &multiPolySet)
{
    if(holedPolySet.size() == 1)
    {
        onePolySet.AddCopy(*(holedPolySet.GetAt(0)->GetRim()));
    }
    else
    {
        for(size_t i = 0;i < holedPolySet.size();++i)
        {
            multiPolySet.AddCopy(*(holedPolySet.GetAt(i)->GetRim()));
        }
    }
}

void QQuickPolygon::getLastPolys(C2DPolygonSet &onePolySet,C2DPolygonSet &multiPolySet, qreal x1, qreal y1, qreal x2, qreal y2)
{
    C2DPolygonSet combineSet;
    if(multiPolySet.size() > 0)
    {
        int interIdx = 0;
        double distance = 10000.0;
        C2DLine line(C2DPoint(x1,y1),C2DPoint(x2,y2));
        for(size_t i = 0;i < multiPolySet.size();++i)
        {
            if(distance > multiPolySet[i].Distance(line))
            {
                distance = multiPolySet[i].Distance(line);
                interIdx = i;
            }
        }
        m_lastPolySet.AddCopy(multiPolySet[interIdx]);
        for(size_t i = 0;i < multiPolySet.size();++i)
        {
            if(i != interIdx)
                combineSet.AddCopy(multiPolySet[i]);
        }

        if(combineSet.size() > 0)
        {
            C2DPolygon onePoly = onePolySet[0];

            for(size_t i = 0;i < combineSet.size();++i)
            {
                combinePolygon(combineSet[i],onePoly,onePoly);
            }
            m_lastPolySet.AddCopy(onePoly);
        }
        else
        {
            m_lastPolySet.AddCopy(onePolySet[0]);
        }
    }
    else
    {
        for(size_t i = 0;i < onePolySet.size();++i)
        {
            m_lastPolySet.AddCopy(onePolySet[i]);
        }
    }
}

void QQuickPolygon::shownPolyUpdate(int flyIdx,int remainIdx)
{
    m_points.clear();
    if(m_poly)
        delete m_poly;

    C2DPointSet pts;
    m_lastPolySet[remainIdx].GetPointsCopy(pts);
    m_poly = new C2DPolygon(pts,false);
    qreal area = m_lastPolySet[flyIdx].GetArea();
    m_flyArea += area;
    m_progress = m_flyArea / m_totalArea;

    for(size_t i = 0;i < pts.size();++i)
    {
        m_points.append(QPointF(pts[i].x,pts[i].y));
    }

    processTriangulation ();
    emit pointsChanged ();
    update ();
}

int QQuickPolygon::isPtRigid(QPointF &pt1, QPointF &pt2)
{
    QPoint ppt1 = pt1.toPoint();
    QPoint ppt2 = pt2.toPoint();
    bool b1 = false,b2 = false;
    for(int i = 0;i < m_rigidPt.size();i++)
    {
        if(isPtEqual(m_rigidPt[i],ppt1))
            b1 = true;
        if(isPtEqual(m_rigidPt[i],ppt2))
            b2 = true;
        if(b1 & b2)
            return 1;
    }

    return 0;
}

int QQuickPolygon::isLineRigid(C2DLineBaseSet &lineSet)
{
    for(int i = 0;i < lineSet.size();i++)
    {
        C2DPoint pt1 = lineSet[i].GetPointFrom();
        C2DPoint pt2 = lineSet[i].GetPointTo();
        QPointF ppt1 = QPointF(pt1.x,pt1.y);
        QPointF ppt2 = QPointF(pt2.x,pt2.y);
        if(isPtRigid(ppt1,ppt2))
            return true;
    }
    return false;
}

int QQuickPolygon::isPtEqual(QPoint &pt1, QPoint &pt2)
{
    if(qAbs(pt1.x() - pt2.x()) < 3 && qAbs(pt1.y() - pt2.y()) < 3)
        return 1;
    return 0;
}

void QQuickPolygon::debugPolygon(C2DPolygon &poly)
{
    C2DPointSet pst;
    poly.GetPointsCopy(pst);
    for(int i = 0;i < pst.size();i++)
    {
        qDebug() << pst[i].x << " " << pst[i].y;
    }
}

void QQuickPolygon::setBorder (qreal border) {
    if (m_border != border) {
        m_border = border;
        emit borderChanged ();
        update ();
    }
}

void QQuickPolygon::setClosed (bool closed) {
    if (m_closed != closed) {
        m_closed = closed;
        emit closedChanged ();
        update ();
    }
}

void QQuickPolygon::setColor (const QColor & color) {
    if (m_color != color) {
        m_color = color;
        emit colorChanged ();
        update ();
    }
}

void QQuickPolygon::setStroke (const QColor & stroke) {
    if (m_stroke != stroke) {
        m_stroke = stroke;
        emit strokeChanged ();
        update ();
    }
}
void QQuickPolygon::setPoints (const QVariantList & points) {
    bool dirty = false;
    C2DPointSet pst;
    const int count = points.size ();
    if (m_points.size () != count/2) {
        m_points.resize (count/2);
        dirty = true;
    }
    for (int idx = 0,i = 0; idx < count; idx+=2,i++) {
        QPointF pt = points.at (idx).value<QPointF> ();
        QPoint rigidPt = points.at (idx+1).value<QPoint> ();
        if (pt != m_points.at (i)) {
            m_points [i] = pt;
            dirty = true;
        }
        if(rigidPt.x())
        {
            QPoint nextPt = points.at ((idx+2)%count).value<QPoint> ();
            m_rigidPt.push_back(pt.toPoint());
            m_rigidPt.push_back(nextPt);
            qDebug() << pt << nextPt;
        }
        C2DPoint cpt(pt.x(),pt.y());
        pst.AddCopy(cpt);
    }
    m_poly = new C2DPolygon(pst,false);
    m_totalArea = m_poly->GetArea();
    if (dirty) {
        processTriangulation ();
        emit pointsChanged ();
        update ();
    }
}

static inline qreal getAngleFromSegment (const QPointF & startPoint, const QPointF & endPoint) {
    return qAtan2 (endPoint.y () - startPoint.y (), endPoint.x () - startPoint.x ());
}

QSGNode * QQuickPolygon::updatePaintNode (QSGNode * oldNode, UpdatePaintNodeData * updatePaintNodeData) {
    Q_UNUSED (oldNode)
    Q_UNUSED (updatePaintNodeData)
    // remove old nodes
    if (m_backMaterial != Q_NULLPTR) {
        delete m_backMaterial;
    }
    if (m_foreMaterial != Q_NULLPTR) {
        delete m_foreMaterial;
    }
    if (m_backGeometry != Q_NULLPTR) {
        delete m_backGeometry;
    }
    if (m_foreGeometry != Q_NULLPTR) {
        delete m_foreGeometry;
    }
    if (m_backNode != Q_NULLPTR) {
        delete m_backNode;
    }
    if (m_foreNode != Q_NULLPTR) {
        delete m_foreNode;
    }
    if (m_node != Q_NULLPTR) {
        delete m_node;
    }
    m_node = new QSGNode;
    // polygon background tesselation
    if (!m_triangles.isEmpty () && m_color.alpha () > 0) {
        m_backGeometry = new QSGGeometry (QSGGeometry::defaultAttributes_Point2D (), m_triangles.size ());
        m_backGeometry->setDrawingMode (GL_TRIANGLES);
        QSGGeometry::Point2D * vertex = m_backGeometry->vertexDataAsPoint2D ();
        const int size = m_triangles.size ();
        for (int idx = 0; idx < size; idx++) {
            vertex [idx].x = m_triangles [idx].x ();
            vertex [idx].y = m_triangles [idx].y ();
        }
        m_backMaterial = new QSGFlatColorMaterial;
        m_backMaterial->setColor (m_color);
        m_backNode = new QSGGeometryNode;
        m_backNode->setGeometry (m_backGeometry);
        m_backNode->setMaterial (m_backMaterial);
        m_node->appendChildNode (m_backNode);
    }
    // polyline stroke generation
    if (m_points.size () >= 2 && m_border > 0 && m_stroke.alpha () > 0) {
        const int pointsCount    = m_points.size ();
        const int linesCount     = (m_closed ? pointsCount : pointsCount -1);
        const int trianglesCount = (linesCount * 2);
        const int vertexCount    = (trianglesCount * 3);
        const qreal halfStroke = (qreal (m_border) * 0.5);
        QVector<QPointF> trianglesStroke;
        trianglesStroke.reserve (vertexCount);
        QPointF firstVec1, firstVec2, lastVec1, lastVec2;
        for (int startPointIdx = 0, endPointIdx = 1; endPointIdx < pointsCount; startPointIdx++, endPointIdx++) {
            const bool isFirst = (startPointIdx == 0);
            const bool isLast  = (endPointIdx   == pointsCount -1);
            const QPointF startPoint = m_points [startPointIdx];
            const QPointF endPoint   = m_points [endPointIdx];
            qreal currAngle = getAngleFromSegment (startPoint, endPoint);
            qreal prevAngle = (!isFirst ? getAngleFromSegment (startPoint, m_points [startPointIdx -1]) : (m_closed ? getAngleFromSegment (startPoint, m_points.last ()) : currAngle + M_PI));
            qreal nextAngle = (!isLast  ? getAngleFromSegment (m_points [endPointIdx +1], endPoint)     : (m_closed ? getAngleFromSegment (m_points.first (), endPoint)  : currAngle + M_PI));
            qreal startAngle = ((currAngle + prevAngle) * 0.5);
            qreal endAngle   = ((currAngle + nextAngle) * 0.5);
            QPointF startPolar = QPointF (qCos (startAngle), qSin (startAngle));
            QPointF endPolar   = QPointF (qCos (endAngle),   qSin (endAngle));
            QPointF startVec1 = (startPoint + startPolar * (halfStroke / qSin (startAngle - currAngle)));
            QPointF startVec2 = (startPoint + startPolar * (halfStroke / qSin (currAngle  - startAngle)));
            QPointF endVec1   = (endPoint   + endPolar   * (halfStroke / qSin (endAngle   - currAngle)));
            QPointF endVec2   = (endPoint   + endPolar   * (halfStroke / qSin (currAngle  - endAngle)));
            trianglesStroke << startVec1 << startVec2 << endVec2;
            trianglesStroke << endVec1   << endVec2   << startVec1;
            if (m_closed) {
                if (isFirst) {
                    firstVec1 = startVec1;
                    firstVec2 = startVec2;
                }
                if (isLast) {
                    lastVec1 = endVec1;
                    lastVec2 = endVec2;
                }
            }
        }
        if (m_closed) {
            trianglesStroke << lastVec1  << lastVec2  << firstVec2;
            trianglesStroke << firstVec1 << firstVec2 << lastVec1;
        }

        QVector<QPointF> finalTrianglesStroke;
        for(int i = 0;i < m_points.size();i++)
        {
            QPointF pt = m_points[i];
            QPointF nextPt = m_points[(i+1)%m_points.size()];
            if(isPtRigid(pt,nextPt))
            {
                for(int j = i * 6;j < (i * 6 + 6);j++)
                {
                    finalTrianglesStroke.push_back(trianglesStroke[j]);
                }
            }
        }

        m_foreGeometry = new QSGGeometry (QSGGeometry::defaultAttributes_Point2D (), finalTrianglesStroke.size ());
        m_foreGeometry->setDrawingMode (GL_TRIANGLES);
        QSGGeometry::Point2D * vertex = m_foreGeometry->vertexDataAsPoint2D ();
        const int size = finalTrianglesStroke.size ();
        for (int idx = 0; idx < size; idx++) {
            vertex [idx].x = finalTrianglesStroke [idx].x ();
            vertex [idx].y = finalTrianglesStroke [idx].y ();
        }
        m_foreMaterial = new QSGFlatColorMaterial;
        m_foreMaterial->setColor (m_stroke);
        m_foreNode = new QSGGeometryNode;
        m_foreNode->setGeometry (m_foreGeometry);
        m_foreNode->setMaterial (m_foreMaterial);
        m_node->appendChildNode (m_foreNode);
    }
    return m_node;
}

void QQuickPolygon::processTriangulation (void) {
    // allocate and initialize list of Vertices in polygon
    const int n = m_points.size ();
    m_triangles.clear ();
    m_triangles.reserve (n * 3);
    if (n >= 3) {
        QVector<int> index (n);
        for (int i = 0; i < n; i++) {
            index [i] = i;
        }
        // remove nv-2 Vertices, creating 1 triangle every time
        int nv = n;
        int count = (2 * nv); // error detection
        QPolygonF triangle (3);
        for (int v = (nv -1), u, w; nv > 2;) {
            // if we loop, it is probably a non-simple polygon
            count--;
            if (count > 0) {
                // three consecutive vertices in current polygon, <u,v,w>
                u = (v    < nv ? v    : 0); // previous
                v = (u +1 < nv ? u +1 : 0); // new v
                w = (v +1 < nv ? v +1 : 0); // next
                triangle [0] = m_points [index [u]];
                triangle [1] = m_points [index [v]];
                triangle [2] = m_points [index [w]];
                QPolygonF result = triangle.intersected (m_points);
                if (result.isClosed ()) {
                    result.removeLast ();
                }
                if (result == triangle) {
                    // output Triangle
                    m_triangles.append (m_points [index [u]]);
                    m_triangles.append (m_points [index [v]]);
                    m_triangles.append (m_points [index [w]]);
                    index.remove (v); // remove v from remaining polygon
                    nv--;
                    count = (2 * nv); // reset error detection counter
                }
            }
            else {
                // Triangulate: ERROR - probable bad polygon!
                break;
            }
        }
    }
}
