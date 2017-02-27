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
#include <QWebEngineView>
#include <QWebEngineContextMenuData>

#include "webengine-page.h"

// ==============================
// LOCAL VIEW CLASS DEFINITION:
// ==============================
class QViewWidget : public QWebEngineView
{
    Q_OBJECT

signals:
    void initiateWindowClosingSignal();

public slots:
    // ==============================
    // Context menu:
    // ==============================
    void contextMenuEvent(QContextMenuEvent *event)
    {
        QWebEngineContextMenuData contextMenuTest =
                QWebEngineView::page()->contextMenuData();
        Q_ASSERT(contextMenuTest.isValid());

        QMenu menu;

        if (contextMenuTest.selectedText().length() > 0) {
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

        if (contextMenuTest.selectedText().length() == 0) {
            QAction *selectAllAct = menu.addAction("Select All");
            QObject::connect(selectAllAct, SIGNAL(triggered()),
                             this, SLOT(qSelectAllAction()));
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

    // ==============================
    // Script errors window:
    // ==============================
    void qDisplayScriptErrorsSlot(QString errors)
    {
        QViewWidget *errorsWindow = new QViewWidget();
        errorsWindow->setHtml(errors);
        errorsWindow->setFocus();
        errorsWindow->show();
    }

public:
    QViewWidget();
};

#endif // VIEW_H
