#include "processor_visualizer.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QFont>
#include <QDebug>
#include <QToolTip>

// NodeItem 实现（带图片）
NodeItem::NodeItem(int nodeId, const QString& imgPath, const QPointF& pos,QSize size,
                   bool keepAspectRatio, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_nodeId(nodeId)
    , m_isHighlighted(false)
    , m_position(pos)
    , m_size(size)
    , m_pixmap(imgPath)
    , m_keepAspectRatio(keepAspectRatio)  // 是否保持图片宽高比
{
    setPos(pos);
    setAcceptHoverEvents(true);

    // 计算合适的尺寸
    if (!m_pixmap.isNull() && m_keepAspectRatio) {
        // 保持图片原始宽高比，最大尺寸为 80x80
        m_size = m_pixmap.size().scaled(80, 80, Qt::KeepAspectRatio);
    } else {
        // 默认正方形尺寸
        m_size = QSize(50, 50);
    }
}

QRectF NodeItem::boundingRect() const
{
    return QRectF(0, 0, m_size.width(), m_size.height());
}

void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!m_pixmap.isNull()) {
        QSize targetSize = m_size;  // 使用自定义尺寸进行缩放
        QPixmap scaledPixmap = m_pixmap.scaled(targetSize,
                                               Qt::KeepAspectRatio,
                                               Qt::SmoothTransformation);

        // 计算绘制位置，让图片在 boundingRect 内居中
        QPointF drawPos = boundingRect().topLeft() +
                          QPointF((boundingRect().width() - scaledPixmap.width()) / 2,
                                  (boundingRect().height() - scaledPixmap.height()) / 2);

        // 计算实际图片显示区域（用于高亮边框）
        QRectF imageRect(drawPos, scaledPixmap.size());

        painter->drawPixmap(drawPos, scaledPixmap);

        // 高亮效果：基于图片实际区域绘制黄色边框
        if (m_isHighlighted) {
            painter->setPen(QPen(Qt::yellow, 3));
            painter->drawRect(imageRect); // 使用图片实际区域
        }

        qDebug() << "节点" << m_nodeId
                 << "boundingRect 高度：" << boundingRect().height()
                 << "图片高度：" << scaledPixmap.height()
                 << "drawPos Y：" << drawPos.y();
    } else {
        // 图片加载失败，画默认矩形
        painter->fillRect(boundingRect(), Qt::gray);
        painter->drawRect(boundingRect());

        // 高亮效果：基于默认矩形
        if (m_isHighlighted) {
            painter->setPen(QPen(Qt::yellow, 3));
            painter->drawRect(boundingRect());
        }
    }
}

QPainterPath NodeItem::shape() const
{
    QPainterPath path;

    if (!m_pixmap.isNull()) {
        // 使用 m_size 作为目标尺寸
        QPixmap scaledPixmap = m_pixmap.scaled(m_size,
                                               Qt::KeepAspectRatio,
                                               Qt::SmoothTransformation);

        QPointF drawPos = boundingRect().topLeft() +
                          QPointF((boundingRect().width() - scaledPixmap.width()) / 2,
                                  (boundingRect().height() - scaledPixmap.height()) / 2);

        path.addRect(QRectF(drawPos, scaledPixmap.size()));
    } else {
        path.addRect(boundingRect());
    }

    return path;
}

void NodeItem::setHighlight(bool highlight)
{
    m_isHighlighted = highlight;
    update(); // 触发重绘
}

void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit nodeClicked(m_nodeId);
    QGraphicsObject::mousePressEvent(event);
}

void NodeItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setHighlight(true);
    QGraphicsObject::hoverEnterEvent(event);
}

void NodeItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setHighlight(false);
    QGraphicsObject::hoverLeaveEvent(event);
}

//ModuleItem 实现
ModuleItem::ModuleItem(const QString &name, const QString &imgPath, const QPointF &pos,
                       QSize size, bool keepAspectRatio, QGraphicsItem *parent)
    : QGraphicsObject(parent),
    m_name(name),
    m_isHighlighted(false),
    m_position(pos),
    m_pixmap(imgPath),
    m_size(size),
    m_keepAspectRatio(keepAspectRatio)
{
    setAcceptHoverEvents(true);
    setPos(pos);
    m_pixmap.load(imgPath);
}

QRectF ModuleItem::boundingRect() const {
    return QRectF(0, 0, m_size.width(), m_size.height());
}

