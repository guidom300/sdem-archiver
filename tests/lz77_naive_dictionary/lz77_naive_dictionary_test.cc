#include <QString>
#include <QtTest>
#include "lz77_naive_dictionary.h"
#include "lz77_data.h"

class LZ77NaiveDictionaryTest : public QObject {
  Q_OBJECT

 public:
  LZ77NaiveDictionaryTest();

 private:
  void t(const char *dictionary_content,
         const char *lookahead_buffer,
         size_t position,
         size_t length);

 private Q_SLOTS:
  void testFindMatch();
  void testFindMatch_data();
};

LZ77NaiveDictionaryTest::LZ77NaiveDictionaryTest() {}

void LZ77NaiveDictionaryTest::t(const char *dictionary_content,
                                const char *lookahead_buffer,
                                size_t position,
                                size_t length) {
  QTest::newRow("") << QString(dictionary_content) << QString(lookahead_buffer)
                    << position << length;
}

void LZ77NaiveDictionaryTest::testFindMatch() {
  QFETCH(QString, dictionary_content);
  QFETCH(QString, lookahead_buffer);
  QFETCH(size_t, position);
  QFETCH(size_t, length);

  LZ77BufferedData<8, 100, QChar *, QChar> data(dictionary_content.begin(),
                                                dictionary_content.end(),
                                                lookahead_buffer.begin(),
                                                lookahead_buffer.end());

  LZ77NaiveDictionary<8, 100, QChar> dictionary;

  auto match = dictionary.find_match(data);

  QCOMPARE(static_cast<size_t>(match.position), position);
  QCOMPARE(static_cast<size_t>(match.length), length);
}

void LZ77NaiveDictionaryTest::testFindMatch_data() {
  QTest::addColumn<QString>("dictionary_content");
  QTest::addColumn<QString>("lookahead_buffer");
  QTest::addColumn<size_t>("position");
  QTest::addColumn<size_t>("length");

  t("ABC ABCDAB ABCDABCDABDE", "ABCDABD", 8, 7);
  t("", "aaca", 0, 0);
  t("a", "acaa", 1, 1);
  t("aac", "aaca", 3, 4);
  t("caacab", "caba", 3, 3);
  t("abcaba", "aac", 1, 2);
  t("abcd", "cdcdcdcdc", 2, 9);
}

QTEST_APPLESS_MAIN(LZ77NaiveDictionaryTest)

#include "lz77_naive_dictionary_test.moc"
