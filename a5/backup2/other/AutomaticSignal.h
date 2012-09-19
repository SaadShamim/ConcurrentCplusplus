#define AUTOMATIC_SIGNAL uCondition IS;

#define WAITUNTIL( pred, before, after )  	\
	if(!(pred)) {							  	\
		while(! IS.empty() ) IS.signal(); 	\
		before;								\
		do {								\
			IS.wait();						\
		} while (!(pred));					\
	}after;									
	
	
#define RETURN(expr...)						\
	while (!IS.empty()) IS.signal(); 		\
	return expr;							
