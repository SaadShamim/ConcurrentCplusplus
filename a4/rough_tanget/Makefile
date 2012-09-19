OPT:=

CXX = u++					# compiler
CXXFLAGS = -g -Wall -Wno-unused-label -MMD ${OPT} 
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS1 = q2Elf.o q2Santa.o q2driver.o q2Workshop.o q2Printer.o q2Reindeer.o # list of object files for question 2 prefixed with "q2"
EXEC1 = northpole

OBJECTS = ${OBJECTS1}		# all object files
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
EXECS = ${EXEC1} 		# all executables

.PHONY : all clean

all : ${EXECS}					# build all executables

#############################################################

-include ImplKind

${EXEC1} : ${OBJECTS1}
	${CXX} ${CXXFLAGS} $^ -o $@
					

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${EXECS} ImplKind ImplType
