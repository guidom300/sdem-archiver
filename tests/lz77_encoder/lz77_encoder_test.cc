#include <QtTest>
#include <QString>
#include <string>
#include <vector>
#include <iterator>
#include "lz77_encoder.h"
#include "lz77_naive_dictionary.h"

using std::string;
using std::vector;
using std::back_inserter;

class LZ77EncoderTest : public QObject {
  Q_OBJECT

 private:
  void t(const char* input, const char* output) {
    QTest::newRow("") << QString(input) << QString(output);
  }

 private Q_SLOTS:
  void testCase1();
  void testCase1_data();
};

void LZ77EncoderTest::testCase1() {
  QFETCH(QString, input);
  QFETCH(QString, output);

  LZ77Encoder<8, 8, 6, 4, char, LZ77NaiveDictionary> encoder;
  vector<char> encoded_stream;
  string char_input(input.toStdString());
  string char_output(output.toStdString());

  auto steps = encoder(
      char_input.cbegin(), char_input.cend(), back_inserter(encoded_stream));

  for (auto&& ch : encoded_stream) {
    if (ch < 'a') {
      ch += '0';
    }
  }

  QCOMPARE(static_cast<decltype(steps)>(output.size()), steps * 3);
  QCOMPARE(encoded_stream.size(), (size_t)output.size());
  QCOMPARE(encoded_stream,
           vector<char>(char_output.begin(), char_output.end()));
}

void LZ77EncoderTest::testCase1_data() {
  QTest::addColumn<QString>("input");
  QTest::addColumn<QString>("output");

  t("aacaacabcabaaac", "00a11c34b33a12c");
}

QTEST_APPLESS_MAIN(LZ77EncoderTest)

#include "lz77_encoder_test.moc"
