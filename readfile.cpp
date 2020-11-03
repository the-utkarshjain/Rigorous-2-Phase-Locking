/*
Function definition of readfile. 
Function declaration in lib.hpp.
*/

#include "lib.hpp"

void readfile(transaction **trans, int *n, map<string, int> *var_id, map<string, int> *variables, LockManager **lock)
{	
	/* 
    Function to release all the locks held by calling thread. 
    
    @params:
    1. **trans: Double pointer to a transaction array.
    2. *n: Pointer to a value storing number of transactions.
    3. *var_id: Pointer to a map storing ID of all variables.
    4. *variables: Pointer to a map storing the variables and their values.
	5. **lock: Double pointer to the lock manager.
    @return: None
    */

	fstream newfile;
	newfile.open("input.txt", ios::in);

	if (newfile.is_open())
	{
		string tp;

		//The first line to get number of transactions
		getline(newfile, tp);
		stringstream geek(tp);
		geek >> *n;
		(*trans) = new transaction[*n];

        //The second line to get all the variables
		getline(newfile, tp);
		stringstream var(tp);
		string intermediate;
		vector <string> tokens;

		while (getline(var, intermediate, ' '))
		{
			tokens.push_back(intermediate);
		}

        //Storing all variables with their ids and values
		int id = 0;
		for(int i=0; i<tokens.size(); i = i+2)
		{
			(*var_id).insert({tokens[i], id});
			(*variables).insert({tokens[i], stoi(tokens[i+1])});
			id++;
		}

        *lock = new LockManager(id);
        
        //Storing all commands
		int index;
		while(getline(newfile, tp))
		{
			if(isdigit(tp[0]))
			{
				index = stoi(tp) - 1;
				(*trans)[index].id = index + 1;
			}
			else
			{   
				(*trans)[index].statements.push_back(tp);
			}
		}
	}

	newfile.close();
}