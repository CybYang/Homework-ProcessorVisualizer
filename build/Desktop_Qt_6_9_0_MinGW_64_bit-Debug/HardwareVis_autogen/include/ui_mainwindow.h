/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpenHw;
    QAction *actionOpenPerf;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QTreeWidget *treeHardware;
    QGraphicsView *graphicsView;
    QTabWidget *tabPerformance;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *lblModuleData;
    QTableWidget *tblModuleData;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QLabel *lblPerformanceData;
    QTableWidget *tblPerformanceData;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_2;
    QTextEdit *txtLog;
    QMenuBar *menubar;
    QMenu *menu;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 800);
        actionOpenHw = new QAction(MainWindow);
        actionOpenHw->setObjectName("actionOpenHw");
        actionOpenPerf = new QAction(MainWindow);
        actionOpenPerf->setObjectName("actionOpenPerf");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Orientation::Horizontal);
        treeHardware = new QTreeWidget(splitter);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeHardware->setHeaderItem(__qtreewidgetitem);
        treeHardware->setObjectName("treeHardware");
        treeHardware->setLineWidth(1);
        splitter->addWidget(treeHardware);
        graphicsView = new QGraphicsView(splitter);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setEnabled(true);
        graphicsView->setMinimumSize(QSize(710, 730));
        splitter->addWidget(graphicsView);
        tabPerformance = new QTabWidget(splitter);
        tabPerformance->setObjectName("tabPerformance");
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        verticalLayout_3 = new QVBoxLayout(tab_3);
        verticalLayout_3->setObjectName("verticalLayout_3");
        lblModuleData = new QLabel(tab_3);
        lblModuleData->setObjectName("lblModuleData");

        verticalLayout_3->addWidget(lblModuleData);

        tblModuleData = new QTableWidget(tab_3);
        if (tblModuleData->columnCount() < 2)
            tblModuleData->setColumnCount(2);
        tblModuleData->setObjectName("tblModuleData");
        tblModuleData->setColumnCount(2);

        verticalLayout_3->addWidget(tblModuleData);

        tabPerformance->addTab(tab_3, QString());
        tab = new QWidget();
        tab->setObjectName("tab");
        verticalLayout = new QVBoxLayout(tab);
        verticalLayout->setObjectName("verticalLayout");
        lblPerformanceData = new QLabel(tab);
        lblPerformanceData->setObjectName("lblPerformanceData");

        verticalLayout->addWidget(lblPerformanceData);

        tblPerformanceData = new QTableWidget(tab);
        if (tblPerformanceData->columnCount() < 2)
            tblPerformanceData->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tblPerformanceData->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tblPerformanceData->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tblPerformanceData->setObjectName("tblPerformanceData");
        tblPerformanceData->setColumnCount(2);

        verticalLayout->addWidget(tblPerformanceData);

        tabPerformance->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        verticalLayout_2 = new QVBoxLayout(tab_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        txtLog = new QTextEdit(tab_2);
        txtLog->setObjectName("txtLog");
        txtLog->setReadOnly(true);

        verticalLayout_2->addWidget(txtLog);

        tabPerformance->addTab(tab_2, QString());
        splitter->addWidget(tabPerformance);

        horizontalLayout->addWidget(splitter);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1200, 18));
        menu = new QMenu(menubar);
        menu->setObjectName("menu");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName("toolBar");
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);

        menubar->addAction(menu->menuAction());
        menu->addAction(actionOpenHw);
        menu->addAction(actionOpenPerf);
        toolBar->addAction(actionOpenHw);
        toolBar->addAction(actionOpenPerf);

        retranslateUi(MainWindow);

        tabPerformance->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "RISC - V \345\244\204\347\220\206\345\231\250\346\200\247\350\203\275\345\217\257\350\247\206\345\214\226", nullptr));
        actionOpenHw->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\347\241\254\344\273\266\351\205\215\347\275\256", nullptr));
        actionOpenPerf->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\346\200\247\350\203\275\346\225\260\346\215\256", nullptr));
        treeHardware->setProperty("headerLabel", QVariant(QCoreApplication::translate("MainWindow", "\347\241\254\344\273\266\346\250\241\345\235\227", nullptr)));
        lblModuleData->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\346\250\241\345\235\227\346\237\245\347\234\213\346\250\241\345\235\227\346\225\260\346\215\256", nullptr));
        tabPerformance->setTabText(tabPerformance->indexOf(tab_3), QCoreApplication::translate("MainWindow", "\346\250\241\345\235\227\346\225\260\346\215\256", nullptr));
        lblPerformanceData->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\346\250\241\345\235\227\346\237\245\347\234\213\346\200\247\350\203\275\346\225\260\346\215\256", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tblPerformanceData->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "\346\214\207\346\240\207", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tblPerformanceData->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "\346\225\260\345\200\274", nullptr));
        tabPerformance->setTabText(tabPerformance->indexOf(tab), QCoreApplication::translate("MainWindow", "\346\200\247\350\203\275\347\273\237\350\256\241", nullptr));
        tabPerformance->setTabText(tabPerformance->indexOf(tab_2), QCoreApplication::translate("MainWindow", "\346\227\245\345\277\227", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
