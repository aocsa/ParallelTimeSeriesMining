//
// Created by aocsa on 6/2/18.
//

// LocalOutlayerFactor.cpp: define el punto de entrada de la aplicaci�n de consola.
//

#include <iostream>
#include <fstream>
#include <ctime>
#include <functional>
#include <algorithm>    // std::reverse
#include <cstdio>

#include "OutlierDetection/ResultSet.h"
#include "OutlierDetection/CUDAHashFile.h"

using namespace std;

void	loadData(std::string datafilepath, std::vector< TimeSeries >  &dataset) {
    std::ifstream ifs(datafilepath.c_str());
    if (ifs.is_open()) {
        cout << "read " << datafilepath << endl;
        std::string line;
        int iter = 0;

        while (getline(ifs, line))
        {
            TimeSeries ts;
            double value;

            std::stringstream ss(line);
            while (ss >> value) {
                ts.push_back(value);
            }
            ts.SetID(iter);
            dataset.push_back(ts);
            iter++;
        }
    }
    else {
        cout << "Error opening file " << datafilepath<<endl;
    }


}

void usage(char *programName) {
    printf("Usage: %s  dimension #neighbors  distaceType[L1=0|L2=1] nlevels BF|LSH data_set_file\n", programName);
}

double get_lrd(CUDAHashFile::tResult& result_p, std::vector<double>& distances, int nNN)
{
    double lrd_p = 0.0;

    // compute lrd
    for (int j = 1; j < result_p.GetNumOfEntries(); j++) {
        int o_id = result_p[j].GetObject().GetID();
        lrd_p += std::max(distances[o_id], result_p[j].GetDistance());
    }
    lrd_p = (double)nNN / lrd_p;
    return lrd_p;
}

string getNameFile(string path) {
    string output;
    for (size_t i = path.size() - 1; i > 0; i--) {
        if (int(path[i]) == 47 || int(path[i]) == 92) {
            break;
        }
        output.push_back(path[i]);
    }
    reverse(output.begin(), output.end());
    return output;
}

int main(int nargs, char *args[])
{
    if (nargs < 6) {
        usage(args[0]);
        //cin.get();//como Pause
        exit(1);
    }

    int dimension =  atoi(args[1]);
    int nNN = atoi(args[2]);

    int distanceType = atof(args[3]);
    int nLevels = atoi(args[4]);

    bool is_LSH = false;
    if (string(args[5]) == ("LSH")) {
        is_LSH = true;
    }
    std::string db =  args[6];

    std::vector< TimeSeries > queries;
    std::vector< TimeSeries > dataset;
    loadData(db, dataset);

    int nOutliers = 20;
    //printf("%d %d %d %d \n", dimension, nNN, distanceType, nLevels);
    MetricEvaluator* metric;
    if (distanceType == 0)
        metric = new L1Distance();  //manhatam
    else
        metric = new L2Distance(); //euclidean

    CUDAHashFile index(metric, nLevels, dimension);
    //iSAXIndex index(metric, dimension);
    index.BulkLoad(dataset);

    double meanQueryTime = 0.0f;
    clock_t begin = clock();
    CUDAHashFile::tResult motif;
    //double best_so_far = MAXDOUBLE;
    typedef std::map<double, int, std::greater<double> > counter_type;
    counter_type outliers;
    std::vector<double> distances(dataset.size());
    nNN = nNN + 1;
    cout << "Llego 1 "<< "dataset.size "<< dataset.size()  << endl;
    std::vector< CUDAHashFile::tResult  > results(dataset.size());
    for (int i = 0; i < dataset.size(); i++) {
        //cout << dataset[i].GetID() << endl;

        CUDAHashFile::tResult result;
        if (is_LSH) {
            result = index.NearestQuery(dataset[i], nNN);
        }
        else {
            result = index.NearestQuery_BF(dataset[i], nNN);
        }

        distances[i] = result.GetMaximumDistance();
        results[i] = result;

        //cout <<" "<< distances[i] <<" ";
    }
    cout << "Llego 2" << endl;
    for (int i = 0; i < dataset.size(); i++) {
        CUDAHashFile::tResult& result = results[i];
        double lrd_p = get_lrd(result, distances, nNN);

        double lof = 0.0;
        //compute LOF
        for (int j = 1; j < nNN; j++) {
            //int o_id = result[j].GetObject().GetID();
            int o_id = j;
            double lrd_o = get_lrd(results[o_id], distances, nNN);
            lof += lrd_o / lrd_p;
        }
        lof /= nNN;
        outliers[lof] = i;

    }
    clock_t end = clock();
    double time = (end - begin) / 1000.0;
    //save in file
    char buffer_tmp[100];
    string nameFile = getNameFile(db) + "." + string(args[5]);
    ofstream save_file(nameFile.c_str());
    if (dataset.size() > 0) {
        sprintf(buffer_tmp,"Mean top-k outliers time exec: %0.6lf\n", (double)time);
        printf(buffer_tmp);
        save_file << buffer_tmp;
    }
    printf("***Outlier***\n");
    save_file << "***Outlier***\n";
    counter_type::iterator iter = outliers.begin();
    for (int i = 0; i < nOutliers && iter != outliers.end(); i++, iter++) {
        sprintf(buffer_tmp, "%09d  \tdist: %0.6lf\n", iter->second, iter->first);
        printf("%09d  \tdist: %0.6lf\n", iter->second, iter->first);
        save_file << buffer_tmp;
    }
    printf("Number of Distance Computations:%d\n", index.GetEvaluatorType()->GetDistanceCount());
    save_file << "Number of Distance Computations:" << index.GetEvaluatorType()->GetDistanceCount() << endl;
    delete metric;
    save_file.close();
    //cin.get();//como Pause
    return 0;
}

