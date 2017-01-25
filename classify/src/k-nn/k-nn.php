<?php

// ベクトルクラス
class Vec
{
    public $classId = null;
    public $vec = [];

    //ベクトルを配列に格納
    function __construct($line)
    {
        $tVec = explode(' ', $line);
        foreach ($tVec as $elem) {
            list($k, $v) = explode(':', $elem);
            $this->vec[$k] = (float)$v;
        }

        return $this;
    }

    // ユークリッド距離
    public function calcMagnitude(){
        return sqrt(
            array_sum(array_map(
                function($val){return $val**2;},
                $this->vec)
            )
        );
    }

}

// k-nn法のクラス
class K_NN
{
    public $k = 1;

    private $trainVecs = [];
    private $testVecs = [];
    private $classNames = [];

    //分類するクラス名を指定する
    public function __construct($classNames)
    {
        $this->classNames = $classNames;
        return $this;
    }

    //教師データを読み込む
    function readTrainVec($filenemes)
    {
        foreach ($filenemes as $classId => $fileName) {
            $trainFile = fopen($fileName, 'r');
            while($line = fgets($trainFile)){
                $tmpVec = new Vec($line);
                $tmpVec->classId = $classId;
                $this->trainVecs[$classId][] = $tmpVec;
            }
        }
        return $this;
    }

    //テストデータを読み込む
    function readTestVec($fileName)
    {
        $trainFile = fopen($fileName, 'r');
        while($line = fgets($trainFile)){
            $this->testVecs[] = new Vec($line);
        }
        return $this;

    }

    //k近傍のコサイン類似度の総和を計算
    public function calcKNN()
    {
        //testVecId番目の入力文書について
        foreach ($this->testVecs as $testVecId => $testVec) {
            $knnSum = [];
            //$classId番目のクラスの教師データ
            foreach ($this->trainVecs as $classId => $classVecs) {
                $sims[$classId] = null;
                //$classVecId番目の教師データ文書
                foreach ($classVecs as $classVecId => $trainVec) {
                    $sim = $this->calcCosSim($testVec, $trainVec);
                    $sims[$classId][] = $sim;
                }
                //上位k個を取り出し
                rsort($sims[$classId]);
                $knn[$classId] = array_slice($sims[$classId], 0, $this->k);
                $knnSums[$classId] = array_sum($knn[$classId]);
            }
            $this->testVecs[$testVecId]->classId = array_keys($knnSums, max($knnSums))[0];
        }
        return $this;
    }

    //2ベクトル間のコサイン類似度の計算
    private function calcCosSim($vec1, $vec2)
    {
        $innerProduct = 0;
        foreach ($vec1->vec as $word => $tfidf) {
            if(array_key_exists($word, $vec2->vec)){
                $innerProduct += $tfidf * $vec2->vec[$word];
            }
        }

        return $innerProduct / ($vec1->calcMagnitude() * $vec2->calcMagnitude());

    }

    //結果の表示
    public function printResult(){
        foreach ($this->testVecs as $testVec) {
            echo $this->classNames[$testVec->classId], "\n";
        }
        return $this;
    }

}

$opts = getopt('a:b:k:t:');

//オプションから各ファイルを取得
$trainFileNames = [$opts['a'], $opts['b']];
$testFileName = $opts['t'];

//ファイル名からクラス名を取得
$tmp = [explode('.', $trainFileNames[0]), explode('.', $trainFileNames[1])];
$classNames = [$tmp[0][count($tmp[0])-3], $tmp[1][count($tmp[1])-3]];

//k-nn法
$knn = new K_NN($classNames);
$knn->k = $opts['k'];
$knn->readTrainVec($trainFileNames)
->readTestVec($testFileName)
->calcKNN()
->printResult();
