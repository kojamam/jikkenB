mecab -O simple ../../data/ex/train.cleaner.txt | python3 train.py > ../../data/ex/train.cleaner.bayes.txt
mecab -O simple ../../data/ex/train.mp3player.txt | python3 train.py > ../../data/ex/train.mp3player.bayes.txt
mecab -O simple ../../data/ex/test.txt | python3 classify.py -a ../../data/ex/train.cleaner.bayes.txt -b ../../data/ex/train.mp3player.bayes.txt > ../../data/ex/result_bayes.txt
ruby ../eval.rb -r ../../data/ex/result_bayes.txt -a ../../data/ex/ans.txt
