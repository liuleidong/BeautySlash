#ifndef QQUICKLINE_H
#define QQUICKLINE_H

#include <QQuickItem>

class QQuickLine : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QPointF p1 READ p1 WRITE setP1 NOTIFY p1Changed)
    Q_PROPERTY(QPointF p2 READ p2 WRITE setP2 NOTIFY p2Changed)

public:
    QQuickLine(QQuickItem *parent = 0);
    ~QQuickLine();

    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    QPointF p1() const { return m_p1; }
    QPointF p2() const { return m_p2; }

    void setP1(const QPointF &p);
    void setP2(const QPointF &p);

signals:
    void p1Changed(const QPointF &p);
    void p2Changed(const QPointF &p);

public slots:

private:
    QPointF m_p1;
    QPointF m_p2;
};

#endif // QQUICKLINE_H