// paint
void ModuleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (!m_pixmap.isNull()) {
        QSize targetSize = m_size;
        QPixmap scaledPixmap = m_pixmap.scaled(targetSize,
                                               Qt::KeepAspectRatio,
                                               Qt::SmoothTransformation);

        QPointF drawPos = boundingRect().topLeft() +
                          QPointF((boundingRect().width() - scaledPixmap.width()) / 2,
                                  (boundingRect().height() - scaledPixmap.height()) / 2);

        QRectF imageRect(drawPos, scaledPixmap.size());
        painter->drawPixmap(drawPos, scaledPixmap);

        if (m_isHighlighted) {
            painter->setPen(QPen(Qt::yellow, 3));
            painter->drawRect(imageRect);
        }
    } else {
        painter->fillRect(boundingRect(), Qt::gray);
        painter->drawRect(boundingRect());

        if (m_isHighlighted) {
            painter->setPen(QPen(Qt::yellow, 3));
            painter->drawRect(boundingRect());
        }
    }
}

// shape
QPainterPath ModuleItem::shape() const
{
    QPainterPath path;
    if (!m_pixmap.isNull()) {
        QPixmap scaledPixmap = m_pixmap.scaled(m_size,
                                               Qt::KeepAspectRatio,
                                               Qt::SmoothTransformation);

        QPointF drawPos = boundingRect().topLeft() +
                          QPointF((boundingRect().width() - scaledPixmap.width()) / 2,
                                  (boundingRect().height() - scaledPixmap.height()) / 2);

        path.addRect(QRectF(drawPos, scaledPixmap.size()));
    } else {
        path.addRect(boundingRect());
    }

    return path;
}

void ModuleItem::mousePressEvent(QGraphicsSceneMouseEvent *) {
    emit moduleClicked(m_name);
}

void ModuleItem::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
    m_isHighlighted = true;
    update();
}

void ModuleItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
    m_isHighlighted = false;
    update();
}

void ModuleItem::setHighlight(bool highlight) {
    m_isHighlighted = highlight;
    update();
}

// PortItem 实现
PortItem::PortItem(int portId, NodeItem* parentNode, QPointF offset, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_portId(portId)
    , m_parentNode(parentNode)
    , m_isHighlighted(false)
{
    // 端口位置：相对于节点右侧偏移
    setPos(parentNode->pos() + offset);
    setAcceptHoverEvents(true);
}

QRectF PortItem::boundingRect() const
{
    return QRectF(-5, -5, 10, 10); // 小方块作为端口
}

void PortItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPen pen(Qt::black);
    QBrush brush(m_isHighlighted ? Qt::yellow : Qt::green);

    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(boundingRect());
}

void PortItem::setHighlight(bool highlight)
{
    m_isHighlighted = highlight;
    update();
}

void PortItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit portClicked(m_portId);  // 左键显示编号
    } else if (event->button() == Qt::RightButton) {
        emit portRightClicked(m_portId);  // 右键用于选中端口
    }
    QGraphicsObject::mousePressEvent(event); // 保留父类处理（hover 等）
}

void PortItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setHighlight(true);
    QGraphicsObject::hoverEnterEvent(event);
}

void PortItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setHighlight(false);
    QGraphicsObject::hoverLeaveEvent(event);
}

// EdgeItem 实现
EdgeItem::EdgeItem(int edgeId, const QPointF &start, const QPointF &end, bool polyline, QGraphicsItem *parent)
    : QGraphicsObject(parent), m_edgeId(edgeId), m_start(start), m_end(end), m_polyline(polyline), m_isHighlighted(false)
{
    setAcceptHoverEvents(true);
}

QRectF EdgeItem::boundingRect() const {
    return QRectF(m_start, QSizeF(m_end.x() - m_start.x(), m_end.y() - m_start.y())).normalized().adjusted(-5, -5, 5, 5);
}

void EdgeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setPen(QPen(m_isHighlighted ? Qt::yellow : Qt::black, 3));

    if (m_polyline) {
        QPainterPath path(m_start);
        path.lineTo(QPointF(m_end.x(), m_start.y()));  // 先横后竖
        path.lineTo(m_end);
        painter->drawPath(path);
    } else {
        painter->drawLine(m_start, m_end);
    }
}

QPainterPath EdgeItem::shape() const {
    QPainterPath path;
    if (m_polyline) {
        // 折线路径：先横后竖
        path.moveTo(m_start);
        path.lineTo(QPointF(m_end.x(), m_start.y()));
        path.lineTo(m_end);
    } else {
        path.moveTo(m_start);
        path.lineTo(m_end);
    }
    QPainterPathStroker stroker;
    stroker.setWidth(5); // 控制判定范围的粗细
    return stroker.createStroke(path);
}

void EdgeItem::mousePressEvent(QGraphicsSceneMouseEvent *) {
    emit edgeClicked(m_edgeId);
}

