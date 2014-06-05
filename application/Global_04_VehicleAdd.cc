
#include "Global_04_VehicleAdd.h"

Define_Module(VehicleAdd);


VehicleAdd::~VehicleAdd()
{

}


void VehicleAdd::initialize(int stage)
{
    if(stage ==0)
    {
        // get the ptr of the current module
        nodePtr = FindModule<>::findHost(this);
        if(nodePtr == NULL)
            error("can not get a pointer to the module.");

        // get a pointer to the TraCI module
        cModule *module = simulation.getSystemModule()->getSubmodule("TraCI");
        TraCI = static_cast<TraCI_Extend *>(module);

        on = par("on").boolValue();
        mode = par("mode").longValue();

        platoonSize = par("platoonSize").longValue();
        platoonNumber = par("platoonNumber").longValue();
        totalVehicles = par("totalVehicles").longValue();
    }
}


void VehicleAdd::handleMessage(cMessage *msg)
{

}


void VehicleAdd::Add()
{
    // if dynamic adding is off, return
    if (!on)
        return;

    if(mode == 1)
    {
        Scenario1();
    }
    else if(mode == 2)
    {
        Scenario2();
    }
    else if(mode == 3)
    {
        Scenario3();
    }
    else if(mode == 4)
    {
        Scenario4();
    }
}


// for incident detection
void VehicleAdd::Scenario1()
{
    int depart = 0;

    for(int i=1; i<=totalVehicles; i++)
    {
        char vehicleName[10];
        sprintf(vehicleName, "Krauss%d", i);
        depart = depart + 9000;

        uint8_t lane = intrand(3);  // random number in [0,3)

        TraCI->commandAddVehicleN(vehicleName, "TypeManual", "route1", depart, 0, 0, lane);
        TraCI->commandSetLaneChangeMode(vehicleName, 0b1000100101);
    }

    // now we add a vehicle as obstacle
    TraCI->commandAddVehicleN("obstacle", "TypeObstacle", "route1", 50, 3200, 0, 1);

    // make it stop on the lane!
    TraCI->commandSetSpeed("obstacle", 0.);
    TraCI->commandSetLaneChangeMode("obstacle", 0);

    // change the color to blue
    TraCIColor newColor = TraCIColor::fromTkColor("red");
    TraCI->commandSetVehicleColor("obstacle", newColor);
}


void VehicleAdd::Scenario2()
{
    int depart = 0;

    for(int i=1; i<=totalVehicles; i++)
    {
        char vehicleName[10];
        sprintf(vehicleName, "ACC%d", i);
        depart = depart + 10000;

        TraCI->commandAddVehicleN(vehicleName, "TypeACC", "route1", depart, 0, 0, 0);
    }
}


void VehicleAdd::Scenario3()
{
    int depart = 0;

     for(int i=1; i<=totalVehicles; i++)
     {
         char vehicleName[10];
         sprintf(vehicleName, "CACC%d", i);
         depart = depart + 10000;

         TraCI->commandAddVehicleN(vehicleName, "TypeCACC", "route1", depart, 0, 0, 1);  // insert into lane 1
     }
}


void VehicleAdd::Scenario4()
{
    for(int i=1; i<=totalVehicles; i++)
    {
        char vehicleName[10];
        sprintf(vehicleName, "CACC%d", i);
        int depart = 1000 * i;

        if( (i-1) % platoonSize == 0 )
        {
            // platoon leader
         //   TraCI->commandAddVehicleN(vehicleName,"TypeCACC1","route1",depart);
            TraCI->commandAddVehicleN(vehicleName, "TypeACC", "route1", depart, 0, 0, 0);
        }
        else
        {
            // following vehicle
        //    TraCI->commandAddVehicleN(vehicleName,"TypeCACC2","route1",depart);
            TraCI->commandAddVehicleN(vehicleName, "TypeACC", "route1", depart, 0, 0, 0);
        }
    }
}


void VehicleAdd::finish()
{


}
