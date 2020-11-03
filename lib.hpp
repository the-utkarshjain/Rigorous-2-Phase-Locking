//User-defined header file contaning all the class and function definition

#ifndef _lib_hpp
#define _lib_hpp

#include <bits/stdc++.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
using namespace std;

class transaction
{
    /*
    Class to store:
    1. id: transaction ID of each thread.
    2. buffer: intermediate values of all the variables.
    3. read_lock_on: set of all variables under read-lock.
    4. write_lock_on: set of all variables under write-lock.
    */

public:
	int id;
	vector<string> statements;
    map<string, int> buffer;
    set<string> read_lock_on;
    set<string> write_lock_on;
};

class LockManager{

    /*
    Class to store:
    1. S: total number of variables.
    2. *wcnt, *waitw: array of active writers, waiting writers for each variable.
    3. *rcnt, *waitr: array of active readers, waiting readers for each variable.
    4. *condlock: array of mutex for each variable.
    5. *canread, *canwrite: array of condition_variable for each variable.
    
    Also, all the member functions are declared here. Definition in lockmanager.cpp.
    */

    int S;
    int *wcnt, *waitw, *rcnt, *waitr;
    mutex *condlock;
    condition_variable_any *canread;
    condition_variable_any *canwrite;

    public:
    LockManager (int count);
    void acquireReadLock(int txId, string var_name, int var_id);
    void acquireWriteLock(int txId, string var_name, int var_id);
    void upgradeToWrite(int txId, string var_name, int var_id);
    void releaseLock(int txId, string var_name, int var_id, char type_lock);
};

//Function declaration of readfile(). Definition in readfile.cpp.
void readfile(transaction **trans, int *n, map<string, int> *var_id, map<string, int> *variables, LockManager **lock);

#endif