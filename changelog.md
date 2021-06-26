# Changelog

## Rev Pre 11

- update .net Version to 4.8
- update C++ Platform Tools to v142
- update sdk to 1.13, remove old sdks, current sdk is now placed in `scs_sdk` instead of `scs_sdk_{version}`
  - Only a few changes: 5 new Truck values (see below)
- update readme.md
### New Values

- Hazard Warning (Ets2/ATS 1.41)
- Differential Lock (Ets2/ATS 1.41)
- Lift Axle and Indicator, Trailer Lift Axle and Indicator (Ets2/ATS 1.41)

### Shared Memory Changes

- 5th Zone added `lightHazard` after `lightsReverse`, changing offset of the following 7 values
- 5th Zone added the differential lock and lift axle values (5 new values). Placed after `shifterToggle`, changing offset of the following 2 values

## Rev 10 Update 7

- fix (again) of the `onJob`, `jobFinished` flag when `jobDelivered` and `jobChancelled` happens. (see Rev 10 Update 5)

Later added:

- fix resetting job values when using a train

## Rev 10 Update 6

- fix of `RemainingDeliveryTime`. Trough positioning of `GameTime` and `DeliveryTime` in the shared memory, this value was never calculated. #66
  - To avoid broke other stuff that use this or the shared memory, the shared memory layout wasn't change
  - Before when setting the `GameTime` the `RemainingDeliveryTime` was set, but `DeliveryTime` wasn't at that point.
  - Now we set `GameTime` before and when `DeliveryTime` is set we also set `RemainingDeliveryTime` which also makes much more sense.
