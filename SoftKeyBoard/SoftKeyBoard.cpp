#include <QStyleOption>
#include <QPainter>
#include <QLineEdit>
#include <QFile>
#include <QDateTime>
#include "SoftKeyBoard.h"

SoftKeyBoard::SoftKeyBoard(QWidget * parent)
    : QWidget(parent, /*Qt::Tool|*/ Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint),m_lastFocusedWidget_(0)
{
    ui.setupUi(this);
    hide();
    m_bIsChinese_ = true;
    m_bIsUppercase_ = false;
    m_bShiftPressed_ = false;
    m_currentPos_ = 0;
    m_currentPage_ = 0;
    ui.btn_caplock_->setStyleSheet(QString("#btn_caplock_{color: rgb(255,255,255);background-color:rgb(67,63,59);border-radius: 7px;}#btn_caplock_:pressed{background-color: rgb(33,30,27);}"));
    ui.btn_shift_->setStyleSheet(QString("#btn_shift_{color: rgb(255,255,255);background-color:rgb(67,63,59);border-radius: 7px;}#btn_shift_:pressed{background-color: rgb(33,30,27);}"));
    on_btn_language__clicked();
    m_btns_[0] = ui.char_a_;
    m_btns_[1] = ui.char_b_;
    m_btns_[2] = ui.char_c_;
    m_btns_[3] = ui.char_d_;
    m_btns_[4] = ui.char_e_;
    m_btns_[5] = ui.char_f_;
    m_btns_[6] = ui.char_g_;
    m_btns_[7] = ui.char_h_;
    m_btns_[8] = ui.char_i_;
    m_btns_[9] = ui.char_j_;
    m_btns_[10] = ui.char_k_;
    m_btns_[11] = ui.char_l_;
    m_btns_[12] = ui.char_m_;
    m_btns_[13] = ui.char_n_;
    m_btns_[14] = ui.char_o_;
    m_btns_[15] = ui.char_p_;
    m_btns_[16] = ui.char_q_;
    m_btns_[17] = ui.char_r_;
    m_btns_[18] = ui.char_s_;
    m_btns_[19] = ui.char_t_;
    m_btns_[20] = ui.char_u_;
    m_btns_[21] = ui.char_v_;
    m_btns_[22] = ui.char_w_;
    m_btns_[23] = ui.char_x_;
    m_btns_[24] = ui.char_y_;
    m_btns_[25] = ui.char_z_;            //字母
    m_btns_[26] = ui.num_0_;
    m_btns_[27] = ui.num_1_;
    m_btns_[28] = ui.num_2_;
    m_btns_[29] = ui.num_3_;
    m_btns_[30] = ui.num_4_;
    m_btns_[31] = ui.num_5_;
    m_btns_[32] = ui.num_6_;
    m_btns_[33] = ui.num_8_;
    m_btns_[34] = ui.num_9_;             //数字
    m_btns_[35] = ui.btn_left_bracket_;  //左括号
    m_btns_[36] = ui.btn_right_bracket_; //右括号
    m_btns_[37] = ui.btn_slash_;         //斜杠
    m_btns_[38] = ui.btn_quota_;         //引号
    m_btns_[39] = ui.btn_semicolon_;     //分号
    m_btns_[40] = ui.btn_backslash_;     //反斜杠
    m_btns_[41] = ui.btn_comma_;         //逗号
    m_btns_[42] = ui.btn_dot_;           //点
    m_btns_[43] = ui.btn_add_;           //加
    m_btns_[44] = ui.btn_dec_;           //减
    m_btns_[45] = ui.num_7_;             //7特殊处理，因为包含&
    m_btns_[46] = ui.word_0_;
    m_btns_[47] = ui.word_1_;
    m_btns_[48] = ui.word_2_;
    m_btns_[49] = ui.word_3_;
    m_btns_[50] = ui.word_4_;
    m_btns_[51] = ui.word_5_;
    m_btns_[52] = ui.word_6_;
    m_btns_[53] = ui.word_7_;
    m_btns_[54] = ui.word_8_;
    m_btns_[55] = ui.word_9_;
    m_btns_[56] = ui.word_10_;
    m_btns_[57] = ui.word_11_;           //汉字
    m_btns_[58] = ui.btn_up_;
    m_btns_[59] = ui.btn_down_;
    m_btns_[60] = ui.btn_left_;
    m_btns_[61] = ui.btn_right_;         //上下左右
    m_btns_[62] = ui.btn_enter_;         //回车
    m_btns_[63] = ui.btn_backspace_;     //退格
    m_btns_[64] = ui.btn_space_;         //空格
    for(int i = 0;i < 12;++i)
        m_btns_[46 + i]->setStyleSheet(QString("#word_") + QString::number(i) + "_{color: rgb(255,255,255);background-color: rgb(67,63,59);border-radius: 7px;}#word_" + QString::number(i) + "_:pressed{background-color: rgb(33,30,27);}");
    for(int i = 0;i < 65;++i)
    {
        m_signalMapper_.setMapping(m_btns_[i],m_btns_[i]);
        connect(m_btns_[i],SIGNAL(clicked()),&m_signalMapper_,SLOT(map()));
    }
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),this, SLOT(saveFocusWidget(QWidget*,QWidget*)));
    connect(&m_signalMapper_, SIGNAL(mapped(QWidget*)),this, SLOT(buttonClicked(QWidget*)));
}

