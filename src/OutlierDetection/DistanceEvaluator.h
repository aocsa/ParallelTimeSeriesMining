//
// Created by aocsa on 6/2/18.
//

#ifndef TSDISCOVERY_DISTANCEEVALUATOR_H
#define TSDISCOVERY_DISTANCEEVALUATOR_H

#pragma once

#include <vector>
#include <cmath>

class TimeSeries : public std::vector<double> {

public:
    void SetID(int id) {
        ID = id;
    }

    int GetID() const {
        return ID;
    }

private:
    long ID;
};

struct TimeSeriesInstance {
    TimeSeries timeseries;
    int  position_in_db;
    TimeSeriesInstance(const TimeSeries &_timeseries, int  _position_in_db)
            : timeseries(_timeseries), position_in_db(_position_in_db)
    {

    }
};


class MetricEvaluator {
public:

    virtual double GetDistance(const TimeSeries &obj1, const TimeSeries & obj2) = 0;

    /**
    * Resets statistics.
    */
    void ResetStatistics() {
        distCount = 0;
    }//end ResetStatistics

    /**
    * Returns the number of distances performed.
    */
    int GetDistanceCount() {
        return distCount;
    }//end GetDistanceCount

protected:
    /**
    * Updates the distance counter by adding 1.
    */
    void UpdateDistanceCount() {
        distCount++;
    }//end UpdateDistanceCount

    /**
    * The distance counter itself.
    */
    int distCount;
};//end stMetricEvaluatorStatistics

class L1Distance : public MetricEvaluator {
public:

    double GetDistance(const TimeSeries &obj1, const TimeSeries & obj2) {
        double distance = 0.0f;
        for (int i = 0; i < obj1.size(); i++) {
            distance += fabs(obj2[i] - obj1[i]);
        }
        this->UpdateDistanceCount();
        return distance;
    }
};

class L2Distance : public MetricEvaluator {
public:

    double GetDistance(const TimeSeries &obj1, const TimeSeries & obj2) {
        double distance = 0.0f;
        for (int i = 0; i < obj1.size(); i++) {
            distance += pow(obj2[i] - obj1[i], 2);
        }
        this->UpdateDistanceCount();
        return sqrt(distance);
    }
};



#endif //TSDISCOVERY_DISTANCEEVALUATOR_H
