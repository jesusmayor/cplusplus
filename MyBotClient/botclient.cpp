#include "botclient.h"
#include "bot.h"
#include <iostream>
#include <limits>

struct node {

    //const bot & _bot;
    const bot::position & _position;
    const bots & _bots;
    float _heuristic;
    direction _best;
    int _depth;

    /**
     * @param my_bot must belong to current_bots
     * @param current_bots
     * @param depth
     * @param heuristic
     * @param best
     */
    node(const bot::position& my_pos, const bots & current_bots, int depth = 0, float heuristic = std::numeric_limits<float>::lowest(), direction best = NOTHING) 
        : _position(my_pos), _bots(current_bots), _heuristic(heuristic), _best(best), _depth(depth) {
        }

    void simple_backtrack(unsigned int max_depth) {
        if(_depth < max_depth) {
            // ugliest hack ever. for-looping over an enum
            int new_depth = _depth + 1;
            for(int i = 0; i < 9; i++) {
                direction new_dir = static_cast<direction>(i);

                const bot & temp_bot = _bots[_position];
                if(_bots.can_move(temp_bot, new_dir) || _bots.attacks(temp_bot, new_dir)) {
                    bots new_bots = bots(_bots);
                    new_bots[_position].try_to_do(new_dir);
                    new_bots.step();
                    node new_node(_position, new_bots, new_depth, _heuristic);
                    
                    new_node.simple_backtrack(max_depth);

                    if(new_node._heuristic > _heuristic) {
                        _heuristic = new_node._heuristic;
                        _best = new_node._best;
                    }
                }
            }
        }
        else {
            auto m = _bots.bot_count();
            bot::team_id team = _bots[_position].get_team();
            int res = 0;
            for(auto kv : m) {
                if(team != kv.first) {
                    res += kv.second;
                }

            }
            _heuristic = (m[team] * 0.5) - (res * 0.3);
        }
    }
};


void bot_client::perform(int depth) {


    _bots.for_each_bot([this, &depth] (bot & my_bot) {
        node my_node(my_bot.get_position(), _bots);
        my_node.simple_backtrack(depth);
        my_bot.try_to_do(my_node._best);
    });
}

