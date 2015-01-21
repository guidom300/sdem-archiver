#include <QtTest>
#include <string>
#include <vector>
#include <iterator>
#include "lzss_encoder.h"
#include "bit_reader.h"

class LZSSEncoderTest : public QObject {
  Q_OBJECT

 public:
  LZSSEncoderTest() : input("aacaacabcabaaac"), bit_reader(output.end()) {
    encoder(input.cbegin(), input.cend(), std::back_inserter(output));
    bit_reader = decltype(bit_reader)(output.begin(), output.end());
  }

 private:
  typedef NaiveLZSSEncoder<8, 4, 3, 6, 4> encoder_type;

  encoder_type encoder;
  std::string input;
  std::vector<char> output;
  BitReader<std::vector<char>::iterator> bit_reader;

  void t(char symbol) { t(LZSS_UNENCODED_FLAG, 0, 0, symbol); }

  void t(int position, int length) {
    t(LZSS_ENCODED_FLAG, position, length, 'x');
  }

  void t(bool flag, int position, int length, char symbol) {
    QTest::newRow("") << flag << position << length << symbol;
  }

 private Q_SLOTS:
  void testCase1();
  void testCase1_data();
};

void LZSSEncoderTest::testCase1() {
  QFETCH(bool, flag);
  bool actual_flag = bit_reader.read();
  QCOMPARE(actual_flag, flag);

  if (flag == LZSS_ENCODED_FLAG) {
    QFETCH(int, position);
    QFETCH(int, length);
    int actual_position, actual_length;
    bit_reader.read(actual_position, 8);
    bit_reader.read(actual_length, 4);

    QCOMPARE(actual_position, position);
    QCOMPARE(actual_length + 3, length);
  } else {
    QFETCH(char, symbol);
    char actual_symbol;
    bit_reader.read(actual_symbol);

    QCOMPARE(actual_symbol, symbol);
  }
}

void LZSSEncoderTest::testCase1_data() {
  QTest::addColumn<bool>("flag");
  QTest::addColumn<int>("position");
  QTest::addColumn<int>("length");
  QTest::addColumn<char>("symbol");

  t('a');
  t('a');
  t('c');
  t(3, 4);
  t('b');
  t(3, 3);
  t('a');
  t('a');
  t('a');
  t('c');
}

QTEST_APPLESS_MAIN(LZSSEncoderTest)

#include "lzss_encoder_test.moc"
