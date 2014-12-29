#include <QtTest>
#include <sstream>
#include "bit_writer.h"
#include "bit_reader.h"

class BitReaderTest : public QObject {
  Q_OBJECT

 private Q_SLOTS:
  void testCase1();
};

void BitReaderTest::testCase1() {
  std::stringstream stream;
  BitWriter<> bit_writer(stream);
  BitReader<> bit_reader(stream);

  for (unsigned char val = 0; val <= 8; ++val) {
    bit_writer.write(val, val);
  }

  bit_writer.flush();

  for (unsigned char expected = 0; expected <= 8; ++expected) {
    unsigned char actual;
    bit_reader.read(actual, expected);

    QCOMPARE(actual, expected);
  }
}

QTEST_APPLESS_MAIN(BitReaderTest)

#include "bit_reader_test.moc"
