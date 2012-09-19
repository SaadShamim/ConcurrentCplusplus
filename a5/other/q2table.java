//java.uril.*;

public class q2table {

	
	boolean []eatArray;
	int []waitingToEat;
	q2Printer printer;
	int numberOfPhil;
	
	//init values
	public q2table(int noOfPhil, q2Printer prt){
		numberOfPhil = noOfPhil;
		eatArray = new boolean[numberOfPhil];
		printer = prt;
		
		for(int i =0; i< numberOfPhil;i++){
			eatArray[i] = false;
		}
	}
    
    	//return left fork value
	private int leftFork(int id){
		if(id == 0)
			return (numberOfPhil-1);
		
		return (id-1);
	}
	
	//return right fork value
	private int rightFork(int id){
		if(id==numberOfPhil-1)
			return 0;
	
		return (id+1);
	}	
	
	
	public synchronized void pickup(int id){
		if(eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false){
			eatArray[id] = true;
		}else{
			printer.print(id, q2Philosopher.States.Waiting, id, rightFork(id));
		
			//wait if neighbours are busy
			while(! (eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false)){
				try{	
					wait(); //wait on current thread
				}catch (InterruptedException ex){
					System.out.println("Exception: " + ex);
					System.exit(1);
				}
			}

			if((eatArray[leftFork(id)] !=false || eatArray[rightFork(id)]!= false) ){
				System.out.print("ASSERT FAILED: LINE 56");
				System.exit(1);
			}
			
			eatArray[id] = true;
		}
		
	}
	
	//put down fork
	public synchronized void putdown( int id ){
		eatArray[id] = false;
		notify();	//release a thread
	}
}
