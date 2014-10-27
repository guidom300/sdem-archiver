#ifndef MYQOBJECT_H
#define MYQOBJECT_H

#include <QObject>
#include <iostream>

class MyQObject : public QObject
{
    Q_OBJECT

    int m_value;

public:
    explicit MyQObject(QObject *parent = 0);

    int value() const { return m_value; }

    void sayHi() const { std::cout << "Hi!/n"; }

signals:
    void valueChanged(int newValue);

public slots:
    void setValue(int value);

};

#endif // MYQOBJECT_H
