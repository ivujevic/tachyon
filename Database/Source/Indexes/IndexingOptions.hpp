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

    int kmerLen = 12;
    RegionParam highFreq = {3, 400, 5, 50};
    RegionParam lowFreq = {2, 400, 7, 50};
    SegParam segParam;

    int numberOfThreads = 24;
};
