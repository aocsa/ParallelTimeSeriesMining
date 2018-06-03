//
// Created by aocsa on 6/2/18.
//

#ifndef TSDISCOVERY_CUDAHASHFILE_H
#define TSDISCOVERY_CUDAHASHFILE_H

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>

#include "NNSearchIndex.h"

using namespace std;

typedef unsigned int id_type;
typedef char byte;
typedef unsigned int uint;
typedef std::vector<float> DATA_NODE_T;

struct iElement {
    uint  id;
    double  hash_value;

    bool operator < (const iElement & b) const {
        return hash_value < b.hash_value;
    }
    bool operator == (const iElement & b) const {
        return hash_value == b.hash_value;
    }
};
class CUDAHashFile : public NNSearchIndex
{
    typedef tObject DATA_NODE_T;

    const static int MAX_TRY = 10;

    const static int B = 10000;
    //const static int DIM = 51;
    const static int W = 200;

    uint m_dim;

    int m_nLevels;
    std::vector<std::vector<iElement> > indexes;
    std::vector< std::vector<int> > hashfunction;//[N_HASH_FUNCTIONS][DIM];

    tDataset dataset;



public:
    CUDAHashFile(MetricEvaluator *_metric, int nLevels, int dim)
            : NNSearchIndex(_metric), hashfunction(nLevels, std::vector<int>(dim)), indexes(nLevels)
    {
        m_dim = dim;
        m_nLevels = nLevels;
    }

    virtual void Init()
    {

    }

    virtual void BulkLoad(const tDataset &S) {
        dataset = S;

        for (int i = 0; i < m_nLevels; i++)
        {
            std::vector<iElement> &index = indexes[i];
            createNewHashFunc(dataset, hashfunction[i]);

            for (int j = 0; j < dataset.size(); j++) {
                double window_value = getWindowValue(hashfunction[i], dataset[j]);
                iElement b;
                b.id = j;
                b.hash_value = window_value;
                index.push_back(b);
            }
            std::sort(index.begin(), index.end());
        }
    }

    tResult NearestQuery_BF(tObject& query, int k) {
        tResult resultSet;
        resultSet.SetQueryInfo(query, tResult::KNEARESTQUERY, k, MAXDOUBLE, true);
        int tam_dataset = dataset.size();
        double rangeK = MAXDOUBLE;
        for (int i = 0; i < tam_dataset; i++) {
            double dist = this->GetEvaluatorType()->GetDistance(dataset[i], query);
            if (dist < rangeK) {
                //cout << dataset[obj_id] << endl;
                resultSet.AddPair(dataset[i], dist);
                if (resultSet.GetNumOfEntries() >= k) {
                    //cut if there is more than k elements
                    resultSet.Cut(k);
                    //may I use this for performance?
                    rangeK = resultSet.GetMaximumDistance();
                }
            }
        }

        return resultSet;
    }

    virtual  tResult NearestQuery(tObject& query, int k) {
        tResult resultSet;
        resultSet.SetQueryInfo(query, tResult::KNEARESTQUERY, k, MAXDOUBLE, true);
        std::vector<bool> visited(dataset.size(), false);

        double rangeK = MAXDOUBLE;
        for (int idx = 0; idx < m_nLevels; idx++)
        {
            std::vector<iElement> &index = indexes[idx];

            double real_hash = getWindowValue(hashfunction[idx], query);
            iElement q;
            q.hash_value = real_hash;
            //cout << "idx:" << idx <<" real_hash "<< real_hash<< endl;
            //int L_q = (real_hash - rangeK - 1);
            //int H_q = (real_hash + rangeK + 1);

            std::vector<iElement>::iterator iter_begin = std::find(index.begin(), index.end(), q);

            int lambdaParam = 500;
            int BEGIN = (iter_begin - index.begin()) - lambdaParam;
            int END = (iter_begin - index.begin()) + lambdaParam;
            for (int i = BEGIN; i <= END; i++) {
                //cout << "i BEGIN:" << i << endl;
                int bucket_idx = i;
                if (bucket_idx >= 0 && bucket_idx < dataset.size()) {
                    int obj_id = index[bucket_idx].id;
                    if (visited[obj_id] == false) {
                        //double cdist = abs(index[obj_id].hash_value - real_hash) - 1; // it works? i dont know! may be for integers
                        //if ( cdist < rangeK) {
                        double dist = this->GetEvaluatorType()->GetDistance(dataset[obj_id], query);
                        //cout << "dist: "<<dist << endl;
                        if (dist < rangeK) {
                            //cout << dataset[obj_id] << endl;
                            resultSet.AddPair(dataset[obj_id], dist);
                            if (resultSet.GetNumOfEntries() >= k) {
                                //cut if there is more than k elements
                                resultSet.Cut(k);
                                //may I use this for performance?
                                rangeK = resultSet.GetMaximumDistance();
                            }
                        }
                        //}
                        visited[obj_id] = true;
                    }
                }
            }
        }
        //cout << "Salio del for" << endl;
        return resultSet;
    }

    virtual  tResult RangeQuery(const tObject& query, double range) {
        tResult r;
        return r;
    }


private:

    double getHashValue(std::vector<int>& hash_func, DATA_NODE_T & feature)
    {
        double hash_val = 0;
        for (id_type i = 0; i < m_dim; i++) {
            hash_val += feature[i] * (2 * hash_func[i] - 1);
        }
        return hash_val;
    }
    double getWindowValue(double hash_value)
    {
        return hash_value;
    }

    double getWindowValue(std::vector<int>& hash_func, DATA_NODE_T & feature)
    {
        double hash_value = getHashValue(hash_func, feature);
        //cout<<"hash value : "<<getWindowValue(hash_value)<<endl;
        return hash_value;//getWindowValue(hash_value);
    }
    bool createNewHashFunc(tDataset& dataset, std::vector<int> & hash_func)
    {
        //cout<<"data num : "<<data_num<<endl;
        std::vector<int> tmp_best(m_dim);
        DATA_NODE_T  data;
        double sum, cur_var, pre_var;
        sum = cur_var = pre_var = -1.0;
        size_t max_bin = 1;

        for (int t = 0; t < MAX_TRY; t++) {
            map<double, int> bin;
            for (id_type i = 0; i < m_dim; i++) {
                tmp_best[i] = rand() % 2;
            }

            for (int i = 0; i < dataset.size(); i++) {
                double window_value = getWindowValue(tmp_best, dataset[i]);
                bin[window_value]++;
            }

            map<double, int>::iterator iter;
            cur_var = sum = 0.0;
            for (iter = bin.begin(); iter != bin.end(); iter++) {
                sum += iter->first*iter->second;
            }
            sum /= dataset.size();
            for (iter = bin.begin(); iter != bin.end(); iter++) {
                cur_var += pow(iter->second * abs(iter->first - sum), 2.0);
            }

            if (cur_var > pre_var) {
                pre_var = cur_var;
                hash_func = tmp_best;
            }

            if (bin.size() > max_bin) {
                max_bin = bin.size();
            }

        }
        return max_bin != 1;
    }

};
//bool success = createNewHashFunc(page, child->total_data_num, child->hash_func);

//std::vector<bucket> index;

#endif //TSDISCOVERY_CUDAHASHFILE_H