bool SoftKeyBoard::event(QEvent *e)
{
    switch (e->type())
    {
    case QEvent::WindowActivate:
        if (m_lastFocusedWidget_)
            m_lastFocusedWidget_->activateWindow();
        break;
    default:
        break;
    }
    return QWidget::event(e);
}

void SoftKeyBoard::saveFocusWidget(QWidget * /*oldFocus*/, QWidget *newFocus)
{
    if (newFocus != 0 && !this->isAncestorOf(newFocus))
        m_lastFocusedWidget_ = newFocus;
}

void SoftKeyBoard::buttonClicked(QWidget *w)
{
    if(!w)
        return;
    QString text;
    int key = -1;
    if(!m_bIsChinese_)                //英文
    {
        if(GetBtnText(w,text,0,26) && text.size() > 0)//英文 字母
            key = m_bShiftPressed_ ? (text[0] >= 'A' && text[0] <= 'Z' ? text[0].toLower().unicode() : text[0].toUpper().unicode()) : text[0].unicode();//按下shift
        else if(m_bShiftPressed_ && GetBtnText(w,text,26,20) && text.size() > 0)
            key = text[0].unicode();
        else if(!m_bShiftPressed_ && GetBtnText(w,text,26,19) && text.size() > 0)
            key = text[2].unicode();
        else if(!m_bShiftPressed_ && GetBtnText(w,text,45,1) && text.size() > 0)
            key = text[3].unicode();
        else if(w == m_btns_[58])
            key = Qt::Key_Up;
        else if(w == m_btns_[59])
            key = Qt::Key_Down;
        else if(w == m_btns_[60])
            key = Qt::Key_Left;
        else if(w == m_btns_[61])
            key = Qt::Key_Right;
        else if(w == m_btns_[62])
            key = Qt::Key_Enter;
        else if(w == m_btns_[63])
            key = Qt::Key_Backspace;
        else if(w == m_btns_[64])
            key = Qt::Key_Space;
        qDebug()<<__PRETTY_FUNCTION__<<text<<key;
        emit characterGenerated(key);
    }
    else                              //中文
    {
        if(GetBtnText(w,text,0,26) && text.size() > 0)//字母
            ShowChinese(m_strPingying_ + text[0].toLower(),0,0);
        else if(GetBtnText(w,text,46,12) && text.size() > 0) //选中文字
        {
            key = text[0].unicode();
            ShowChinese("",0,0);
        }
        else if(m_bShiftPressed_ && GetBtnText(w,text,26,20) && text.size() > 0)
            key = text[0].unicode();
        else if(!m_bShiftPressed_ && GetBtnText(w,text,26,19) && text.size() > 0)
            key = text[2].unicode();
        else if(!m_bShiftPressed_ && GetBtnText(w,text,45,1) && text.size() > 0)
            key = text[3].unicode();
        if(!m_strPingying_.isEmpty())   //有拼音就操作拼音
        {
            if(w == m_btns_[58])        //上一页
                on_btn_last__clicked();
            else if(w == m_btns_[59])   //下一页
                on_btn_next__clicked();
            else if(w == m_btns_[60] && m_currentPos_ > 0)   //上一个，选中
                ShowChinese(m_strPingying_,m_currentPage_,m_currentPos_ - 1);
            else if(w == m_btns_[61] && m_currentPos_ < 11)   //下一个，选中
                ShowChinese(m_strPingying_,m_currentPage_,m_currentPos_ + 1);
            else if((w == m_btns_[62] || w == m_btns_[64]) && m_currentPos_ >= 0 && m_currentPos_ <= 11)   //enter、空格输入
            {
                text = m_btns_[m_currentPos_ + 46]->text();
                if(text.size() > 0)
                    key = text[0].unicode();
                ShowChinese("",0,0);
            }
            else if(w == m_btns_[63])   //删除一个拼音
                ShowChinese(m_strPingying_.left(m_strPingying_.size() - 1),0,0);
        }
        else                            //没有拼音就直接输入
        {
            if(w == m_btns_[58])
                key = Qt::Key_Up;
            else if(w == m_btns_[59])
                key = Qt::Key_Down;
            else if(w == m_btns_[60])
                key = Qt::Key_Left;
            else if(w == m_btns_[61])
                key = Qt::Key_Right;
            else if(w == m_btns_[62])
                key = Qt::Key_Enter;
            else if(w == m_btns_[63])
                key = Qt::Key_Backspace;
            else if(w == m_btns_[64])
                key = Qt::Key_Space;
        }
        if(-1 != key)
            emit characterGenerated(key);
        qDebug()<<__PRETTY_FUNCTION__<<text<<key;
    }
}

