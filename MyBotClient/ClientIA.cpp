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

/**
 * Se encarga de encontrar la ruta entre el start y el target sin
 * chocar con bots
 */
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
	if (sol!=NULL) return sol[1].getLast_movement();
	else return 0;
}

/**
 * Es llamado desde el bucle de la conexión. Decide el target para un bot.
 * Este
 */
int ClientIA::decideMovement(const bot & mybot){
	const bot::position & pos = mybot.get_position();
	start.x = pos.first; start.y = pos.second;
	bool aggresive;
	//Ataca si su puntuacón (incluyendo la posición global) es mejor que la del enemigo.
	float myPunt= -(1.0/(float)mybot.get_energy())-(5*(((*mapE)(pos.first,pos.second)+(*mapA)(pos.first,pos.second))+10.0));
	float enemyPunt;
	if (!enemies.empty()){
		enemyPunt= -(1.0/(float)enemies[0].get_energy())+(5*(((*mapE)(enemies[0].get_position().first,enemies[0].get_position().second)+(*mapA)(enemies[0].get_position().first, enemies[0].get_position().second))-10.0));
		aggresive= myPunt>=enemyPunt;
	}else enemyPunt=-10;

	//traza
	//std::cout << mybot.get_team() << "e " << aggresive << "my " << myPunt << "enemy " << enemyPunt << std::endl;

	//Si es mejor su posición, entrará en modo agresivo y se moverá hacia el enemigo.
	if (aggresive) {
		const bot::position & epos = enemies[0].get_position();
		target.x = epos.first; target.y = epos.second;
	}else{ //Si no lo es, huirá a la parte mas segura de al lado del bot aliado mas fuerte (incluyendo posición estratégica)
		if (!allies.empty()){
			const bot::position & apos = allies[0].get_position();
			float minor = 0.0;
			float act = 0.0;
			target.x = apos.first; target.y=apos.second;
			for(int i=apos.first-1;i<=(signed)(apos.first+1);i++) //3
				for(int j=apos.second-1;j<=(signed)(apos.second+1);j++) //3x3
				{
					if (i>=0 && i<sy && j>=0 && j<sx){
						if(std::abs(i-(signed)apos.first)==1 || std::abs(j-(signed)apos.second)==1){
							act = (*mapE)(i,j)+(*mapA)(i,j);
							if (act<minor && act>-7.0F){
								minor=act;
								target.x = i; target.y=j;
							}
						}
					}
				}
		}
	}

	//Si el destino es el mismo bot, no hace nada. (Pocas veces pasará)
	if (start.x==target.x && start.y==target.y) return 0;

	//Busca el camino hacia el target. Si no es alcanzable, attkdir será 0
	int attkdir = pathFind(start,target);
	if (attkdir == 0) {
		//Si no es alcanzable (está encerrado) Atacará al enemigo con menos vida.
		//KAMIKAZE MODE! xD
		std::vector<bot *> bs= const_cast<bots*>(&_bots)->adjacent(pos);
		if (!bs.empty()){
			for(unsigned int i = 0; i<bs.size(); ++i) {
				if (_id == bs[i]->get_team()){
					bs.erase(bs.begin()+i);
				}
			}

			if (!bs.empty()) {
				std::sort(bs.begin(), bs.end(), [](bot* a, bot* b){
					return (a->get_energy()<=b->get_energy());
				});
				target.x=bs[0]->get_position().first;
				target.y=bs[0]->get_position().second;
				return pathFind(start,target);
			}
		}

	}
	return attkdir;
}

/**
 * Genera mapas de pesos. Los enemigos puntuan positivo y los aliados negativo
 * Zonas negativas serán mas seguras. Tendrémos dos mapas diferentes, uno solo con
 * la parte positiva y otra con la negativa. Juntados darán un mapa completo.
 *
 * En este mapa se basa la heuristica base de la IA.
 */
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

/**
 * Genera un vector con los bots mas debiles y más fuertes ordenados.
 * Realmente solo se usa la posición 0, pero una posible mejora sería
 * buscar las X primeras opciones y atacar a la mas cercana o algo así.
 * No ha sido hecho por falta de tiempo.
 */
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

/**
 * Constructor de la IA. Se instancia una vez y luego se llama a decideMovement(bot).
 */
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
