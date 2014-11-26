#include <QtTest/QtTest>

class TemplateTest : public QObject {
  Q_OBJECT

 private Q_SLOTS:
  void testCase1();
  void testCase1_data();
};

void TemplateTest::testCase1() {}

void TemplateTest::testCase1_data() {}

QTEST_APPLESS_MAIN(TemplateTest)

#include "template_test.moc"
