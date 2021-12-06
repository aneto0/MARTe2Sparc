/**
 * @file MDSPlusEventDS.cpp
 * @brief Source file for class MDSPlusEventDS
 * @date 10/02/2017
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

 * @details This source file contains the definition of all the methods for
 * the class MDSPlusEventDS (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "MDSPlusEventDS.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {


MARTeMDSTriggerEvent::MARTeMDSTriggerEvent(const char8* eventName) : MDSplus::Event(eventName) {
    (void)synchSem.Create();
    synchSem.Reset();
    triggered = false;
}

void MARTeMDSTriggerEvent::Setup(uint64 *eventTimeIn) {
    eventTime = eventTimeIn;
}

void MARTeMDSTriggerEvent::run() {
    *eventTime = getTime()->getLongUnsigned();
    synchSem.Post();
    triggered = true;
}

void MARTeMDSTriggerEvent::Wait() {
    if (!triggered) {
        synchSem.Wait();
    }
}

MDSPlusEventDS::MDSPlusEventDS() :
        DataSourceI() {
    triggerEvent = NULL_PTR(MARTeMDSTriggerEvent *);
}

/*lint -e{1551} -e{1579} the destructor must guarantee that the MDSplus are deleted and the shared memory freed. The brokerAsyncTrigger is freed by the ReferenceT */
MDSPlusEventDS::~MDSPlusEventDS() {
    if (triggerEvent != NULL_PTR(MARTeMDSTriggerEvent *)) {
        delete triggerEvent;
    }
}

bool MDSPlusEventDS::AllocateMemory() {
    return true;
}

uint32 MDSPlusEventDS::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The signalAddress is independent of the bufferIdx.*/
bool MDSPlusEventDS::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {
    /*lint -e{613} dataSourceMemory cannot be NULL here*/
    signalAddress = &eventTime;
    return true;
}

const char8* MDSPlusEventDS::GetBrokerName(StructuredDataI& data, const SignalDirection direction) {
    return "MemoryMapSynchronisedInputBroker";
}

bool MDSPlusEventDS::Synchronise() {
    triggerEvent->Wait(); 
    return true;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: NOOP at StateChange, independently of the function parameters.*/
bool MDSPlusEventDS::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    return true;
}

bool MDSPlusEventDS::Initialise(StructuredDataI& data) {
    bool ok = DataSourceI::Initialise(data);
    StreamString eventName;
    if (ok) {
        ok = data.Read("EventName", eventName);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "EventName shall be specified");
        }
    }
    if (ok) {
        triggerEvent = new MARTeMDSTriggerEvent(eventName.Buffer());
        triggerEvent->Setup(&eventTime);
        triggerEvent->start();
    }
    return ok;
}

bool MDSPlusEventDS::SetConfiguredDatabase(StructuredDataI& data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    //Only one and one GAM allowed to interact with this DataSourceI
    if (ok) {
        ok = (GetNumberOfFunctions() == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Exactly one Function allowed to interact with this DataSourceI");
        }
    }
    if (ok) {
        ok = (GetSignalType(0) == UnsignedInteger64Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The input signal type shall be uint64");
        }
    }
    return ok;
}


CLASS_REGISTER(MDSPlusEventDS, "1.0")
}
