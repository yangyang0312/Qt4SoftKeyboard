#include <QtCore>
#include "SoftKeyBoardContext.h"

SoftKeyBoardContext::SoftKeyBoardContext()
{
    keyboard_ = new SoftKeyBoard();
    connect(keyboard_, SIGNAL(characterGenerated(int)), SLOT(sendCharacter(int)));
}

SoftKeyBoardContext::~SoftKeyBoardContext()
{
}

bool SoftKeyBoardContext::filterEvent(const QEvent* event)
{
    if (event->type() == QEvent::RequestSoftwareInputPanel)
    {
        updatePosition();
        keyboard_->show();
        return true;
    }
    else if (event->type() == QEvent::CloseSoftwareInputPanel)
    {
        keyboard_->hide();
        return true;
    }
    return false;
}

QString SoftKeyBoardContext::identifierName()
{
    return "SoftKeyBoardContext";
}

void SoftKeyBoardContext::reset()
{

}

bool SoftKeyBoardContext::isComposing() const
{
    return false;
}

QString SoftKeyBoardContext::language()
{
    return "en_US";
}

void SoftKeyBoardContext::sendCharacter(int key)
{
    QPointer<QWidget> w = focusWidget();
    if (w)
    {
        QKeyEvent keyPress(QEvent::KeyPress, key, Qt::NoModifier, QString(key));
        QApplication::sendEvent(w, &keyPress);
    }
}

void SoftKeyBoardContext::updatePosition()
{
    QWidget *widget = focusWidget();
    if (!widget)
        return;
    keyboard_->move(widget->mapToGlobal(QPoint(widget->rect().left(), widget->rect().bottom())));
}
