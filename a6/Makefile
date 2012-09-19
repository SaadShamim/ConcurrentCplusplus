OPT:=

CXX = u++					# compiler
CXXFLAGS = -g -Wall -Wno-unused-label -MMD ${OPT} 
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS2 = sodaDriver.o WATCardOffice.o WATCard.o NameServer.o Student.o WATCardOffice.o Bank.o Parent.o VendingMachine.o Truck.o BottlingPlant.o Printer.o config.o # list of object files 
EXEC2 = soda 

OBJECTS = ${OBJECTS2}		# all object files
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
EXECS =  ${EXEC2}			# all executables

.PHONY : all clean

all : ${EXECS}					# build all executables

#############################################################
${EXEC2} : ${OBJECTS2}
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${EXECS}