void EdgeItem::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
    m_isHighlighted = true;
    update();
}

void EdgeItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
    m_isHighlighted = false;
    update();
}

void EdgeItem::setHighlight(bool highlight) {
    m_isHighlighted = highlight;
    update();
}



ProcessorVisualizer::ProcessorVisualizer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("RISC-V Processor Visualizer");

    ui->graphicsView->setFixedSize(730, 710);

    m_scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(m_scene);

    ui->treeHardware->setHeaderHidden(true);  // 隐藏表头

    // 连接菜单动作
    connect(ui->actionOpenHw, &QAction::triggered, this, &ProcessorVisualizer::onActionOpenHardware);
    connect(ui->actionOpenPerf, &QAction::triggered, this, &ProcessorVisualizer::onActionOpenPerformance);
    connect(ui->treeHardware, &QTreeWidget::itemClicked, this, &ProcessorVisualizer::onHardwareItemClicked);
}

ProcessorVisualizer::~ProcessorVisualizer()
{
    delete ui;
}

void ProcessorVisualizer::onHardwareItemClicked(QTreeWidgetItem *item, int column)
{
    // 获取点击的模块名称
    QString moduleName = item->text(0);

    // 在状态栏显示信息
    ui->statusbar->showMessage("选中模块: " + moduleName);

    // 切换到第一个标签页
    ui->tabPerformance->setCurrentIndex(0);

    // 清空表格并设置列数
    ui->tblModuleData->setRowCount(0);
    ui->tblModuleData->setColumnCount(2);
    ui->tblModuleData->setHorizontalHeaderLabels({"属性", "值"});

    // 清空表格并设置列数
    ui->tblPerformanceData->setRowCount(0);
    ui->tblPerformanceData->setColumnCount(2);
    ui->tblPerformanceData->setHorizontalHeaderLabels({"属性", "值"});

    // 根据模块类型填充表格
    if (moduleName.contains("CPU")) {
        fillCpuModuleDetails(moduleName);
    } else if (moduleName.contains("Cache")) {
        fillCacheModuleDetails(moduleName);
    } else if (moduleName.contains("Bus")) {
        fillBusModuleDetails(moduleName);
    } else if (moduleName.contains("Memory")){
        addTableItem(0,"tblModuleData","模块名称", moduleName);
        addTableItem(0,"tblModuleData","Latency", "1");
        addTableItem(0,"tblModuleData","port_id", "4");
        addTableItem(0,"tblModuleData","data_width", "32");
        ui->tabPerformance->setCurrentIndex(1);
        addTableItem(1,"tblPerformanceData","message_precossed", "1238");
        addTableItem(1,"tblPerformanceData","busy_rate", "0.008014");
        ui->tabPerformance->setCurrentIndex(0);
    } else if(moduleName.contains("Node")){
        fillNodeModuleDetails(moduleName);
    }else if(moduleName.contains("edge")){
        fillEdgeModuleDetails(moduleName);
    }else if(moduleName.contains("cache")){
        fillCacheEventTraceModuleDetails(moduleName);
    }
    else
    {
        addTableItem(0,"tblModuleData","模块名称", moduleName);
        addTableItem(0,"tblModuleData","Latency", "1");
    }
}

// 填充CPU模块信息
void ProcessorVisualizer::fillCpuModuleDetails(const QString &moduleName)
{
    ui->tabPerformance->setCurrentIndex(0);
    addTableItem(0, "tblModuleData", "模块名称", moduleName);
    addTableItem(0, "tblModuleData", "Latency", "1");
    ui->tabPerformance->setCurrentIndex(1);

    QFile file(":/prefix/statistics/CPU_statistics.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        appendLog("无法打开 CPU 性能数据文件！");
        return;
    }

    QTextStream in(&file);
    QString currentModule;
    bool readingTarget = false;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        if (line.endsWith(':') && !line.contains(' ')) {
            // 新模块
            currentModule = line.left(line.length() - 1);
            readingTarget = (currentModule == moduleName);
            continue;
        }
        if (readingTarget && line.contains(':')) {
            QStringList parts = line.split(':');
            if (parts.size() == 2) {
                QString key = parts[0].trimmed();
                QString value = parts[1].trimmed();
                addTableItem(1, "tblPerformanceData", key, value);
            }
        } else if (readingTarget) {
            break;
        }
    }
    file.close();
    ui->tabPerformance->setCurrentIndex(0);
}

