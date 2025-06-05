#ifndef PROCESSOR_VISUALIZER_H
#define PROCESSOR_VISUALIZER_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTreeWidgetItem>
#include <QMap>
#include <QList>
#include <QPair>
#include <QGraphicsObject>
#include <QPixmap>
#include <Qsize>
#include <QGraphicsSceneMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// 自定义带图片的节点类，支持悬停、点击
class NodeItem : public QGraphicsObject
{
    Q_OBJECT
public:
    // 构造函数：id-节点编号，imgPath-图片路径，pos-初始位置
    explicit NodeItem(int nodeId, const QString& imgPath, const QPointF& pos,QSize size,bool keepAspectRatio = false,
                      QGraphicsItem *parent = nullptr);
    int nodeId() const { return m_nodeId; }

    // QGraphicsObject 必须实现的纯虚函数
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    // 设置高亮（悬停/点击用）
    void setHighlight(bool highlight);
    QPainterPath shape() const override;

signals:
    void nodeClicked(int nodeId);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    int m_nodeId;              // 节点编号
    bool m_isHighlighted;      // 是否高亮
    QPointF m_position;        // 节点位置
    QPixmap m_pixmap;          // 节点图片
    QSize m_size;              // 节点尺寸
    bool m_keepAspectRatio;    // 节点是否保持宽高比
};

// 自定义端口类（固定在节点上，支持交互）
class PortItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit PortItem(int portId, NodeItem* parentNode, QPointF offset, QGraphicsItem *parent = nullptr);
    int portId() const { return m_portId; }
    NodeItem* parentNode() const { return m_parentNode; }

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    void setHighlight(bool highlight);

signals:
    void portClicked(int portId);
    void portRightClicked(int portId);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    int m_portId;
    NodeItem* m_parentNode;
    bool m_isHighlighted;
};

// 自定义连线类：用于节点之间连接，支持折线或直线显示
class EdgeItem : public QGraphicsObject {
    Q_OBJECT
public:
    EdgeItem(int edgeId, const QPointF &start, const QPointF &end, bool polyline = false, QGraphicsItem *parent = nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath shape() const override;

    void setHighlight(bool highlight);

signals:
    void edgeClicked(int edgeId);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    int m_edgeId;
    QPointF m_start, m_end;
    bool m_polyline;
    bool m_isHighlighted;
};

// 自定义模块类，显示模块图像并支持点击和悬停
class ModuleItem : public QGraphicsObject {
    Q_OBJECT
public:
    explicit ModuleItem(const QString &name, const QString &imgPath, const QPointF &pos, QSize size, bool keepAspectRatio = false,
                        QGraphicsItem *parent = nullptr);
    QString moduleName() const { return m_name; }

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath shape() const override;

    void setHighlight(bool highlight);

signals:
    void moduleClicked(const QString &name);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    QString m_name;
    bool m_isHighlighted;
    QPointF m_position;
    QPixmap m_pixmap;
    QSize m_size;
    bool m_keepAspectRatio;
};


class ProcessorVisualizer : public QMainWindow
{
    Q_OBJECT

public:
    ProcessorVisualizer(QWidget *parent = nullptr);
    ~ProcessorVisualizer();

private slots:
    void onActionOpenHardware();    // 打开文件功能
    void onActionOpenPerformance();
    void onNodeClickedHandler(int nodeId); // 节点点击响应
    void onPortLeftClickedHandler(int portId);   // 左键：只显示端口编号
    void onPortRightClickedHandler(int portId);  // 右键：选中两个端口并显示统计
    void onModuleClickedHandler(const QString &name);
    void onEdgeClickedHandler(int edgeId);
    void onHardwareItemClicked(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *m_scene;       // 场景

    // 节点存储：编号 -> NodeItem
    QMap<int, NodeItem*> m_nodeItems;
    // 端口存储：编号 -> PortItem
    QMap<int, PortItem*> m_portItems;

    QMap<int, EdgeItem*> m_edgeItems; // 存储边对象

    QMap<QString, ModuleItem*> m_moduleItems;// 存储模块对象

    int m_selectedPortA = -1;
    int m_selectedPortB = -1;
    EdgeItem* m_tempEdge = nullptr;

    QMap<QPair<int, int>, int> m_portCommStats; // 存储通信数据

    void addModuleToTree(QTreeWidgetItem *parent, const QString &name);
    void setupInitialNodes(); // 初始化 Bus、node0 - node5 节点（带图）
    void addNode(int nodeId, const QString& imgPath, const QPointF& pos,QSize size,bool keepAspectRatio = false);
    void addModule(const QString &name, const QString &imgPath, const QPointF &pos, QSize size, bool keepAspectRatio = false);
    void addPortToNode(int portId, NodeItem* node, QPointF offset);
    void addEdge(int edgeId, const QPointF &start, const QPointF &end, bool polyline = false);
    void addTableItem(int tabIndex, const QString &tableName, const QString &key, const QString &value);
    void appendLog(const QString &logText);
    void fillCpuModuleDetails(const QString &moduleName);
    void fillCacheModuleDetails(const QString &moduleName);
    void fillBusModuleDetails(const QString &moduleName);
    void fillNodeModuleDetails(const QString &moduleName);
    void fillEdgeModuleDetails(const QString &moduleName);
    void fillCacheEventTraceModuleDetails(const QString &moduleName);
    void loadPortStatistics(const QString &filePath);
};
#endif // PROCESSOR_VISUALIZER_H
