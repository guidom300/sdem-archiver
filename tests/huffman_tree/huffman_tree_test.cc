#include <QtTest>
#include <QString>
#include <string>
#include "huffman_tree.h"
#include "huffman_encoder.h"

using std::stringstream;
using std::string;

class HuffmanTreeTest : public QObject {
  Q_OBJECT

 private:
  typedef HuffmanTree<char> tree_type;
  typedef HuffmanEncoder<char> encoder_type;
  typedef typename encoder_type::bitset_type bitset_type;

  void t(const bitset_type& bitset, const QString& expected) {
    // The actual codes depend on the algorithm
    QCOMPARE(bitset.size(), static_cast<size_t>(expected.size()));
  }

 private Q_SLOTS:
  void testCase1();
};

void HuffmanTreeTest::testCase1() {
  std::string input("this is an example of a huffman tree");
  tree_type tree(input.cbegin(), input.cend());

  encoder_type encoder(tree);

  t(encoder[' '], "111");
  t(encoder['a'], "010");
  t(encoder['e'], "000");
  t(encoder['f'], "1101");
  t(encoder['h'], "1010");
  t(encoder['i'], "1000");
  t(encoder['m'], "0111");
  t(encoder['n'], "0010");
  t(encoder['s'], "1011");
  t(encoder['t'], "0110");
  t(encoder['l'], "11001");
  t(encoder['o'], "00110");
  t(encoder['p'], "10011");
  t(encoder['r'], "11000");
  t(encoder['u'], "00111");
  t(encoder['x'], "10010");
}

QTEST_APPLESS_MAIN(HuffmanTreeTest)

#include "huffman_tree_test.moc"
