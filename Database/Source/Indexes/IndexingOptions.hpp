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

    int kmerLen = 20;
    RegionParam highFreq = {3, 1000, 3, 100};
    RegionParam lowFreq = {2, 1000, 7, 100};
    SegParam segParam;

    int numberOfThreads = 24;
};