void SoftKeyBoard::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_ptPress_ = event->globalPos();        //记录按下位置
        m_bLeftBtnPress_ = true;
    }
    event->ignore();
}

void SoftKeyBoard::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bLeftBtnPress_)                        //移动窗口
    {
        move(pos() + event->globalPos() - m_ptPress_);
        m_ptPress_ = event->globalPos();
    }
    event->ignore();
}

void SoftKeyBoard::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        m_bLeftBtnPress_ = false;
    event->ignore();
}

void SoftKeyBoard::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void SoftKeyBoard::on_btn_language__clicked()
{
    static bool bFirstInit = true;
    if(m_bIsUppercase_)
        on_btn_caplock__clicked();
    m_bIsChinese_ = !m_bIsChinese_;
    if( bFirstInit && m_bIsChinese_ && !InitChinese())
        qDebug()<<__PRETTY_FUNCTION__<<"open chinese.txt failed.";
    ui.btn_language_->setText(m_bIsChinese_ ? "中文/英文" : "英文/中文");
    m_bIsChinese_  ? ui.widget_chinese_->show() : ui.widget_chinese_->hide();
    ui.widget_keyboard_->move(0,m_bIsChinese_ ? 100 : 2);
    resize(1188,m_bIsChinese_ ? 520 : 420);
}

void SoftKeyBoard::on_btn_caplock__clicked()
{
    m_bIsUppercase_ = !m_bIsUppercase_;
    ui.btn_caplock_->setStyleSheet(QString("#btn_caplock_{color: rgb(255,255,255);background-color:") + (m_bIsUppercase_ ? "rgb(127,122,117)" : "rgb(67,63,59)") + ";border-radius: 7px;}#btn_caplock_:pressed{background-color: rgb(33,30,27);}");
    for(int i = 0;i < 26;++i)
        m_btns_[i]->setText(QString(m_bIsUppercase_ ? 'A' + i : 'a' + i ));
}

