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
		for(int i=0; i<amount; i++){
			yield();
			   		try { sleep(1);
			}catch(InterruptedException ex){
				System.out.println("ERROR " + ex);
			}
			
		}
	}
		
	public q2Philosopher(int id, int noodles, q2table table, q2Printer prt){
		this.id = id;
		numberOfNoodles = noodles;
		this.table = table;
		printer = prt;	
	}	
	
	//start thread
	public void run(){

		while(true){
			printer.print(id, States.Hungry);
			
			//yield
			doYield(q2PRNG.getRand(0,4));
			
			//pickup forks from table
			table.pickup(id);
			
			//calc amount to eat
			int eatAmount = (q2PRNG.getRand(1,5));
			
			//upper bound
			if(eatAmount > numberOfNoodles)
				eatAmount = numberOfNoodles;
			
			numberOfNoodles -= eatAmount;
			
			printer.print(id, States.Eating, eatAmount, numberOfNoodles);
			
			//finish condition
			if(numberOfNoodles == 0) {
				table.putdown(id);
				break;
			}
					
			doYield(q2PRNG.getRand(0,4));		
			
			//putdown forks
			table.putdown(id);
			
			printer.print(id, States.Thinking);
			
			doYield(q2PRNG.getRand(0,19));
		}
		
		printer.print(id, States.Finished);
	}



}
