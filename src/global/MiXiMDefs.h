//
// Copyright (C) 2004 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.

#ifndef __MIXIM_MIXIMDEFS_H
#define __MIXIM_MIXIMDEFS_H

#include <omnetpp.h>

#define MIXIM_VERSION 0x0202

#define MIXIM_SIGNAL_PACKET_NAME          "packetName"
#define MIXIM_SIGNAL_UWBIRPACKET_NAME     "uwbirpacket"
#define MIXIM_SIGNAL_DROPPEDPACKET_NAME   "droppedpacket"
#define MIXIM_SIGNAL_PASSEDMSG_NAME       "passedmsg"
#define MIXIM_SIGNAL_HOSTSTATE_NAME       "hoststate"
#define MIXIM_SIGNAL_MOBILITY_CHANGE_NAME "veinsmobilityStateChanged"
#define MIXIM_SIGNAL_BATTERY_CHANGE_NAME  "batterystate"
#define MIXIM_SIGNAL_MOBANMSG_NAME        "bbmobanmessage"

#if defined(MIXIM_EXPORT)
#  define MIXIM_API OPP_DLLEXPORT
#elif defined(MIXIM_IMPORT)
#  define MIXIM_API OPP_DLLIMPORT
#else
#  define MIXIM_API
#endif

/**
 * @brief Helper function to initialize signal change identifier on use and
 *        not on initializing static sections.
 */
class MIXIM_API simsignalwrap_t
{
private:

    mutable volatile omnetpp::simsignal_t ssChangeSignal;
    const char *const            sSignalName;
    mutable const char *         sRunId;

public:

    simsignalwrap_t(const char *const pSignalName): ssChangeSignal(SIMSIGNAL_NULL), sSignalName(pSignalName), sRunId(NULL) { }

    simsignalwrap_t(const simsignalwrap_t& pCpy) : ssChangeSignal(pCpy.ssChangeSignal), sSignalName(pCpy.sSignalName), sRunId(pCpy.sRunId) { }

    /** Cast operator to simsignal_t, we initialize the signal here if it is empty ;). */
    operator omnetpp::simsignal_t () const
    {
        // if this signal was never used (or if this is a new run): register the signal
        if ((ssChangeSignal == SIMSIGNAL_NULL) || (getRunId() != sRunId))
        {
            ASSERT(sSignalName);
            sRunId = getRunId();
            ssChangeSignal = omnetpp::cComponent::registerSignal(sSignalName);
            // opp_warning("%d = cComponent::registerSignal(\"%s\")", ssChangeSignal, sSignalName);
        }

        return ssChangeSignal;
    }

protected:

    const char* getRunId() const
    {
        return omnetpp::cSimulation::getActiveSimulation()->getEnvir()->getConfigEx()->getVariable(CFGVAR_RUNID);
    }

private:

    // not allowed
    simsignalwrap_t(): ssChangeSignal(SIMSIGNAL_NULL), sSignalName(NULL), sRunId(NULL) { }
};

#endif
