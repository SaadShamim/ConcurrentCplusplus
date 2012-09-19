#include "station.h"
#include <vector>
#include <iostream>
#include "PRNG.h"

vector<Station *> Station::active;
unsigned int Station::value;
Station* Station::coordinator;

extern PRNG myPrng;

///MAKE VALUE GLOBAl
//iFailed to failed()
//remove starterFlag

///MAKE VALUE GLOBAl
//iFailed to failed()
//remove starterFlag
//change enum
//55 179 977


// 89 sends 23
// ./tokenring 18 11 32 > me.out
//transmit in one place

void Station::main() {
	//decides whether to hold token or not
	holdCounter = myPrng(1);
	suspend();
	//used for referencing in election
	Station *This = this;
	
	try {
		
		_Enable {   
			// allow delivery of nonlocal exceptions
			// establish starter for termination
			
			// _Enable
			while(true){
				//check to see if partner has failed
				while(myPartner->failed()){
					cout << idx << "(fail " << myPartner->idx << ", part " << (myPartner->partner())->id() << ") "; 
					
					//if failed partner is a coordinator, hold an election
					if(myPartner->idx == coordinator->idx){
						cout << idx << "(elec";
						_Resume Election(*this) _At *(myPartner->partner());
						myPartner->partner()->vote(); //resume
					}
				
					//if frame is being sent to failed partner	
					if(localFrame.dstId == myPartner->idx){
						//if frame is an ack, change frame to become token holder
						if(localFrame.type == Frame::ACK){
							//convert to token
							cout << idx << "(drop Ack)" << endl;
							localFrame.type = Frame::TOKEN;
							localFrame.dstId = idx;
						//if frame is a Nack, change frame to become token
						}else if (localFrame.type == Frame::NACK){
							cout << idx << "(drop Nack)" << endl;
							localFrame.type = Frame::TOKEN;
							localFrame.dstId = idx;
						//if data frame
						} else if(localFrame.type == Frame::DF){
							cout << idx << "(drop Data)";
							//become token if the data being delivered to partner originated from partner
							if(localFrame.srcId == localFrame.dstId){
								cout << endl;
								//covert to token
								localFrame.type = Frame::TOKEN;
								localFrame.dstId = idx;
							}else{
							//if data send from else where, send a nack
								cout << " ";
								//convert to nack
								localFrame.type = Frame::NACK;
								localFrame.dstId = localFrame.srcId;
								localFrame.srcId = idx;
							}
							
						}
					//if data is sent to current station, but the source is your partner become token
					} else if ( localFrame.dstId == idx && localFrame.srcId == myPartner->id()) {
						if ( localFrame.type == Frame::DF ) {
							cout << idx << "(rec " << value << ", from " << localFrame.srcId << ") "; 
							cout << idx << "(drop Ack)" <<endl;
							localFrame.type = Frame::TOKEN;
							localFrame.dstId = idx;
						}
					}
					
					//call on coordinator to remove failed station
					myPartner = coordinator->remove(myPartner);
				}
				
				//generate failed random number
				if(myPrng((failurex-1)) == 0){
					iFailed = true; //decides whether you fail
				}
				
				Frame tmpFrame;
				
				//check to see if data sent to current station is from a failed partner, if so change frame to token
				if((localFrame.type==Frame::DF) && (localFrame.dstId == idx) && (check(localFrame.srcId) == false)){	
					cout << idx << "(drop Ack)" << endl;
					localFrame.type = Frame::TOKEN;
					localFrame.dstId = idx;
				}
				
				//process the frame
				//if frame is token
				if(localFrame.type == Frame::TOKEN){
					//if counter is 0, reset counter and send data
					if(holdCounter == 0){
						//reset counter
						holdCounter = myPrng(1,3);
						value++;
						int dstIndex = myPrng((active.size()-1));
						//create dataframe
						tmpFrame.type = Frame::DF;
						tmpFrame.value = value;
						tmpFrame.srcId = idx;
						tmpFrame.dstId =  active[dstIndex]->idx;
						//send dataframe
						cout << idx << "(send " << value << ", to " << active[dstIndex]->idx << ") ";
					}else{ //if counter not 0, then pass on the token
						cout << idx << "(skip) ";
						holdCounter--;
						tmpFrame.type = Frame::TOKEN;
						tmpFrame.value = value;
						tmpFrame.srcId = idx; //doesn't matter, never checked
						tmpFrame.dstId = myPartner->idx; //doesn't matter never checked
					}	
				//if frame is nack or ack 
				} else if(localFrame.type == Frame::NACK || localFrame.type==Frame::ACK){
					//if sent to you pass on token
					if(localFrame.dstId == idx){
						tmpFrame.type = Frame::TOKEN;
						tmpFrame.value = value;
						tmpFrame.srcId = idx; 
						tmpFrame.dstId = myPartner->idx; 
					
						//print out valid message	
						if(localFrame.type == Frame::ACK){
							cout << idx << "(ack)"<< endl;
						}else{
							cout << idx << "(nack)"<<endl;
						}
						
					//if not sent to you, pass on ack	
					}else{
						//pass frame
						if(localFrame.type == Frame::ACK){
							cout << idx << "(ack " << localFrame.dstId << ") ";
						}else{
							cout << idx << "(nack " << localFrame.dstId << ") ";
						}
						
						tmpFrame.type = localFrame.type;
						tmpFrame.value = localFrame.value;
						tmpFrame.srcId = localFrame.srcId; //doesn't matter, never checked
						tmpFrame.dstId = localFrame.dstId; //doesn't matter never checked
					}
				//if frame is a data frame
				} else if(localFrame.type == Frame::DF){
					//if data sent to you, receive it and send ack
					if(localFrame.dstId == idx){
						cout << idx << "(rec " << value << ", from " << localFrame.srcId << ") "; 
						tmpFrame.type = Frame::ACK;
						tmpFrame.value = value;
						tmpFrame.srcId = idx; 
						tmpFrame.dstId = localFrame.srcId; 
					}else{//pass on the data frame
						cout << idx << "(for " <<  localFrame.dstId << ", " << value << ") "; 
						tmpFrame.type = localFrame.type;
						tmpFrame.value = localFrame.value;;
						tmpFrame.srcId = localFrame.srcId; 
						tmpFrame.dstId = localFrame.dstId; 
					}
				}
				
				//send frame to partner	
				myPartner->transmit(tmpFrame);

				//if only one station left, break
				if(myPartner->idx == idx){
					break;
				}
				
			}//end while
			
		} //close Enable
	} _CatchResume(Election &election) (Station *This)    {
		//deal with election
		cout << " ";
		//if cycle is not finished
		if(This->partner() != Station::coordinator){
			//if station is failed, do not vote, pass on election
			if(This->failed()){
				cout << election.candidate.id() << ">" << This->id();
				_Resume Election(election.candidate) _At *(This->partner());
			//if current candidate has highest id, pass on candidate
			}else if((election.candidate).id() > This->id()){
				cout << election.candidate.id() << "?" << This->id();
				_Resume Election(election.candidate) _At *(This->partner());	
			}else{
			//if current user has highest id, pass on current user
				cout << election.candidate.id() << "?" << This->id();
				_Resume Election(*This) _At *(This->partner());
			}
			//resume partner
			(This->partner())->vote();
		}else{
			//set candidate as coordinator
			cout << "win " << election.candidate.id() << ") ";
			(Station::coordinator) = &(election.candidate);
			
		}
	} // try
	
	cout << "no more partners"<<endl;
	return;
}

