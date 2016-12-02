#include "morp.h"

static int loadInput(Utf8Char*);
static int vitarbi(Utf8Char*, int);

int main(int argc, char *argv[]) {

    Utf8Char *input;
    int numChars = 0;

    clock_t time1 = clock();

    /*辞書の読み込み*/
    initializeDicModule(argv[1]);

    //入力の読み込み
    input = (Utf8Char*)malloc(sizeof(Utf8Char)*MAX_INPUT_LEN);
    numChars = loadInput(input);

    clock_t time2 = clock();

    //Viterbiで単語コスト最小法
    vitarbi(input, numChars);

    clock_t time3 = clock();

    free(input);

    clock_t time4 = clock();

    //処理時間の表示
    printf( "読み込みの処理時間:%lf[s]\n", ((double)time2-time1)/CLOCKS_PER_SEC);
    printf( "形態素解析の処理時間:%lf[s]\n", ((double)time3-time2)/CLOCKS_PER_SEC);
    printf( "全体の処理時間:%lf[s]\n", ((double)time4-time1)/CLOCKS_PER_SEC);

    return 0;
}

static int loadInput(Utf8Char *input){
    int numChars = 0;

    /* 入力の読み込み */
    while(1){
       explode(&input[numChars]);
       if(input[numChars].c[0] == EOF){
           break;
       }
       if(input[numChars].type == TYPE_CONTROL){
           numChars--;
       }
       numChars++;
   }

    return numChars;
}

static int vitarbi(Utf8Char *input, int numChars){
    //単語コスト最小法
    int x = 0; //x
    int ent; //w
    int i; //i
    int len = 1;
    char word[MAX_WORDLEN*4];
    double *mincost = (double*)malloc(sizeof(double)*(numChars+1)); //N()
    int *wordList = (int*)malloc(sizeof(int)*(numChars+1)); //W()
    int *start = (int*)malloc(sizeof(int)*(numChars+1)); //B()

    // N()の初期化
    mincost[x++] = 0;
    for (i = 1; i < numChars+1; i++) {
        mincost[i] = 20 * numChars + 1;
    }

    // vitarbiでラティス作成
    for(x = 1; x <= numChars; x++){
        if(MAX_WORDLEN < x){
            i = x - MAX_WORDLEN;
        }else{
            i = 1;
        }
        int foundFlag = 0;

        for (; i <= x; i++) {
            int x2 = x - 1;
            int i2 = i - 1;
            len = x2 - i2 + 1;

            combineChars(input, word, i2, len);
            if((ent = lookupTrie(word)) != -1){
                //見つかった
                double tCost = getCost(ent);

                //累計コストが現在より小さければ更新
                if(mincost[i-1] + tCost < mincost[x]){
                    foundFlag = 1;
                    mincost[x] = mincost[i-1] + tCost;
                    wordList[x] = ent;
                    start[x] = i;
                }
            }
        }
        // 辞書にない単語の処理
        if(foundFlag == 0){
            i = x;
            mincost[x] = mincost[i-1] + 20;
            wordList[x] = -1;
            start[x] = i;
        }
    }

    //終了処理
    int *correctWordListR = (int*)malloc(sizeof(int)*(numChars+1));
    int *correctStartR = (int*)malloc(sizeof(int)*(numChars+1));

    correctWordListR[0] = wordList[x-1];
    x = start[x-1] - 1;

    // バックトラック
    int k;
    for (k=1; x >= 1; k++) {
        correctWordListR[k] = wordList[x];
        correctStartR[k] = start[x];
        x = start[x] - 1;

    }
    int numWord = k;

    free(wordList);
    free(start);

    //表示
    for (k--; k >= 0; k--) {
        if(correctWordListR[k] == -1){
            //辞書なし文字
            printf("%s\t", input[correctStartR[k+1]].c);
        }
        printWord(correctWordListR[k]);
    }

    free(correctWordListR);
    free(correctStartR);

    return numWord;
}
