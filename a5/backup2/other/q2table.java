//java.uril.*;

public class q2table {

	
	boolean []eatArray;
	Object []waitingToPickup;
	int []waitingToEat;
	q2Printer printer;
	int numberOfPhil;
	
	public q2table(int noOfPhil, q2Printer prt){
		numberOfPhil = noOfPhil;
		//waitingToPickup = new Object[numberOfPhil];
		eatArray = new boolean[numberOfPhil];
		printer = prt;
		
		for(int i =0; i< numberOfPhil;i++){
			eatArray[i] = false;
		}
	}
    
	private int leftFork(int id){
		if(id == 0)
			return (numberOfPhil-1);
		
		return (id-1);
	}

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
		
			//waitingToPickup[id].wait();
			while(! (eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false)){
				try{	
					wait();
				}catch (InterruptedException ex){
					//System.out.println("ERRORRRRRRRRRRRRRRRRRRRRRRRr:" + ex);
				}
				//System.out.println("waking id: " + id + " left: " + eatArray[leftFork(id)] + " right: " + eatArray[rightFork(id)]);
			}
			//System.out.println("woken id: " + id + " left: " + eatArray[leftFork(id)] + " right: " + eatArray[rightFork(id)]);
			if((eatArray[leftFork(id)] !=false || eatArray[rightFork(id)]!= false) )
				System.out.print("ASSSSSSSSSSSSSSSSERT");
			//if(debug) cerr << "wake: " << id << endl;
			///waitingToEat[id] = 0;
		
		//	assert(eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false);
			eatArray[id] = true;
		}
		
	}
	
	
	public synchronized void putdown( int id ){
		eatArray[id] = false;
		notify();	
	}
}
