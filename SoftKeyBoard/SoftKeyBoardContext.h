#ifndef SOFTKEYBOARDCONTEXT_H
#define SOFTKEYBOARDCONTEXT_H

#include <QtGui/QInputContext>
#include "SoftKeyBoard.h"
#include <QDebug>

class MainWindow;

class SoftKeyBoardContext : public QInputContext
{
    Q_OBJECT

public:
    SoftKeyBoardContext();
    ~SoftKeyBoardContext();

    bool filterEvent(const QEvent* event);
    QString identifierName();
    QString language();
    bool isComposing() const;
    void reset();

private slots:
    void sendCharacter(int key);

private:
    void updatePosition();

private:
    SoftKeyBoard *keyboard_;
};

#endif 