// 填充缓存模块信息
void ProcessorVisualizer::fillCacheModuleDetails(const QString &moduleName)
{
    ui->tabPerformance->setCurrentIndex(0);
    addTableItem(0, "tblModuleData", "模块名称", moduleName);
    addTableItem(0, "tblModuleData", "Latency", "1");

    QFile file(":/prefix/statistics/Cache_statistics.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        appendLog("无法打开 Cache_statistics.txt 文件");
        return;
    }
    QTextStream in(&file);
    QString line;
    bool inTargetBlock = false;
    bool isTopSection = true;

    while (!in.atEnd()) {
        line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        // 目标模块
        if (!inTargetBlock) {
            if (line == moduleName + ":") {
                inTargetBlock = true;
                isTopSection = true;
                continue;
            }
        } else {
            // 另一模块
            if (line.endsWith(":") && line != moduleName + ":") {
                break;
            }
            // 如果遇到分隔符，表示切换到页签 1
            if (line == "----------") {
                ui->tabPerformance->setCurrentIndex(1);
                isTopSection = false;
                continue;
            }
            // 添加到表格
            QStringList parts = line.split(':');
            if (parts.size() == 2) {
                QString key = parts[0].trimmed();
                QString value = parts[1].trimmed();
                if (isTopSection) {
                    addTableItem(0, "tblModuleData", key, value);
                } else {
                    addTableItem(1, "tblPerformanceData", key, value);
                }
            } else {
                appendLog("无法解析行：" + line);
            }
        }
    }
    ui->tabPerformance->setCurrentIndex(0);
}

// 填充总线模块信息
void ProcessorVisualizer::fillBusModuleDetails(const QString &moduleName)
{
    ui->tabPerformance->setCurrentIndex(0);
    addTableItem(0, "tblModuleData", "总线", moduleName);

    QFile file(":/prefix/statistics/Bus_statistics.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        appendLog("无法打开 Bus_statistics.txt 文件");
        return;
    }

    QTextStream in(&file);
    QString line;
    bool isTopSection = true;

    while (!in.atEnd()) {
        line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        if (line == "----------") {
            ui->tabPerformance->setCurrentIndex(1);
            isTopSection = false;
            continue;
        }

        QStringList parts = line.split(':');
        if (parts.size() == 2) {
            QString key = parts[0].trimmed();
            QString value = parts[1].trimmed();

            if (key == moduleName) continue;

            if (isTopSection) {
                addTableItem(0, "tblModuleData", key, value);
            } else {
                addTableItem(1, "tblPerformanceData", key, value);
            }
        } else {
            appendLog("无法解析行：" + line);
        }
    }

    ui->tabPerformance->setCurrentIndex(0);
}

void ProcessorVisualizer::fillNodeModuleDetails(const QString &moduleName)
{
    ui->tabPerformance->setCurrentIndex(0);
    addTableItem(0, "tblModuleData", "节点", moduleName);

    QFile file(":/prefix/statistics/Node_statistics.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        appendLog("无法打开 Node_statistics.txt 文件");
        return;
    }

    QTextStream in(&file);
    QString line;
    bool inTargetBlock = false;
    bool isTopSection = true;

    while (!in.atEnd()) {
        line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        if (!inTargetBlock) {
            if (line == moduleName + ":") {
                inTargetBlock = true;
                isTopSection = true;
                continue;
            }
        } else {
            if (line.endsWith(":") && line != moduleName + ":") {
                break;  // 遇到其他节点
            }

            if (line == "----------") {
                ui->tabPerformance->setCurrentIndex(1);
                isTopSection = false;
                continue;
            }

            QStringList parts = line.split(':');
            if (parts.size() == 2) {
                QString key = parts[0].trimmed();
                QString value = parts[1].trimmed();
                if (isTopSection) {
                    addTableItem(0, "tblModuleData", key, value);
                } else {
                    addTableItem(1, "tblPerformanceData", key, value);
                }
            } else {
                appendLog("无法解析行：" + line);
            }
        }
    }
    ui->tabPerformance->setCurrentIndex(0);
}

void ProcessorVisualizer::fillEdgeModuleDetails(const QString &moduleName)
{
    ui->tabPerformance->setCurrentIndex(0);
    QFile file(":/prefix/statistics/Edge_statistics.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        appendLog("无法打开 Edge_statistics.txt 文件");
        return;
    }

    QTextStream in(&file);
    QString line;
    bool inTargetBlock = false;
    bool isTopSection = true;

    while (!in.atEnd()) {
        line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        if (!inTargetBlock) {
            if (line == moduleName + ":") {
                inTargetBlock = true;
                isTopSection = true;
                continue;
            }
        } else {
            if (line.endsWith(":") && line != moduleName + ":") {
                break;  // 遇到下一个模块，退出
            }

            if (line == "----------") {
                ui->tabPerformance->setCurrentIndex(1);
                isTopSection = false;
                continue;
            }

            // 特殊处理第一行：模块名 + 边描述
            if (isTopSection && line.startsWith(moduleName + ":")) {
                QString desc = line.section(':', 1).trimmed();
                if (!desc.isEmpty()) {
                    addTableItem(0, "tblModuleData", moduleName, desc);
                }
                continue;
            }
            QStringList parts = line.split(':');
            if (parts.size() == 2) {
                QString key = parts[0].trimmed();
                QString value = parts[1].trimmed();
                if (isTopSection) {
                    addTableItem(0, "tblModuleData", key, value);
                } else {
                    addTableItem(1, "tblPerformanceData", key, value);
                }
            } else {
                appendLog("无法解析行：" + line);
            }
        }
    }
    ui->tabPerformance->setCurrentIndex(0);
}

