#include "myqobject.h"

MyQObject::MyQObject(QObject *parent) :
    QObject(parent), m_value(0)
{
}

void MyQObject::setValue(int value)
{
    if (value != m_value) {
        m_value = value;
        emit valueChanged(value);
    }
}
