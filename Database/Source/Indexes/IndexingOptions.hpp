#pragma once

struct RegionParam {
    int cutOff;
    int regionSize;
    int number;
    int overlapSize;
};

struct SegParam {
    int window = 12;
    float segLowCutOff = 2.2;
    float segHighCutOff = 2.5;
};

struct IndexingOptions {

    int kmerLen = 5;
    RegionParam highFreq = {3, 300, 5, 100};
    RegionParam lowFreq = {2, 300, 7, 100};
    SegParam segParam;

    int numberOfThreads = 24;
};
