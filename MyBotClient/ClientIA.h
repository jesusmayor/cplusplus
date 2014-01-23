/*
 * ClientIA.h
 *
 *  Created on: Dec 26, 2013
 *      Author: root
 */

#ifndef CLIENTIA_H_
#define CLIENTIA_H_

#include "bots.h"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace KBOT {

namespace ublas = boost::numeric::ublas;

class ClientIA {
private:

	struct Cord { int x; int y;};

	const bots & _bots;
	bot::team_id & _id;
	short int sx,sy;
	ublas::matrix<float>* mapE;
	ublas::matrix<float>* mapA;
	Cord start;
	Cord target;
	std::vector<bot> enemies;
	std::vector<bot> allies;
	int pathFind(ClientIA::Cord, ClientIA::Cord);
	void generateMaps();
	void potentialObjetives();
public:
	ClientIA(const bots &,bot::team_id & _id, short int x, short int y);
	int decideMovement(const bot &);
	virtual ~ClientIA();
};

}

#endif /* CLIENTIA_H_ */
