// This file defines an interface for the 'Experiment' class, which is represented by a series of 'Evidence' objects.

#include "evidence.hpp" //Include the experimental 'Evidence' class

class Experiment
{
    std::vector<Evidence> dataset; //An experiment should contain a set of evidence data collected under similar circumstances/conditions.
    
  public:
    Experiment() {}

    //Adds a trial to the dataset. Trial order is immaterial.
    void addTrial(const Evidence& trial) { dataset.push_back(trial); }
   
    //Produces an indication of the relative probability of each evidence statement.
    vector<double> summary();
    
    //Returns the experiment without trials which give conflicting results or results below a certain probability
    Experiment outlierCull(double tolerance=0.0);
    
    //Adds trials from two experiments to form a larger experiment context
    Experiment operator+(const Experiment& add)
    {   Experiment local(add); 
        local.dataset.insert(cend,this->dataset);
        return local;
    }

    //Add trials from another experiment object to this one
    Experiment& operator+=(const Experiment& add)
    {   dataset.insert(cend,add.dataset);
        return this;
    }

    unsigned int size() { return dataset.size(); }
};

vector<double> Experiment::summary() {
    vector<double> summary;
    for(j=0;j<(int)MyEnum::Max;j++){
        for(i=0;i<dataset.size();i++){
            if(dataset[i].isTrue(j)){
                summary[j]++;
            }
            elseif(dataset[i].isFalse(j)){
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
    vector<double> summary=this->summary();
    for(i=0;i<dataset.size();i++){
        double agree=(int)MyEnum::Max;
        for(j=0;j<(int)MyEnum::Max;j++){
            if(dataset[j].isConflicting(i)){
                agree=-2*(int)MyEnum::Max; //Throw out any Evidence element with a conflicting result
                break;
            }
            if(dataset[j].isTrue(i)){ //Agreement value is the local result times the summary value
                agree+=summary[i];
            }
            elseif(dataset[j].isFalse(i)){
                agree-=summary[i];
            }
        }
        agree/=(int)MyEnum::Max;
        if(agree>=tolerance){
            culled.addTrial(this->dataset[i]);
        }
    }
    return culled;
}

vector<double> Diff(Experiment base,Experiment contrast){ //Highlights the differences in the probability summary of two different experiments
    vector<double> diff;
    for(i=0;i<(int)MyEnum::Max;i++){
        diff[i]=base.summary()[i]-contrast.summary()[i];
    }
    return diff;
}

vector<double> StdDev(vector<Experiment> exps) { //Returns the standard deviation in probability summary of several different experiments, weighting by trial count
    vector<double> dev;
    for(j=0;j<(int)MyEnum::Max;j++){
        vector<double> avg=0;
        for(i=0;i<exps.size();i++){
            avg+=exps[i].summary()[j];
        }
        avg[j]/=exps.size();
    }
    for(j=0;j<(int)MyEnum::Max;j++){
        vector<unsigned int> size;
        for(i=0;i<exps.size();i++){
            size[j]+=exps[i].size();
            dev[j]+=(avg[j]-exps[i].summary()[j])*(avg[j]-exps[i].summary()[j])*exps[i].size();
        }
        dev[j]/=size[j];
    }
    return dev;
}

vector<double> AStdDev(vector<Experiment> exps) { //Takes the standard deviation in probability summary of several different experiments WITHOUT weighting by trial count, assuming each experiment as a whole is equally valid.
    vector<double> dev;
    for(j=0;j<(int)MyEnum::Max;j++){
        vector<double> avg=0;
        for(i=0;i<exps.size();i++){
            avg+=exps[i].summary()[j];
        }
        avg[j]/=exps.size();
    }
    for(j=0;j<(int)MyEnum::Max;j++){
        for(i=0;i<exps.size();i++){
            dev[j]+=(avg[j]-exps[i].summary()[j])*(avg[j]-exps[i].summary()[j]);
        }
        dev[j]/=exps.size();
    }
    return dev;
}
