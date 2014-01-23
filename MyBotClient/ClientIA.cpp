/*
 * ClientIA.cpp
 *
 *  Created on: Dec 26, 2013
 *      Author: root
 */

#include "ClientIA.h"
#include "State.h"
#include "A_Star.h"

namespace KBOT {

int ClientIA::pathFind(ClientIA::Cord start, ClientIA::Cord target){

	int **array = new int*[sy];
	for(int i = 0; i < sy; ++i) {
	    array[i] = new int[sx];
	    for(int j=0; j<sx; j++) array[i][j] = 0;
	}

	_bots.for_each_bot([&array] (const bot & bot) {
		const bot::position & pos = bot.get_position();
		array[pos.first][pos.second] = 1;
	});
	/* the initial state (the root of the a-star tree) */
	State *x = new State(array, sx, sy, start.x, start.y, target.x, target.y);
	//x->get_env()->print();

	A_Star as;
	int number_of_states;
	State *sol = as.solve(x,number_of_states);

	//std::cout << "\nThe path followed:\n";
    //for(int i=0;i<number_of_states;i++)
    //	sol[i].print();
	//sol[0].get_env()->print();

	//Clean-up
	for(int i = 0; i < sy; ++i) {
	    delete [] array[i];
	}
	delete [] array;
	delete x;

	return sol[1].getLast_movement();
}


int ClientIA::decideMovement(const bot & mybot){
	const bot::position & pos = mybot.get_position();
	start.x = pos.first; start.y = pos.second;
	bool aggresive;
	float myPunt= -(1.0/(float)mybot.get_energy())-(5*(((*mapE)(pos.first,pos.second)+(*mapA)(pos.first,pos.second))+10.0));
	float enemyPunt;
	if (!enemies.empty()){
		enemyPunt= -(1.0/(float)enemies[0].get_energy())+(5*(((*mapE)(enemies[0].get_position().first,enemies[0].get_position().second)+(*mapA)(enemies[0].get_position().first, enemies[0].get_position().second))-10.0));

		aggresive= myPunt>enemyPunt;
	}else enemyPunt=-10;

	//traza
	std::cout << mybot.get_team() << "e " << aggresive << "x " << pos.first << "y " << pos.second << "my " << myPunt << "enemy " << enemyPunt << std::endl;

	if (aggresive) {
		const bot::position & epos = enemies[0].get_position();
		target.x = epos.first; target.y = epos.second;
	}else{
		target.x = 0; target.y= 0;
	}
	if (start.x==target.x && start.y==target.y) return 0;
	return pathFind(start,target);
}

void ClientIA::generateMaps(){
	int numMyT = 0;
	mapE = new ublas::matrix<float>(sx,sy);
	mapA = new ublas::matrix<float>(sx,sy);
    for (short int i = 0; i < (signed)mapE->size1(); ++ i)
        for (short int j = 0; j < (signed)mapA->size2(); ++ j){
            (*mapE)(i, j) = 0.0;
            (*mapA)(i, j) = 0.0;
        }

	_bots.for_each_bot([&,&numMyT] (const bot & bot) {
		if (_id == bot.get_team()) numMyT++;
	});

	if (numMyT!=0) _bots.for_each_bot([&,&numMyT] (const bot & bot) {
		const bot::position & pos = bot.get_position();
		bool is_ally = (_id == bot.get_team());
		for(int i=pos.first-2;i<=(signed)(pos.first+2);i++) //5
			for(int j=pos.second-2;j<=(signed)(pos.second+2);j++) //5x5
				if (i>=0 && i<sy && j>=0 && j<sx){
					if(std::abs(i-(signed)pos.first)==2 || std::abs(j-(signed)pos.second)==2){
						if (is_ally) (*mapA)(i,j)+=((float)-1/(numMyT*2));
						else (*mapE)(i,j)+=((float)1/(numMyT*2));
					}else if (std::abs(i-(signed)pos.first)==1 || std::abs(j-(signed)pos.second)==1){
						if (is_ally) (*mapA)(i,j)+=((float)-1/numMyT);
						else (*mapE)(i,j)+=((float)1/numMyT);
					}else{
						if (is_ally) (*mapA)(i,j)+=-10;
						else (*mapE)(i,j)+=10;
					}
				}
	});
}

void ClientIA::potentialObjetives(){
	_bots.for_each_bot([&] (const bot & bot) {
		if (_id == bot.get_team()){
			allies.push_back(bot);
		}else{
			enemies.push_back(bot);
		}
	});

    std::sort(enemies.begin(), enemies.end(), [&](bot a, bot b){
		const bot::position & posA = a.get_position();
		const bot::position & posB = b.get_position();
		return (-(1.0/(float)a.get_energy())+(5*((*mapE)(posA.first,posA.second)-10.0)))<=(-(1.0/(float)b.get_energy())+(5*((*mapE)(posB.first,posB.second)-10.0)));
	});

    std::sort(allies.begin(), allies.end(), [&](bot a, bot b){
		const bot::position & posA = a.get_position();
		const bot::position & posB = b.get_position();
		return (-(1.0/(float)a.get_energy())-(5*((*mapA)(posA.first,posA.second)+10.0)))<=(-(1.0/(float)b.get_energy())-(5*((*mapA)(posB.first,posB.second)+10.0)));
	});
}

ClientIA::ClientIA(const bots & bots,bot::team_id & id,short int x, short int y)
: _bots(bots),
  _id(id),
  sx(x),
  sy(y)
{
	generateMaps();
	potentialObjetives();
}

ClientIA::~ClientIA()
{
	delete mapA;
	delete mapE;
}
}
