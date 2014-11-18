#include <QtTest>
#include <sstream>
#include "bit_writer.h"

class BitWriterTest : public QObject {
  Q_OBJECT

 public:
  BitWriterTest();

 private Q_SLOTS:
  void testCase1();
  void testCase1_data();
};

BitWriterTest::BitWriterTest() {}

void BitWriterTest::testCase1() {
  std::stringstream stream;
  BitWriter<> bit_writer(stream);

  QFETCH(int, value);
  QFETCH(int, bits);
  QFETCH(int, result);

  bit_writer.write(value, bits);
  bit_writer.flush();

  QCOMPARE(stream.get(), result);
}

void BitWriterTest::testCase1_data() {
  QTest::addColumn<int>("value");
  QTest::addColumn<int>("bits");
  QTest::addColumn<int>("result");

  int result = 128;

  for (int i = 1; i < 8; ++i) {
    QTest::newRow("") << 1 << i << result;
    result /= 2;
  }
}

QTEST_APPLESS_MAIN(BitWriterTest)

#include "bit_writer_test.moc"
