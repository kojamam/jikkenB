<?php

<<<<<<< HEAD
public function countWord(){
$doc = [];
=======
/**
 *
 */
class WordVec
{
    protected $tf = [];
    protected $df = [];
    protected $idf = [];
    protected $tfIdf = [];
    public $numSen = 0;
>>>>>>> fbf0a7ccb0e9e162f86db5044145d9c12cbc6172

    public function readSen()
    {
        $senNo = 0;
        $tempTf = [];

        // 読み込み
        while($line = trim(fgets(STDIN))){
            $term = explode("\t", $line);

            //TODO 同音異義語への対応
            if($term[0] != "EOS"){
                isset($tempTf[$term[0]]) ? $tempTf[$term[0]]++ : $tempTf[$term[0]] = 1;
                isset($this->df[$term[0]]) ? $this->df[$term[0]]++ : $this->df[$term[0]] = 1;
            }else{
                $this->tf[$senNo] = $this->sortDesc($tempTf);
                $tempTf = [];
                $senNo++;
            }
        }
        $this->numSen = $senNo;
        $this->df = $this->sortDesc($this->df);
        $this->calcIdf();
        $this->calcTfIdf();
        return $this;
    }

    protected function sortDesc($ary)
    {
        // 降順ソート
        arsort($ary);
        return $ary;
    }

    public function printBinary()
    {
        echo "-----binary-----\n";

        // 表示
        foreach ($this->tf as $senNo => $sen) {
            foreach ($sen as $term => $count) {
                print("$term:1 ");
            }
            echo "\n";
        }

    }
    public function printTf()
    {
        echo "-----tf-----\n";

        // 表示
        foreach ($this->tf as $senNo => $sen) {
            foreach ($sen as $term => $count) {
                print("$term:$count ");
            }
            echo "\n";
        }

    }

    public function printDf()
    {
        echo "-----df-----\n";

        // 表示
        foreach ($this->df as $term => $count) {
            print("$term:$count ");
        }

        echo "\n";
    }

    public function printIdf()
    {

        echo "-----idf-----\n";
        foreach ($this->idf as $term => $idf) {
            print("$term:$idf ");
        }

        echo "\n";
    }

    public function printTfIdf()
    {
        echo "-----tf*idf-----\n";

        // 表示
        foreach ($this->tfIdf as $senNo => $sen) {
            foreach ($sen as $term => $tfIdf) {
                print("$term:$tfIdf ");
            }
            echo "\n";
        }

    }

    public function calcIdf()
    {
        foreach ($this->df as $term => $count) {
            $this->idf[$term] = log(($this->numSen)/$this->df[$term]) + 1;
        }
        return $this;
    }

    public function calcTfIdf()
    {
        foreach ($this->tf as $senNo => $sen) {
            foreach ($sen as $term => $tf) {
                $this->tfIdf[$senNo][$term] = $tf * $this->idf[$term];
            }
            $this->tfIdf[$senNo] = $this->sortDesc($this->tfIdf[$senNo]);
        }

        return $this;
    }

}

<<<<<<< HEAD
return $doc;

}
=======
$WordVec = new WordVec();
$WordVec->readSen();
$WordVec->printTf();
echo "\n";
$WordVec->printBinary();
echo "\n";
$WordVec->printDf();
echo "\n";
$WordVec->printIdf();
echo "\n";
$WordVec->printTfIdf();
echo "\n";
>>>>>>> fbf0a7ccb0e9e162f86db5044145d9c12cbc6172
