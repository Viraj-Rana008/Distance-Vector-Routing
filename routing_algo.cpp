#include "node.h"
#include <iostream>
#include <unordered_map>
#include <map>

using namespace std;

void printRT(vector<RoutingNode*> nd){
/*Print routing table entries*/
	for (int i = 0; i < nd.size(); i++) {
	  nd[i]->printTable();
	}
}

void converge(vector<RoutingNode*> nd, vector<int> prev_state){     // Same loop as in Q1, but in a function

  while(1){
    int change_count = 0;

    for(int i=0; i<nd.size(); i++){
      RoutingNode* node = nd[i];
      
      if(node->getState() != prev_state[i]){
        node->sendMsg();
        prev_state[i] = node->getState();

        change_count+=1;
      }
    }

    if(change_count==0)
      break;
  }

}

void routingAlgo(vector<RoutingNode*> nd){

  vector<int> prev_state;
  for(int i=0; i<nd.size(); i++){
    prev_state.push_back(0);
    nd[i]->setState(1);
  }
  

  converge(nd, prev_state);

  // Updating node-B
  nd[1]->updateTbl("C");

  // Updating node-C
  nd[2]->updateTbl("B");

  converge(nd, prev_state);

  /*Print routing table entries after routing algo converges */
  printRT(nd);
}


void RoutingNode::recvMsg(RouteMsg *msg) {
  for(auto msg_entry : msg->mytbl->tbl){
    int msg_entry_found = 0;

    for(auto my_entry : mytbl.tbl){
      if(!my_entry.dstip.compare(msg_entry.dstip)){
        msg_entry_found = 1;

        if(1+msg_entry.cost < my_entry.cost){
          my_entry.nexthop = msg->from;
          my_entry.ip_interface = msg->recvip;
          my_entry.cost = 1+msg_entry.cost;

          this->setState(this->getState() + 1);
        }
      }
    }

    if(!msg_entry_found){
      
      if(1+msg_entry.cost > 16)  //maximum hops reached
        continue;

      this->addTblEntry(
      msg_entry.dstip, 
        msg->from, 
        msg->recvip, 
        1+msg_entry.cost);
      this->setState(this->getState() + 1);
    }
  }    
}




