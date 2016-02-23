#ifndef UNICODE_TEXT
#include <QApplication>
#define UNICODE_TEXT(text) QApplication::translate("vbnhfylbh", QString(text).toStdString().c_str(), 0, QApplication::UnicodeUTF8)
#endif