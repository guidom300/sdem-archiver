#include <QtTest>
#include <QString>
#include <string>
#include "lz77_boyer_moore_dictionary.h"
#include "lz77_data.h"

using std::string;

class LZ77BoyerMooreDictionaryTest : public QObject {
  Q_OBJECT

 private:
  void t(const char *dictionary_content,
         const char *lookahead_buffer,
         size_t position,
         size_t length);

 private Q_SLOTS:
  void testCase1();
  void testCase1_data();
};

void LZ77BoyerMooreDictionaryTest::t(const char *dictionary_content,
                                     const char *lookahead_buffer,
                                     size_t position,
                                     size_t length) {
  QTest::newRow("") << QString(dictionary_content) << QString(lookahead_buffer)
                    << position << length;
}

void LZ77BoyerMooreDictionaryTest::testCase1() {
  QFETCH(QString, dictionary_content);
  QFETCH(QString, lookahead_buffer);
  QFETCH(size_t, position);
  QFETCH(size_t, length);

  string content(dictionary_content.toStdString());
  string pattern(lookahead_buffer.toStdString());

  LZ77BufferedData<100, 100, char *, char> data(
      content.begin(), content.end(), pattern.begin(), pattern.end());

  LZ77BoyerMooreDictionary<100, 100, char> dictionary;

  auto match = dictionary.find_match(data);

  QCOMPARE(static_cast<size_t>(match.position), position);
  QCOMPARE(static_cast<size_t>(match.length), length);
}

void LZ77BoyerMooreDictionaryTest::testCase1_data() {
  QTest::addColumn<QString>("dictionary_content");
  QTest::addColumn<QString>("lookahead_buffer");
  QTest::addColumn<size_t>("position");
  QTest::addColumn<size_t>("length");

  t("GCATGACATATGAGAGACGCTACG", "GAGAGACG", 13, 8);
  t("GCATCGCAGAGAGTATACAGTACG", "GCAGAGAG", 19, 8);
  t("ABC ABCDAB ABCDABCDABDE", "ABCDABD", 8, 7);
  t("", "aaca", 0, 0);
  t("a", "acaa", 1, 1);
  t("aac", "aaca", 3, 4);
  t("caacab", "caba", 3, 3);
  t("abcaba", "aac", 1, 2);
  t("abcd", "cdcdcdcdc", 2, 9);
}

QTEST_APPLESS_MAIN(LZ77BoyerMooreDictionaryTest)

#include "lz77_boyer_moore_dictionary_test.moc"
