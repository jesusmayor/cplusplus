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

int ClientIA::pathFind(int sx, int sy, ClientIA::Cord start, ClientIA::Cord target){

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

	return sol[1].getLast_movement();
}


int ClientIA::decideMovement(const bot & mybot){
	const bot::position & pos = mybot.get_position();
	const bot::position & epos = enemies[0].get_position();
	start.x = pos.first; start.y = pos.second;

	float myPunt= -(1.0/(float)mybot.get_energy())-(5*((maps[0][pos.first][pos.second]+maps[1][pos.first][pos.second])+10.0));
	float enemyPunt= -(1.0/(float)enemies[0].get_energy())+(5*((maps[0][epos.first][epos.second]+maps[1][epos.first][epos.second])-10.0));
	bool aggresive = myPunt>enemyPunt;
	//traza
	std::cout << mybot.get_team() << "e " << aggresive << "x " << pos.first << "y " << pos.second << "my " << myPunt << "enemy " << enemyPunt << std::endl;

	if (aggresive) {
		target.x = epos.first; target.y = epos.second;
	}
	if (start.x==target.x && start.y==target.y) return 0;
	return pathFind(10,10,start,target);
}

void ClientIA::generateMaps(){
	int msy=10;
	int msx=10;
	//std::vector <bot *> myTeam =_bots.team_bots(_id);
	int numMyT = 5;
	maps[0]=new float*[msy];
	maps[1]=new float*[msy];
	for(int i = 0; i < msy; ++i) {
		maps[0][i] = new float[msx];
		maps[1][i] = new float[msx];
	    for(int j=0; j<msx; j++) {
	    	maps[0][i][j] = 0.0;
	    	maps[1][i][j] = 0.0;
	    }
	}

	if (numMyT!=0) _bots.for_each_bot([&,&numMyT,&msx,&msy] (const bot & bot) {
		const bot::position & pos = bot.get_position();
		bool is_ally = (_id == bot.get_team());
		for(int i=pos.first-2;i<=(signed)(pos.first+2);i++){ //5
			for(int j=pos.second-2;j<=(signed)(pos.second+2);j++){ //5x5
				if (i>=0 && i<msy && j>=0 && j<msx){
					if(std::abs(i-(signed)pos.first)==2 || std::abs(j-(signed)pos.second)==2){
						if (is_ally) maps[1][i][j]+=((float)-1/(numMyT*2));
						else maps[0][i][j]+=((float)1/(numMyT*2));
					}else if (std::abs(i-(signed)pos.first)==1 || std::abs(j-(signed)pos.second)==1){
						if (is_ally) maps[1][i][j]+=((float)-1/numMyT);
						else maps[0][i][j]+=((float)1/numMyT);
					}else{
						if (is_ally) maps[1][i][j]+=-10;
						else maps[0][i][j]+=10;
					}
				}
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
		return (-(1.0/(float)a.get_energy())+(5*(maps[0][posA.first][posA.second]-10.0)))<=(-(1.0/(float)b.get_energy())+(5*(maps[0][posB.first][posB.second]-10.0)));
	});

    std::sort(allies.begin(), allies.end(), [&](bot a, bot b){
		const bot::position & posA = a.get_position();
		const bot::position & posB = b.get_position();
		return (-(1.0/(float)a.get_energy())-(5*(maps[1][posA.first][posA.second]+10.0)))<=(-(1.0/(float)b.get_energy())-(5*(maps[1][posB.first][posB.second]+10.0)));
	});
}

ClientIA::ClientIA(const bots & bots,bot::team_id & id)
: _bots(bots),
  _id(id)
{
	generateMaps();
	potentialObjetives();
}

}