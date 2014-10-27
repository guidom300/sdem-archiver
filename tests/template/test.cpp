#include "test.h"

void Test::initTestCase()
{
    qDebug("called before every test");
}

void Test::aTestCase()
{
    QVERIFY(1 == 1);
}

void Test::cleanupTestCase()
{
    qDebug("called after every test");
}

QTEST_MAIN(Test)
