#pragma once

// This file contains "Common definitions" for this ETS2 telemetry plug-in.
// This includes:
// - Debug logging detail options
// - Shared memory map struct layout
// - [..]

#define PLUGIN_REVID					11

#define ETS2                            1
#define ATS                             2
#define UnknownGame                     0

#define ETS2_PLUGIN_LOGGING_ON				0
#define ETS2_PLUGIN_LOGGING_SHAREDMEMORY	0
#define ETS2_PLUGIN_FILENAME_PREFIX "C:\ets2telem_"

#if ETS2_PLUGIN_LOGGING_ON == 1
#define SDK_ENABLE_LOGGING
#endif

#include "scssdk.h"
#define SCS_PLUGIN_MMF_NAME TEXT("Local\\SCSTelemetry")
#define SCS_PLUGIN_MMF_SIZE (32*1024)

#define SCS_PLUGIN_IPC_TCP_PORT 30002
/**
 * \brief string size for all strings (most of them) the amount of fields in the shared memory field
 */
#define stringsize  64

 /**
  * \brief Actual used wheel size of the SDK   not the amount of fields in the shared memory field
  */
#define WHEEL_SIZE 14

  // macro: SUBSTANCE_SIZE
  // The maximum number of substances that are saved to the memory
#define SUBSTANCE_SIZE 25

bool check_min_version(unsigned const int min_ets2, unsigned const int min_ats);
bool check_max_version(unsigned const int min_ets2, unsigned const int min_ats);
enum class configType { substances, controls, hshifter, truck, trailer, job };
enum class gameplayType { cancelled, delivered, fined, tollgate, ferry, train };
void log_line(scs_log_type_t type, const char* text, ...);
void log_line(const char* text, ...);

enum class topic { telemetry };

typedef struct scsTrailer_s { // Size: 1528
	//----- START OF FIRST ZONE AT OFFSET 0 -----//
	struct {
		bool wheelSteerable[16] = { false };
		bool wheelSimulated[16] = { false };
		bool wheelPowered[16] = { false };
		bool wheelLiftable[16] = { false };
	}con_b;
	struct {
		bool wheelOnGround[16] = { false };
		bool attached = false;
	}com_b;
	char buffer_b[3];
	//----- END OF FIRST ZONE AT OFFSET 83 -----//
	//----- START OF SECOND ZONE AT OFFSET 84 -----//
	struct {
		unsigned int wheelSubstance[16] = { 0 };
	}com_ui;
	struct {
		unsigned int wheelCount = 0;
	}con_ui;
	//----- END OF SECOND ZONE AT OFFSET 151 -----//
	//----- START OF THIRD ZONE AT OFFSET 152 -----//
	struct {
		float cargoDamage = 0.0f;
		float wearChassis = 0.0f;
		float wearWheels = 0.0f;
		float wheelSuspDeflection[16] = { 0.0f };
		float wheelVelocity[16] = { 0.0f };
		float wheelSteering[16] = { 0.0f };
		float wheelRotation[16] = { 0.0f };
		float wheelLift[16] = { 0.0f };
		float wheelLiftOffset[16] = { 0.0f };
	}com_f;
	struct {
		float wheelRadius[16] = { 0.0f };
	}con_f;
	//----- END OF THIRD ZONE AT OFFSET 611 -----//
		//----- START OF 4TH ZONE AT OFFSET 612 -----//
	struct {
		float linearVelocityX = 0.0f;
		float linearVelocityY = 0.0f;
		float linearVelocityZ = 0.0f;
		float angularVelocityX = 0.0f;
		float angularVelocityY = 0.0f;
		float angularVelocityZ = 0.0f;
		float linearAccelerationX = 0.0f;
		float linearAccelerationY = 0.0f;
		float linearAccelerationZ = 0.0f;
		float angularAccelerationX = 0.0f;
		float angularAccelerationY = 0.0f;
		float angularAccelerationZ = 0.0f;
	}com_fv;
	struct {
		float hookPositionX = 0.0f;
		float hookPositionY = 0.0f;
		float hookPositionZ = 0.0f;
		float wheelPositionX[16] = { 0.0f };
		float wheelPositionY[16] = { 0.0f };
		float wheelPositionZ[16] = { 0.0f };
	}con_fv;
	//----- END OF 4TH ZONE AT OFFSET 863 -----//
		//----- START OF 5TH ZONE AT OFFSET 864 -----//
	struct {
		double worldX = 0.0;
		double worldY = 0.0;
		double worldZ = 0.0;
		double rotationX = 0.0;
		double rotationY = 0.0;
		double rotationZ = 0.0;
	}com_dp;

	//----- END OF 5TH ZONE AT OFFSET 911 -----//
		//----- START OF 6TH ZONE AT OFFSET 912 -----//
	struct {
		char id[stringsize] = { 0 };
		char cargoAcessoryId[stringsize] = { 0 };
		char bodyType[stringsize] = { 0 };
		char brandId[stringsize] = { 0 };
		char brand[stringsize] = { 0 };
		char name[stringsize] = { 0 };
		char chainType[stringsize] = { 0 };
		char licensePlate[stringsize] = { 0 };
		char licensePlateCountry[stringsize] = { 0 };
		char licensePlateCountryId[stringsize] = { 0 };
	}con_s;
	//----- END OF 6TH ZONE AT OFFSET 1527 -----//
}scsTrailer_t;

