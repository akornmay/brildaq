#ifndef _bril_pltslinkprocessor_receiver_h
#define _bril_pltslinkprocessor_receiver_h

#include <string>
#include <vector>
#include <map>
#include "xdaq/Application.h"
#include "xgi/framework/UIManager.h"
#include "xgi/framework/Method.h"
#include "xgi/exception/Exception.h"
#include "xdata/InfoSpace.h"
#include "xdata/ActionListener.h"
#include "xdata/String.h"
#include "toolbox/ActionListener.h"
#include "toolbox/EventDispatcher.h"
#include "toolbox/task/WorkLoop.h"
#include "eventing/api/Member.h"
#include "b2in/nub/exception/Exception.h"
#include "b2in/nub/Method.h"
#include "toolbox/BSem.h"

namespace bril {
    namespace pltslinkprocessor {
        class receiver: public xdaq::Application, 
                        public xdata::ActionListener, 
                        public toolbox::ActionListener, 
                        public toolbox::EventDispatcher, 
                        public eventing::api::Member {
        public:
            XDAQ_INSTANTIATOR();

            receiver(xdaq::ApplicationStub* s) throw (xdaq::exception::Exception);
            ~receiver ();
            void actionPerformed(xdata::Event&);
            void actionPerformed(toolbox::Event&);     
            void onMessage(toolbox::mem::Reference*, xdata::Properties&) throw (b2in::nub::exception::Exception);

        private:
            xdata::String bus_name_;
            xdata::String topic_name_;
            toolbox::BSem m_applock;

        private: 
            receiver(const receiver&);
            receiver& operator=(const receiver&);

        };
    } // end namepace pltslinkprocessor
} // end namespace bril
#endif
