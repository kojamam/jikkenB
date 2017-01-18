<?php

class Vec
{
    public $classId = null;
    public $className = '';
    public $vec = [];

    function __construct($line)
    {
        $tVec = explode(' ', $line);
        foreach ($tVec as $elem) {
            list($k, $v) = explode(':', $elem);
            $this->vec[$k] = (float)$v;
        }

        return $this;
    }

    public function setClassId($classId)
    {
        $this->classId = $classId;
        if($classId === 0){
            $this->className = 'mp3player';
        }else if($classId === 1){
            $this->className = 'cleaner';
        }
        return $this;
    }

    public function calcMagnitude(){
        return sqrt(
            array_sum(array_map(
                function($val){return $val**2;},
                $this->vec)
            )
        );
    }

}


class K_NN
{

    public $trainVecs = [];
    public $testVecs = [];
    public $k;
    public $knns = [];

    function readTrainVec($filenemes)
    {
        foreach ($filenemes as $classId => $fileName) {
            $trainFile = fopen($fileName, 'r');
            while($line = fgets($trainFile)){
                $tmpVec = new Vec($line);
                $tmpVec->setClassId($classId);
                $this->trainVecs[$classId][] = $tmpVec;
            }
        }
    }

    function readTestVec($fileName)
    {
        $trainFile = fopen($fileName, 'r');
        while($line = fgets($trainFile)){
            $this->testVecs[] = new Vec($line);
        }

    }

    public function calcKNN()
    {
        //testVecId番目の入力文書に対して
        foreach ($this->testVecs as $testVecId => $testVec) {
            $knnSum = [];
            foreach ($this->trainVecs as $classId => $classVecs) {
                $this->knns[$classId] = null;
                foreach ($classVecs as $classVecId => $trainVec) {
                    $sim = $this->calcCosSim($testVec, $trainVec);
                    $this->knns[$classId][] = $sim;
                }
                rsort($this->knns[$classId]);
                $this->knns[$classId] = array_slice($this->knns[$classId], 0, $this->k);
                $knnSum[$classId] = array_sum($this->knns[$classId]);
            }
            $this->testVecs[$testVecId]->setClassId(array_keys($knnSum, max($knnSum))[0]);
        }
    }

    public function calcCosSim($vec1, $vec2)
    {
        $innerProduct = 0;
        foreach ($vec1->vec as $word => $tfidf) {
            if(array_key_exists($word, $vec2->vec)){
                $innerProduct += $tfidf * $vec2->vec[$word];
            }
        }

        return $innerProduct / ($vec1->calcMagnitude() * $vec2->calcMagnitude());

    }

    public function printResult(){
        foreach ($this->testVecs as $testVec) {
            echo $testVec->className, "\n";
        }
    }

}


$knn = new K_NN();
$knn->k = 5;
$knn->readTrainVec(['../data/train.mp3player.vec=tfidf.txt', '../data/train.cleaner.vec=tfidf.txt']);
$knn->readTestVec('../data/test.vec=tfidf.txt');
$knn->calcKNN();
$knn->printResult();
