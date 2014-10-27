#ifndef TEST_H
#define TEST_H

#include <QObject>
#include <QtTest/QtTest>

#include "myqobject.h"


class Test : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void aTestCase();

    void cleanupTestCase();
};

#endif /* TEST_H */
