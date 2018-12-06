#include "MainWindow.h"
#include <QApplication>
#include "SoftKeyBoardContext.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr( QTextCodec::codecForName("UTF-8"));
    MainWindow w;
    SoftKeyBoardContext * ic = new SoftKeyBoardContext();
    a.setInputContext(ic);
    w.show();

    return a.exec();
}
