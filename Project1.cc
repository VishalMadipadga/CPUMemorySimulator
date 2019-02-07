#include "Project1.h"

using namespace std;

//declaring two pipes to be used
int pfds[2];
int ps[2];

int main(int argc, char* argv[])
{ 
  int writeFlag = 32767;
  string fileName = argv[1]; //set filename to first command line arg
  int instructions = atoi(argv[2]); //set number of instructions to second command line arg

  int validate; //variable to validate pipe and fork

  validate = pipe(pfds); 
  if(validate == -1) //1st pipe failed to open
    {
      cout << "Pipe failed, exiting program." << endl;
      _exit(0);
    }
   validate = pipe(ps);
   if(validate == -1) //2nd pipe failed to open
       {
         cout << "Pipe failed, exiting program." << endl;
	 _exit(0);
       
       }


  validate = fork();
  if(validate == -1) //fork failed
    {
      cout << "Fork failed, exiting program." << endl;
      _exit(0);
    }
    
    if (validate == 0) //child process, contains main memory
       {
	 int memory [2000]; //main memory array
	 ifstream inputFile; //input file stream
	 inputFile.open(fileName.c_str()); //open file stored in filenmae
	 int address;
	 int getAC;
	  
	 //if the file is open, FILE PARSING HERE
	 if(inputFile.is_open())
	   {
	    string line;
	    int i = 0;
	    int val;
	    int loadVal;

             while(getline(inputFile >> std::ws, line)) //parsing file with instructions
	     	  {

		  // if the first element of line is a space or contains a /, continue
		  if (line[0] == ' ' || line[0] == '/')
		     {
			continue;

		     }
		   //find .
		   std::string::size_type m = line.find(".");
		   if( m != std::string::npos)
		     {
		     //erase the .
		       line.erase(std::remove(line.begin(), line.end(), '.'), line.end());  
		       //read line through as a stream
                       istringstream ss(line);
		       //read from stringstream object to int variable
		       ss >> loadVal;
		       //set index i to loadVal
		       i = loadVal;
		       //continue
                       continue;
		     }

                   //find // in line
                   std::string::size_type n = line.find(" //");
		    if( n != std::string::npos)
		    {
		    //erase it
		      line.erase (n);
		     
                    }
		    //read line as a stream
		    std::istringstream iss(line);
                    //read stringstream object into int variable
		    iss >> val;
		    //store variable in memory
		    memory[i] = val;
		    //increment i to next index in memory array
		    i++;
		  
		  }
             
	   }
	 else 
	  {
             cout << "Can't open file." << endl;
	     _exit(0);

	  }
           
	  //close file
	  inputFile.close(); 
	  //while reading from pfds pipe, MAIN MEMORY OPERATIONS HERE
          while (read(pfds[0], &address, sizeof(int)) >0)
	  {
	  //close write end of pipe
	   close(pfds[1]);
	   //if address is writeFlag
          if(address == writeFlag)
	    {
	      if (read(pfds[0], &address, sizeof(int)) == -1) //read address to store data in 
		_exit(0);
              
	      if (read(pfds[0], &getAC, sizeof(int)) == -1) //read data to store
                _exit(0);
	      memory[address] = getAC; //store data at address in memory array
	     continue;  

	    }
	   //otherwise write data memory address requested
           write(ps[1], &memory[address], sizeof(int));
	      close(ps[0]);
         
        }
       }

    if (validate > 0)  //parent process, contains CPU
    	{
	 //create CPU object
         CPU *cpu = new CPU(instructions);
	 //call run CPU function
	 cpu->runCPU();
	
	}

    }
