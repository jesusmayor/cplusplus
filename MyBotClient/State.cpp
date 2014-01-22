/*
 *
 * State.cpp
 *
 *
 * */

#include<iostream>
#include "State_Base.h"
#include "State.h"

using namespace std;

/* Initialization of the environment */
Environment::Environment(int **array, int sx, int sy, Location s, Location t){
	map = array;
	size_x  = sx;
	size_y  = sy;
	start = s;
	target = t;
	map[s.x][s.y] = START;
	map[t.x][t.y] = TARGET;
}

void Environment::print(){
	for(int i=size_x-1; i>=0; i--,cout << endl){
		for(int j=0; j<size_y; j++){
			switch(map[j][i]){
				case EMPTY: cout << '-'; break;
				case TOUCHED: cout << 'x'; break;
				case OBSTACLE: cout << 'O'; break;
				case START: cout << 's'; break;
				case TARGET: cout << 't'; break;
			}
		}
	}
}

void Environment::touch(Location ll){
	int tmp = map[ll.x][ll.y];
	if(EMPTY==tmp)
		map[ll.x][ll.y] = TOUCHED;
}

int** Environment::get_map(){ return map; }

Location Environment::get_target() const { return target; }

int Environment::get_size_x() const {return size_x;}
int Environment::get_size_y() const {return size_y;}

Environment::~Environment() {}

Environment* State::env = NULL;

State::State(){
	calculate_f(NULL);
	last_movement = START;
}

State::~State(){}

State::State(int **initial_map, int size_x, int size_y, int start_x, int start_y, int target_x, int target_y){
	Location start, target;
	start.x = start_x; start.y = start_y;
	target.x = target_x; target.y = target_y;
	env = new Environment(initial_map,size_x,size_y,start,target);
	coor = start;
	calculate_f(NULL);
	last_movement = START;
}

State::State(State& parent,Direction operation){
	*this = parent;
	switch(operation){
		case W: --coor.x; break;
		case S: --coor.y; break;
		case E: ++coor.x; break;
		case N: ++coor.y; break;
		case SW: --coor.x; --coor.y; break;
		case NW: --coor.x; ++coor.y; break;
		case SE: ++coor.x; --coor.y; break;
		case NE: ++coor.x; ++coor.y; break;
		default: break;
	}

	env->touch(coor);
	last_movement = operation;

	calculate_f(&parent);
	last_movement = operation;
}

/* For each branch branched_nodes are initialized with a four element array. The
 * members of the array with NULL are inappropriate nodes. These are not
 * taken into account in A_Star::solve()  */
State_Base** State::Branch(int &nodes_n){
	nodes_n = 8;
	State_Base **branched_nodes = new State_Base* [nodes_n];

	for(int i=0;i<nodes_n;i++)
		branched_nodes[i] = NULL;

	int tmp;
	if(coor.x>0 && 
		(EMPTY==(tmp=env->get_map()[coor.x-1][coor.y]) || TARGET==tmp))
			branched_nodes[0] = new State(*this,W);
	if(coor.x<env->get_size_x()-1 && 
		(EMPTY==(tmp=env->get_map()[coor.x+1][coor.y]) || TARGET==tmp))
			branched_nodes[1] = new State(*this,E);
	if(coor.y>0 && 
		(EMPTY==(tmp=env->get_map()[coor.x][coor.y-1]) || TARGET==tmp))
			branched_nodes[2] = new State(*this,S);
	if(coor.y<env->get_size_y()-1 && 
		(EMPTY==(tmp=env->get_map()[coor.x][coor.y+1]) || TARGET==tmp))
			branched_nodes[3] = new State(*this,N);
	if(coor.x>0 && coor.y>0 &&
		(EMPTY==(tmp=env->get_map()[coor.x-1][coor.y-1]) || TARGET==tmp))
			branched_nodes[4] = new State(*this,SW);
	if(coor.x>0 && coor.y<env->get_size_y()-1 &&
		(EMPTY==(tmp=env->get_map()[coor.x-1][coor.y+1]) || TARGET==tmp))
			branched_nodes[5] = new State(*this,NW);
	if(coor.x<env->get_size_x()-1 && coor.y>0 &&
		(EMPTY==(tmp=env->get_map()[coor.x+1][coor.y-1]) || TARGET==tmp))
			branched_nodes[6] = new State(*this,SE);
	if(coor.x<env->get_size_x()-1 && coor.y<env->get_size_y()-1 &&
		(EMPTY==(tmp=env->get_map()[coor.x+1][coor.y+1]) || TARGET==tmp))
			branched_nodes[7] = new State(*this,NE);

	return branched_nodes;
}

/* Prints the state information and the last operation is 
 * shown as the move of the appropriate neighboring block 
 * of the empty space */
void State::print(){
	cout << last_movement << endl;
} 

/* Number of steps gone so far */
int State::calculate_g(State_Base *parent){
	if(!parent)
		return g=0;
	return (g = parent->get_g() + 1);
}

int State::abs(int x){ return x<0?-x:x; }

//heuristic!!
/* Approximate number of displacements that will be taken to reach the goal.
 * This is the total of Euclidean distance of each block to its exact place
 * in the final state */
int State::calculate_h(State_Base *parent){
	if(!parent)	return h=1;
	int dx = abs(coor.x - env->get_target().x);
	int dy = abs(coor.y - env->get_target().y);
	if (dx>dy) return h = dx;
	else return h = dy;
}

Environment* State::get_env() { return env; }

