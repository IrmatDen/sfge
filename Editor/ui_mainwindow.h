/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Wed 14. Aug 03:02:24 2013
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "properties_editor.hpp"
#include "sfge_render_widget.hpp"
#include "world_tree_widget.hpp"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionOpen;
    QAction *actionNew;
    SFGERenderWidget *sfgeRenderWidget;
    QMenuBar *menubar;
    QMenu *menu_File;
    QMenu *menu_View;
    QStatusBar *statusbar;
    QDockWidget *dockWidgetWorld;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    WorldTreeWidget *treeWidgetWorld;
    QToolBar *toolBar;
    QDockWidget *dockWidgetGODefs;
    QWidget *dockWidgetContentsGODefs;
    QVBoxLayout *verticalLayout_2;
    QListWidget *listWidgetGODefs;
    QDockWidget *dockWidgetConsole;
    QWidget *dockWidgetContents_2;
    QHBoxLayout *horizontalLayout;
    QTextEdit *textEditConsole;
    QDockWidget *dockWidgetProperties;
    QWidget *dockWidgetContents_3;
    QVBoxLayout *verticalLayout_3;
    PropertiesEditor *treeWidgetProperties;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(852, 820);
        MainWindow->setUnifiedTitleAndToolBarOnMac(false);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionQuit->setShortcutContext(Qt::ApplicationShortcut);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        sfgeRenderWidget = new SFGERenderWidget(MainWindow);
        sfgeRenderWidget->setObjectName(QString::fromUtf8("sfgeRenderWidget"));
        MainWindow->setCentralWidget(sfgeRenderWidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 852, 18));
        menu_File = new QMenu(menubar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menu_View = new QMenu(menubar);
        menu_View->setObjectName(QString::fromUtf8("menu_View"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        dockWidgetWorld = new QDockWidget(MainWindow);
        dockWidgetWorld->setObjectName(QString::fromUtf8("dockWidgetWorld"));
        dockWidgetWorld->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        treeWidgetWorld = new WorldTreeWidget(dockWidgetContents);
        new QTreeWidgetItem(treeWidgetWorld);
        treeWidgetWorld->setObjectName(QString::fromUtf8("treeWidgetWorld"));
        treeWidgetWorld->setFocusPolicy(Qt::StrongFocus);
        treeWidgetWorld->setAlternatingRowColors(false);
        treeWidgetWorld->setRootIsDecorated(true);
        treeWidgetWorld->header()->setVisible(false);

        verticalLayout->addWidget(treeWidgetWorld);

        dockWidgetWorld->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidgetWorld);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        dockWidgetGODefs = new QDockWidget(MainWindow);
        dockWidgetGODefs->setObjectName(QString::fromUtf8("dockWidgetGODefs"));
        dockWidgetGODefs->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockWidgetContentsGODefs = new QWidget();
        dockWidgetContentsGODefs->setObjectName(QString::fromUtf8("dockWidgetContentsGODefs"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContentsGODefs);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        listWidgetGODefs = new QListWidget(dockWidgetContentsGODefs);
        listWidgetGODefs->setObjectName(QString::fromUtf8("listWidgetGODefs"));

        verticalLayout_2->addWidget(listWidgetGODefs);

        dockWidgetGODefs->setWidget(dockWidgetContentsGODefs);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(4), dockWidgetGODefs);
        dockWidgetConsole = new QDockWidget(MainWindow);
        dockWidgetConsole->setObjectName(QString::fromUtf8("dockWidgetConsole"));
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        horizontalLayout = new QHBoxLayout(dockWidgetContents_2);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        textEditConsole = new QTextEdit(dockWidgetContents_2);
        textEditConsole->setObjectName(QString::fromUtf8("textEditConsole"));
        textEditConsole->setReadOnly(true);

        horizontalLayout->addWidget(textEditConsole);

        dockWidgetConsole->setWidget(dockWidgetContents_2);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidgetConsole);
        dockWidgetProperties = new QDockWidget(MainWindow);
        dockWidgetProperties->setObjectName(QString::fromUtf8("dockWidgetProperties"));
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QString::fromUtf8("dockWidgetContents_3"));
        verticalLayout_3 = new QVBoxLayout(dockWidgetContents_3);
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        treeWidgetProperties = new PropertiesEditor(dockWidgetContents_3);
        treeWidgetProperties->setObjectName(QString::fromUtf8("treeWidgetProperties"));
        treeWidgetProperties->setAlternatingRowColors(true);
        treeWidgetProperties->setColumnCount(2);
        treeWidgetProperties->header()->setVisible(true);

        verticalLayout_3->addWidget(treeWidgetProperties);

        dockWidgetProperties->setWidget(dockWidgetContents_3);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidgetProperties);

        menubar->addAction(menu_File->menuAction());
        menubar->addAction(menu_View->menuAction());
        menu_File->addAction(actionNew);
        menu_File->addAction(actionOpen);
        menu_File->addSeparator();
        menu_File->addAction(actionQuit);
        toolBar->addAction(actionNew);
        toolBar->addAction(actionOpen);
        toolBar->addSeparator();

        retranslateUi(MainWindow);
        QObject::connect(actionQuit, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "SFGE Editor - %1 [*]", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("MainWindow", "&Quit", 0, QApplication::UnicodeUTF8));
        actionQuit->setShortcut(QString());
        actionOpen->setText(QApplication::translate("MainWindow", "&Open", 0, QApplication::UnicodeUTF8));
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionNew->setText(QApplication::translate("MainWindow", "&New", 0, QApplication::UnicodeUTF8));
        actionNew->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menu_View->setTitle(QApplication::translate("MainWindow", "&View", 0, QApplication::UnicodeUTF8));
        dockWidgetWorld->setWindowTitle(QApplication::translate("MainWindow", "World view", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = treeWidgetWorld->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWindow", "1", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = treeWidgetWorld->isSortingEnabled();
        treeWidgetWorld->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidgetWorld->topLevelItem(0);
        ___qtreewidgetitem1->setText(0, QApplication::translate("MainWindow", "World", 0, QApplication::UnicodeUTF8));
        treeWidgetWorld->setSortingEnabled(__sortingEnabled);

        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
        dockWidgetGODefs->setWindowTitle(QApplication::translate("MainWindow", "Available Game object definitions", 0, QApplication::UnicodeUTF8));
        dockWidgetConsole->setWindowTitle(QApplication::translate("MainWindow", "Console", 0, QApplication::UnicodeUTF8));
        dockWidgetProperties->setWindowTitle(QApplication::translate("MainWindow", "Properties", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem2 = treeWidgetProperties->headerItem();
        ___qtreewidgetitem2->setText(1, QApplication::translate("MainWindow", "Value", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem2->setText(0, QApplication::translate("MainWindow", "Property", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
