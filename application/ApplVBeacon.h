
#ifndef ApplVBeacon_H
#define ApplVBeacon_H

#include "ApplVBase.h"

class ApplVBeacon : public ApplVBase
{
	public:
        ~ApplVBeacon();
		virtual void initialize(int stage);
        virtual void finish();

	protected:
        // NED variables (beaconing parameters)
        bool sendBeacons;
		double beaconInterval;
		double maxOffset;
        int beaconLengthBits;
        int beaconPriority;

        // Class variables
        simtime_t individualOffset;
        cMessage* sendBeaconEvt;
        bool pauseBeaconing;

        // Methods
        virtual void handleLowerMsg(cMessage*);
        virtual void handleSelfMsg(cMessage*);
        virtual void handlePositionUpdate(cObject*);

        virtual void onBeaconVehicle(BeaconVehicle*);

        BeaconVehicle* prepareBeacon();
        void printBeaconContent(BeaconVehicle*);
};

#endif
