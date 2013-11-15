#ifndef _BOT_H_INCLUDED_
#define _BOT_H_INCLUDED_

#include <vector>

class bots;

enum direction { NOTHING, N, NE, E, SE, S, SW, W, NW };

/**
 * Non-const methods are private and container class is declared as a friend.
 * This enforces a very strict encapsulation pattern.
 */
class bot
{
    public:

        typedef unsigned short status;

        typedef unsigned int team_id ;

        typedef unsigned int field_size ;

        typedef std::pair<field_size, field_size> position ;

        bot (team_id id, const position & pos);

        virtual ~bot ();

        status get_energy() const;

        inline status get_base_attack() const {
            return _base_attack;
        };

        status get_attack() const;

        inline status get_base_defense() const {
            return _base_defense;
        };

        inline team_id get_team() const {
            return _team;
        }

        status get_defense() const;

        status get_experience() const;

        inline const position & get_position() const {
            return _position;
        }

        inline const direction & get_next_direction() const {
            return _next_direction;
        }

    private:

        friend class bots;

        status _base_attack;
        status _base_defense;
        status _experience;
        status _energy;
        team_id _team;
        position _position;
        direction _next_direction;

        /**
         * this enforces modification of the data structure through container class
         */
        inline void move(const direction & dir) {
            _next_direction = dir;
        }
};

#endif
