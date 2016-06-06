// This file defines an interface for the 'Experiment' class, which is represented by a series of 'Evidence' objects.

#include "Evidence.h" //Include the experimental 'Evidence' class
#include <vector>

class Experiment
{
    std::vector<Evidence> dataset; //An experiment should contain a set of evidence data collected under similar circumstances/conditions.
    
  public:
    Experiment() {}

    //Adds a trial to the dataset. Trial order is immaterial.
    void addTrial(const Evidence& trial) { dataset.push_back(trial); }
   
    //Produces an indication of the relative probability of each evidence statement.
    std::vector<double> summary();
    
    //Returns the experiment without trials which give conflicting results or results below a certain probability
    Experiment outlierCull(double tolerance=0.0);
    
    //Adds trials from two experiments to form a larger experiment context
    Experiment operator+(const Experiment& add)
    {   Experiment local(add); 
        local.dataset.insert(this->dataset.cend());
        return local;
    }

    //Add trials from another experiment object to this one
    Experiment& operator+=(const Experiment& add)
    {   dataset.insert(this->dataset.cend(),add.dataset);
        return *this;
    }

    unsigned int size() { return dataset.size(); }
};

std::vector<double> Experiment::summary() {
    std::vector<double> summary;
    for(int j=0;j<(int)trait_count;j++){
        for(int i=0;i<dataset.size();i++){
            if(dataset[i].isTrue(j)){
                summary[j]++;
            }
            else if(dataset[i].isFalse(j)){
                summary[j]--;
            }
        }
        summary[j]/=dataset.size(); //Each summary element is the positive results minus the negative results divided by number of trials.
                                      //1 indicates true for all trials, -1 false for all trials.
    }
}

Experiment Experiment::outlierCull(double tolerance) { //Removes Evidence elements with an improbably high number of inaccurate or conflicting results.
                                                       //O removes only evidence with conflicting results, 1 will remove all evidence not in agreement with the majority.
    Experiment culled;
    std::vector<double> summary=this->summary();
    for(int i=0;i<dataset.size();i++){
        double agree=(int)trait_count;
        for(int j=0;j<(int)trait_count;j++){
            if(dataset[j].isConflicting(i)){
                agree=-2*(int)trait_count; //Throw out any Evidence element with a conflicting result
                break;
            }
            if(dataset[j].isTrue(i)){ //Agreement value is the local result times the summary value
                agree+=summary[i];
            }
            else if(dataset[j].isFalse(i)){
                agree-=summary[i];
            }
        }
        agree/=(int)trait_count;
        if(agree>=tolerance){
            culled.addTrial(this->dataset[i]);
        }
    }
    return culled;
}

std::vector<double> Diff(Experiment base,Experiment contrast){ //Highlights the differences in the probability summary of two different experiments
    std::vector<double> diff;
    for(int i=0;i<(int)trait_count;i++){
        diff[i]=base.summary()[i]-contrast.summary()[i];
    }
    return diff;
}

std::vector<double> StdDev(std::vector<Experiment> exps) { //Returns the standard deviation in probability summary of several different experiments, weighting by trial count
    vector<double> dev;
    for(j=0;j<(int)trait_count;j++){
        vector<double> avg=0;
        for(i=0;i<exps.size();i++){
            avg+=exps[i].summary()[j];
        }
        avg[j]/=exps.size();
    }
    for(j=0;j<(int)trait_count;j++){
        vector<unsigned int> size;
        for(i=0;i<exps.size();i++){
            size[j]+=exps[i].size();
            dev[j]+=(avg[j]-exps[i].summary()[j])*(avg[j]-exps[i].summary()[j])*exps[i].size();
        }
        dev[j]/=size[j];
    }
    return dev;
}

std::vector<double> AStdDev(vector<Experiment> exps) { //Takes the standard deviation in probability summary of several different experiments WITHOUT weighting by trial count, assuming each experiment as a whole is equally valid.
    vector<double> dev;
    for(j=0;j<(int)trait_count;j++){
        vector<double> avg=0;
        for(i=0;i<exps.size();i++){
            avg+=exps[i].summary()[j];
        }
        avg[j]/=exps.size();
    }
    for(j=0;j<(int)trait_count;j++){
        for(i=0;i<exps.size();i++){
            dev[j]+=(avg[j]-exps[i].summary()[j])*(avg[j]-exps[i].summary()[j]);
        }
        dev[j]/=exps.size();
    }
    return dev;
}
