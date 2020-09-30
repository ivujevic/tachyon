#include "Seg.hpp"

#include "LogArray.hpp"

/**
 * Return masked sequence
 */
void Seg::mask(string &sequence) {

    downset = (L + 1) / 2 - 1;
    upset = L - downset;

    vector<pair<int, int>> segments;
    segSeq(sequence, segments, 0);

    for (pair<int, int> p : segments) {
        for (int i = p.first; i <= p.second; i++) sequence.at(i) = maskChar;
    }
}

void Seg::segSeq(string &seq, vector<pair<int, int>> &segs, int offset) {
    int first = downset;
    int len = (int) seq.length();
    int last = (int) seq.length() - upset;
    int lowlim = first;
    int leftend, rightend, hii, loi;

    vector<double> H(len);

    getEntropies(seq, len, H);

    vector<pair<int, int>> leftsegs;
    for (int i = first; i < last + 1; i += 1) {
        if (H[i] < k1 && H[i] != -1) {
            loi = findLo(i, lowlim, H);
            hii = findHi(i, last, H);
            leftend = loi - downset;
            rightend = hii + upset - 1;
            trim(seq, leftend, rightend, leftend, rightend);
            if (i + upset < leftend) {
                int lend = loi - downset;
                int rend = leftend - 1;
                vector<pair<int, int>> leftsegs;
                string sub = seq.substr(lend, rend - lend + 1);
                segSeq(sub, leftsegs, offset + lend);
                segs.insert(segs.end(), leftsegs.begin(), leftsegs.end());
            }
            pair<int, int> p = make_pair(leftend + offset, rightend + offset);
            segs.push_back(p);
            i = min(hii, rightend + downset);
            lowlim = i + 1;
        }
    }
}

void Seg::trim(string &seq1, int left, int right, int &leftEnd, int &rightEnd) {
    int lend = 0;
    string seq = seq1.substr(left, right - left + 1);
    int seqLen = seq.length();
    int rend = seqLen - 1;
    int minlen = 1;
    double minprob = 1;
    if ((seqLen - maxtrim) > minlen) minlen = seqLen - maxtrim;
    for (int len = seqLen; len > minlen; len--) {
        int i = 0;
        bool shift = true;
        while (shift) {
            string win = seq.substr(i, len);
            double prob = getProb(win, len);
            if (prob < minprob) {
                minprob = prob;
                lend = i;
                rend = len + i - 1;
            }
            shift = (i + len) < seqLen;
            i++;
        }
    }
    leftEnd = leftEnd + lend;
    rightEnd = rightEnd - (seqLen - rend - 1);
}

double Seg::getProb(string &seq, const int L) {
    double C = -L * log(N) + logFactorial(L) + logFactorial(N);
    int letterCounter[26] = {};
    int numbCounter[L + 1];

    for (int i = 0; i < L + 1; i++) {
        numbCounter[i] = 0;
    }

    for (char &c : seq) letterCounter[toupper(c) - 'A']++;

    double F = 0;
    int notNull = 0;
    for (int i = 0; i < 26; i++) {
        if (letterCounter[i] == 0) continue;
        notNull++;
        int c = letterCounter[i];
        F += logFactorial(c);
        numbCounter[c]++;
    }
    double S = 0;
    for (int c : numbCounter) {
        if (c == 0) continue;
        S += logFactorial(c);
    }
    S += logFactorial(N - notNull);
    return (C - F - S);
}

int Seg::findHi(int i, int limit, vector<double> &H) {
    int j = i;
    for (j = i; j <= limit; j++) {
        if (H[j] == -1) break;
        if (H[j] > k2) break;
    }
    return (j - 1);
}

int Seg::findLo(int i, int limit, vector<double> &H) {
    int j = i;
    for (j = i; j >= limit; j--) {
        if (H[j] == -1) break;
        if (H[j] > k2) break;
    }
    return (j + 1);
}

double Seg::logFactorial(int n) {
    double res = 0;
    if (n < 10000) { // log 10000! is maximum value in array
        res = logs[n];
    } else { // calculate log of n factorial
        for (int i = 1; i < n + 1; i += 1) {
            res += log(i);
        }
    }
    return res;
}

void Seg::getEntropies(string &seq, int len, vector<double> &H) {
    for (int i = 0; i < len; i++) {
        H[i] = -1;
    }

    int first = downset;    // first = 5
    int last = len - upset; // len = 7

    int j = 0;
    for (int i = first; i <= last; i++) {
        string subSeq = seq.substr(j, L);
        int letterCounter[26] = {};
        for (char &c : subSeq) letterCounter[toupper(c) - 'A']++;

        double entropy = 0;
        for (int p : letterCounter) {
            if (p == 0) continue;
            entropy += -(float) p / L * log2((float) p / L);
        }
        H[i] = entropy;
        j++;
    }
}
