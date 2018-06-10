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
    SoftKeyBoardContext * ic = new SoftKeyBoardContext(&w);
    a.setInputContext(ic);        //就这两三句就好
    w.show();
    return a.exec();
}
```
效果： 
 
中文处理的比较简单，只能输入单字，也没有联想功能啥的。。。嵌入式也可以使用