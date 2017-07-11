#include "bril/pltslinkprocessor/receiver.h"
#include "bril/pltslinkprocessor/Events.h"
#include "bril/pltslinkprocessor/exception/Exception.h"
#include "xdata/InfoSpaceFactory.h"
#include "cgicc/Cgicc.h"
#include "cgicc/HTMLClasses.h"
#include "b2in/nub/Method.h"
#include "toolbox/task/WorkLoop.h"
#include "toolbox/task/WorkLoopFactory.h"
#include "toolbox/mem/HeapAllocator.h"
#include "toolbox/mem/MemoryPoolFactory.h"
#include "toolbox/mem/AutoReference.h"
#include "xcept/tools.h"
#include "interface/bril/BCM1FTopics.hh"
#include "toolbox/task/Guard.h"

XDAQ_INSTANTIATOR_IMPL(bril::pltslinkprocessor::receiver)
namespace bril {
    namespace pltslinkprocessor {
        
        receiver::receiver (xdaq::ApplicationStub* s)
        throw (xdaq::exception::Exception) : xdaq::Application(s), eventing::api::Member(this), 
        m_applock(toolbox::BSem::FULL)
        {
            b2in::nub::bind(this, &bril::pltslinkprocessor::receiver::onMessage);
            this->getApplicationInfoSpace()->fireItemAvailable("bus", &bus_name_);
            this->getApplicationInfoSpace()->fireItemAvailable("topic", &topic_name_);
            this->getApplicationInfoSpace()->addListener(this, "urn:xdaq-event:setDefaultValues");
        }

        void receiver::actionPerformed(xdata::Event& e)
        {
            if (e.type() == "urn:xdaq-event:setDefaultValues") {
                try {this->getEventingBus(bus_name_.value_).subscribe(topic_name_.value_);}
                catch (eventing::api::exception::Exception &e) {
                    std::cout << "Failed to subscribe - " << 
                    xcept::stdformat_exception(e) << std::endl;
                }
            }
        }

        void receiver::actionPerformed(toolbox::Event& e){}

        void receiver::onMessage(toolbox::mem::Reference *ref, xdata::Properties &plist)
        throw (b2in::nub::exception::Exception) {
            toolbox::mem::AutoReference refguard(ref); // guarantee ref released 
                                                       // when refguard out of scope
            std::string action = plist.getProperty("urn:b2in-eventing:action"); 
            if (action == "notify" && ref!= 0) {
                std::string topic = plist.getProperty("urn:b2in-eventing:topic");
                if (topic == topic_name_.value_) {
                    interface::bril::DatumHead* thead; 
                    thead = (interface::bril::DatumHead*)(ref->getDataLocation());
                    std::string payloaddict = plist.getProperty("PAYLOAD_DICT");
                    interface::bril::CompoundDataStreamer tc(payloaddict);
                    void *avgraw = malloc(sizeof(float)+1);
                    void *avg = malloc(sizeof(float)+1);
                    tc.extract_field(avgraw, "avgraw", thead->payloadanchor);
                    tc.extract_field(avg, "avg", thead->payloadanchor);
                    std::cout << "avgraw = " << *(float *)avgraw << std::endl;
                    std::cout << "avg = " << *(float *)avg << std::endl;
                }
            }                
        }

        receiver::~receiver(){}

    }  //end namespace pltslinkprocessor
} //end namespace bril
    
 
