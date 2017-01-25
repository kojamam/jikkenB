import sys
import argparse
import math
import re
from collections import defaultdict

class Classify(object):
    dormA = dormB = 0
    n_ci_A = n_ci_B = defaultdict(lambda:0)
    testSenVecs = []
    p = []

    def readTrainFile(self, fileNameA, fileNameB):
        classAfile = open(fileNameA, 'r')
        classBfile = open(fileNameB, 'r')

        self.dormA = int(classAfile.readline().rstrip())
        self.dormB = int(classBfile.readline().rstrip())

        self.n_ci_A = self.readTerms(classAfile)
        self.n_ci_B = self.readTerms(classBfile)

        classAfile.close()
        classBfile.close()

    def readTerms(self, file):
        n_ci = defaultdict(lambda:0)
        for line in file:
            line = line.rstrip()
            (term, freq) = line.split("\t")
            n_ci[term] = int(freq)

        return n_ci

    def calcP(self, fileNameA):
        pass

    def readTestSen(self):
        senVec = defaultdict(lambda:0)
        kigoup = re.compile(r"(記号)|(助詞)|(助動詞)")

        for line in sys.stdin:
            if len(line.split("\t")) == 2:
                (term, pos) = map(lambda str:str.rstrip() ,line.split("\t"))

                if not kigoup.match(pos):
                    senVec[term] += 1

            else: #EOSの時
                self.testSenVecs.append(senVec)
                senVec = defaultdict(lambda:0)


    def naiveBays(self):
        for senVec in self.testSenVecs:
            sumA = sumB = 0
            for termVec in senVec.items():
                sumA += termVec[1] * math.log((self.n_ci_A[termVec[0]] + 1) / self.dormA)
                sumB += termVec[1] * math.log((self.n_ci_B[termVec[0]] + 1) / self.dormB)
            if sumA > sumB:
                print('cleaner')
            else:
                print('mp3player')



parser = argparse.ArgumentParser()
parser.add_argument('-a', required=True)
parser.add_argument('-b', required=True)
args = parser.parse_args()

classify = Classify()
classify.readTrainFile(args.a, args.b)
classify.readTestSen()
classify.naiveBays()
