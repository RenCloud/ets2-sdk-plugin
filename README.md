
  <a href="https://rencloud.github.io/scs-sdk-plugin/docs/" title="Documentation">
    <img alt="" src="https://img.shields.io/badge/documentation-09.08-green.svg?style=for-the-badge" />
  </a>

  <a href="https://discord.gg/JDqkZZd" title="Discord">
    <img alt="" src="https://img.shields.io/badge/Discord-blue.svg?style=for-the-badge" />
  </a>

fork of [nlhans](https://github.com/nlhans/ets2-sdk-plugin) work

# SCS Telemetry for EuroTruckSimulator 2 and AmericanTruckSimulator

SCS has kindly released a SDK that allows developers and users to stream telemetry data from the game to any 3rd party applications. An example program was provided (and often used) which enabled streaming data by using text files stored on the users harddisk. This puts unnecessary stress on the users harddrive (not the mention the number of re-writes that would hurt SSDs), and moreover requires the user to manually configure the telemetry data source.

This SDK plug-in transports the telemetry stream via a Memory Mapped File. This is a special Windows (file)stream which resides completely in RAM and can be read from multiple applications.

## Installation

Actually you need to build this branch yourself. I will add a release later. Stay tuned for more information.

Installation is easy inside Euro Truck Simulator 2. Place the acquired DLL inside bin/win_x64/plugins/ of your ETS2/ATS installation. It is possible the plugins directory doesn't exists yet (as with every default installation). In that case you need to create the plugins folder. Place the DLL inside the plugins folder.

You will now notice that each time ETS2/ATS now starts it prompts the SDK has been activated. Unfortunately you have to press OK to this message every time, but it's a small price to pay for the added features that are possible via the SDK.

## Developers Information

### Note to current changes (Rev 10 Update 4)

Because the memset in the shutdown function of the c++ code seems sometimes leads to long closing times of ets2/ats this function is removed.
That means at the moment that while the shared memory is open, closing the game will not erase the shared memory values. To identify if the sdk is loaded
use the `SdkActive` field of the SCSTelemetry object. The Dll to Rev 10 Update 4 does not contains the fix that set the value to 0 if the game is closed.
If you do not want to build the dll by yourself you could use the `Timestamp` field. That should be reseted when the game is started.

### Documentation (not completed at the moment)

There is also an Documentation. It tells a lot about the values. An installation, build , etc. guide will also follow. Should there still be questions, feature request or other changes visit the discord server linked at the top.

Sadly the usage of the documentation generating syntax leads to a lot of `warnings`, while compiling the c# part. That's because the documentation use `<` and `>`. But that's no valid xml. Therefore the compiler cry's :cry: and give the warning about invalid xml. With the help of `#pragma` it wont show up.

### Overview

This plug-in stores it's data inside a Memory Mapped File, or "Shared Memory". This allows it to operate without any access to harddrive, or configuration hassle by the user to locate the memory map.

### Rev Numbers

Rev Numbers shows big changes on the shared memory and sometimes on the C# object. That means Rev 10 wont work with Rev 9. Doesn't matter which side is not updated. Sub Versions that you can see in changelog.md should work with small errors or completely without. The C# object is mostly not changed. Only if needed, because of new values (most of the cases) or structure changes (less the case). If this occurs i will notice that. (See changelog.md. If you directly access the shared memory you will find an overview about the changes here.)

### Plugin for 1.41/SDK1.13

Lower SDK Version means there are less values / values that are zero. To get an overview which values that are look at the list at the middle of this document.
Note to the SDK Version: SDK 1.13 is not the same like the sdk version of ETS2 or ATS. Both games have an own SDK version. See list under ATS.

### ETS2

|Game Version|SDK Version|Plugin State|
|------------|-----------|------------|
|1.26 and before|1.12 and before|Not Tested, could work with errors|
|1.27 - 1.34 |1.13       | Should work        |
|1.35        |1.14       | Should work|
|1.36        |1.15       | Should work|
| - 1.40      |1.16       | Should work|
|1.41 beta   |1.17       | Works, Test Version|

### ATS

|Game Version|SDK Version|Plugin State|
|------------|-----------|------------|
|1.34 and before|1.0     |Should Work |
|1.35        |1.01       |Works|
|1.36        |1.02      | Should work|
| - 1.40      |1.03       | Should work|
|1.41 beta   |1.04       | Works, Test Version|

### SDK VERSION AND GAME SDK VERSION

|SDK VERSION|ETS2 SDK Version|ATS SDK VERSION|
|-----------|----------------|---------------|
|1_1        |1.07            | -             |
|1_2        |1.08            | -             |
|1_4        |1.10            | -             |
|1_5        |1.12            | -             |
|1_9        |1.13            |1.00           |
|1_10       |1.14            |1.01           |
|1_11       |1.15            |1.02           |
|1_12       |1.16            |1.03           |
|1_13       |1.17            |1.04           |


### Telemetry fields and the c# object

The following telemetry fields are supported, structure is similar the C# object. Starting with sdk 1.10, game patch 1.35 and ETS2 1.14, ATS 1.01 code for some part of the need different versions of the sdk. The plugin handles this. If a game lower than 1.35 is used, only the values without (1.14/1.01) are possible:

Edit: for better overview it is now (ETS2 SDK/ATS SDK/Game Version). I added the game version, because it is the same for ats and ets2 and most of you probably doesn't know the specific Game SDK Version.

Changes are marked with the <del>deleted</del> Tag.
New stuff is marked with the <ins>inserted</ins> Tag.

<pre>

<strong>Game Values (V.1.10.5)</strong>:
│    ├── Telemetry Timestamp (<mark>not the in-game time</mark>, only for usage in code, see documentation for more information #todo add link) (<mark>now ulong</mark>)
│    ├── <ins>Simulation</ins> Timestamp
│    ├── <ins>Render</ins> Timestamp
│    ├── Paused, game state
│    ├── SCSGame identifier as enum, currently ets2/ats/unknown
│    ├── GameVersion and Game Telemetry Version (major.minor)
│    ├── Dll version (usage in code)
│    ├── TelemetryVersion
│    ├── Substances
│    ├── <strong>Common Values</strong>:
│    │    ├── Scale
│    │    ├── Game Time (Time object with in-game minutes and datetime object)
│    │    ├── NextRestStop (Frequency object, more a time span)
│    │    └── NextRestStopTime (Specific date, calculated)
│    ├── <strong>Truck Values (Contains 2 big parts, and a small one)</strong>:
│    │    ├── <strong>Constants/Configs (Values that barely change)</strong>:
│    │    │    ├── <strong>Motor Values</strong>:
│    │    │    │    ├── Forward Gear Count
│    │    │    │    ├── Reverse Gear Count
│    │    │    │    ├── Retarder Step Count
│    │    │    │    ├── Selector Count
│    │    │    │    ├── Engine RPM Max
│    │    │    │    ├── Differential Ration
│    │    │    │    ├── Gear Ratios Forward
│    │    │    │    ├── Gear Ratios Reverse
│    │    │    │    ├── Shifter Type Value (Enum)
│    │    │    │    ├── SlotGear
│    │    │    │    ├── SlotHandlePosition
│    │    │    │    └── SlotSelectors
│    │    │    ├── <strong>Capacity Values</strong>:
│    │    │    │    ├── Fuel
│    │    │    │    └── Adblue
│    │    │    ├── <strong>Warning Factors</strong>:
│    │    │    │    ├── Fuel
│    │    │    │    ├── Adblue
│    │    │    │    ├── Air Pressure
│    │    │    │    ├── Air Pressure Emergency
│    │    │    │    ├── Oil Pressure
│    │    │    │    ├── Water Temperature
│    │    │    │    └── Battery Voltage
│    │    │    ├── <strong>Wheels</strong>:
│    │    │    │    ├── Count
│    │    │    │    ├── Radius
│    │    │    │    ├── Simulated
│    │    │    │    ├── Powered
│    │    │    │    ├── Liftable
│    │    │    │    └── Steerable
│    │    │    ├── Brand Id (code)
│    │    │    ├── Brand
│    │    │    ├── Id (code)
│    │    │    ├── Name
│    │    │    ├── LicensePlate (1.14/1.01/1.35)
│    │    │    ├── LicensePlateCountryId (1.14/1.01/1.35)
│    │    │    └── LicensePlateCountry (1.14/1.01/1.35)
│    │    ├── <strong>Current Values (Values that change a lot)</strong>:
│    │    │    ├── Electric Enabled
│    │    │    ├── Engine Enabled
│    │    │    ├── <ins>LiftAxle</ins> (1.17/1.04/1.41)
│    │    │    ├── <ins>LiftAxleIndicator</ins> (1.17/1.04/1.41)
│    │    │    ├── <ins>TrailerLiftAxle</ins> (1.17/1.04/1.41)
│    │    │    ├── <ins>TrailerLiftAxleIndicator</ins> (1.17/1.04/1.41)
│    │    │    ├── <strong>Motor Values</strong>:
│    │    │    │    ├── <strong>Gear Values</strong>:
│    │    │    │    │    ├── HShifterSlot
│    │    │    │    │    ├── Selected
│    │    │    │    │    └── HShifterSelector
│    │    │    │    └── <strong>Brake Values</strong>:
│    │    │    │         ├── RetarderLevel
│    │    │    │         ├── Air Pressure
│    │    │    │         ├── Temperature
│    │    │    │         ├── Parking Brake
│    │    │    │         └── Motor Brake
│    │    │    ├── <strong>Dashboard</strong>:
│    │    │    │    ├── <strong>Fuel Values</strong>:
│    │    │    │    │    ├── Amount
│    │    │    │    │    ├── Average Consumption
│    │    │    │    │    └── Range (estimated range with current amount of fuel)
│    │    │    │    ├── <strong>Warnings</strong>:
│    │    │    │    │    ├── Air Pressure
│    │    │    │    │    ├── Air Pressure Emergency
│    │    │    │    │    ├── Fuel warning
│    │    │    │    │    ├── Adblue
│    │    │    │    │    ├── oil pressure
│    │    │    │    │    ├── adblue
│    │    │    │    │    ├── water temperature
│    │    │    │    │    └── battery voltage
│    │    │    │    ├── Gear Dashboards
│    │    │    │    ├── Speed (m/s,km/h,mph)
│    │    │    │    ├── CruiseControlSpeed (m/s,km/h,mph)
│    │    │    │    ├── Adblue amount
│    │    │    │    ├── Oil Pressure
│    │    │    │    ├── Oil Temperature
│    │    │    │    ├── Water Temperature
│    │    │    │    ├── BatteryVoltage
│    │    │    │    ├── RPM
│    │    │    │    ├── Odometer
│    │    │    │    ├── Wipers
│    │    │    │    └── Cruise Control ("special field", same like `CruiseControlspeed == 0`)
│    │    │    ├── <strong>Acceleration</strong>:
│    │    │    │    ├── Linear Velocity
│    │    │    │    ├── Angular Velocity
│    │    │    │    ├── Linear Acceleration
│    │    │    │    ├── Angular Acceleration
│    │    │    │    ├── Cabin Angular Velocity
│    │    │    │    └── Cabin Angular Acceleration
│    │    │    ├── <strong>Lights</strong>:
│    │    │    │    ├── Aux Front (enum for 3 states)
│    │    │    │    ├── Aux Roof (enum for 3 states)
│    │    │    │    ├── Dashboard Backlight
│    │    │    │    ├── Blinker Left Active
│    │    │    │    ├── Blinker Right Active
│    │    │    │    ├── Blinker Left On
│    │    │    │    ├── Blinker Right On
│    │    │    │    ├── Parking
│    │    │    │    ├── Beam Low
│    │    │    │    ├── Beam High
│    │    │    │    ├── Beacon
│    │    │    │    ├── Brake
│    │    │    │    ├── Reverse
│    │    │    │    └── HazardWarningLights (1.17/1.04/1.41)
│    │    │    ├── <strong>Wheels</strong>:
│    │    │    │    ├── Substance
│    │    │    │    ├── SuspDeflection
│    │    │    │    ├── Velocity
│    │    │    │    ├── Steering
│    │    │    │    ├── Rotation
│    │    │    │    ├── Lift
│    │    │    │    ├── Lift Offset
│    │    │    │    ├── on ground
│    │    │    │    └── position
│    │    │    ├── <strong>Damage</strong>:
│    │    │    │    ├── Engine
│    │    │    │    ├── Transmission
│    │    │    │    ├── Cabin
│    │    │    │    ├── chassis
│    │    │    │    └── wheels (avg. of all wheels)
│    │    │    └── Position (position in world space with position and orientation)
│    │    └── <strong>Positioning</strong>:
│    │         ├── Cabin (vehicle space)
│    │         ├── Head (cabin space)
│    │         ├── Hook (vehicle space)
│    │         ├── Head Offset
│    │         ├── Cabin Offset
│    │         └── Contains "more fields" see at the bottom of the list
│    ├── <strong>Trailer Values (will be set to 0, false, etc. if you have no trailer, while on job or with trailer ownership detached wont reset the values)[0-9] (array starting with 1.14/1.01 so 0 for 1 trailer or version lower than that)</strong>:
│    │    ├── Attached
│    │    ├── Hook
│    │    ├── Position
│    │    ├── <strong>Wheel Values</strong>:
│    │    │    ├── Substance
│    │    │    ├── SuspDeflection
│    │    │    ├── Velocity
│    │    │    ├── Steering
│    │    │    ├── Rotation
│    │    │    ├── On Ground
│    │    │    ├── Lift
│    │    │    └── LiftOffset
│    │    ├── <strong>WheelsConstants</strong>:
│    │    │    ├── Count
│    │    │    ├── Radius
│    │    │    ├── Simulated
│    │    │    ├── Powered
│    │    │    ├── Liftable
│    │    │    └── Steerable
│    │    ├── <strong>Acceleration</strong>:
│    │    │    ├── Linear Velocity
│    │    │    ├── Angular Velocity
│    │    │    ├── Linear Acceleration
│    │    │    └── Angular Acceleration
│    │    ├── <strong>DamageValues</strong>:
│    │    │    ├── Cargo (1.14/1.01/1.35)
│    │    │    ├── Wheels
│    │    │    └── Chassis
│    │    ├── Chassis (code)
│    │    ├── Id (code)
│    │    ├── Name
│    │    ├── CargoAccessoryId
│    │    ├── BodyType (1.14/1.01/1.35)
│    │    ├── BrandId (1.14/1.01/1.35)
│    │    ├── Brand (1.14/1.01/1.35)
│    │    ├── Name (1.14/1.01/1.35)
│    │    ├── ChainType (1.14/1.01/1.35)
│    │    ├── LicensePlate (1.14/1.01/1.35)
│    │    ├── LicensePlateCountryId (1.14/1.01/1.35)
│    │    └── LicensePlateCountry (1.14/1.01/1.35)
│    ├── <strong>Job Values(will be reset after the job finished flag is disappeared)</strong>:
│    │    ├── Delivery Time (time object -> in-game minutes and datetime object)
│    │    ├── Remaining Delivery Time (calculated)
│    │    ├── CargoLoaded (1.14/1.01/1.35)
│    │    ├── SpecialJob (1.14/1.01/1.35)
│    │    ├── Market (1.14/1.01/1.35)
│    │    ├── City Destination Id (code)
│    │    ├── City Destination
│    │    ├── Company Destination Id (code)
│    │    ├── Company Destination
│    │    ├── City Source Id (code)
│    │    ├── City Source
│    │    ├── Company Source Id (code)
│    │    ├── Company Source
│    │    ├── Income
│    │    ├── Planned Distance Km (1.15/1.02/1.36)
│    │    └── <strong>Cargo Values</strong>:
│    │         ├── Mass
│    │         ├── Name (code)
│    │         ├── Id (1.14/1.01/1.35)
│    │         ├── UnitCount (1.14/1.01/1.35)
│    │         ├── UnitMass (1.14/1.01/1.35)
│    │         └── CargoDamage (1.14/1.01/1.35)
│    ├── <strong>Control Values</strong>:
│    │    ├── <strong>User Input</strong>:
│    │    │    ├── Steering
│    │    │    ├── Throttle
│    │    │    ├── Brake
│    │    │    └── Clutch
│    │    └── <strong>Game Values</strong>:
│    │         ├── Steering
│    │         ├── Throttle
│    │         ├── Brake
│    │         └── Clutch
│    ├── <strong>Navigation Values</strong>:
│    │    ├── Navigation Distance
│    │    ├── Navigation Time
│    │    └── Speed Limit
│    ├── <strong>SpecialEvents</strong>:
│    │    ├── On Job
│    │    ├── Job Cancelled (1.14/1.01/1.35) (may not work atm?)
│    │    ├── Job Delivered (1.14/1.01/1.35)
│    │    ├── Fined (1.14/1.01/1.35)
│    │    ├── Tollgate (1.14/1.01/1.35)
│    │    ├── Ferry (1.14/1.01/1.35)
│    │    ├── Train (1.14/1.01/1.35)
│    │    ├── Refuel Start
│    │    ├── Refuel End (Fired on refuel stop)
│    │    └── Refuel Payed (Fired on refuel end/engine start)
│    └── <strong>GameplayEvents (1.14/1.01/1.35)</strong>:
│         ├── <strong>Refuel</strong>:
│         │    └── Amount (No Version note)  (see changelog update 10.4 for some notes to that)
│         ├── <strong>Cancelled</strong>:
│         │    ├── Started (This value could be wrong if profile is closed or game is restarted, see changelog update 10.4)
│         │    ├── Finished
│         │    └── Penalty
│         ├── <strong>Delivered</strong>:
│         │    ├── Started (This value could be wrong if profile is closed or game is restarted, see changelog update 10.4)
│         │    ├── Finished>
│         │    ├── StartedBackup (See changelog update 10.4 and documentation for notes to that property)
│         │    ├── AutoLoaded
│         │    ├── AutoParked
│         │    ├── CargoDamage
│         │    ├── DeliveryTime
│         │    ├── DistanceKm
│         │    ├── EarnedXp
│         │    └── Revenue
│         ├── <strong>Fined</strong>:
│         │    ├── Amount
│         │    └── Offence
│         ├── <strong>Tollgate</strong>:
│         │    └── PayAmount
│         └── <strong>Transport</strong>:
│              ├── PayAmount
│              ├── SourceId
│              ├── SourceName
│              ├── TargetId
│              └── TargedName  

</pre>

Also there are a few more fields you can use:

	Truck.Positioning:
		- Head position in Cabin Space
		- Head position in Vehicle Space
		- Head position in World Space
		- Head Position in World Space 
		- Cabin Position in World Space
		- Hook Position in World Space
  
	Methods:
		- In-game Minutes to Date(Time)
		- Add 2 FVectors
		- Add a FVector and a DVector
		- Rotate: Rotates specified vector by specified orientation 

May I forgot something or there is a missing version information. When you found missing values or something else create an issue that would be great.

The fields are updated as fast as ETS2/ATS can and will do, as this is how the SDK has been designed by SCS. When a telemetry value has changed the SDK will immediately call a handler. This plug-in implements this handler which stores the data to the right field inside the data structure.
There is no "sample ticker" yet. This must be done at the client side, by regularly checking if the timestamp has been updated.

## Client Implementations

### C#

Actually I'm not fully happy with the actual demo. But I didn't reached my plan that works like the old one. Later I will change the current demo so that they will be a lot times better.

### Javascript

If you want to use javascript have a look here [Kniffen TruckSim-Telemetry](https://github.com/kniffen/TruckSim-Telemetry).

### Other

For other languages you need to create/find a library that can open and read MemoryMapped files. The data storage format is binary and can be found in "scs-telemetry/inc/scs-telemetry-common.hpp". The shared memory map name is "Local\SCSTelemetry". I will add some more documentary in this header later.
