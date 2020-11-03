#include "lib.hpp"

LockManager::LockManager (int count)
{   
    /* 
    Constructor for Lock Manager. 
    Allocates locks, cvs and different reader-writer variables.
    @params:
    1. count: The total number of variables in the database.
    @return: None
    */

    S = count;
    wcnt = new int[S]();
    waitw = new int[S]();
    rcnt = new int[S]();
    waitr = new int[S]();

    condlock = new mutex[S];
    canread = new condition_variable_any[S];
    canwrite = new condition_variable_any[S];
}

void LockManager::acquireReadLock(int txId, string var_name, int var_id)
{
    /* 
    Function to acquire read-lock. 
    
    @params:
    1. txId: TransactionId of the calling thread.
    2. var_name: Name of the variable to be locked.
    3. var_id: Id of the var_name.
    @return: None
    */

    condlock[var_id].lock();

    if (wcnt[var_id] == 1 || waitw[var_id] > 0)
    {
        waitr[var_id]++;

        cout << "Wait_R-Lock["<< txId<< ", "<< var_name<< "]\n";
        while (wcnt[var_id] == 1 || waitw[var_id] > 0)
            canread[var_id].wait(condlock[var_id]);

        waitr[var_id]--;
    }

    rcnt[var_id]++;
    cout << "R-Lock["<< txId<< ", "<< var_name<< "]\n";
    condlock[var_id].unlock();
    canread[var_id].notify_all();
}

void LockManager::acquireWriteLock(int txId, string var_name, int var_id)
{   
    /* 
    Function to acquire write-lock. 
    
    @params:
    1. txId: TransactionId of the calling thread.
    2. var_name: Name of the variable to be locked.
    3. var_id: Id of the var_name.
    @return: None
    */

    condlock[var_id].lock();

    if (wcnt[var_id] == 1 || rcnt[var_id] > 0)
    {
        ++waitw[var_id];

        cout << "Wait_W-Lock["<< txId<< ", "<< var_name<< "]\n";
        while (wcnt[var_id] == 1 || rcnt[var_id] > 0)
            canwrite[var_id].wait(condlock[var_id]);

        --waitw[var_id];
    }

    wcnt[var_id] = 1;
    cout << "W-Lock["<< txId<< ", "<< var_name<< "]\n";
    condlock[var_id].unlock();
}

void LockManager::upgradeToWrite(int txId, string var_name, int var_id)
{
    /* 
    Function to upgrade read-lock to write-lock. 
    
    @params:
    1. txId: TransactionId of the calling thread.
    2. var_name: Name of the variable to be locked.
    3. var_id: Id of the var_name.
    @return: None
    */

    condlock[var_id].lock();
    --rcnt[var_id];

    if (wcnt[var_id] == 1 || rcnt[var_id] > 0)
    {
        ++waitw[var_id];

        cout << "Wait_W-Lock["<< txId<< ", "<< var_name<< "]\n";
        while (wcnt[var_id] == 1 || rcnt[var_id] > 0)
            canwrite[var_id].wait(condlock[var_id]);

        --waitw[var_id];
    }

    wcnt[var_id] = 1;
    cout << "upgrade["<< txId<< ", "<< var_name<< "]\n";
    condlock[var_id].unlock();
}

void LockManager::releaseLock(int txId, string var_name, int var_id, char type_lock)
{
    /* 
    Function to release all the locks held by calling thread. 
    
    @params:
    1. txId: TransactionId of the calling thread.
    2. var_name: Name of the variable to be locked.
    3. var_id: Id of the var_name.
    4. type_lock: Type (read or write lock) to be released.
    @return: None
    */

    if (type_lock == 'r')
    {
        condlock[var_id].lock();
        if (--rcnt[var_id] == 0)
            canwrite[var_id].notify_one();
        condlock[var_id].unlock();
    }
    else if (type_lock == 'w')
    {
        condlock[var_id].lock();
        wcnt[var_id] = 0;
        if (waitr[var_id] > 0)
            canread[var_id].notify_one();
        else
            canwrite[var_id].notify_one();
        condlock[var_id].unlock();
    }
}