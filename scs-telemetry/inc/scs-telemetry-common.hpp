#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#ifndef SCS_TELEMETRY_COMMON_HPP
#define SCS_TELEMETRY_COMMON_HPP

// This file contains "Common definitions" for this ETS2 telemetry plug-in.
// This includes:
// - Debug logging detail options
// - [..]

#define PLUGIN_REVID					11

#define ETS2                            1
#define ATS                             2
#define UnknownGame                     0

#define ETS2_PLUGIN_LOGGING_ON				0

#if ETS2_PLUGIN_LOGGING_ON == 1
#define SDK_ENABLE_LOGGING
#endif

#ifdef WIN32
#define COPY(destination, source, count) strncpy_s(destination, source, count)
#else
#define COPY(destination, source, count) strncpy(destination, source, count)
#endif

#include "scssdk.h"
/**
 * \brief string size for all strings (most of them) the amount of fields in the shared memory field
 */
#define STRING_SIZE  64

 /**
  * \brief Actual used wheel size of the SDK   not the amount of fields in the shared memory field
  */
#define WHEEL_SIZE 14

  // macro: SUBSTANCE_SIZE
  // The maximum number of substances that are saved to the memory
#define SUBSTANCE_SIZE 25

bool check_min_version(unsigned int min_ets2, unsigned int min_ats);
bool check_max_version(unsigned int min_ets2, unsigned int min_ats);
enum configType { substances, controls, hshifter, truck, trailer, job };
enum gameplayType { cancelled, delivered, fined, tollgate, ferry, train };
void log_line(scs_log_type_t type, const char* text, ...);
void log_line(const char* text, ...);

typedef struct scsTrailer_s { // Size: 1552
	//----- START OF FIRST ZONE AT OFFSET 0 -----//
	struct {
		bool wheelSteerable[16];
		bool wheelSimulated[16];
		bool wheelPowered[16];
		bool wheelLiftable[16];
	}con_b;
	struct {
		bool wheelOnGround[16];
		bool attached;
	}com_b;
    char buffer_b[3];
	//----- END OF FIRST ZONE AT OFFSET 83 -----//
	//----- START OF SECOND ZONE AT OFFSET 84 -----//
	struct {
		unsigned int wheelSubstance[16];
	}com_ui;
	struct {
		unsigned int wheelCount;
	}con_ui;
	//----- END OF SECOND ZONE AT OFFSET 151 -----//
	//----- START OF THIRD ZONE AT OFFSET 152 -----//
	struct {
		float cargoDamage;
		float wearChassis;
		float wearWheels;
		float wheelSuspensionDeflection[16];
		float wheelVelocity[16];
		float wheelSteering[16];
		float wheelRotation[16];
		float wheelLift[16];
		float wheelLiftOffset[16];
	}com_f;
	struct {
		float wheelRadius[16];
	}con_f;
	//----- END OF THIRD ZONE AT OFFSET 611 -----//
		//----- START OF 4TH ZONE AT OFFSET 612 -----//
	struct {
		float linearVelocityX;
		float linearVelocityY;
		float linearVelocityZ;
		float angularVelocityX;
		float angularVelocityY;
		float angularVelocityZ;
		float linearAccelerationX;
		float linearAccelerationY;
		float linearAccelerationZ;
		float angularAccelerationX;
		float angularAccelerationY;
		float angularAccelerationZ;
	}com_fv;
	struct {
		float hookPositionX;
		float hookPositionY;
		float hookPositionZ;
		float wheelPositionX[16];
		float wheelPositionY[16];
		float wheelPositionZ[16];
	}con_fv;
	//----- END OF 4TH ZONE AT OFFSET 863 -----//
		//----- START OF 5TH ZONE AT OFFSET 864 -----//
	struct {
		double worldX;
		double worldY;
		double worldZ;
		double rotationX;
		double rotationY;
		double rotationZ;
	}com_dp;

	//----- END OF 5TH ZONE AT OFFSET 911 -----//
		//----- START OF 6TH ZONE AT OFFSET 912 -----//
	struct {
		char id[STRING_SIZE];
		char cargoAccessoryId[STRING_SIZE];
		char bodyType[STRING_SIZE];
		char brandId[STRING_SIZE];
		char brand[STRING_SIZE];
		char name[STRING_SIZE];
		char chainType[STRING_SIZE];
		char licensePlate[STRING_SIZE];
		char licensePlateCountry[STRING_SIZE];
		char licensePlateCountryId[STRING_SIZE];
	}con_s;
	//----- END OF 6TH ZONE AT OFFSET 1551 -----//
}scsTrailer_t;

