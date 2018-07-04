#include "qquickline.h"

#include <QtQuick/qsgnode.h>
#include <QtQuick/qsgflatcolormaterial.h>

QQuickLine::QQuickLine(QQuickItem *parent)
    : QQuickItem(parent)
    , m_p1(0, 0)
    , m_p2(0, 0)
{
    setFlag(ItemHasContents, true);
}

QQuickLine::~QQuickLine()
{

}

void QQuickLine::setP1(const QPointF &p)
{
    if (p == m_p1)
        return;

    m_p1 = p;
    emit p1Changed(p);
    update();
}

void QQuickLine::setP2(const QPointF &p)
{
    if (p == m_p2)
        return;

    m_p2 = p;
    emit p2Changed(p);
    update();
}

QSGNode *QQuickLine::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
    QSGGeometryNode *node = 0;
    QSGGeometry *geometry = 0;

    if (!oldNode) {
        node = new QSGGeometryNode;

        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 2);
        geometry->setLineWidth(10);
        geometry->setDrawingMode(QSGGeometry::DrawLineStrip);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);

        QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
        material->setColor(QColor(255,229,160));
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);

    } else {
        node = static_cast<QSGGeometryNode *>(oldNode);
        geometry = node->geometry();
        geometry->allocate(2);
    }

    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();

    vertices[0].set(m_p1.x(), m_p1.y());
    vertices[1].set(m_p2.x(), m_p2.y());

    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}