void ProcessorVisualizer::fillCacheEventTraceModuleDetails(const QString &moduleName)
{
    addTableItem(0,"tblModuleData","模块名称", moduleName);
    addTableItem(0,"tblModuleData","Latency", "1");
    ui->tabPerformance->setCurrentIndex(1);  // 只放在第二页签
    QFile file(":/prefix/statistics/cache_event_trace_statistics.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        appendLog("无法打开 cache_event_trace_statistics.txt 文件");
        return;
    }

    QTextStream in(&file);
    QString line;
    while (!in.atEnd()) {
        line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(':');
        if (parts.size() == 2) {
            QString key = parts[0].trimmed();
            QString value = parts[1].trimmed();
            if(key==moduleName) continue;
            addTableItem(1, "tblPerformanceData", key, value);
        } else {
            appendLog("无法解析行：" + line);
        }
    }

    ui->tabPerformance->setCurrentIndex(0);  // 恢复默认页签
}

// 辅助函数：向表格添加一行数据
void ProcessorVisualizer::addTableItem(int tabIndex, const QString &tableName, const QString &key, const QString &value)
{
    QTabWidget *tabWidget = ui->tabPerformance;
    if (tabIndex >= 0 && tabIndex < tabWidget->count()) {
        QWidget *tabPage = tabWidget->widget(tabIndex);
        QTableWidget *table = tabPage->findChild<QTableWidget*>(tableName);
        if (table) {
            int row = table->rowCount();
            table->insertRow(row);
            table->setItem(row, 0, new QTableWidgetItem(key));
            table->setItem(row, 1, new QTableWidgetItem(value));
        }
    }
}

