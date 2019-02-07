#include "Project1.h"

//constructor, intalize instructions till interrupt
CPU::CPU(int instructions)
{
 this->instructions = instructions;
}

//entering kernel mode
void CPU::enter(int interruptHandler)
{
//set kernelMode flag to true, we are in kernel mode
  kernelMode = true;
  int tempAddress = 1999; //address where we will store SP
  if(write(pfds[1], &writeFlag, sizeof(int)) == -1) //signal to memory that we're writing
    _exit(0);
  if(write(pfds[1], &tempAddress, sizeof(int)) == -1) //write address we're storing SP in
    _exit(0);
  if(write(pfds[1], &SP, sizeof(int)) == -1) //write SP to memory
    _exit(0);
  tempAddress--;   //address where we will store PC
  if(write(pfds[1], &writeFlag, sizeof(int)) == -1) //signal to memory that we are writing
    _exit(0);
  if(write(pfds[1], &tempAddress, sizeof(int)) == -1) //write address we're storing PC in
    _exit(0);
  if(write(pfds[1], &PC, sizeof(int)) == -1) //write PC to memory
    _exit(0);
  SP = tempAddress;	//set SP to 1998
  PC = interruptHandler; //set PC to address of interrupt handler
}

//exit kernel mode
void CPU::exit()
{
  PC = pop(); //pop PC from system stack
  SP = pop(); //pop SP from system stack
  kernelMode = false; //set kernel mode flag to false

}

//send a request via pipe to get data from memory
int CPU::requestMemory(int address)
{
  int medium; //buffer variable
  //if we are not in kernel mode CPU requests access to data at address more than 1000
  if(!kernelMode && address >= 1000)
    {
     //print error for trying to access system data
     cout << "Memory violation: accessing system address " <<  address << "  in user mode" << endl;
     _exit(0);
    }
  write(pfds[1], &address, sizeof(int)); //get contents of memory at location in PC
  close(pfds[0]);
  read(ps[0], &medium, sizeof(int)); //write value from address into variable
  close(ps[1]);
  return medium; //return value from memory

}

//pop value from stack
int CPU::pop()
{
 popVar = requestMemory(SP++); //read from memory location indexed by the stack pointer
 return popVar;   //return that value

}

//push data onto stack
void CPU::push(int data)
{
   SP--;  //decrement Stack Pointer (SP) 
   if(write(pfds[1], &writeFlag, sizeof(int)) == -1) //signal to memory that we are writing
   _exit(0);
   if(write(pfds[1], &SP, sizeof(int)) == -1) //write the address that we still store data in (SP)
   _exit(0);
   if(write(pfds[1], &data, sizeof(int)) == -1) //write data that we are storing
   _exit(0);
   

}

//process instructions
void CPU::instructionProcessing(int instruction)
{
     switch(instruction)
     {
     	//load value into AC
	case 1:
		buffer = requestMemory(PC++);
		AC = buffer;
		break;
	//load value at address into AC
	case 2:
		buffer = requestMemory(PC++);
                buffer = requestMemory(buffer);
		AC = buffer;
		break;
	//load value from address found at given address
	case 3:
		buffer = requestMemory(PC++);
		buffer = requestMemory(buffer);
		buffer = requestMemory(buffer);
		AC = buffer;
		break;
	//load value at address + X
	case 4:
		buffer = requestMemory(PC++);
		buffer = buffer + X;
		buffer = requestMemory(buffer);
		AC = buffer;
		break;
	//load value at address + Y
	case 5:
		buffer = requestMemory(PC++);
		buffer = buffer + Y;
		buffer = requestMemory(buffer);
		AC = buffer;
		break;
	//load value at address SP + X
	case 6:
		buffer = SP + X;
		buffer = requestMemory(buffer);
		AC = buffer;
		break;
	//store value in AC into address
	case 7:
		buffer = requestMemory(PC++);
                if(write(pfds[1], &writeFlag, sizeof(int)) == -1) //signal to memory that we are writing
		_exit(0);
                if(write(pfds[1], &buffer, sizeof(int)) == -1) //write address that we will store data in
		_exit(0);
		if(write(pfds[1], &AC, sizeof(int)) == -1) //write data that we will store
		_exit(0);
		break;
	//get random int into AC
	case 8:
		 random = rand() % 100+1;
		 AC = random;
		 break;
	//if port = 1 print AC as int if port = 2 print AC as char
	case 9:
		port = requestMemory(PC++);
		if(port == 1)
		{
		  cout << AC;
		  cout.flush();
		}
		if(port == 2)
		{
		  cout << char(AC);
		  cout.flush();
		}
		break;
	//increment AC by X
	case 10:
		AC += X;
		break;
	//increment AC by Y
	case 11:
		AC += Y;
		break;
	//decrement AC by X
	case 12:
		AC -= X;
		break;
	//decrement AC by Y
	case 13:
		AC -= Y;
		break;
	//set X to AC
	case 14:
		X = AC;
		break;
	//set AC to X
	case 15:
		AC = X;
		break;
	//set Y to AC
	case 16:
		Y = AC;
		break;
	//set AC to Y
	case 17:
		AC = Y;
		break;
	//set SP to AC
	case 18:
		SP = AC;
		break;
	//set AC to SP
	case 19:
		AC = SP;
		break;
	//jump to address
	case 20:
		buffer = requestMemory(PC++);
		PC = buffer;
		break;
	//if AC is 0 jump to address
	case 21:
		if(AC == 0)
		{
		   buffer = requestMemory(PC++);
		   PC = buffer;
		}
		else
		 PC++;
		break;
	//if AC is not 0, jump to address
	case 22:
	        if(AC != 0)
	        {
	          buffer = requestMemory(PC++);
		  PC = buffer;
		}
		else 
		  PC++;

		break;
	//push return address onto stack, jump to address
	case 23:
		buffer = requestMemory(PC++);
		push(PC);
		PC = buffer;
		break;
	//pop return address from stack, jump to address
	case 24:
		PC = pop();
		break;
	//increment X
	case 25:
		X++;
		break;
	//decrement X
	case 26:
		X--;
		break;
	//push AC onto Stack
	case 27:
		push(AC);
		break;
	//pop AC from stack into AC
	case 28:
		AC = pop();
		break;
	//interrupt, execute instruction at 1500
	case 29:
	        enter(1500);
		break;
	//return from interrupt
	case 30:
	        exit();
		break;
	//exit program
	case 50:
		running = false;
		_exit(0);
		break;
     }
  

}

void CPU::runCPU()
{
  while(running)
  {
    if(instructionCounter <  instructions)
    {
     IR = requestMemory(PC++); //get instruction from memory and store in IR
     instructionProcessing(IR);	//process instruction in IR
     if(!kernelMode) //only keep track of instructions when not in kernel mode
     {
	instructionCounter++;
     }

    }
    else
    {
      instructionCounter = 0; //restart instructionCounter
      enter(1000);      //enter kernel mode at address 1000
    }
  }
 
  _exit(0);

}
