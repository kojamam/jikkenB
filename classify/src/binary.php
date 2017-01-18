<?php

public function countWord(){
$doc = [];

// 読み込み
while($line = trim(fgets(STDIN))){
    $morp = explode("\t", $line);

    //TODO 同音異義語への対応
    if($morp[0] != "EOS"){
        isset($doc[$morp[0]]) ? $doc[$morp[0]]++ : $doc[$morp[0]] = 1;
    }
}

// 降順ソート
arsort($doc);

// 表示
foreach ($doc as $morp => $count) {
    print("$morp\t$count\n");
}

return $doc;

}
