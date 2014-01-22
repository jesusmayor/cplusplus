/*
 * ClientIA.h
 *
 *  Created on: Dec 26, 2013
 *      Author: root
 */

#ifndef CLIENTIA_H_
#define CLIENTIA_H_

#include "bots.h"

namespace KBOT {

class ClientIA {
private:

	struct Cord { int x; int y;};

	const bots & _bots;
	bot::team_id & _id;
	float ** maps[2];
	Cord start;
	Cord target;
	std::vector<bot> enemies;
	std::vector<bot> allies;
	int pathFind(int, int, ClientIA::Cord, ClientIA::Cord);
	void generateMaps();
	void potentialObjetives();
public:
	ClientIA(const bots &,bot::team_id & _id);
	int decideMovement(const bot &);
	virtual ~ClientIA(){};
};

}

#endif /* CLIENTIA_H_ */
