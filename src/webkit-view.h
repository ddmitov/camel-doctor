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
 Dimitar D. Mitov, 2014 - 2017
 Valcho Nedelchev, 2014 - 2017
 https://github.com/ddmitov/camel-doctor
*/

#ifndef VIEW_H
#define VIEW_H

#include <QContextMenuEvent>
#include <QDesktopWidget>
#include <QMenu>
#include <QWebView>

#include "webkit-page.h"

// ==============================
// LOCAL VIEW CLASS DEFINITION:
// ==============================
class QViewWidget : public QWebView
{
    Q_OBJECT

public slots:
    // ==============================
    // Context menu:
    // ==============================
    void contextMenuEvent(QContextMenuEvent *event)
    {
        QWebHitTestResult qWebHitTestResult =
                page()->mainFrame()->hitTestContent(event->pos());
        QMenu menu;

        if (qWebHitTestResult.isContentEditable()) {
            if (qWebHitTestResult.isContentSelected()) {
            QAction *cutAct = menu.addAction("Cut");
            QObject::connect(cutAct, SIGNAL(triggered()),
                             this, SLOT(qCutAction()));

            QAction *copyAct = menu.addAction("Copy");
            QObject::connect(copyAct, SIGNAL(triggered()),
                             this, SLOT(qCopyAction()));

            QAction *pasteAct = menu.addAction("Paste");
            QObject::connect(pasteAct, SIGNAL(triggered()),
                             this, SLOT(qPasteAction()));

            QAction *selectAllAct = menu.addAction("Select All");
            QObject::connect(selectAllAct, SIGNAL(triggered()),
                             this, SLOT(qSelectAllAction()));
            }

            if (!qWebHitTestResult.isContentSelected()) {
            QAction *pasteAct = menu.addAction("Paste");
            QObject::connect(pasteAct, SIGNAL(triggered()),
                             this, SLOT(qPasteAction()));

            QAction *selectAllAct = menu.addAction("Select All");
            QObject::connect(selectAllAct, SIGNAL(triggered()),
                             this, SLOT(qSelectAllAction()));
            }
        }

        if (!qWebHitTestResult.isContentEditable() and
                qWebHitTestResult.isContentSelected()) {
            QAction *copyAct = menu.addAction("Copy");
            QObject::connect(copyAct, SIGNAL(triggered()),
                             this, SLOT(qCopyAction()));
        }

        menu.exec(mapToGlobal(event->pos()));
        this->focusWidget();
    }

    void qCutAction()
    {
        QViewWidget::triggerPageAction(QWebPage::Cut);
    }

    void qCopyAction()
    {
        QViewWidget::triggerPageAction(QWebPage::Copy);
    }

    void qPasteAction()
    {
        QViewWidget::triggerPageAction(QWebPage::Paste);
    }

    void qSelectAllAction()
    {
        QViewWidget::triggerPageAction(QWebPage::SelectAll);
    }

public:
    QViewWidget();
};

#endif // VIEW_H