void SoftKeyBoard::on_btn_next__clicked()
{
    ShowChinese(m_strPingying_,m_currentPage_ + 1,0);
}

void SoftKeyBoard::on_btn_last__clicked()
{
    ShowChinese(m_strPingying_,m_currentPage_ - 1,0);
}

void SoftKeyBoard::on_btn_shift__clicked()
{
    m_bShiftPressed_ = !m_bShiftPressed_;
    ui.btn_shift_->setStyleSheet(QString("#btn_shift_{color: rgb(255,255,255);background-color:") + (m_bShiftPressed_ ? "rgb(127,122,117)" : "rgb(67,63,59)") + ";border-radius: 7px;}#btn_shift_:pressed{background-color: rgb(33,30,27);}");
}

bool SoftKeyBoard::InitChinese()
{
    qDebug()<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
    QFile file(":/FontLibrary.txt");
    int i = 0;
    if(file.open(QIODevice::ReadOnly))
    {
        QString str;
        QString key;
        while (true)
        {
            str = file.readLine();
            if(str.size() < 4)
                break;
            key = str.mid(2,str.size() - 4);
            m_map_[key].push_back(str.left(1));
            ++i;
        }
        file.close();
        qDebug()<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")<<i;
        //for(std::map<QString,std::vector<QString> >::iterator it = m_map_.begin();it != m_map_.end();++it)
        //{
        //    qDebug()<<it->first;
        //    int c = 0;
        //    for(size_t i = 0;i < it->second.size();++i,++c)
        //    {
        //        qDebug()<<it->second[i];
        //    }
        //    qDebug()<<c;
        //}
        //qDebug()<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
        return true;
    }
    return false;
}

bool SoftKeyBoard::GetBtnText(QWidget *widget, QString &text, int start, int count)
{
    text = "";
    for(int i = start;i < start + count;++i)
    {
        if(widget == m_btns_[i])
        {
            text = m_btns_[i]->text();
            return true;
        }
    }
    return true;
}

void SoftKeyBoard::ShowChinese(QString str,int page,int pos)
{
    qDebug()<<__PRETTY_FUNCTION__<<str<<page<<pos<<"current"<<m_strPingying_<<m_currentPage_<<m_currentPos_;
    if(page < 0)
        page = 0;
    if(pos < 0 || pos > 11)
        pos = 0;
    std::map<QString,std::vector<QString> >::iterator it = m_map_.find(str);
    ui.lab_pinying_->setText(str);
    if(it != m_map_.end() && it->second.size() < page * 12)//没有下一页
        return;
    for(int i = 0;i < 12;++i)
    {
        m_btns_[46 + i]->setText(it->second.size() > page * 12 + i && it != m_map_.end() ? it->second[page * 12 + i] : "");
        qDebug()<<__PRETTY_FUNCTION__<<m_btns_[46 + i]->text()<<i;
    }
    for(int i = 0;i < 12;++i)
        m_btns_[46 + i]->setStyleSheet(QString("#word_") + QString::number(i) + "_{color: rgb(255,255,255);background-color: rgb(67,63,59);border-radius: 7px;}#word_" + QString::number(i) + "_:pressed{background-color: rgb(33,30,27);}");
    if(m_btns_[46 + pos]->text().isEmpty())
        pos = m_currentPos_;
    if(!m_btns_[46 + pos]->text().isEmpty())
        m_btns_[46 + pos]->setStyleSheet(QString("#word_") + QString::number(pos) + "_{color: rgb(255,255,255);background-color: rgb(127,122,117);border-radius: 7px;}#word_" + QString::number(pos) + "_:pressed{background-color: rgb(33,30,27);}");
    m_strPingying_ = str;
    m_currentPage_ = page;
    m_currentPos_ = pos;
}
