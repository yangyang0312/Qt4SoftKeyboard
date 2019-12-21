# Qt4SoftKeyboard
SoftKeyBoardContext类继承自QInputContext，处理与输入控件的联系，更新键盘位置等 
SoftKeyBoard继承自QWidget，每一个按键都是button，但是button的focus属性都需要清除掉 
使用，将上述两个类的.cpp和.h还有SoftKeyBoard.ui加到项目中，把FontLibrary.txt加入到项目的rc中，然后在main.cpp中：
```cpp
#include "SoftKeyBoardContext.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    SoftKeyBoardContext * ic = new SoftKeyBoardContext();
    a.setInputContext(ic);        //就这两三句就好
    w.show();
    return a.exec();
}
```

如果是QDialog则需要在QDialog构造函数中加上：
```cpp
    ui->setupUi(this);
    //QDialog使用软键盘的关键设置
    setWindowModality(Qt::WindowModal);
```
 
中文处理的比较简单，只能输入单字，也没有联想功能啥的。。。嵌入式也可以使用

**注意：最近发现，新的代码在linux平台上存在异常，但是windows平台正常，使用第一版使用正常，但是不支持dialog的输入，怀疑是平台差异引起，目前暂未解决该问题，使用此代码是自己注意一下**


**注意2：因为git上传的时候，默认把字库文件FontLibrary.txt的windows换行符，换成了linux换行符，所以在加载字库文件时，可能存在问题，如果输入结果不对，请修改 bool SoftKeyBoard::InitChinese() 函数 key = str.mid(2,str.size() - 3); ，使用此代码时注意一下。**
