/*
 * FrontendView.h
 *
 * SINGLETON
 *
 *  Created on: Dec 20, 2013
 *      Author: kasios
 */

#ifndef _KBOT_FV_
#define _KBOT_FV_

#include <boost/thread/mutex.hpp>
#include "bots.h"

#include <SDL/SDL.h>

#include <GL/gl.h>
#include <GL/glu.h>

namespace KBOT{
	class FrontendView{
		public:
			static FrontendView* getInstance();
			void draw(bots &, boost::mutex &,int &,int &);
			bool gameover;
		private:
			int _sx=0;
			int _sy=0;
		    SDL_Event event;
			static FrontendView* m_pInstance;
			FrontendView();  // Private so that it can  not be called
			FrontendView(FrontendView const&){};             // copy constructor is private
			FrontendView& operator=(FrontendView const&){};  // assignment operator is private
	};
}
#endif
