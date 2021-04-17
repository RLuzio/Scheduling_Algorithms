#include <iostream>
#include <fstream>
#include <string>
#include <vector>


double fcfs_algorithm(std::vector<int> list) {
  int completion_time = 0, wait_time = 0;

  for(int i = 0; i < list.size(); ++i) {
    wait_time += completion_time;
    completion_time += list[i];
  }

  return wait_time / list.size();
}


double sjf_algorithm(std::vector<int> list) {
  int tmp, index, completion_time = 0, wait_time = 0, size = list.size();

  while(!list.empty()) {
    tmp = list[0];
    index = 0;

    for(int i = 0; i < list.size(); ++i) {
      if(list[i] < tmp) {
        index = i;
        tmp = list[i];
      }
    }

    wait_time += completion_time;
    completion_time += tmp;

    list.erase(list.begin() + index);
  }

  return wait_time / size;
}


double rr_algorithm(std::vector<int> list) {
  int tmp, sum = 0, completion_time = 0, wait_time = 0, size = list.size();
  bool all_zero = 0;
  std::vector<int> copy = list;
  std::vector<int> orig_time;
  std::vector<int> comp_time;

  while(!all_zero) {  //go through list
     for(int j = 0; j < list.size(); ++j) { //use for indexes
       if(list[j] != 0) {
         tmp = list[j]; // access burst time in list to decrement

         for(int i = 0; i < 10 && tmp != 0; ++i) { //While we can decrement in quantums of 10
           ++completion_time;  //increment completion time
           --tmp;

           if(tmp == 0) {  //if the burst time is 0 before loop ends, then do below
             //std::cout << completion_time << " " << copy[j] << std::endl;
             comp_time.push_back(completion_time); //Push the current completion time into a vector
             orig_time.push_back(copy[j]); //Push the original burst time into another vector
           }
         }
         list[j] = tmp; // set current index to new burst time
       }
    }
    for(int k = 0; k < list.size(); ++k) {  // Loop to check if all values are 0
      if(list[k] > 0) { //If we find one value in list that is greater than zero...
        all_zero = 0; // set flag to false and break
        break;
      }
      all_zero = 1; // If all values in list are 0, set flag to 1
    }
  }

  for(int p = 0; p < list.size(); ++p) {  // Loop through any now useless vector (list or copy) to store the completion times
    list[p] = comp_time[p] - orig_time[p];  // Set the completion time
    sum += list[p]; // Sum all completion times
  }

  return sum / size;  // return the average waiting time
}


int main(int argc, char* argv[]) {
  std::vector<int> burst_time;
  double fcfs, sjf, rr;
  int data;
  std::string tmp;
  std::ifstream fin;

  if(argc < 2) {
    std::cout << "USAGE ERROR: Please use as so: " << "./program file.txt" << std::endl;
    exit(1);
  }

  fin.open(argv[1]);
  if(fin.is_open()) {
    while(!fin.eof()) {
      fin >> tmp >> data;
      burst_time.push_back(data);
    }
    burst_time.pop_back();
  }


  fcfs = fcfs_algorithm(burst_time);
  std::cout << "FCFC avg wait time: " << fcfs << std::endl;
  sjf = sjf_algorithm(burst_time);
  std::cout << "SJF avg wait time: " << sjf << std::endl;
  rr = rr_algorithm(burst_time);
  std::cout << "RR avg wait time: " << rr << std::endl;

  if(fcfs < sjf && fcfs < rr) {
    std::cout << "FCFS has the least minimum average waiting time of: " << fcfs << std::endl;
  }
  else if(sjf < fcfs && sjf < rr) {
    std::cout << "SJF has the least minimum average waiting time of: " << sjf << std::endl;
  }
  else if(rr < sjf && rr < fcfs) {
    std::cout << "RR has the least minimum average waiting time: " << rr << std::endl;
  }
  else {
    std::cout << "ERROR" << std::endl;
  }

  fin.close();
  return 0;
}