- fix that the gameplay events are called before the data is updated. Now the data is updated and then events are called.
- the `amount` by the `RefuelEvent` should now delivere better values (sometimes it was a tick behind, that shouldn't be the case anymore).
- fix that `JobCancelledEvent` is called when a profile is changed, but had an active job (seems to happen on the first profile change).

### SDK Changes

- Update to 1.12
  - only adds new fine cases to the `FinedEvent`

### known issues

- when changing the profile or truck an `RefuelPayedEvent` can be called. I thought i fixed it, but i need to rethink how to 

### Shared Memory Changes

- Because new fine values can be longer than before (instead of 16 Byte it is now 32 Byte) some values moved at the end of Zone 9.
	- ferry and train event values 

## Rev 10 Update 5

- now change `onJob` and `jobFinished` with the present of `jobDelivered` and `jobChancelled`. Before it is changed after leaving the experience screen or sometimes when enter drive mode.
  - Job values are also reseted directly with the event
- remove variable `i` from `scs_config_handlers.cpp` and `scs_gameplay_event_handlers.cpp`
- now `isCargoLoaded` won't be reseted twice by calling `set_job_values_zero`, instead the `plannedDistanceKm` is reseted correctly
- **value type changed** timestamp is know a `ulong`  and not a `uint` anymore (sdk value is `unsigned long long`)
- added 2 new values: `simulation timestamp` and `render timestamp` both are `ulong` values similar to the `timestamp` -> changes in Shared Memory
- the `update` or `timestamp` bool of the Data Event is now for some special cases also set to true:
  - (normal case) `timestamp` change
  - one of the critical events values change (`jobDelivered` , `jobCancelled`, `Train`, `Ferry` and `onJob`). Why? they could happen if the `timestamp` did not change
  - if both cases are false we check if the `pause` state change. Why? similar to above the `timestamp` did not change sometimes

- **Shared Memory Changes**:
  - > Zone 1 is modified  -> `time` change to `unsigned long long`, `simulatedTime` and `renderTime` added, moved `paused` behind `sdkActive` 

## Rev 10 Update 4 (with small fix)

- Update `Readme.md`
- Change `update.md` to `changelog.md`
- Add `refuel end` event and change `refuel` to `refuel start`
- Add `refuel payed` event. Instead to fire when you stop refuel like `refuel end` it will fired when you start the engine. Also this is the event for the later coming `refuel amount` value.
- Add `refuel amount`. After the `refuel payed` event this value contains the amount of fuel that was refueled between the `refuel start` and `refuel payed` event.
  - This value could be a bit different to the in-game value, but most of the time it should be really similar. **NOTE:** if you pay in game, but didn't start the engine and start again to refuel the value in-game is the whole refuel amount and this value will only be the new refuel amount
  - I tried to calculate the exact value. But there could be a really small difference to that value (maybe up to `+-0.1`, maybe in some special cases more. If you think it is completely wrong create an issue)
- Add two fields to the GamplayEvents `JobDelivered` and `JobCancelled`: `Started` and `Finished`. The values in `JobDelivered` and `JobCancelled` will be the same. That means for best results you should get the data when the event is fired
  - **NOTE:** `JobCancelled`: If the game is closed after or the profile is changed the start value will be **wrong**, because the Starting Time is set through the appearance of the `job` values of the sdk. On the start of a profile the value will be overwritten, because the `job` values are "new". Same thing happens when the sdk is restarted. If you want a "save" value, you may need to make a backup yourself. The value `Started` is set in the moment a new job is started and not bounded to the event itself. I will add some note to that in the documentation of course
  - **Note:** `JobDelivered`: Similar problem to `JobCancelled`, but it has an backup property. The backup property calculate the starting time through the `finished` and `DeliveringTime` value. The values should be equals. You can choose which values you want to use.
  - Additional: If you do not start the simulation (start driving) the timestamp won't updated to the game time of the current profile and can lead to wrong starting times
- removed `memset` at shutdown call to avoid waiting time (leeds to shutdown time of ets2/ats of multiple minutes)  
- **fix update** through the remove of `memset` the `sdkaktive` field was not set to false. Fixed that (**the DLL of this release does not contains this fix**)
  - some additional values will be set to 0:
    - SDK Version
    - Game
    - Game Version
    - Telemetry Version
    - TimeStamp
    - Common:
      - Scale
      - GameTime
       
**Note to that** may in a future release all values will be reset again, but i need to find a way that want create that high closing time for some users.

- **Shared Memory Changes**:
  - > Zone 4 end is modified (floats) -> `gameplas_f.refuelAmount` added, so `job_f.cargoDamage` moved 4 bytes
  - > Zone 2 end is modified (uints) -> `jobStartingTime` and `jobFinishedTime` added, nothing moved

- **Known Bugs**
  - if you don't own a truck and starts the 2nd job you will receive a "Refuel payed" event. (issue #55)

## Rev 10 Update 3 (Game Version 1.36, small Shared Memory changes)

update to SDK Version 1.11

- Added PlannedDistanceKm value (Unsigned Integer Value)
- **Note to the shared Memory:** zone 2 is modified (Unsigned Integer, Config Values). -> One new value was added (plannedDistanceKm). The following unsigned integer values has so a new offset. Other Zones shouldn't change.

## Rev 10 Update 2

- Add refuel event (based on last fuel value and current fuel value).

## Rev 10 Update 1.1 (fixes)

- fix code ... documentation says `Local\SCSTelemetry` code says still the old location. Now the **shared memory files** is **`Local\SCSTelemetry`** like the `readme.md` tells.
- update sdk from 1.9 to 1.10 for the other compiler settings in the solution file (thanks to kniffen #47)
- fix for external contracts (thanks danielalmeida1481 #48)
- also remove redundant event `jobFinished`, because it is fired together with `jobDelivered` or `jobCancelled`. The value in c++ and shared memory is still there, but no functionality for other projects for the moment.

## Rev 10 Update 1

- change `DefaultUpdateInterval` from 25ms to 100ms of the Shared Memory File refresh timer to avoid crashed until i (or someone) have a better solution (see issue #38)
- added value `SdkActive` that could create problems if not update both parts of the project. It's only a byte/bool but on the first place in the shared memory -> the update function won't work when not updated both without small fix (offset + 1, 2 or 3 at the begin)
- c# library will going in `paused` state when `SdkActive == false`. Means that the update rate of the shared memory is now 1 second and not higher, get back to high update rate when sdk is active again
- values are now reset when the game is closed correct (see #39). Crashes or kills of the game won't reset the values
- c# demo now has a status label that shows the current update rate of the sdk

## Rev 10 (Game Version 1.35)

A lot changes here: update to SDK Version 1.10

- shared memory files size increased (16kb to 32kb), because it can handle up to 10 trailers... that needs already a lot of space
- structure of the shared memory file changed a lot through new order and new attributes
- implement gameplay events from the api
- added versions support (game sdk version) eg. for ets2 it is actually 1.14, but 1.13 is still working. With comming updates 1.13 will may deleted, because it could leed to errors or to many code. So should work for 1.27 up to 1.35+ for now. Possible also lower versions. 
- mostly added the new cool stuff of the sdk for now, more (e.g. linux support, logging, ...) will come in a later update
- find and fixes a few bugs will creating new content (hopefully no new bugs)
- notice: job canceled ~and tollgate~(tollgate works, but only if you pay and not your customer) seems not to work atm  
- demo was updated but it's not working good with the 10 trailer updated. Avoid to open tje trailer page or you need to restart the demo. It will freeze the ui. (To high update rate of that mutch text, when game is paused ui will also stop freezing) 
- new events to register (tollgate, train, ferry, fine, job delivered,... ) and some removed (trailer connected, disconnected) but you could create your own connected/disconnected listener if you need them again
- a lot of other small thinks. i will start early to write update notes the next time (hopefuly)
- some documentation for new functions, values etc.
