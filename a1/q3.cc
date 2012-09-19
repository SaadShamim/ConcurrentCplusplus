#include <uC++.h>
#include <fstream>
#include <iostream>
using namespace std;
#include "q3binsertsort.h"

template<typename T> _Coroutine Binsertsort {
    const T Sentinel;                                   // value denoting end of set
    T value;                                            // communication: value being passed down/up the tree
    void main(){
    
    //define left and right branches along with root node (p)
	Binsertsort left(Sentinel);
	Binsertsort right(Sentinel);
	T p = value;
	
	suspend();
    
        //checks value can be sorted, if less then equal put on left, else right
	while(value != Sentinel){
	
		if(value <= p){
			left.sort(value);
		}else{
			right.sort(value);
		}

        //stops coroutine, when recalled resumes and checks backup in the while argument if value is appropriate or not
		suspend();

	}

    //check if the current node exists or not, the node exists if its value is anything but sentinel
	if(p != Sentinel){
            //tell left branch list has ended (basically sets the root node,p, to sentinel if it doesn't have any values (empty branch)
			left.sort(Sentinel);
			suspend();
        
            //loops until recieves a sentinel value, indicating that left branch is "finished" and has sent up sentinel
            //else pass the value from left branch up
			while(true) {
 				T message = left.retrieve();
  				 if (message == Sentinel) break;
    				value = message;
    				suspend();
			}	
            
            //since infix order, root must not pass its value (at this point left branch has returned all its values)
			value = p;
			suspend();
            
            //rell right branch that list ended
			right.sort(Sentinel);
			
            //passup all the values from right branch
			while(true) {
    				T message = right.retrieve();
   				if (message == Sentinel) break;
    				value = message;
    				suspend();
			}
            
            //pass up sentinel indicating end of retrieval
			value = Sentinel;

	}else{
        //passup -1 indicating that its an empty branch 
		value = Sentinel;
	}
	suspend();	
	}                                        
  public:
    Binsertsort( T Sentinel ) : Sentinel( Sentinel ) {}
    void sort( T value ) {                              // value to be sorted
        Binsertsort::value = value;
        resume();
    }
    T retrieve() {                                      // retrieve sorted value
        resume();
        return value;
    }
};
void uMain::main(){
    //makesure file for read is present 
	if(argc < 2){
		cout << "Please have atleast 2 paramenters" << endl;
	}
	
	ifstream myfile(argv[1]);
    
    //make sure file is readable
	if(!myfile){
		cout<<"Error opening input file"<<endl;
		exit(1);
	}

    //open output file if avaialable
	ofstream outputFile;
	if(argc >= 3){
		outputFile.open(argv[2]);
	}

    //n is used to read the first int from a list, which corresponds to the number of values to expect in the list
	int n = 0;
	TYPE r = 0; //values read from data file
	
    //goes through data file
	while(1){
        //determine number of values in list
		myfile >> n;
        //if eof break
		if(myfile.eof()){
			break;
		}
		
        //instantiate main tree, go through the list of values from data file and pass into the tree (coroutine)
		Binsertsort<TYPE>tree(SENTINEL);	
		for(int i =0;i<n;i++){	
			myfile >> r;
			tree.sort(r);
            //print out orignal output
			if(outputFile.is_open())outputFile << r << " ";
			else cout << r << " ";
		}
	
		if(outputFile.is_open())outputFile << endl;	
		else cout << endl;
        
        //send sentinel, signaling end of list
		tree.sort(SENTINEL);
        
            //loop until all values have been retrieved
        	while(1){
                	TYPE x=tree.retrieve();
                	if(x == SENTINEL){
                        	break;
                	} 
               		if(outputFile.is_open())outputFile << x << " "; 
			else cout << x << " ";
       		 }

		if(outputFile.is_open())outputFile << "\n" << endl;	
		else cout << "\n" << endl;
	}

}
