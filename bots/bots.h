#ifndef _BOTS_H_INCLUDED_
#define _BOTS_H_INCLUDED_

#include <map>
#include <iostream>
#include <random>
#include <memory>
#include <algorithm>

#include "bot.h"


class bots {


  private:

    typedef std::vector < bot > field_bots;

    /**
     * beware, there is no emptiness checking!
     */
    inline void create_bot(bot::position position, bot::team_id team) {
	bot new_bot(team, position);
	 _bots.push_back(std::move(new_bot));
    } bot *would_attack(const bot & the_bot, const direction & dir) const;

    void perform_action(bot & the_bot);

    bot::field_size _width;
    bot::field_size _height;
    field_bots _bots;

  public:

  class too_many_bots:std::exception {
    };

    bots(bot::field_size width, bot::field_size height);

    virtual ~ bots();

    void generate(size_t number_teams,
		  size_t bots_per_team) throw(too_many_bots);

    const bot *find_at(const bot::position & p) const;

    bool empty(const bot::position & p) const;

    bool can_move(const bot & the_bot, const direction & dir) const;

    void move(bot & the_bot, const direction & dir);

    bool can_attack(const bot & the_bot, const direction & dir) const;

    void attack(bot & the_bot, const direction & dir);

    void step(int time);

    inline void for_each_bot(std::function < void (bot & the_bot) > fun) {
	for_each(_bots.begin(), _bots.end(), fun);
    }
};


#endif
