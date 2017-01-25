# dfの計算
cat ../../data/ex/test.txt ../../data/ex/train.cleaner.txt ../../data/ex/train.mp3player.txt  | mecab -O simple | php calcdf.php > ../../data/ex/df.txt

# tfidfの計算
mecab -O simple ../../data/ex/train.cleaner.txt | php calctfidf.php ../../data/ex/df.txt > ../../data/ex/train.cleaner.vec=tfidf.txt
mecab -O simple ../../data/ex/train.mp3player.txt | php calctfidf.php ../../data/ex/df.txt > ../../data/ex/train.mp3player.vec=tfidf.txt
mecab -O simple ../../data/ex/test.txt | php calctfidf.php ../../data/ex/df.txt > ../../data/ex/test.vec=tfidf.txt

# k近傍法で分類
php k-nn.php -t ../../data/ex/test.vec=tfidf.txt -a ../../data/ex/train.mp3player.vec=tfidf.txt -b ../../data/ex/train.cleaner.vec=tfidf.txt -k $1 > ../../data/ex/result.txt

# 評価
ruby eval.rb -r ../../data/ex/result.txt -a ../../data/ex/ans.txt
