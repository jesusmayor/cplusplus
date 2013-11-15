#ifndef _BOTCLIENT_H_INCLUDED_
#define _BOTCLIENT_H_INCLUDED_

#include "bots.h"

/**
 * class for ia
 */
class bot_client
{
public:

    /**
     * @param bots a reference to an instance of bots
     */
    bot_client (bots & bots) : _bots(bots) // in this way I can have a reference as a class attribute
    {
        // this wouldn't work
        // _bots = bots;
    };

    virtual ~bot_client () {};

    /**
     * computes next moves for bots
     */
    void perform(int depth);

private:
    bots & _bots;

    std::pair<int, direction> simple_backtrack(bot & my_bot, const bots & current_bots, std::pair<int, direction> best, 
            unsigned int max_depth, unsigned int current_depth = 0);

    static int heuristic(const bot & bot, const bots & current_bots);
};

#endif
