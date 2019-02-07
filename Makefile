#set the compiler to g++
CXX = g++
CXXFLAGS =-std=c++11 -Wall
#set OBJS to all the object files
OBJS = Project1.o CPU.o
PROJECTNAME = project1
#all target
all: project1


# Backup Target
backup:clean
	@mkdir -p ~/backups; chmod 700 ~/backups
	@$(eval CURDIRNAME := $(shell basename `pwd`))
	@$(eval MKBKUPNAME := ~/backups/$(PROJECTNAME)-$(shell date +'%Y.%m.%d-%H:%M:%S').tar.gz)
	@echo
	@echo Writing Backup file to: $(MKBKUPNAME)
	@echo
	@-tar zcfv $(MKBKUPNAME) ../$(CURDIRNAME)
	@chmod 600 $(MKBKUPNAME)
	@echo
	@echo Done!


#program3 target, depends on OBJS
project1: $(OBJS)
	$(CXX) $(CXXFLAGS) -o project1 $(OBJS)

#OBJS target, depends on .cc files
#Project1.o: Project1.cc
#	$(CXX)  -c -o Project1.o Project1.cc 
#CPU.o: CPU.cc
#	$(CXX) -c -o CPU.o CPU.cc
#CPUProcessing.o: CPUProcessing.cc
#	$(CXX) -c -o CPUProcessing.o  CPUProcessing.cc
#fetchInstructions.o: fetchInstructions.cc
#	$(CXX) -c -o fetchInstructions.o fetchInstructions.cc

#clean target
clean:
	rm -f $(OBJS) project1 *~ 
