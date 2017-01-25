import sys
import re
from collections import defaultdict

delta = 1

n_ci = defaultdict(lambda: 0)

wordCount = 0
denominator = 0

excludePat = re.compile(r"(記号)|(助詞)|(助動詞)")

for line in sys.stdin:
    if len(line.split("\t")) == 2: # EOSじゃないとき
        (term, pos) = map(lambda str:str.rstrip() ,line.split("\t"))

        if not excludePat.match(pos):
            wordCount += 1
            n_ci[term] += 1

denominator = wordCount + len(n_ci)

print(denominator)

for term, freq  in n_ci.items():
    print(term+"\t"+str(freq))