/**
   *  \brief Telemetry object
   *
   * instead to use a clear object like that in c# we want to create this one easy to parse and modifiable
   *
   * Look in the c header of the sdk for more description or in the c# description
   *
   * Arrays must be set here and have all local parameter for their size
   */
typedef struct scsTelemetryMap_s
{
	//----- START OF FIRST ZONE AT OFFSET 0 -----//
	// Staring with values needed to handle the data
	// display if game / sdk runs
	bool sdkActive;
	char placeHolder[3];
    // check if the game and the telemetry is paused
	bool paused;
	char placeHolder2[3];

	// not the game time, only a timestamp. Used to update the values on the other site of the shared memory
	unsigned long long time;	
    unsigned long long simulatedTime;
    unsigned long long renderTime;

	// to make a buffer for changes here and avoid errors later we create a empty room so we fill the first 40 fields, shrink it when you add something above here
	char buffer[8];

	//----- END OF FIRST ZONE AT OFFSET 39 -----//in

	//----- START OF SECOND ZONE AT OFFSET 40 -----//
	// The Second zone contains unsigned integers and it sorted in sub structures

	// Contains Game independent values and plugin version
	struct
	{
		// Telemetry Plugin Version
		unsigned int telemetry_plugin_revision;
		// Game major version
		unsigned int version_major;
		// Game minor version
		unsigned int version_minor;
		// Game identifier
		unsigned int game; // actually 0 for unknown,1 for ets2 and 2 for ats
		// Game telemetry version major
		unsigned int telemetry_version_game_major;
		// Game telemetry version minor
		unsigned int telemetry_version_game_minor;
	} scs_values;

	// Contains common unsigned integers
	struct {
		// In game time in minutes
		unsigned int time_abs;
	}common_ui;

	// Contains config unsigned integers
	struct {
		unsigned int gears;
		unsigned int gears_reverse;
		unsigned int retarderStepCount;
		unsigned int truckWheelCount;
		unsigned int selectorCount;
		unsigned int time_abs_delivery;
		unsigned int maxTrailerCount;
		unsigned int unitCount;
        unsigned int plannedDistanceKm;
	}config_ui;

	// Contains trailer/truck channel unsigned integers
	struct {
		unsigned int shifterSlot;
		unsigned int retarderBrake;
		unsigned int lightsAuxFront;
		unsigned int lightsAuxRoof;
		unsigned int truck_wheelSubstance[16];
		unsigned int hshifterPosition[32];
		unsigned int hshifterBitmask[32];
	}truck_ui;

	struct {
		unsigned int jobDeliveredDeliveryTime;
        unsigned int jobStartingTime;
        unsigned int jobFinishedTime; 
	}gameplay_ui;
	char buffer_ui[48];
	//----- END OF SECOND ZONE AT OFFSET 499 -----//

	//----- START OF Third ZONE AT OFFSET 500 -----//
	// The third zone contains integers and is sorted in sub structures

	struct {
		int restStop;
	}common_i;

	struct {
		int gear;
		int gearDashboard;
		int hshifterResulting[32];
	}truck_i;

	struct {
		int jobDeliveredEarnedXp;
	}gameplay_i;

	char buffer_i[56];
	//----- END OF third ZONE AT OFFSET 699 -----//

	//----- START OF FOURTH ZONE AT OFFSET 700 -----//
	// The fourth zone contains floats and is sorted in sub structures

	struct {
		float scale;
	}common_f;

	struct {
		float fuelCapacity;
		float fuelWarningFactor;
		float adBlueCapacity;
		float adBlueWarningFactor;
		float airPressureWarning;
		float airPressureEmergency;
		float oilPressureWarning;
		float waterTemperatureWarning;
		float batteryVoltageWarning;
		float engineRpmMax;
		float gearDifferential;
		float cargoMass;
		float truckWheelRadius[16];
		float gearRatiosForward[24];
		float gearRatiosReverse[8];
		float unitMass;
	}config_f;

	struct {
		float speed;
		float engineRpm;
		float userSteer;
		float userThrottle;
		float userBrake;
		float userClutch;
		float gameSteer;
		float gameThrottle;
		float gameBrake;
		float gameClutch;
		float cruiseControlSpeed;
		float airPressure;
		float brakeTemperature;
		float fuel;
		float fuelAvgConsumption;
		float fuelRange;
		float adBlue;
		float oilPressure;
		float oilTemperature;
		float waterTemperature;
		float batteryVoltage;
		float lightsDashboard;
		float wearEngine;
		float wearTransmission;
		float wearCabin;
		float wearChassis;
		float wearWheels;
		float truckOdometer;
		float routeDistance;
		float routeTime;
		float speedLimit;
		float truckWheelSuspensionDeflection[16];
		float truckWheelVelocity[16];
		float truckWheelSteering[16];
		float truckWheelRotation[16];
		float truckWheelLift[16];
		float truckWheelLiftOffset[16];
	}truck_f;

	struct {
		float jobDeliveredCargoDamage;
		float jobDeliveredDistanceKm;
        float refuelAmount;
	}gameplay_f;

	struct {
		float cargoDamage;
	}job_f;
	char buffer_f[28];
	//----- END OF FOURTH ZONE AT OFFSET 1499 -----//

	//----- START OF FIFTH ZONE AT OFFSET 1500 -----//
	// The fifth zone contains bool and is sorted in sub structures

	struct {
		bool truckWheelSteerable[16];
		bool truckWheelSimulated[16];
		bool truckWheelPowered[16];
		bool truckWheelLiftable[16];

		bool isCargoLoaded;
		bool specialJob;
	}config_b;

	struct {
		bool parkBrake;
		bool motorBrake;
		bool airPressureWarning;
		bool airPressureEmergency;
		bool fuelWarning;
		bool adBlueWarning;
		bool oilPressureWarning;
		bool waterTemperatureWarning;
		bool batteryVoltageWarning;
		bool electricEnabled;
		bool engineEnabled;
		bool wipers;
		bool blinkerLeftActive;
		bool blinkerRightActive;
		bool blinkerLeftOn;
		bool blinkerRightOn;
		bool lightsParking;
		bool lightsBeamLow;
		bool lightsBeamHigh;
		bool lightsBeacon;
		bool lightsBrake;
		bool lightsReverse;
		bool lightsHazard;
		bool cruiseControl; // special field not a sdk field
		bool truck_wheelOnGround[16];
		bool shifterToggle[2];
		bool differentialLock;
		bool liftAxle;
		bool liftAxleIndicator;
		bool trailerLiftAxle;
		bool trailerLiftAxleIndicator;
	}truck_b;

	struct
	{
		bool jobDeliveredAutoParkUsed;
		bool jobDeliveredAutoloadUsed;
	}gameplay_b;

	char buffer_b[25];
	//----- END OF FIFTH ZONE AT OFFSET 1639 -----//

	//----- START OF SIXTH ZONE AT OFFSET 1640 -----//
	// The sixth zone contains fvector and is sorted in sub structures

	struct {
		float cabinPositionX;
		float cabinPositionY;
		float cabinPositionZ;
		float headPositionX;
		float headPositionY;
		float headPositionZ;
		float truckHookPositionX;
		float truckHookPositionY;
		float truckHookPositionZ;
		float truckWheelPositionX[16];
		float truckWheelPositionY[16];
		float truckWheelPositionZ[16];
	}config_fv;
	struct {
		float lv_accelerationX;
		float lv_accelerationY;
		float lv_accelerationZ;
		float av_accelerationX;
		float av_accelerationY;
		float av_accelerationZ;
		float accelerationX;
		float accelerationY;
		float accelerationZ;
		float aa_accelerationX;
		float aa_accelerationY;
		float aa_accelerationZ;
		float cabinAVX;
		float cabinAVY;
		float cabinAVZ;
		float cabinAAX;
		float cabinAAY;
		float cabinAAZ;
	}truck_fv;
	char buffer_fv[60];
	//----- END OF SIXTH ZONE AT OFFSET 1999 -----//

	//----- START OF 7TH ZONE AT OFFSET 2000 -----//
	// The 7th zone contains fplacement and is sorted in sub structures
	struct {
		float cabinOffsetX;
		float cabinOffsetY;
		float cabinOffsetZ;
		float cabinOffsetRotationX;
		float cabinOffsetRotationY;
		float cabinOffsetRotationZ;
		float headOffsetX;
		float headOffsetY;
		float headOffsetZ;
		float headOffsetRotationX;
		float headOffsetRotationY;
		float headOffsetRotationZ;
	}truck_fp;
	char buffer_fp[152];
	//----- END OF 7TH ZONE AT OFFSET 2199 -----//

	//----- START OF 8TH ZONE AT OFFSET 2200 -----//
	// The 8th zone contains dplacement and is sorted in sub structures

	struct {
		double coordinateX;
		double coordinateY;
		double coordinateZ;
		double rotationX;
		double rotationY;
		double rotationZ;
	}truck_dp;
	char buffer_dp[52];
	//----- END OF 8TH ZONE AT OFFSET 2299 -----//

	//----- START OF 9TH ZONE AT OFFSET 2300 -----//
	// The 9th zone contains strings and is sorted in sub structures

	struct {
		char truckBrandId[STRING_SIZE];
		char truckBrand[STRING_SIZE];
		char truckId[STRING_SIZE];

		char truckName[STRING_SIZE];
		char cargoId[STRING_SIZE];
		char cargo[STRING_SIZE];
		char cityDstId[STRING_SIZE];
		char cityDst[STRING_SIZE];
		char compDstId[STRING_SIZE];
		char compDst[STRING_SIZE];
		char citySrcId[STRING_SIZE];
		char citySrc[STRING_SIZE];
		char compSrcId[STRING_SIZE];
		char compSrc[STRING_SIZE];
		char shifterType[16];

		char truckLicensePlate[STRING_SIZE];
		char truckLicensePlateCountryId[STRING_SIZE];
		char truckLicensePlateCountry[STRING_SIZE];

		char jobMarket[32];
	}config_s;
	struct {
		char fineOffence[32];
		char ferrySourceName[STRING_SIZE];
		char ferryTargetName[STRING_SIZE];
		char ferrySourceId[STRING_SIZE];
		char ferryTargetId[STRING_SIZE];
		char trainSourceName[STRING_SIZE];
		char trainTargetName[STRING_SIZE];
		char trainSourceId[STRING_SIZE];
		char trainTargetId[STRING_SIZE];
	}gameplay_s;

	char buffer_s[20];
	//----- END OF 9TH ZONE AT OFFSET 3999 -----//

	//----- START OF 10TH ZONE AT OFFSET 4000 -----//
	// The 10th zone contains unsigned long long and is sorted in sub structures

	struct {
		unsigned long long jobIncome;
	}config_ull;
	char buffer_ull[192];
	//----- END OF 10TH ZONE AT OFFSET 4199 -----//

		//----- START OF 11TH ZONE AT OFFSET 4200 -----//
	// The 11th zone contains long long and is sorted in sub structures

	struct {
		long long jobCancelledPenalty;
		long long jobDeliveredRevenue;
		long long fineAmount;
		long long tollgatePayAmount;
		long long ferryPayAmount;
		long long trainPayAmount;
	}gameplay_ll;
	char buffer_ll[52];
	//----- END OF 11TH ZONE AT OFFSET 4299 -----//

	//----- START OF 12TH ZONE AT OFFSET 4300 -----//
	// The 12th zone contains special events and is sorted in sub structures

	struct {
		bool onJob;
		bool jobFinished; 
		bool jobCancelled;
		bool jobDelivered;
		bool fined;
		bool tollgate;
		bool ferry;
		bool train;
		bool refuel;
		bool refuelPayed;
	}special_b;

	char buffer_special[90];
	//----- END OF 12TH ZONE AT OFFSET 4399 -----//

	//----- START OF 13TH ZONE AT OFFSET 4400 -----//
	// The 13th zone contains substances, place for 25 of them

	struct {
		char substance[SUBSTANCE_SIZE][STRING_SIZE];
	}substances;
	//----- END OF 13TH ZONE AT OFFSET 5999 -----//

	//----- START OF 14TH ZONE AT OFFSET 6000 -----//
	// The 14th zone contains values of up to 10 trailers (each have a size of 1552)
	struct {
		scsTrailer_t trailer[10];
	}trailer;

	//----- END OF 14TH ZONE AT OFFSET 21519 -----//
} scsTelemetryMap_t;

#endif