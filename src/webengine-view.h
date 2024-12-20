/*
Camel Doctor

This program is free software;
you can redistribute it and/or modify it under the terms of the
GNU Lesser General Public License,
as published by the Free Software Foundation;
either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.

Dimitar D. Mitov, 2014 - 2017, 2024
Valcho Nedelchev, 2014 - 2017
https://github.com/ddmitov/camel-doctor
*/

#ifndef VIEW_H
#define VIEW_H

#include <QContextMenuEvent>
#include <QDesktopWidget>
#include <QMenu>
#include <QWebEngineView>
#include <QWebEngineContextMenuData>


class QViewWidget : public QWebEngineView
{
    Q_OBJECT

signals:

    void initiateWindowClosingSignal();

public slots:

    // Context menu:
    void contextMenuEvent(QContextMenuEvent *event)
    {
        QWebEngineContextMenuData contextMenuTest =
            QWebEngineView::page()->contextMenuData();

        Q_ASSERT(contextMenuTest.isValid());

        QMenu menu;

        if (contextMenuTest.isContentEditable()) {
            QAction *pasteAct = menu.addAction("Paste");

            QObject::connect(
                pasteAct,
                SIGNAL(triggered()),
                this,
                SLOT(qPasteAction())
            );

            QAction *selectAllAct = menu.addAction("Select All");

            QObject::connect(
                selectAllAct,
                SIGNAL(triggered()),
                this,
                SLOT(qSelectAllAction())
            );

            if (contextMenuTest.selectedText().length() > 0) {
                QAction *copyAct = menu.addAction("Copy");

                QObject::connect(
                    copyAct,
                    SIGNAL(triggered()),
                    this,
                    SLOT(qCopyAction())
                );

                QAction *cutAct = menu.addAction("Cut");

                QObject::connect(
                    cutAct,
                    SIGNAL(triggered()),
                    this,
                    SLOT(qCutAction())
                );
            }
        }

        if (!contextMenuTest.isContentEditable()) {
            if (contextMenuTest.selectedText().length() > 0) {
                QAction *copyAct = menu.addAction("Copy");

                QObject::connect(
                    copyAct,
                    SIGNAL(triggered()),
                    this,
                    SLOT(qCopyAction())
                );
            }
        }

        menu.exec(mapToGlobal(event->pos()));
        this->focusWidget();
    }

    void qCutAction()
    {
        QViewWidget::triggerPageAction(QWebEnginePage::Cut);
    }

    void qCopyAction()
    {
        QViewWidget::triggerPageAction(QWebEnginePage::Copy);
    }

    void qPasteAction()
    {
        QViewWidget::triggerPageAction(QWebEnginePage::Paste);
    }

    void qSelectAllAction()
    {
        QViewWidget::triggerPageAction(QWebEnginePage::SelectAll);
    }

public:

    QViewWidget();
};

#endif // VIEW_H