void ProcessorVisualizer::setupInitialNodes()
{
    m_scene->setSceneRect(0, 0, 730, 710);
    QGraphicsRectItem* border = new QGraphicsRectItem(m_scene->sceneRect());
    border->setPen(QPen(Qt::black, 5));
    m_scene->addItem(border);

    addNode(-1, ":/prefix/images/bus.png", QPointF(319, 319), QSize(80, 80),true);

    addNode(0, ":/prefix/images/node0.png", QPointF(338, 157), QSize(50, 50),false);
    addPortToNode(1, m_nodeItems[0], QPointF(10, -5)); // 2 个端口，间距 20
    addPortToNode(0, m_nodeItems[0], QPointF(30, -5));

    addNode(1, ":/prefix/images/node1.png", QPointF(171, 235), QSize(50, 50),false);
    addPortToNode(2, m_nodeItems[1], QPointF(-5, 10)); // 2 个端口，间距 20
    addPortToNode(3, m_nodeItems[1], QPointF(-5, 30));

    addNode(2, ":/prefix/images/node2.png", QPointF(172, 404), QSize(50, 50),false);
    addPortToNode(4, m_nodeItems[2], QPointF(-5, 10)); // 1 个端口

    addNode(3, ":/prefix/images/node3.png", QPointF(341, 479), QSize(50, 50),false);
    addPortToNode(5, m_nodeItems[3], QPointF(10, 54)); // 2 个端口，间距 20
    addPortToNode(6, m_nodeItems[3], QPointF(30, 54));

    addNode(4, ":/prefix/images/node4.png", QPointF(512, 405), QSize(50, 50),false);
    addPortToNode(8, m_nodeItems[4], QPointF(55, 10)); // 2 个端口，间距 20
    addPortToNode(7, m_nodeItems[4], QPointF(55, 30));

    addNode(5, ":/prefix/images/node5.png", QPointF(511, 237), QSize(50, 50),false);
    addPortToNode(9, m_nodeItems[5], QPointF(55, 10)); // 1 个端口

    qDebug()<<m_nodeItems[0]->pos();
    addEdge(1, QPointF(338,171), QPointF(182,234), true);  // 0->1 折线
    addEdge(2, QPointF(338,186), QPointF(202,234), true);  // 1->0 折线
    addEdge(3, QPointF(182,286), QPointF(182,404), false); // 1->2 直线
    addEdge(4, QPointF(202,286), QPointF(202,404), false); // 2->1 直线
    addEdge(5, QPointF(340,513), QPointF(182,454), true);  // 2->3 折线
    addEdge(6, QPointF(340,492), QPointF(202,454), true);  // 3->2 折线
    addEdge(7, QPointF(392,513), QPointF(547,455), true);  // 3->4 折线
    addEdge(8, QPointF(392,492), QPointF(527,455), true);  // 4->3 折线
    addEdge(9, QPointF(547,405), QPointF(547,286), false); // 4->5 直线
    addEdge(10,QPointF(527,405), QPointF(527,286), false);  // 5->4 直线
    addEdge(11,QPointF(389,171), QPointF(547,236), true);  // 5->0 折线
    addEdge(12,QPointF(389,191), QPointF(527,236), true);  // 0->5 折线

    addModule("cache_event_trace",":/prefix/images/cache_event.png",QPointF(628,17),QSize(75,75),false);
    addModule("DMA",":/prefix/images/DMA.png",QPointF(629,83),QSize(70,70),false);
    addModule("L3Cache0",":/prefix/images/L3Cache0.png",QPointF(199,43),QSize(70,70),false);
    addModule("L2Cache0",":/prefix/images/L2Cache0.png",QPointF(396,41),QSize(75,75),false);
    addModule("CPU0",":/prefix/images/CPU0.png",QPointF(471,41),QSize(75,75),false);
    addModule("L3Cache1",":/prefix/images/L3Cache1.png",QPointF(30,271),QSize(70,70),false);
    addModule("L2Cache1",":/prefix/images/L2Cache1.png",QPointF(30,166),QSize(70,70),false);
    addModule("CPU1",":/prefix/images/CPU1.png",QPointF(30,131),QSize(70,70),false);
    addModule("MemoryNode0",":/prefix/images/memory_node0.png",QPointF(54,445),QSize(80,80),false);
    addModule("L3Cache2",":/prefix/images/L3Cache2.png",QPointF(437,570),QSize(70,70),false);
    addModule("L2Cache2",":/prefix/images/L2Cache2.png",QPointF(232,570),QSize(70,70),false);
    addModule("CPU2",":/prefix/images/CPU2.png",QPointF(167,574),QSize(65,65),true);
    addModule("L3Cache3",":/prefix/images/L3Cache3.png",QPointF(620,327),QSize(70,70),false);
    addModule("L2Cache3",":/prefix/images/L2Cache3.png",QPointF(620,446),QSize(70,70),false);
    addModule("CPU3",":/prefix/images/CPU3.png",QPointF(620,489),QSize(70,70),true);

    addEdge(13,QPointF(271,78), QPointF(346,146), true);  // L3Cache0 折线
    addEdge(14,QPointF(375,152), QPointF(434,102), true);  // L2Cache0 折线
    addEdge(15,QPointF(160,264), QPointF(65,287), true);  // L3Cache1 折线
    addEdge(16,QPointF(101,201), QPointF(166,238), true);  // L2Cache1 折线
    addEdge(17,QPointF(161,414), QPointF(94,464), true);  // MemoryNode0 折线
    addEdge(18,QPointF(436,605), QPointF(371,539), true);  // L3Cache2 折线
    addEdge(19,QPointF(345,534), QPointF(267,584), true);  // L2Cache2 折线
    addEdge(20,QPointF(618,362), QPointF(567,409), true);  // L3Cache3 折线
    addEdge(21,QPointF(573,435), QPointF(655,461), true);  // L2Cache3 折线
}

void ProcessorVisualizer::addNode(int nodeId, const QString& imgPath,
                                  const QPointF& pos, QSize size,bool keepAspectRatio)
{
    NodeItem* node = new NodeItem(nodeId, imgPath, pos,size,keepAspectRatio);
    m_scene->addItem(node);
    m_nodeItems[nodeId] = node;

    connect(node, &NodeItem::nodeClicked, this, &ProcessorVisualizer::onNodeClickedHandler);
}

void ProcessorVisualizer::addModule(const QString &name, const QString &imgPath, const QPointF &pos,
                                    QSize size, bool keepAspectRatio) {
    auto *module = new ModuleItem(name, imgPath, pos, size, keepAspectRatio);
    m_scene->addItem(module);
    m_moduleItems[name] = module;
    connect(module, &ModuleItem::moduleClicked, this, &ProcessorVisualizer::onModuleClickedHandler);
}

