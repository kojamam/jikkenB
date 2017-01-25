<?php

$df = [];
$numSen = 0;

$dfFlag = [];

while($line = trim(fgets(STDIN))){
    $term = explode("\t", $line)[0];
    if($term != "EOS"){
        if (!isset($dfFlag[$term])) {
            isset($df[$term]) ? $df[$term]++ : $df[$term] = 1;
            $dfFlag[$term] = true;
        }
    }else{
        $numSen++;
        $dfFlag = [];
    }
}

arsort($df);
echo $numSen, "\n";
$last_i  = key(array_slice($df, -1, 1, true));
foreach ($df as $term=> $val) {
    echo "$term:$val";
    if ($term == $last_i) {
        echo "\n";
    }else{
        echo " ";
    }
}
