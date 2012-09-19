public class q2Philosopher extends Thread {
	
	private int id, numberOfNoodles;
	private q2table table;
	private q2Printer printer;
    
    public enum States {
		Thinking( 'T' ), Hungry( 'H' ), Eating( 'E' ), Waiting( 'W' ), Finished( 'F' );
		public final char val;
		States( char v ) { val = v; }
    } // Philosopher.states
    
    private void doYield(int amount){
   		// System.out.println("id: " + id + " amount: " + amount);

		for(int i=0; i<amount; i++){
			yield();
			   		try { sleep(1);
			}catch(InterruptedException ex){
				System.out.println("ERROR " + ex);
			}
			
			//have to remove this
		//	System.out.println("id: " + id + " i: " + i);
		}
	}
		
	public q2Philosopher(int id, int noodles, q2table table, q2Printer prt){
	//System.out.println("id: " + id + " started");
		this.id = id;
		numberOfNoodles = noodles;
		this.table = table;
		printer = prt;	
	}	
	
	public void run(){
		//System.out.println("id: " + id + " running");
		while(true){
			printer.print(id, States.Hungry);
			
			doYield(q2PRNG.getRand(0,4));
			
			table.pickup(id);
			
			int eatAmount = (q2PRNG.getRand(1,5));
			
			if(eatAmount > numberOfNoodles)
				eatAmount = numberOfNoodles;
			
			numberOfNoodles -= eatAmount;
			
			printer.print(id, States.Eating, eatAmount, numberOfNoodles);
			
			if(numberOfNoodles == 0) {
				table.putdown(id);
				break;
			}
					
			doYield(q2PRNG.getRand(0,4));		

			table.putdown(id);
			
			printer.print(id, States.Thinking);
			
			doYield(q2PRNG.getRand(0,19));
		}
		
		printer.print(id, States.Finished);
	}



}