void ProcessorVisualizer::addPortToNode(int portId, NodeItem* node, QPointF offset)
{
    PortItem* port = new PortItem(portId, node, offset);
    m_scene->addItem(port);
    m_portItems[portId] = port;

    connect(port, &PortItem::portClicked, this, &ProcessorVisualizer::onPortLeftClickedHandler);
    connect(port, &PortItem::portRightClicked, this, &ProcessorVisualizer::onPortRightClickedHandler);
}

void ProcessorVisualizer::addEdge(int edgeId, const QPointF &start, const QPointF &end, bool polyline)
{
    EdgeItem *edge = new EdgeItem(edgeId, start, end, polyline);
    m_scene->addItem(edge);
    m_edgeItems[edgeId] = edge;

    connect(edge, &EdgeItem::edgeClicked, this, &ProcessorVisualizer::onEdgeClickedHandler);
}

void ProcessorVisualizer::addModuleToTree(QTreeWidgetItem *parent, const QString &name)
{
    QTreeWidgetItem *item = new QTreeWidgetItem({name});
    parent->addChild(item);
}

void ProcessorVisualizer::onActionOpenHardware()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open Hardware File", "", "Hardware Files (*.txt)");
    if (!filePath.isEmpty()) {
        // 先清空树部件现有内容
        ui->treeHardware->clear();

        // 创建 “硬件模块” 顶层节点
        QTreeWidgetItem *hardwareRoot = new QTreeWidgetItem({"硬件模块"});
        ui->treeHardware->addTopLevelItem(hardwareRoot);
        hardwareRoot->setExpanded(true); // 默认展开

        // 向 “硬件模块” 添加子模块
        addModuleToTree(hardwareRoot, "cache_event_trace");
        addModuleToTree(hardwareRoot, "MemoryNode0");
        addModuleToTree(hardwareRoot, "DMA");
        addModuleToTree(hardwareRoot, "CPU0");
        addModuleToTree(hardwareRoot, "CPU1");
        addModuleToTree(hardwareRoot, "CPU2");
        addModuleToTree(hardwareRoot, "CPU3");
        addModuleToTree(hardwareRoot, "L3Cache0");
        addModuleToTree(hardwareRoot, "L3Cache1");
        addModuleToTree(hardwareRoot, "L3Cache2");
        addModuleToTree(hardwareRoot, "L3Cache3");
        addModuleToTree(hardwareRoot, "L2Cache0");
        addModuleToTree(hardwareRoot, "L2Cache1");
        addModuleToTree(hardwareRoot, "L2Cache2");
        addModuleToTree(hardwareRoot, "L2Cache3");

        // 创建 “节点总线” 顶层节点
        QTreeWidgetItem *busRoot = new QTreeWidgetItem({"节点总线"});
        ui->treeHardware->addTopLevelItem(busRoot);
        busRoot->setExpanded(true); // 默认展开

        // 向 “节点总线” 添加子模块
        addModuleToTree(busRoot, "Bus");
        addModuleToTree(busRoot, "Node0");
        addModuleToTree(busRoot, "Node1");
        addModuleToTree(busRoot, "Node2");
        addModuleToTree(busRoot, "Node3");
        addModuleToTree(busRoot, "Node4");
        addModuleToTree(busRoot, "Node5");
        addModuleToTree(busRoot, "edge1");
        addModuleToTree(busRoot, "edge2");
        addModuleToTree(busRoot, "edge3");
        addModuleToTree(busRoot, "edge4");
        addModuleToTree(busRoot, "edge5");
        addModuleToTree(busRoot, "edge6");
        addModuleToTree(busRoot, "edge7");
        addModuleToTree(busRoot, "edge8");
        addModuleToTree(busRoot, "edge9");
        addModuleToTree(busRoot, "edge10");
        addModuleToTree(busRoot, "edge11");
        addModuleToTree(busRoot, "edge12");

        ui->statusbar->showMessage("硬件配置已加载");
        appendLog(QString("Opened hardware file: %1").arg(filePath));
        // 后续可扩展解析逻辑
    }
    setupInitialNodes();
}

void ProcessorVisualizer::onActionOpenPerformance()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open Performance File", "", "Performance Files (*.txt)");
    if (!filePath.isEmpty()) {
        ui->statusbar->showMessage("性能数据已加载");
        appendLog(QString("Opened performance file: %1").arg(filePath));
        loadPortStatistics(filePath);
    }
}

void ProcessorVisualizer::loadPortStatistics(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "无法打开文件:" << filePath;
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.startsWith("transmit_package_number_from_")) {
            // 解析格式：transmit_package_number_from_X_to_Y: value
            QRegularExpression re("transmit_package_number_from_(\\d+)_to_(\\d+):\\s*(\\d+)");
            QRegularExpressionMatch match = re.match(line);
            if (match.hasMatch()) {
                int from = match.captured(1).toInt();
                int to = match.captured(2).toInt();
                int value = match.captured(3).toInt();
                m_portCommStats[{from, to}] = value;
            }
        }
    }
    file.close();
}

