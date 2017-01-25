<?php

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
    public $numAllSen = 0;

    public function readSen($dfFileName)
    {
        $senNo = 0;
        $tempTf = [];
        // $dfFlag = [];

        // 読み込み
        while($line = trim(fgets(STDIN))){
            $term = explode("\t", $line)[0];
            //TODO 同音異義語への対応
            if($term != "EOS"){
                isset($tempTf[$term]) ? $tempTf[$term]++ : $tempTf[$term] = 1;
            }else{
                $this->tf[$senNo] = $this->sortDesc($tempTf);
                $tempTf = [];
                $senNo++;
            }
        }
        $this->numSen = $senNo;
        // $this->df = $this->sortDesc($this->df);
        $this->dfParcer($dfFileName);
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

    public function calcIdf()
    {
        foreach ($this->df as $term => $count) {
            $this->idf[$term] = log($this->numAllSen/$count) + 1;
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

    public function printTfIdf()
    {
        // echo "-----tf*idf-----\n";

        // 表示
        foreach ($this->tfIdf as $senNo => $sen) {
            $last_term  = key(array_slice($sen, -1, 1, true));
            foreach ($sen as $term => $tfIdf) {
                print("$term:$tfIdf");
                if($term == $last_term){
                    echo "\n";
                }else{
                    echo " ";
                }
            }
        }

    }

    public function dfParcer($fileName)
    {
        $dfFile = fopen($fileName, 'r');
        $this->numAllSen = fgets($dfFile);

        $line = fgets($dfFile);
        $tVec = explode(' ', $line);
        foreach ($tVec as $elem) {
            list($k, $v) = explode(':', $elem);
            $this->df[$k] = (float)$v;
            // var_dump($this->df[$k]);
        }
        return $this;
    }

}

$WordVec = new WordVec();
$WordVec->readSen($argv[1]);
$WordVec->printTfIdf();
