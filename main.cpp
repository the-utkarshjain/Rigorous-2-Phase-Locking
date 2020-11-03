#include "lib.hpp"

transaction *trans;
LockManager *lockmanage;
map<string, int> var_id;
map<string, int> variables;
int n;
mutex l;

void execute_transaction(transaction t)
{	
    /*
    Entry function for each transaction.
    @params:
    1. t: A class containing information of each transaction.
    @returns: None.
    */

	for(auto i = t.statements.begin(); i != t.statements.end(); i++)
	{
		string command = *i;
        
		if(command[0] == 'R')
		{
			string varname = string(1,command[2]);
			lockmanage->acquireReadLock(t.id, varname, var_id[varname]);
			t.read_lock_on.insert(varname);
			t.buffer[varname] = variables[varname];
		}
		else if(command[0] == 'W')
		{	
			string varname = string(1,command[2]);

			if(find(t.read_lock_on.begin(), t.read_lock_on.end(), varname) != t.read_lock_on.end())
			{
				lockmanage->upgradeToWrite(t.id, varname, var_id[varname]);
				t.read_lock_on.erase(varname);
				t.write_lock_on.insert(varname);
			}
			else
			{
				lockmanage->acquireWriteLock(t.id, varname, var_id[varname]);
				t.write_lock_on.insert(varname);
			}
			
			for(auto i : t.buffer)
				variables[i.first] = t.buffer[i.first];

		}
		else if(command[0] == 'C' || command[0] == 'A')
		{
			for(auto itr = t.read_lock_on.begin(); itr != t.read_lock_on.end(); itr++)
				lockmanage->releaseLock(t.id, *itr, var_id[*itr], 'r');

			for(auto itr1 = t.write_lock_on.begin(); itr1 != t.write_lock_on.end(); itr1++)
				lockmanage->releaseLock(t.id, *itr1, var_id[*itr1], 'w');
		}
		else
		{	
			vector <string> tokens; 
			stringstream check1(command); 
			string intermediate;  

			while(getline(check1, intermediate, ' ')) 
			{ 
				tokens.push_back(intermediate); 
			} 

			string varname1 = tokens[0];
			string varname3 = tokens[4];
			string maths = tokens[3];

			int bias = -1;
			if(isdigit(varname3[0]))
				bias = stoi(varname3);
			
			if(maths[0] == '+')
			{
				if(bias == -1)
					t.buffer[varname1] = t.buffer[varname1] + t.buffer[varname3];
				else
					t.buffer[varname1] = t.buffer[varname1] + bias;
			}

			if(maths[0] == '-')
			{
				if(bias == -1)
					t.buffer[varname1] = t.buffer[varname1] - t.buffer[varname3];

				else
					t.buffer[varname1] = t.buffer[varname1] - bias;
			}
		}		
	}
}

int main()
{
	readfile(&trans, &n, &var_id, &variables, &lockmanage);

	thread *threads = new thread[n];

	for(int i = 0; i<n; i++)
		threads[i] = thread(execute_transaction, trans[i]);

	for(int i = 0; i<n ;i++)
		threads[i].join();

	for(auto i : variables)
			cout<<i.first << " "<< i.second <<" ";

	cout<<"\nSuccessfully executed all transactions.\n";
	return 0;
}