void ProcessorVisualizer::onNodeClickedHandler(int nodeId)
{
    QString info = QString("Node %1 clicked").arg(nodeId);
    ui->statusbar->showMessage(info);

    // 仅当前节点高亮
    for (auto& node : m_nodeItems) {
        node->setHighlight(node->nodeId() == nodeId);
    }
    QTreeWidget *tree = ui->treeHardware;
    QTreeWidgetItemIterator it(tree);
    QString targetName = QString("Node%1").arg(nodeId);
    if(nodeId<0) targetName=QString("Bus");
    while (*it) {
        if ((*it)->text(0) == targetName) {
            tree->setCurrentItem(*it);
            onHardwareItemClicked(*it, 0); // 主调用填充信息逻辑
            break;
        }
        ++it;
    }
}

void ProcessorVisualizer::onModuleClickedHandler(const QString &name) {
    ui->statusbar->showMessage(QString("Module %1 clicked").arg(name));

    for (auto &module : m_moduleItems) {
        module->setHighlight(module->moduleName() == name);
    }

    QTreeWidget *tree = ui->treeHardware;
    QTreeWidgetItemIterator it(tree);
    while (*it) {
        if ((*it)->text(0) == name) {
            tree->setCurrentItem(*it);
            onHardwareItemClicked(*it, 0);
            break;
        }
        ++it;
    }
}

void ProcessorVisualizer::onPortLeftClickedHandler(int portId)
{
    QString info = QString("Port %1 clicked").arg(portId);
    ui->statusbar->showMessage(info);
}

void ProcessorVisualizer::onPortRightClickedHandler(int portId)
{
    QString info = QString("Port %1 clicked").arg(portId);
    ui->statusbar->showMessage(info);

    if (m_portItems.contains(portId)) {
        m_portItems[portId]->setHighlight(true);
    }
    if (m_selectedPortA == -1) {
        m_selectedPortA = portId;
        appendLog(QString("已选中源端口 %1").arg(portId));
        return;
    }

    if (m_selectedPortB == -1) {
        if (portId == m_selectedPortA) return; // 不允许两次同一端口

        m_selectedPortB = portId;
        appendLog(QString("已选中目标端口 %1").arg(portId));

        QPair<int, int> key(m_selectedPortA, m_selectedPortB);
        if (m_portCommStats.contains(key)) {
            int value = m_portCommStats[key];
            appendLog(QString("从端口 %1 到 %2 传输包数量为：%3")
                          .arg(m_selectedPortA).arg(m_selectedPortB).arg(value));

            // 连线
            QPointF posA = m_portItems[m_selectedPortA]->sceneBoundingRect().center();
            QPointF posB = m_portItems[m_selectedPortB]->sceneBoundingRect().center();

            if (m_tempEdge) {
                m_scene->removeItem(m_tempEdge);
                delete m_tempEdge;
            }
            m_tempEdge = new EdgeItem(-1, posA, posB, false); // 不需要 polyline
            m_tempEdge->setZValue(0); // 保证在后面
            m_scene->addItem(m_tempEdge);

        } else {
            appendLog("未找到这两个端口之间的通信数据");
        }
    } else {
        // 第三个点击：重置状态
        m_selectedPortA = portId;
        m_selectedPortB = -1;

        appendLog(QString("重新选择源端口 %1").arg(portId));

        if (m_tempEdge) {
            m_scene->removeItem(m_tempEdge);
            delete m_tempEdge;
            m_tempEdge = nullptr;
        }
    }
}

void ProcessorVisualizer::onEdgeClickedHandler(int edgeId)
{
    QString info = QString("Edge %1 clicked").arg(edgeId);
    ui->statusbar->showMessage(info);

    for (auto &edge : m_edgeItems) {
        edge->setHighlight(false);
    }
    if (m_edgeItems.contains(edgeId)) {
        m_edgeItems[edgeId]->setHighlight(true);
    }

    QString edgeName = QString("edge%1").arg(edgeId);
    QTreeWidget *tree = ui->treeHardware;
    QTreeWidgetItemIterator it(tree);
    while (*it) {
        if ((*it)->text(0) == edgeName) {
            tree->setCurrentItem(*it);
            onHardwareItemClicked(*it, 0);
            break;
        }
        ++it;
    }
}

void ProcessorVisualizer::appendLog(const QString &logText)
{
    ui->txtLog->append(logText);
}
