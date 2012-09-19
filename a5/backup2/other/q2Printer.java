public class q2Printer {
	
char []bufferArray;
int []leftValue;
int []rightValue;
boolean []changedValue;
int numberOfPhil;
	
public synchronized void print(int id, q2Philosopher.States state) {
	//special case: voting finished
	if(state == q2Philosopher.States.Finished){
		for(int i =0;i<numberOfPhil;i++){
			if(bufferArray[i] != 'x'){
				clearBuffer();
				break;
			}
		}
	
		for(int i=0;i<numberOfPhil;i++){
			if(id != i)
				System.out.print("...");
			else
				System.out.print(state.val);
			
			if(i != numberOfPhil-1)
				System.out.print("\t");
				
				bufferArray[i] = 'x'; //reset buffer values
				//leftValue[i] = (Philosopher::States)NULL;
				//rightValue[i] = (Philosopher::States)NULL;
		}
		
		System.out.print("\n");
	}else{
	
		//print if new value, else add to buffer
		if(bufferArray[id] == 'x'){
			bufferArray[id] = state.val; //add to buffer array	
		}else{
			clearBuffer(); //flush if change detected and print
			bufferArray[id] = state.val; //store new value
		}
	
	}

}

public synchronized void print(int id, q2Philosopher.States state, int bite, int noodles) {
	if(bufferArray[id] == 'x'){
		bufferArray[id] = state.val;	//store new value
		leftValue[id] = bite; 
		rightValue[id] = noodles;
	}else{
		clearBuffer(); //clear buffer and print
		bufferArray[id] = state.val; //store new values
		leftValue[id] = bite; 
		rightValue[id] = noodles;
	}
}

private void print_tabs(int tabs){
	for (int i = 0; i < tabs; i++) {
		System.out.print("\t");
	}
}

private void  clearBuffer(){
	int tabs = 0;
	
	for(int i=0; i<numberOfPhil;i++){
		if(bufferArray[i] != 'x'){
			print_tabs(tabs);
			System.out.print(bufferArray[i]);
			 if (bufferArray[i] == q2Philosopher.States.Waiting.val || bufferArray[i] == q2Philosopher.States.Eating.val) {
			 		System.out.print(leftValue[i] + "," + rightValue[i]);
			}
			tabs=1;
		} else {
			tabs++;
		}
		
		
		//reset buffer
		bufferArray[i] = 'x';
		
	}
	
	System.out.print("\n");
	
}

	public q2Printer(int NoOfPhil){
		//make arrays global
		int tabs = 0;
		//q2Philosopher.States []bufferArray = new q2Philosopher.States[NoOfPhil];
		bufferArray = new char[NoOfPhil];
		leftValue = new int[NoOfPhil];
		rightValue = new int[NoOfPhil];
		changedValue = new boolean[NoOfPhil];
		numberOfPhil = NoOfPhil;
		
		for(int i=0;i<NoOfPhil;i++){
			print_tabs(tabs);
			if(i== NoOfPhil-1) System.out.print("Phil" + i);
			else System.out.print("Phil" + i + "\t");
			changedValue[i] = false;
		}
		System.out.print("\n");
		
		for(int i=0; i<NoOfPhil;i++){
			if (i == NoOfPhil - 1) System.out.print("******");
			else System.out.print("******" + "\t");
		}
		//System.out.print("\n");
		
	}
	
	public void finishPrinter(){
	    	System.out.println("***********************");
		System.out.println("Philosophers terminated");
	}

//remove printer and print closing argument
/*Printer::~Printer(){
	delete[] bufferArray;
   	delete[] leftValue;
   	delete[] rightValue;
    	
    	cout << "***********************" << endl;
	cout << "Philosophers terminated" << endl;
}*/
	
	
	
	
}
