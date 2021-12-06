/**
 * @file MDSPlusEventDS.h
 * @brief Header file for class MDSPlusEventDS
 * @date 06/12/2021
 * @author Andre Neto
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the class MDSPlusEventDS
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCES_MDSPLUSEVENT_MDSPLUSEVENT_H_
#define DATASOURCES_MDSPLUSEVENT_MDSPLUSEVENT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "mdsobjects.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "EventSem.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

class MARTeMDSTriggerEvent: public MDSplus::Event {
public:
    MARTeMDSTriggerEvent(const char8 *eventName);
    virtual void run();
    void Setup(uint64 *eventTimeIn);
    void Wait();
private:
    EventSem synchSem;
    uint64 *eventTime;
    bool triggered;
};

/**
 * @brief A simple DataSourceI that blocks until an MDSPlus event is received and subsequently NOOPs.
 *
 * The configuration syntax is (names are only given as an example):
 *
 * <pre>
 * +MDSPlusEventDS_0 = {
 *     Class = MDSPlusEventDS
 *     EventName = "MYEVENT" //Compulsory. Name of the MDSPlus event that will be used to trigger the message.
 *     Signals = {
 *         EventTime = {
 *             Type = uint64 //Time of the event as provided by MDSPlus
 *         }
 *     }
 * }
 * </pre>
 */
class MDSPlusEventDS: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor. NOOP.
     */
    MDSPlusEventDS    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~MDSPlusEventDS();

    /**
     * @brief See DataSourceI::AllocateMemory. NOOP.
     * @return true.
     */
    virtual bool AllocateMemory();

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     * @return 1.
     */
    virtual uint32 GetNumberOfMemoryBuffers();

    /**
     * @brief See DataSourceI::GetSignalMemoryBuffer.
     * @pre
     *   SetConfiguredDatabase
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    /**
     * @brief See DataSourceI::GetBrokerName.
     * @details Only InputSignals are supported.
     * @return MemoryMapSynchronisedInputBroker.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @brief Waits on an EventSem for the MDSPlus event to arrive.
     * @return true. 
     */
    virtual bool Synchronise();

    /**
     * @brief See DataSourceI::PrepareNextState. NOOP.
     * @return true.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    /**
     * @brief Loads and verifies the configuration parameters detailed in the class description.
     * @return true if all the mandatory parameters are correctly specified and if the specified optional parameters have valid values.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Final verification of all the parameters.
     * @details This method verifies that all the parameters requested by the GAMs interacting with this DataSource
     *  are valid and consistent with the parameters set during the initialisation phase.
     * In particular the following conditions shall be met:
     * - One and only one signal shall be set and shall be of type uint64.
     * - At most one GAM shall interact with this DataSourceI.
     * @return true if the conditions above are met.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

private:

    /**
     * Time of the event.
     */
    uint64 eventTime;

    /**
     * @brief The event class to implement the interface between MDSPlus and the DataSourceI Execute method.
     */
    MARTeMDSTriggerEvent *triggerEvent;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCES_MDSPLUSEVENT_MDSPLUSEVENT_H_ */

