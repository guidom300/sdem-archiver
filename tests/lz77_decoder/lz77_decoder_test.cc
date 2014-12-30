#include <QtTest>
#include <QString>
#include <string>
#include <iterator>
#include <cctype>
#include "lz77_decoder.h"

using std::string;
using std::back_inserter;

class Lz77DecoderTest : public QObject {
  Q_OBJECT

 private:
  void t(const char* input, const char* output) {
    QTest::newRow("") << QString(input) << QString(output);
  }

 private Q_SLOTS:
  void testCase1();
  void testCase1_data();
};

void Lz77DecoderTest::testCase1() {
  QFETCH(QString, input);
  QFETCH(QString, output);

  LZ77Decoder<8, 8> d;
  string decoded_stream;
  string char_input(input.toStdString());
  string char_output(output.toStdString());

  for (auto&& ch : char_input) {
    if (isdigit(ch)) {
      ch -= '0';
    }
  }

  d(char_input.cbegin(), char_input.cend(), back_inserter(decoded_stream));

  QCOMPARE(decoded_stream.size(), static_cast<size_t>(output.size()));
  QCOMPARE(decoded_stream, char_output);
}

void Lz77DecoderTest::testCase1_data() {
  QTest::addColumn<QString>("input");
  QTest::addColumn<QString>("output");

  t("00a11c34b33a12c", "aacaacabcabaaac");
}

QTEST_APPLESS_MAIN(Lz77DecoderTest)

#include "lz77_decoder_test.moc"
