#include <QtTest>
#include <QString>
#include <string>
#include <iterator>
#include <cctype>
#include "lzss_decoder.h"

class LZSSDecoderTest : public QObject {
  Q_OBJECT

 private Q_SLOTS:
  void testCase1();
};

void LZSSDecoderTest::testCase1() {
  std::string input("aacaacabcabaaac");
  std::vector<char> encoded;

  NaiveLZSSEncoder<8, 4, 3, 6, 4> encoder;
  size_t steps =
      encoder(input.begin(), input.end(), std::back_inserter(encoded));

  std::vector<char> decoded;
  LZSSDecoder<8, 4, 3> decoder;
  decoder(encoded.begin(), encoded.end(), std::back_inserter(decoded), steps);

  std::vector<char> input_vector(input.begin(), input.end());
  QCOMPARE(input.size(), decoded.size());
  QCOMPARE(decoded, input_vector);
}

QTEST_APPLESS_MAIN(LZSSDecoderTest)

#include "lzss_decoder_test.moc"
