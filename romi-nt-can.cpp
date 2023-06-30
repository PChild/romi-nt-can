#define Phoenix_No_WPI // remove WPI dependencies
#include "ctre/Phoenix.h"
#include "ctre/phoenix/cci/Unmanaged_CCI.h"
#include "ctre/phoenix/platform/can/PlatformCAN.h"
#include <networktables/NetworkTableInstance.h>
#include <networktables/NetworkTable.h>
#include <networktables/DoubleTopic.h>
#include <chrono>
#include <thread>

void sleepAndFeedProgram(int ms)
{
    c_FeedEnable(ms);
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
}

int main()
{
    c_RegisterCANbus("can0");
    c_SetPhoenixDiagnosticsStartTime(0);

    std::map<int, std::shared_ptr<TalonSRX>> talons;
    std::map<int, std::shared_ptr<VictorSPX>> victors;
    std::map<int, std::shared_ptr<TalonFX>> falcons;
    std::map<int, std::shared_ptr<CANCoder>> cancoders;

    auto inst = nt::NetworkTableInstance::GetDefault();
    auto table = inst.GetTable("datatable");
    auto xSub = table->GetDoubleTopic("x").Subscribe(0.0);
    auto ySub = table->GetDoubleTopic("y").Subscribe(0.0);

    inst.StartClient4("romi-nt-can");
    inst.SetServerTeam(401); // or use inst.setServer("hostname")
    inst.StartDSClient();    // recommended if running on DS computer; this gets the robot IP from the DS

    int id = 11;

    talons.try_emplace(id, std::make_shared<TalonSRX>(id));
    printf("\n volts: %f \n", talons[id]->GetBusVoltage());

    while (true)
    {
        double x = xSub.Get();
        double y = ySub.Get();

        sleepAndFeedProgram(20);
    }

    return 0;
}