<?php

class Vec
{
    public $classId = null;
    public $vec = [];

    function __construct(string $line)
    {
        $tVec = explode($string, ' ');
        foreach ($tVec as $elem) {
            $this->vec[] = explode($elem, ':');
        }

        return $this;
    }

    public function setClassId($classId)
    {
        $this->classId = $classId;
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

    function readTestVec($fileneme)
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
            foreach ($this->trainVecs as $classId => $classVecs) {
                foreach ($classVecs as $classVecId => $trainVec) {
                    $sim = $this->calcCosSim($testVec, $trainVec);
                    if(count($knns[$classId] < $k)){
                        $knns[$classId][] = $sim;
                    }elseif($sim > min($knns[$classId])){
                        array_pop($knns[$classId]);
                        $knns[$classId][] = $sim;
                    }
                    $knns[$classId] = rsort($knns[$classId]);
                }
            }
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

}
