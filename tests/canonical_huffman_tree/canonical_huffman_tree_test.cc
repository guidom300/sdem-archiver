#include <QtTest>
#include "huffman_encoder.h"

class CanonicalHuffmanTreeTest : public QObject {
  Q_OBJECT

 private:
  typedef HuffmanEncoder<char> encoder_type;
  typedef typename encoder_type::codebook_type codebook_type;
  typedef typename encoder_type::bitset_type bitset_type;

 private Q_SLOTS:
  void testCase1();
};

void CanonicalHuffmanTreeTest::testCase1() {
  codebook_type codebook{{'D', {true, false, false}},
                         {'B', {false}},
                         {'A', {true, true}},
                         {'C', {true, false, true}}};

  encoder_type encoder(codebook.begin(), codebook.end());
  encoder.make_canonical();

  codebook_type canonical_codebook{{'B', {false}},
                                   {'A', {true, false}},
                                   {'C', {true, true, false}},
                                   {'D', {true, true, true}}};

  for (auto&& code : canonical_codebook) {
    QCOMPARE(encoder[code.first], code.second);
  }
}

QTEST_APPLESS_MAIN(CanonicalHuffmanTreeTest)

#include "canonical_huffman_tree_test.moc"