bool Station::check(int dst){
	//go through the list of stations to check which one is dead	
	for(int i=0;i<active.size();i++){
		if(active[i]->id() == dst){
			return true;
		}
	}
	return false;
}
//constructor for station
Station::Station(unsigned int id , unsigned int failure) : idx(id),failurex(failure){
	active.push_back(this);
	
}

//return id
unsigned int Station::id(){
	return idx;
}

//return faile status
bool Station::failed(){
	return iFailed;
}

//recieve frame value
void Station::transmit(const Frame &f){
	localFrame.type = f.type;
	localFrame.value = f.value;
	localFrame.srcId = f.srcId;
	localFrame.dstId = f.dstId;
	resume();
}

//setup stations partner, call resume
void Station::start(Station *partner){
	iFailed = false;
	myPartner = partner;
	resume();
}

//remove station
Station* Station::remove(Station *victim){
	Station *newPartner;
	//go through active list, find victim, remove and get victims partner
	for(unsigned int i=0; i<active.size(); i++){
		if(active[i]->idx == victim->idx){
			newPartner = active[i]->partner();
			active.erase(active.begin()+i);
			break;
		}
	}
	delete victim; //clear mem
	return newPartner; //return the victims partner
}

//current stations partner
Station* Station::partner(){
	return myPartner;
}

//resumption for vote
void Station::vote(){
	resume();
}

