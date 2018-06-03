//
// Created by aocsa on 6/2/18.
//

#ifndef TSDISCOVERY_NNSEARCHINDEX_H
#define TSDISCOVERY_NNSEARCHINDEX_H

#pragma once

#define MAXDOUBLE   1.7976931348623158E+308

#include "DistanceEvaluator.h"
#include "ResultSet.h"

class NNSearchIndex {
private:
    MetricEvaluator *metric;

public:
    typedef TimeSeries tObject;
    typedef std::vector<tObject> tDataset;
    typedef ResultSet<tObject> tResult;


    NNSearchIndex(MetricEvaluator *_metric)
            : metric(_metric) {}

    virtual void Init() = 0;

    virtual void BulkLoad(const tDataset &data) = 0;

    virtual  tResult NearestQuery(tObject& query, int k) = 0;

    virtual  tResult RangeQuery(const tObject& query, double range) = 0;

    MetricEvaluator * GetEvaluatorType() {
        return metric;
    }
};

#endif //TSDISCOVERY_NNSEARCHINDEX_H