/**
   *  \brief Telemetry object
   *
   * instead to use a clear object like that in c# we want to create this one easy to parse and modifyable
   *
   * Look in the c header of the sdk for more desription or in the c# description
   *
   * Arrays must be set here and have all local parameter for their size
   */
typedef struct scsTelemetryMap_s
{
	//----- START OF FIRST ZONE AT OFFSET 0 -----//
	// Staring with values needed to handle the data
	// display if game / sdk runs
	bool sdkActive = false;
	char placeHolder[3] = { 0 };
    // check if the game and the telemetry is paused
	bool paused = false;
	char placeHolder2[3] = { 0 };

	// not the game time, only a timestamp. Used to update the values on the other site of the shared memory
	unsigned long long time = 0;	
    unsigned long long simulatedTime = 0;
    unsigned long long renderTime = 0;

	// to make a buffer for changes here and avoid errors later we create a empty room so we fill the first 40 fields, shrink it when you add something above here
	char buffer[8];

	//----- END OF FIRST ZONE AT OFFSET 39 -----//in

	//----- START OF SECOND ZONE AT OFFSET 40 -----//
	// The Secon zone contains unsigned integers and it sorted in sub structures

	// Contains Game independent values and plugin version
	struct
	{
		// Telemetry Plugin Version
		unsigned int telemetry_plugin_revision = 0;
		// Game major version
		unsigned int version_major = 0;
		// Game minor version
		unsigned int version_minor = 0;
		// Game identifier
		unsigned int game = 0; // actually 0 for unknown,1 for ets2 and 2 for ats
		// Game telemetry version major
		unsigned int telemetry_version_game_major = 0;
		// Game telemetry version minor
		unsigned int telemetry_version_game_minor = 0;
	} scs_values;

	// Contains common unsigned integers
	struct {
		// In game time in minutes
		unsigned int time_abs = 0;
	}common_ui;

	// Contains config unsigned integers
	struct {
		unsigned int gears = 0;
		unsigned int gears_reverse = 0;
		unsigned int retarderStepCount = 0;
		unsigned int truckWheelCount = 0;
		unsigned int selectorCount = 0;
		unsigned int time_abs_delivery = 0;
		unsigned int maxTrailerCount = 0;
		unsigned int unitCount = 0;
        unsigned int plannedDistanceKm = 0;
	}config_ui;

	// Contains trailer/truck channel unsigned integers
	struct {
		unsigned int shifterSlot = 0;
		unsigned int retarderBrake = 0;
		unsigned int lightsAuxFront = 0;
		unsigned int lightsAuxRoof = 0;
		unsigned int truck_wheelSubstance[16] = { 0 };
		unsigned int hshifterPosition[32] = { 0 };
		unsigned int hshifterBitmask[32] = { 0 };
	}truck_ui;

	struct {
		unsigned int jobDeliveredDeliveryTime = 0;
        unsigned int jobStartingTime = 0;
        unsigned int jobFinishedTime = 0;
	}gameplay_ui;
	char buffer_ui[48];
	//----- END OF SECOND ZONE AT OFFSET 499 -----//

	//----- START OF Third ZONE AT OFFSET 500 -----//
	// The third zone contains integers and is sorted in sub structures

	struct {
		int restStop = 0;
	}common_i;

	struct {
		int gear = 0;
		int gearDashboard = 0;
		int hshifterResulting[32] = { 0 };
	}truck_i;

	struct {
		int jobDeliveredEarnedXp = 0;
	}gameplay_i;

	char buffer_i[56];
	//----- END OF third ZONE AT OFFSET 699 -----//

	//----- START OF FOURTH ZONE AT OFFSET 700 -----//
	// The fourth zone contains floats and is sorted in sub structures

	struct {
		float scale = 0.0f;
	}common_f;

	struct {
		float fuelCapacity = 0.0f;
		float fuelWarningFactor = 0.0f;
		float adblueCapacity = 0.0f;
		float adblueWarningFactor = 0.0f;
		float airPressureWarning = 0.0f;
		float airPressurEmergency = 0.0f;
		float oilPressureWarning = 0.0f;
		float waterTemperatureWarning = 0.0f;
		float batteryVoltageWarning = 0.0f;
		float engineRpmMax = 0.0f;
		float gearDifferential = 0.0f;
		float cargoMass = 0.0f;
		float truckWheelRadius[16] = { 0.0f };
		float gearRatiosForward[24] = { 0.0f };
		float gearRatiosReverse[8] = { 0.0f };
		float unitMass = 0.0f;
	}config_f;

	struct {
		float speed = 0.0f;
		float engineRpm = 0.0f;
		float userSteer = 0.0f;
		float userThrottle = 0.0f;
		float userBrake = 0.0f;
		float userClutch = 0.0f;
		float gameSteer = 0.0f;
		float gameThrottle = 0.0f;
		float gameBrake = 0.0f;
		float gameClutch = 0.0f;
		float cruiseControlSpeed = 0.0f;
		float airPressure = 0.0f;
		float brakeTemperature = 0.0f;
		float fuel = 0.0f;
		float fuelAvgConsumption = 0.0f;
		float fuelRange = 0.0f;
		float adblue = 0.0f;
		float oilPressure = 0.0f;
		float oilTemperature = 0.0f;
		float waterTemperature = 0.0f;
		float batteryVoltage = 0.0f;
		float lightsDashboard = 0.0f;
		float wearEngine = 0.0f;
		float wearTransmission = 0.0f;
		float wearCabin = 0.0f;
		float wearChassis = 0.0f;
		float wearWheels = 0.0f;
		float truckOdometer = 0.0f;
		float routeDistance = 0.0f;
		float routeTime = 0.0f;
		float speedLimit = 0.0f;
		float truck_wheelSuspDeflection[16] = { 0.0f };
		float truck_wheelVelocity[16] = { 0.0f };
		float truck_wheelSteering[16] = { 0.0f };
		float truck_wheelRotation[16] = { 0.0f };
		float truck_wheelLift[16] = { 0.0f };
		float truck_wheelLiftOffset[16] = { 0.0f };
	}truck_f;

	struct {
		float jobDeliveredCargoDamage = 0.0f;
		float jobDeliveredDistanceKm = 0.0f;
        float refuelAmount = 0.0f;
	}gameplay_f;

	struct {
		float cargoDamage = 0.0f;
	}job_f;
	char buffer_f[28];
	//----- END OF FOURTH ZONE AT OFFSET 1499 -----//

	//----- START OF FIFTH ZONE AT OFFSET 1500 -----//
	// The fifth zone contains bool and is sorted in sub structures

	struct {
		bool truckWheelSteerable[16] = { false };
		bool truckWheelSimulated[16] = { false };
		bool truckWheelPowered[16] = { false };
		bool truckWheelLiftable[16] = { false };

		bool isCargoLoaded = false;
		bool specialJob = false;
	}config_b;

	struct {
		bool parkBrake = false;
		bool motorBrake = false;
		bool airPressureWarning = false;
		bool airPressureEmergency = false;
		bool fuelWarning = false;
		bool adblueWarning = false;
		bool oilPressureWarning = false;
		bool waterTemperatureWarning = false;
		bool batteryVoltageWarning = false;
		bool electricEnabled = false;
		bool engineEnabled = false;
		bool wipers = false;
		bool blinkerLeftActive = false;
		bool blinkerRightActive = false;
		bool blinkerLeftOn = false;
		bool blinkerRightOn = false;
		bool lightsParking = false;
		bool lightsBeamLow = false;
		bool lightsBeamHigh = false;
		bool lightsBeacon = false;
		bool lightsBrake = false;
		bool lightsReverse = false;
		bool cruiseControl = false; // special field not a sdk field
		bool truck_wheelOnGround[16] = { false };
		bool shifterToggle[2] = { false };
	}truck_b;

	struct
	{
		bool jobDeliveredAutoparkUsed = false;
		bool jobDeliveredAutoloadUsed = false;
	}gameplay_b;

	char buffer_b[31];
	//----- END OF FIFTH ZONE AT OFFSET 1639 -----//

	//----- START OF SIXTH ZONE AT OFFSET 1640 -----//
	// The sixth zone contains fvector and is sorted in sub structures

	struct {
		float cabinPositionX = 0.0f;
		float cabinPositionY = 0.0f;
		float cabinPositionZ = 0.0f;
		float headPositionX = 0.0f;
		float headPositionY = 0.0f;
		float headPositionZ = 0.0f;
		float truckHookPositionX = 0.0f;
		float truckHookPositionY = 0.0f;
		float truckHookPositionZ = 0.0f;
		float truckWheelPositionX[16] = { 0.0f };
		float truckWheelPositionY[16] = { 0.0f };
		float truckWheelPositionZ[16] = { 0.0f };
	}config_fv;
	struct {
		float lv_accelerationX = 0.0f;
		float lv_accelerationY = 0.0f;
		float lv_accelerationZ = 0.0f;
		float av_accelerationX = 0.0f;
		float av_accelerationY = 0.0f;
		float av_accelerationZ = 0.0f;
		float accelerationX = 0.0f;
		float accelerationY = 0.0f;
		float accelerationZ = 0.0f;
		float aa_accelerationX = 0.0f;
		float aa_accelerationY = 0.0f;
		float aa_accelerationZ = 0.0f;
		float cabinAVX = 0.0f;
		float cabinAVY = 0.0f;
		float cabinAVZ = 0.0f;
		float cabinAAX = 0.0f;
		float cabinAAY = 0.0f;
		float cabinAAZ = 0.0f;
	}truck_fv;
	char buffer_fv[60];
	//----- END OF SIXTH ZONE AT OFFSET 1999 -----//

	//----- START OF 7TH ZONE AT OFFSET 2000 -----//
	// The 7th zone contains fplacement and is sorted in sub structures
	struct {
		float cabinOffsetX = 0.0f;
		float cabinOffsetY = 0.0f;
		float cabinOffsetZ = 0.0f;
		float cabinOffsetrotationX = 0.0f;
		float cabinOffsetrotationY = 0.0f;
		float cabinOffsetrotationZ = 0.0f;
		float headOffsetX = 0.0f;
		float headOffsetY = 0.0f;
		float headOffsetZ = 0.0f;
		float headOffsetrotationX = 0.0f;
		float headOffsetrotationY = 0.0f;
		float headOffsetrotationZ = 0.0f;
	}truck_fp;
	char buffer_fp[152];
	//----- END OF 7TH ZONE AT OFFSET 2199 -----//

	//----- START OF 8TH ZONE AT OFFSET 2200 -----//
	// The 8th zone contains dplacement and is sorted in sub structures

	struct {
		double coordinateX = 0.0;
		double coordinateY = 0.0;
		double coordinateZ = 0.0;
		double rotationX = 0.0;
		double rotationY = 0.0;
		double rotationZ = 0.0;
	}truck_dp;
	char buffer_dp[52];
	//----- END OF 8TH ZONE AT OFFSET 2299 -----//

	//----- START OF 9TH ZONE AT OFFSET 2300 -----//
	// The 9th zone contains strings and is sorted in sub structures

	struct {
		char truckBrandId[stringsize] = { 0 };
		char truckBrand[stringsize] = { 0 };
		char truckId[stringsize] = { 0 };

		char truckName[stringsize] = { 0 };
		char cargoId[stringsize] = { 0 };
		char cargo[stringsize] = { 0 };
		char cityDstId[stringsize] = { 0 };
		char cityDst[stringsize] = { 0 };
		char compDstId[stringsize] = { 0 };
		char compDst[stringsize] = { 0 };
		char citySrcId[stringsize] = { 0 };
		char citySrc[stringsize] = { 0 };
		char compSrcId[stringsize] = { 0 };
		char compSrc[stringsize] = { 0 };
		char shifterType[16] = { 0 };

		char truckLicensePlate[stringsize] = { 0 };
		char truckLicensePlateCountryId[stringsize] = { 0 };
		char truckLicensePlateCountry[stringsize] = { 0 };

		char jobMarket[32] = { 0 };
	}config_s;
	struct {
		char fineOffence[32] = { 0 };
		char ferrySourceName[stringsize] = { 0 };
		char ferryTargetName[stringsize] = { 0 };
		char ferrySourceId[stringsize] = { 0 };
		char ferryTargetId[stringsize] = { 0 };
		char trainSourceName[stringsize] = { 0 };
		char trainTargetName[stringsize] = { 0 };
		char trainSourceId[stringsize] = { 0 };
		char trainTargetId[stringsize] = { 0 };
	}gameplay_s;

	char buffer_s[20];
	//----- END OF 9TH ZONE AT OFFSET 3999 -----//

	//----- START OF 10TH ZONE AT OFFSET 4000 -----//
	// The 10th zone contains unsigned long long and is sorted in sub structures

	struct {
		unsigned long long jobIncome = 0;
	}config_ull;
	char buffer_ull[192];
	//----- END OF 10TH ZONE AT OFFSET 4199 -----//

		//----- START OF 11TH ZONE AT OFFSET 4200 -----//
	// The 11th zone contains long long and is sorted in sub structures

	struct {
		long long jobCancelledPenalty = 0;
		long long jobDeliveredRevenue = 0;
		long long fineAmount = 0;
		long long tollgatePayAmount = 0;
		long long ferryPayAmount = 0;
		long long trainPayAmount = 0;
	}gameplay_ll;
	char buffer_ll[52];
	//----- END OF 11TH ZONE AT OFFSET 4299 -----//

	//----- START OF 12TH ZONE AT OFFSET 4300 -----//
	// The 12th zone contains special events and is sorted in sub structures

	struct {
		bool onJob = false;
		bool jobFinished = false;
		bool jobCancelled = false;
		bool jobDelivered = false;
		bool fined = false;
		bool tollgate = false;
		bool ferry = false;
		bool train = false;
		bool refuel = false;
		bool refuelPayed = false;
	}special_b;

	char buffer_special[90];
	//----- END OF 12TH ZONE AT OFFSET 4399 -----//

	//----- START OF 13TH ZONE AT OFFSET 4400 -----//
	// The 13th zone contains substances, place for 25 of them

	struct {
		char substance[SUBSTANCE_SIZE][stringsize] = { {0} };
	}substances;
	//----- END OF 13TH ZONE AT OFFSET 5999 -----//

	//----- START OF 14TH ZONE AT OFFSET 6000 -----//
	// The 14th zone contains values of up to 10 trailers (each have a size of 1552)
	struct {
		scsTrailer_t trailer[10];
	}trailer;

	//----- END OF 14TH ZONE AT OFFSET 22420 -----//
} scsTelemetryMap_t;
