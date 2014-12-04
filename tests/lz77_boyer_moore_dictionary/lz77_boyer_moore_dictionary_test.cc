#include <QtTest/QtTest>
#include <QString>
#include "lz77_boyer_moore_dictionary.h"

class Lz77BoyerMooreDictionaryTest : public QObject {
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

void Lz77BoyerMooreDictionaryTest::t(const char *dictionary_content,
                                const char *lookahead_buffer,
                                size_t position,
                                size_t length) {
    QTest::newRow("") << QString(dictionary_content) << QString(lookahead_buffer)
    << position << length;
}

void Lz77BoyerMooreDictionaryTest::testCase1() {
    QFETCH(QString, dictionary_content);
    QFETCH(QString, lookahead_buffer);
    QFETCH(size_t, position);
    QFETCH(size_t, length);
    std::string content(dictionary_content.toStdString());
    
    LZ77BoyerMooreDictionary<8, char> dictionary(content.begin(), content.end());
    
    auto match =
    dictionary.find_match(lookahead_buffer.toStdString());
    
    QCOMPARE(match.position, position);
    QCOMPARE(match.length, length);
}

void Lz77BoyerMooreDictionaryTest::testCase1_data() {
    QTest::addColumn<QString>("dictionary_content");
    QTest::addColumn<QString>("lookahead_buffer");
    QTest::addColumn<size_t>("position");
    QTest::addColumn<size_t>("length");
    
    t("GCATCGCAGAGAGTATACAGTACG", "GCAGAGAG", 19, 8);
    t("ABC ABCDAB ABCDABCDABDE", "ABCDABD", 8, 7);
    t("", "aaca", 0, 0);
    t("a", "acaa", 1, 1);
    t("aac", "aaca", 3, 4);
    t("caacab", "caba", 3, 3);
    t("abcaba", "aac", 1, 2);
    t("abcd", "cdcdcdcdc", 2, 9);
}

QTEST_APPLESS_MAIN(Lz77BoyerMooreDictionaryTest)

#include "lz77_boyer_moore_dictionary_test.moc"
