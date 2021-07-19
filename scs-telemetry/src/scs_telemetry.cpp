//TODO: cleanup file 900 lines are to many and there is much to do in an other file or class


#ifdef WIN32
//Windows stuff.
#define WINVER 0x0500
#define WIN32_WINNT 0x0500

#include <windows.h>
#endif

#include <cassert>
#include <cstdarg>
#include <algorithm>
#include <string>
#include <cstring>
// SDK
#include "scssdk_telemetry.h"
#include "eurotrucks2/scssdk_eut2.h"
#include "eurotrucks2/scssdk_telemetry_eut2.h"
#include "amtrucks/scssdk_ats.h"
#include "amtrucks/scssdk_telemetry_ats.h"

// Plug-in
#include "scs-telemetry-common.hpp"
#include "scs_config_handlers.hpp"
#include "scs_gameplay_event_handlers.hpp"
#include <log.hpp>

#define UNUSED(x)
/**
 * These macro's are a shortcut to register channels inside the scs_telemetry_init function
 * They require the channel definition name (without prefix SCS_TELEMETRY_), type and destination.
 * Not all channel types are implemented; the handler function for a type should be created like so:
 * telemetry_store_[Type](const scs_string_t name, const scs_u32_t index, const scs_value_t *const value, const scs_context_t context)
 *
 * RegisterSpecificChannel allows for your own handler name, without the telemetry_store_ prefix.
 */
#define REGISTER_CHANNEL(name, type, to) version_params->register_for_channel(SCS_TELEMETRY_##name, SCS_U32_NIL, SCS_VALUE_TYPE_##type, SCS_TELEMETRY_CHANNEL_FLAG_no_value, telemetry_store_##type, &( to ))
#define REGISTER_CHANNEL_TRAILER(id, name, type, to) version_params->register_for_channel((std::string("trailer.")+std::to_string(id)+std::string("."#name)).c_str(), SCS_U32_NIL, SCS_VALUE_TYPE_##type, SCS_TELEMETRY_CHANNEL_FLAG_no_value, telemetry_store_##type, &( to ))
#define REGISTER_CHANNEL_INDEX(name, type, to, index) version_params->register_for_channel(SCS_TELEMETRY_##name, index, SCS_VALUE_TYPE_##type, SCS_TELEMETRY_CHANNEL_FLAG_no_value, telemetry_store_##type, &( to ))
#define REGISTER_CHANNEL_TRAILER_INDEX(id, name, type, to, index) version_params->register_for_channel((std::string("trailer.")+std::to_string(id)+std::string("."#name)).c_str(), index, SCS_VALUE_TYPE_##type, SCS_TELEMETRY_CHANNEL_FLAG_no_value, telemetry_store_##type, &( to ))
#define REGISTER_SPECIFIC_CHANNEL(name, type, handler,to) version_params->register_for_channel(SCS_TELEMETRY_##name, SCS_U32_NIL, SCS_VALUE_TYPE_##type, SCS_TELEMETRY_CHANNEL_FLAG_no_value, handler, &( to ))

scsTelemetryMap_t* telemetryPtr;

//// const: scs_mmf_name
//// Name/Location of the Shared Memory
//const wchar_t* scs_mmf_name = SCS_PLUGIN_MMF_NAME;

// ptr: game_log
// Used to write to the game log
scs_log_t game_log = nullptr;

// About: Game log
//
// - Use function log_line(const scs_log_type_t type, const char*const text,...) to write to the in game console log with chosen message type
// - or use log_line(const char*const text, ...) to write to the in game console log with error type (more for debugging purpose)

// use for values
// char buff[100];
// snprintf(buff, sizeof(buff), "%f", value->value_dplacement.position.x);	 
// log_line(SCS_LOG_TYPE_warning, buff);

// Function: log_line
// Used to write to the in game console log
void log_line(const scs_log_type_t type, const char* const text, ...) {
    if (!game_log) {
        return;
    }
    char formatted[1000];

    va_list args;
    va_start(args, text);
#ifdef WIN32
    vsnprintf_s(formatted, sizeof formatted, _TRUNCATE, text, args);
#else
    vsnprintf(formatted, sizeof formatted, text, args);
#endif
    formatted[sizeof formatted - 1] = 0;
    va_end(args);

    game_log(type, formatted);
}

// Function: log_line
// Used to write to the in game console log as error (debugging)
void log_line(const char* const text, ...) {
    if (!game_log) {
        return;
    }
    char formatted[1000];

    va_list args;
    va_start(args, text);
#ifdef WIN32
    vsnprintf_s(formatted, sizeof formatted, _TRUNCATE, text, args);
#else
    vsnprintf(formatted, sizeof formatted, text, args);
#endif
    formatted[sizeof formatted - 1] = 0;
    va_end(args);

    game_log(SCS_LOG_TYPE_error, formatted);
}

// check if the version is correct
bool check_min_version(unsigned const int min_ets2, unsigned const int min_ats) {

    return telemetryPtr->scs_values.game == ETS2 && telemetryPtr->scs_values.version_minor >= min_ets2 || telemetryPtr
                                                                                                    ->scs_values.game ==
                                                                                                          ATS && telemetryPtr->scs_values.version_minor >= min_ats;

}

bool check_max_version(unsigned const int max_ets2, unsigned const int max_ats) {

    return telemetryPtr->scs_values.game == ETS2 && telemetryPtr->scs_values.version_minor <= max_ets2 || telemetryPtr
                                                                                                    ->scs_values.game ==
                                                                                                          ATS && telemetryPtr->scs_values.version_minor <= max_ats;

}

// Function: log_configs
// It print every config event that appears to the in game log
// careful, create a lot of logs so that fast parts are not readable anymore in the log window
void log_configs(const scs_telemetry_configuration_t* info) {
    log_line("Configuration: %s", info->id);
    for (auto current = info->attributes; current->name; ++current) {

        if (current->index != SCS_U32_NIL) {
            // log_line("[%u]", static_cast<unsigned>(current->index));
        }
        switch (current->value.type) {
        case SCS_VALUE_TYPE_INVALID: {
            log_line(" %s none", current->name);
            break;
        }
        case SCS_VALUE_TYPE_bool: {
            log_line(" %s bool = %s", current->name, current->value.value_bool.value ? "true" : "false");
            break;
        }
        case SCS_VALUE_TYPE_s32: {
            log_line(" %s s32 = %d", current->name, static_cast<int>(current->value.value_s32.value));
            break;
        }
        case SCS_VALUE_TYPE_u32: {
            log_line(" %s u32 = %u", current->name, static_cast<unsigned>(current->value.value_u32.value));
            break;
        }
        case SCS_VALUE_TYPE_u64: {
            log_line(" %s u64 = %" SCS_PF_U64, current->name, current->value.value_u64.value);
            break;
        }
        case SCS_VALUE_TYPE_float: {
            log_line(" %s float = %f", current->name, current->value.value_float.value);
            break;
        }
        case SCS_VALUE_TYPE_double: {
            log_line(" %s double = %f", current->name, current->value.value_double.value);
            break;
        }
        case SCS_VALUE_TYPE_fvector: {
            log_line(
                " %s fvector = (%f,%f,%f)", current->name,
                current->value.value_fvector.x,
                current->value.value_fvector.y,
                current->value.value_fvector.z
            );
            break;
        }
        case SCS_VALUE_TYPE_dvector: {
            log_line(
                " %s dVector = (%f,%f,%f)", current->name,
                current->value.value_dvector.x,
                current->value.value_dvector.y,
                current->value.value_dvector.z
            );
            break;
        }
        case SCS_VALUE_TYPE_euler: {
            log_line(
                " %s euler = h:%f p:%f r:%f", current->name,
                current->value.value_euler.heading * 360.0f,
                current->value.value_euler.pitch * 360.0f,
                current->value.value_euler.roll * 360.0f
            );
            break;
        }
        case SCS_VALUE_TYPE_fplacement: {
            log_line(
                " %s fplacement = (%f,%f,%f) h:%f p:%f r:%f", current->name,
                current->value.value_fplacement.position.x,
                current->value.value_fplacement.position.y,
                current->value.value_fplacement.position.z,
                current->value.value_fplacement.orientation.heading * 360.0f,
                current->value.value_fplacement.orientation.pitch * 360.0f,
                current->value.value_fplacement.orientation.roll * 360.0f
            );
            break;
        }
        case SCS_VALUE_TYPE_dplacement: {
            log_line(
                " %s dplacement = (%f,%f,%f) h:%f p:%f r:%f", current->name,
                current->value.value_dplacement.position.x,
                current->value.value_dplacement.position.y,
                current->value.value_dplacement.position.z,
                current->value.value_dplacement.orientation.heading * 360.0f,
                current->value.value_dplacement.orientation.pitch * 360.0f,
                current->value.value_dplacement.orientation.roll * 360.0f
            );
            break;
        }
        case SCS_VALUE_TYPE_string: {
            log_line(" %s string = %s", current->name, current->value.value_string.value);
            break;
        }
        default: {
            log_line(" %s unknown", current->name);
            break;
        }

        }

    }
}

// Function: log_events
// It print every gameplay event that appears to the in game log
// careful, create a lot of logs so that fast parts are not readable anymore in the log window
void log_events(const scs_telemetry_gameplay_event_t* info) {
    log_line("^GameplayEvents: %s", info->id);
    for (auto current = info->attributes; current->name; ++current) {

        if (current->index != SCS_U32_NIL) {
            // log_line("[%u]", static_cast<unsigned>(current->index));
        }
        switch (current->value.type) {
        case SCS_VALUE_TYPE_INVALID: {
            log_line(" %s none", current->name);
            break;
        }
        case SCS_VALUE_TYPE_bool: {
            log_line(" %s bool = %s", current->name, current->value.value_bool.value ? "true" : "false");
            break;
        }
        case SCS_VALUE_TYPE_s32: {
            log_line(" %s s32 = %d", current->name, static_cast<int>(current->value.value_s32.value));
            break;
        }
        case SCS_VALUE_TYPE_u32: {
            log_line(" %s u32 = %u", current->name, static_cast<unsigned>(current->value.value_u32.value));
            break;
        }
        case SCS_VALUE_TYPE_u64: {
            log_line(" %s u64 = %" SCS_PF_U64, current->name, current->value.value_u64.value);
            break;
        }
        case SCS_VALUE_TYPE_float: {
            log_line(" %s float = %f", current->name, current->value.value_float.value);
            break;
        }
        case SCS_VALUE_TYPE_double: {
            log_line(" %s double = %f", current->name, current->value.value_double.value);
            break;
        }
        case SCS_VALUE_TYPE_fvector: {
            log_line(
                " %s fvector = (%f,%f,%f)", current->name,
                current->value.value_fvector.x,
                current->value.value_fvector.y,
                current->value.value_fvector.z
            );
            break;
        }
        case SCS_VALUE_TYPE_dvector: {
            log_line(
                " %s dVector = (%f,%f,%f)", current->name,
                current->value.value_dvector.x,
                current->value.value_dvector.y,
                current->value.value_dvector.z
            );
            break;
        }
        case SCS_VALUE_TYPE_euler: {
            log_line(
                " %s euler = h:%f p:%f r:%f", current->name,
                current->value.value_euler.heading * 360.0f,
                current->value.value_euler.pitch * 360.0f,
                current->value.value_euler.roll * 360.0f
            );
            break;
        }
        case SCS_VALUE_TYPE_fplacement: {
            log_line(
                " %s fplacement = (%f,%f,%f) h:%f p:%f r:%f", current->name,
                current->value.value_fplacement.position.x,
                current->value.value_fplacement.position.y,
                current->value.value_fplacement.position.z,
                current->value.value_fplacement.orientation.heading * 360.0f,
                current->value.value_fplacement.orientation.pitch * 360.0f,
                current->value.value_fplacement.orientation.roll * 360.0f
            );
            break;
        }
        case SCS_VALUE_TYPE_dplacement: {
            log_line(
                " %s dplacement = (%f,%f,%f) h:%f p:%f r:%f", current->name,
                current->value.value_dplacement.position.x,
                current->value.value_dplacement.position.y,
                current->value.value_dplacement.position.z,
                current->value.value_dplacement.orientation.heading * 360.0f,
                current->value.value_dplacement.orientation.pitch * 360.0f,
                current->value.value_dplacement.orientation.roll * 360.0f
            );
            break;
        }
        case SCS_VALUE_TYPE_string: {
            log_line(" %s string = %s", current->name, current->value.value_string.value);
            break;
        }
        default: {
            log_line(" %s unknown", current->name);
            break;
        }

        }

    }
}

/**
 * @brief Last timestamp we received.
 */
scs_timestamp_t lastTimestamp = static_cast<scs_timestamp_t>(-1);
scs_timestamp_t lastSimulatedTimestamp = static_cast<scs_timestamp_t>(-1);
scs_timestamp_t lastRenderTimestamp = static_cast<scs_timestamp_t>(-1);

scs_timestamp_t timestamp;
scs_timestamp_t simulatedTimestamp;
scs_timestamp_t renderTimestamp;

static auto clearJobTicker = 0;
static auto clearCancelledTicker = 0;
static auto clearDeliveredTicker = 0;
static auto clearFinedTicker = 0;
static auto clearTollgateTicker = 0;
static auto clearFerryTicker = 0;
static auto clearTrainTicker = 0;
static auto clearRefuelPayedTicker = 0;


//TODO: REWORK BOTH CLEAN FUNCTION AND ADD MORE FOR SINGLE CONFIG attribute
// Function: set_job_values_zero
// set every job (cargo) values to 0/empty string
void set_job_values_zero() {
    telemetryPtr->config_ull.jobIncome = 0;
    telemetryPtr->config_ui.time_abs_delivery = 0;
    telemetryPtr->config_f.cargoMass = 0;
    telemetryPtr->config_ui.unitCount = 0;
    telemetryPtr->config_f.unitMass = 0;
    telemetryPtr->job_f.cargoDamage = 0;
    telemetryPtr->config_b.isCargoLoaded = false;
    telemetryPtr->config_ui.plannedDistanceKm = 0;
    memset(telemetryPtr->config_s.compDstId, 0, STRING_SIZE);
    memset(telemetryPtr->config_s.compSrcId, 0, STRING_SIZE);
    memset(telemetryPtr->config_s.cityDstId, 0, STRING_SIZE);
    memset(telemetryPtr->config_s.citySrcId, 0, STRING_SIZE);
    memset(telemetryPtr->config_s.citySrc, 0, STRING_SIZE);
    memset(telemetryPtr->config_s.cityDst, 0, STRING_SIZE);
    memset(telemetryPtr->config_s.compSrc, 0, STRING_SIZE);
    memset(telemetryPtr->config_s.compDst, 0, STRING_SIZE);
    memset(telemetryPtr->config_s.cargoId, 0, STRING_SIZE);
    memset(telemetryPtr->config_s.cargo, 0, STRING_SIZE);
    memset(telemetryPtr->config_s.jobMarket, 0, 32);
}

// Function: set_trailer_values_zero
// set every trailer value 0/empty string 
void set_trailer_values_zero(unsigned int trailer_id = 0) {
    //(&telemetryPtr->trailer.trailer[trailer_id])->~scsTrailer_t();
    new(&telemetryPtr->trailer.trailer[trailer_id]) scsTrailer_t();
}


// Last Fuel Value (set to a high value to avoid to trigger the event directly on start)
static auto fuel_ticker = 0;
static auto fuel_ticker2 = 0;
static auto last_fuel_value = 0.0f;
static auto current_fuel_value = 0.0f;
static auto refuel = false;
static auto fuel_tmp = 0.0f;
static auto start_fuel = 0.0f;

// Function: telemetry_frame_start
// Register telemetry values
SCSAPI_VOID telemetry_frame_start(const scs_event_t UNUSED(event), const void* const event_info,
                                  scs_context_t UNUSED(context)) {

    const auto info = static_cast<const scs_telemetry_frame_start_t *>(event_info);
    // The following processing of the timestamps is done so the output
    // from this plugin has continuous time, it is not necessary otherwise.

    // When we just initialized itself, assume that the time started
    // just now.

    if (lastTimestamp == static_cast<scs_timestamp_t>(-1)) {
        lastTimestamp = info->paused_simulation_time;
    }
    if (lastSimulatedTimestamp == static_cast<scs_timestamp_t>(-1)) {
        lastSimulatedTimestamp = info->simulation_time;
    }
    if (lastRenderTimestamp == static_cast<scs_timestamp_t>(-1)) {
        lastRenderTimestamp = info->render_time;
    }

    // The timer might be sometimes restarted (e.g. after load) while
    // we want to provide continuous time on our output.

    if (info->flags & SCS_TELEMETRY_FRAME_START_FLAG_timer_restart) {
        lastTimestamp = 0;
        lastSimulatedTimestamp = 0;
        lastRenderTimestamp = 0;
    }

    // Advance the timestamp by delta since last frame.

    timestamp += info->paused_simulation_time - lastTimestamp;
    simulatedTimestamp += info->simulation_time - lastSimulatedTimestamp;
    renderTimestamp += info->render_time - lastRenderTimestamp;
    lastTimestamp = info->paused_simulation_time;
    lastSimulatedTimestamp = info->simulation_time;
    lastRenderTimestamp = info->render_time;

    /* Copy over the game timestamp to our telemetry memory */
    if (telemetryPtr != nullptr) {
        telemetryPtr->time = static_cast<unsigned long long>(timestamp);
        telemetryPtr->simulatedTime = static_cast<unsigned long long>(simulatedTimestamp);
        telemetryPtr->renderTime = static_cast<unsigned long long>(renderTimestamp);

        // Do a non-conventional periodic update of this field:
        telemetryPtr->truck_b.cruiseControl = telemetryPtr->truck_f.cruiseControlSpeed > 0;

        // check fuel value
        current_fuel_value = telemetryPtr->truck_f.fuel;
 
        if(!refuel) {
            start_fuel = fuel_tmp;
            fuel_tmp = telemetryPtr->truck_f.fuel;
        }
        if (current_fuel_value > last_fuel_value && last_fuel_value > 0) {
            fuel_ticker2 = 0;
            telemetryPtr->special_b.refuel = true;
            if(!refuel) {                
                refuel = true;
                clearRefuelPayedTicker = 0;
                
            }
        }else if (current_fuel_value < last_fuel_value) {
            fuel_ticker2 = 0;
            telemetryPtr->special_b.refuel = false;
        }

        // refuel is true, but engine is now active? than refuel is finished and payed, fire event   
        if(refuel && telemetryPtr->truck_b.engineEnabled) {
            refuel = false;

            telemetryPtr->gameplay_f.refuelAmount = telemetryPtr->truck_f.fuel - start_fuel;
            telemetryPtr->special_b.refuelPayed = true;
            
        }

        // update last value every few ticks (refuel rate is not constant and the plugin side did check every 25 ms so to try a
        // constant refuel event for the whole time a few strange things :D atm
        if (fuel_ticker > 10) {
            fuel_ticker = 0;

            if (current_fuel_value == last_fuel_value) {
                fuel_ticker2++;
            }
            else {
                fuel_ticker2 = 0;
            }
            if (fuel_ticker2 >= 5) {
                fuel_ticker2 = 0;
                telemetryPtr->special_b.refuel = false;
            }
           
        }
        fuel_ticker++;
        last_fuel_value = current_fuel_value;
       
         
        //TODO: better way for that mess here
        if (telemetryPtr->special_b.jobFinished) {
            clearJobTicker++;

            if (telemetryPtr->special_b.jobCancelled) {
                clearCancelledTicker++;

                if (clearCancelledTicker > 10) {
                    set_job_values_zero();
                    telemetryPtr->special_b.jobCancelled = false;
                    telemetryPtr->special_b.jobFinished = false;
                }
            }
            else if (telemetryPtr->special_b.jobDelivered) {
                clearDeliveredTicker++;

                if (clearDeliveredTicker > 10) {
                    set_job_values_zero();
                    telemetryPtr->special_b.jobDelivered = false;
                    telemetryPtr->special_b.jobFinished = false;
                }
            }
            else {
                // job is cancelled -> user input
                // job is delivered -> user
                // this case ? seems to be called the first time the user leaves the profile
                // else there should no case (i hope and think atm so)
                 if (clearJobTicker > 10) {
                    set_job_values_zero();
                     telemetryPtr->special_b.jobFinished = false;
                }
            }
        }
        if (telemetryPtr->special_b.refuelPayed) {
            clearRefuelPayedTicker++;

            if (clearRefuelPayedTicker > 10) {
                telemetryPtr->special_b.refuelPayed = false;
            }
        }
        if (telemetryPtr->special_b.fined) {
            clearFinedTicker++;

            if (clearFinedTicker > 10) {
                telemetryPtr->special_b.fined = false;
            }
        }
        if (telemetryPtr->special_b.tollgate) {
            clearTollgateTicker++;

            if (clearTollgateTicker > 10) {
                telemetryPtr->special_b.tollgate = false;
            }
        }
        if (telemetryPtr->special_b.ferry) {
            clearFerryTicker++;

            if (clearFerryTicker > 10) {
                telemetryPtr->special_b.ferry = false;
            }
        }
        if (telemetryPtr->special_b.train) {
            clearTrainTicker++;

            if (clearTrainTicker > 10) {
                telemetryPtr->special_b.train = false;
            }
        }
    }

}

// Function: telemetry_pause
// called if the game fires the event start/pause. Used to set the paused value
SCSAPI_VOID telemetry_pause(const scs_event_t event, const void* const UNUSED(event_info),
                            scs_context_t UNUSED(context)) {
#if LOGGING
	logger::flush();
#endif
    if (telemetryPtr != nullptr) {
        telemetryPtr->paused = event == SCS_TELEMETRY_EVENT_paused;
    }
}


SCSAPI_VOID telemetry_gameplay(const scs_event_t UNUSED(event), const void* const event_info, scs_context_t UNUSED(context)) {
    //  An event called when a gameplay event such as job finish happens
    const auto info = static_cast<const scs_telemetry_gameplay_event_t *>(
        event_info);

    // uncomment to log every config, should work but with function not tested ^^`
    //log_events(info);
    // check which type the event has
    gameplayType type = {};
    if (strcmp(info->id, SCS_TELEMETRY_GAMEPLAY_EVENT_job_cancelled) == 0) {
        type = cancelled;
        telemetryPtr->special_b.jobCancelled = true;
        telemetryPtr->gameplay_ui.jobFinishedTime = telemetryPtr->common_ui.time_abs;
        clearCancelledTicker = 0;
        telemetryPtr->special_b.onJob = false;
        telemetryPtr->special_b.jobFinished = true;
        clearJobTicker = 0;
    }
    else if (strcmp(info->id, SCS_TELEMETRY_GAMEPLAY_EVENT_job_delivered) == 0) {
        type = delivered;
        telemetryPtr->special_b.jobDelivered = true;
        telemetryPtr->gameplay_ui.jobFinishedTime = telemetryPtr->common_ui.time_abs;
        clearDeliveredTicker = 0;
        telemetryPtr->special_b.onJob = false;
        telemetryPtr->special_b.jobFinished = true;
        clearJobTicker = 0;

    }
    else if (strcmp(info->id, SCS_TELEMETRY_GAMEPLAY_EVENT_player_fined) == 0) {
        type = fined;
        telemetryPtr->special_b.fined = true;
        clearFinedTicker = 0;
    }
    else if (strcmp(info->id, SCS_TELEMETRY_GAMEPLAY_EVENT_player_tollgate_paid) == 0) {
        type = tollgate;
        telemetryPtr->special_b.tollgate = true;
        clearTollgateTicker = 0;
    }
    else if (strcmp(info->id, SCS_TELEMETRY_GAMEPLAY_EVENT_player_use_ferry) == 0) {
        type = ferry;
        telemetryPtr->special_b.ferry = true;
        clearFerryTicker = 0;
    }
    else if (strcmp(info->id, SCS_TELEMETRY_GAMEPLAY_EVENT_player_use_train) == 0) {
        type = train;
        telemetryPtr->special_b.train = true;
        clearTrainTicker = 0;
    }
    else {
        log_line(SCS_LOG_TYPE_warning, "Something went wrong with this gameplay event %s", info->id);
    }


    // attribute is a pointer array that is never null so ... i have no clue how to check it on another way than this
    // if for loop can't loop it is empty so simple

    for (auto current = info->attributes; current->name; ++current) {

        if (!handleGpe(current, type)) {
            // actually only for testing/debug purpose, so should there be a message in game with that line there is missed something
            log_line("attribute not handled id: %i attribute: %s", type, current->name);
        }
    }


}


// Function: telemetry_configuration
// called if the game fires the event configuration. Used to handle all the configuration values
SCSAPI_VOID telemetry_configuration(const scs_event_t UNUSED(event), const void* const event_info,
                                    scs_context_t UNUSED(context)) {
    // On configuration change, this function is called.
    const auto info = static_cast<const scs_telemetry_configuration_t *>(
        // TODO: DELETE ENTRIES WHEN CALLED SO NO VALUE IS THERE to avoid wrong values when changes occur but not in arrays up to that slot or so 
        event_info);
    unsigned int trailer_id = 0;
    // check which type the event has
    configType type = {};
    if (strcmp(info->id, SCS_TELEMETRY_CONFIG_substances) == 0) {
        type = substances;
    }
    else if (strcmp(info->id, SCS_TELEMETRY_CONFIG_controls) == 0) {
        type = controls;
    }
    else if (strcmp(info->id, SCS_TELEMETRY_CONFIG_hshifter) == 0) {
        type = hshifter;
    }
    else if (strcmp(info->id, SCS_TELEMETRY_CONFIG_truck) == 0) {
        type = truck;
    }
    else if (strcmp(info->id, SCS_TELEMETRY_CONFIG_job) == 0) {
        type = job;
    }
    else {
        // check if it is trailer with backwards compatibility
        if (check_max_version(13, 0)) {
            if (strcmp(info->id, SCS_TELEMETRY_CONFIG_trailer) == 0) {
                type = trailer;
                trailer_id = 0;

            }
            else {
                log_line(SCS_LOG_TYPE_warning, "Something went wrong with this configuration %s", info->id);
            }
            set_trailer_values_zero();
        }
        else {
            if (strstr(info->id, SCS_TELEMETRY_CONFIG_trailer)) {
                if (strcmp(info->id, SCS_TELEMETRY_CONFIG_trailer) == 0) {
                    return;
                }
                type = trailer;
                auto last = info->id[strlen(info->id) - 1];
                trailer_id = last - '0';
                if (trailer_id > 9 || trailer_id < 0) {
                    log_line(SCS_LOG_TYPE_warning, "Something went wrong while parsing trailer id", info->id);
                }
                else {
                    set_trailer_values_zero(trailer_id);
                }
            }
            else {
                log_line(SCS_LOG_TYPE_warning, "Something went wrong with this configuration %s", info->id);
            }
        }

    }

    // uncomment to log every config, should work but with function not tested ^^`
    // log_configs(info); 

    // attribute is a pointer array that is never null so ... i have no clue how to check it on another way than this
    // if for loop can't loop it is empty so simple 
    auto is_empty = true;

    for (auto current = info->attributes; current->name; ++current) {
        if (!handleCfg(current, type, trailer_id)) {
            // actually only for testing/debug purpose, so should there be a message in game with that line there is missed something
            log_line("attribute not handled id: %i attribute: %s", type, current->name);
        }
        is_empty = false;
    }
    // if id of config is "job" but without element and we are on a job -> we finished it now
    if (type == job && is_empty && telemetryPtr->special_b.onJob) {
        telemetryPtr->special_b.onJob = false;
        telemetryPtr->special_b.jobFinished = true;
        clearJobTicker = 0;
    }
    else if (!telemetryPtr->special_b.onJob && type == job && !is_empty) {
        // oh hey no job but now we have fields in this array so we start a new job
        telemetryPtr->special_b.onJob = true;
        telemetryPtr->gameplay_ui.jobStartingTime = telemetryPtr->common_ui.time_abs;
    }

}

/******* STORING OF SEVERAL SCS DATA TYPES *******/
SCSAPI_VOID telemetry_store_float(const scs_string_t UNUSED(name), const scs_u32_t UNUSED(index), const scs_value_t* const value,
                                  scs_context_t context) {
    if (!value) return;
    assert(value);
    assert(value->type == SCS_VALUE_TYPE_float);
    assert(context);
    *static_cast<float *>(context) = value->value_float.value;
}

SCSAPI_VOID telemetry_store_s32(const scs_string_t UNUSED(name), const scs_u32_t UNUSED(index), const scs_value_t* const value,
                                scs_context_t context) {
    if (!value) return;
    assert(value);
    assert(value->type == SCS_VALUE_TYPE_s32);
    assert(context);
    *static_cast<int *>(context) = value->value_s32.value;
}

SCSAPI_VOID telemetry_store_u32(const scs_string_t UNUSED(name), const scs_u32_t UNUSED(index), const scs_value_t* const value,
                                scs_context_t context) {
    if (!value) return;
    assert(value);
    assert(value->type == SCS_VALUE_TYPE_u32);
    assert(context);
    *static_cast<unsigned int *>(context) = value->value_u32.value;
}

SCSAPI_VOID telemetry_store_bool(const scs_string_t UNUSED(name), const scs_u32_t UNUSED(index), const scs_value_t* const value,
                                 scs_context_t context) {
    if (!context) return;
    if (value) {
        if (value->value_bool.value == 0) {
            *static_cast<bool *>(context) = false;
        }
        else {
            *static_cast<bool *>(context) = true;
        }
    }
    else {
        *static_cast<bool *>(context) = false;
    }
}

SCSAPI_VOID telemetry_store_fvector(const scs_string_t UNUSED(name), const scs_u32_t UNUSED(index), const scs_value_t* const value,
                                    scs_context_t context) {
    if (!value) return;
    assert(value);
    assert(value->type == SCS_VALUE_TYPE_fvector);
    assert(context);
    *(static_cast<float *>(context) + 0) = value->value_fvector.x;
    *(static_cast<float *>(context) + 1) = value->value_fvector.y;
    *(static_cast<float *>(context) + 2) = value->value_fvector.z;
}

SCSAPI_VOID telemetry_store_dplacement(const scs_string_t UNUSED(name), const scs_u32_t UNUSED(index), const scs_value_t* const value,
                                       scs_context_t context) {
    if (!value) return;
    assert(value);
    assert(value->type == SCS_VALUE_TYPE_dplacement);
    assert(context);

    // Messy hack to store the acceleration and orientation values into our telemetry struct
    // It is necessary that these are put together, otherwise it may overwrite over values.
    *(static_cast<double *>(context) + 0) = value->value_dplacement.position.x;
    *(static_cast<double *>(context) + 1) = value->value_dplacement.position.y;
    *(static_cast<double *>(context) + 2) = value->value_dplacement.position.z;

    *(static_cast<double *>(context) + 3) = value->value_dplacement.orientation.heading;
    *(static_cast<double *>(context) + 4) = value->value_dplacement.orientation.pitch;
    *(static_cast<double *>(context) + 5) = value->value_dplacement.orientation.roll;

}

SCSAPI_VOID telemetry_store_fplacement(const scs_string_t UNUSED(name), const scs_u32_t UNUSED(index), const scs_value_t* const value,
                                       scs_context_t context) {
    if (!value) return;
    assert(value);
    assert(value->type == SCS_VALUE_TYPE_fplacement);
    assert(context);

    // Messy hack to store the acceleration and orientation values into our telemetry struct
    // It is necessary that these are put together, otherwise it may overwrite over values.
    *(static_cast<float *>(context) + 0) = value->value_fplacement.position.x;
    *(static_cast<float *>(context) + 1) = value->value_fplacement.position.y;
    *(static_cast<float *>(context) + 2) = value->value_fplacement.position.z;

    *(static_cast<float *>(context) + 3) = value->value_fplacement.orientation.heading;
    *(static_cast<float *>(context) + 4) = value->value_fplacement.orientation.pitch;
    *(static_cast<float *>(context) + 5) = value->value_fplacement.orientation.roll;
}


/**
 * @brief Telemetry API initialization function.
 *
 * See scssdk_telemetry.h
 */

SCSAPI_RESULT scs_telemetry_init(const scs_u32_t version, const scs_telemetry_init_params_t* const params) {

    // We currently support only two version.
    //TODO test this first test seems to work 
    const scs_telemetry_init_params_v100_t* version_params;
    if (version == SCS_TELEMETRY_VERSION_1_00) {
        version_params = (const scs_telemetry_init_params_v100_t*)params;
    }
    else if (version == SCS_TELEMETRY_VERSION_1_01) {
        version_params = (const scs_telemetry_init_params_v101_t*)params;
    }
    else {
        return SCS_RESULT_unsupported;
    }


    game_log = version_params->common.log;
#if LOGGING
	log_line("LOGGING is active find at %s", logger::path.c_str());
	logger::out << "start logging" << '\n';
#endif
//
//    /*** ACQUIRE SHARED MEMORY BUFFER ***/
//    telem_mem = new SharedMemory(scs_mmf_name, SCS_PLUGIN_MMF_SIZE);
//
//    if (telem_mem == nullptr) {
//        return SCS_RESULT_generic_error;
//    }
//
//    if (!telem_mem->Hooked()) {
//        return SCS_RESULT_generic_error;
//    }

    // TODO initialize TCP server

//    telemetryPtr = static_cast<scsTelemetryMap_t*>(telem_mem->GetBuffer());
//
//    if (telemetryPtr == nullptr) {
//        return SCS_RESULT_generic_error;
//    }

    // set sdk active bit to true
    telemetryPtr->sdkActive = true;
    /*** INITIALIZE TELEMETRY MAP TO DEFAULT ***/
    telemetryPtr->paused = true;
    telemetryPtr->time = 0;
    // Get SCS Game Version and Set Plugin Version
    telemetryPtr->scs_values.telemetry_plugin_revision = PLUGIN_REVID;
    telemetryPtr->scs_values.version_major = SCS_GET_MAJOR_VERSION(version_params->common.game_version);
    telemetryPtr->scs_values.version_minor = SCS_GET_MINOR_VERSION(version_params->common.game_version);

    // Set Game ID 
    if (strcmp(version_params->common.game_id, SCS_GAME_ID_EUT2) == 0) {
        telemetryPtr->scs_values.game = ETS2;
        telemetryPtr->scs_values.telemetry_version_game_major = SCS_GET_MAJOR_VERSION(
            SCS_TELEMETRY_EUT2_GAME_VERSION_CURRENT);
        telemetryPtr->scs_values.telemetry_version_game_minor = SCS_GET_MINOR_VERSION(
            SCS_TELEMETRY_EUT2_GAME_VERSION_CURRENT);
    }
    else if (strcmp(version_params->common.game_id, SCS_GAME_ID_ATS) == 0) {
        telemetryPtr->scs_values.game = ATS;
        telemetryPtr->scs_values.telemetry_version_game_major = SCS_GET_MAJOR_VERSION(
            SCS_TELEMETRY_ATS_GAME_VERSION_CURRENT);
        telemetryPtr->scs_values.telemetry_version_game_minor = SCS_GET_MINOR_VERSION(
            SCS_TELEMETRY_ATS_GAME_VERSION_CURRENT);
    }
    else {
        // unknown game

        log_line(SCS_LOG_TYPE_error, "Unknown Game SDK will not work correctly");
        telemetryPtr->scs_values.game = UnknownGame;
        telemetryPtr->scs_values.telemetry_version_game_major = 0;
        telemetryPtr->scs_values.telemetry_version_game_minor = 0;
    }

    // Model & trailer type are stored in configuration event.


    /*** REGISTER GAME EVENTS (Pause/Unpause/Start/Time) ***/
    const auto events_registered =
        version_params->register_for_event(SCS_TELEMETRY_EVENT_frame_start, telemetry_frame_start, nullptr) ==
        SCS_RESULT_ok &&
        version_params->register_for_event(SCS_TELEMETRY_EVENT_paused, telemetry_pause, nullptr) == SCS_RESULT_ok &&
        version_params->register_for_event(SCS_TELEMETRY_EVENT_started, telemetry_pause, nullptr) == SCS_RESULT_ok;

    // Register configuration event, because it sends data like truck make, etc.
    version_params->register_for_event(SCS_TELEMETRY_EVENT_configuration, telemetry_configuration, nullptr);


    if (check_min_version(14, 1)) {

        // Register gameplay event, for event such as job finish or canceled
        version_params->register_for_event(SCS_TELEMETRY_EVENT_gameplay, telemetry_gameplay, nullptr);
        // this seems to be a constant so fetch it here? need to test this
        telemetryPtr->config_ui.maxTrailerCount = SCS_TELEMETRY_trailers_count;
    }

    if (!events_registered) {
        return SCS_RESULT_generic_error;
    }

    // ETS Version 1.0 - 1.13 (up to patch 1.34) and ATS Version 1.0 (up to patch 1.34) or simple SDK version 1.0
    /*** REGISTER ALL TELEMETRY CHANNELS TO OUR SHARED MEMORY MAP ***/
    REGISTER_CHANNEL(CHANNEL_game_time, u32, telemetryPtr->common_ui.time_abs);


    REGISTER_CHANNEL(TRUCK_CHANNEL_speed, float, telemetryPtr->truck_f.speed);
    REGISTER_CHANNEL(TRUCK_CHANNEL_local_linear_acceleration, fvector, telemetryPtr->truck_fv.accelerationX);
    REGISTER_CHANNEL(TRUCK_CHANNEL_local_linear_velocity, fvector, telemetryPtr->truck_fv.lv_accelerationX);
    REGISTER_CHANNEL(TRUCK_CHANNEL_local_angular_acceleration, fvector, telemetryPtr->truck_fv.aa_accelerationX);
    REGISTER_CHANNEL(TRUCK_CHANNEL_local_angular_velocity, fvector, telemetryPtr->truck_fv.av_accelerationX);

    REGISTER_CHANNEL(TRUCK_CHANNEL_world_placement, dplacement, telemetryPtr->truck_dp.coordinateX);

    REGISTER_CHANNEL(TRUCK_CHANNEL_engine_gear, s32, telemetryPtr->truck_i.gear);
    REGISTER_CHANNEL(TRUCK_CHANNEL_displayed_gear, s32, telemetryPtr->truck_i.gearDashboard);

    REGISTER_CHANNEL(TRUCK_CHANNEL_engine_rpm, float, telemetryPtr->truck_f.engineRpm);

    REGISTER_CHANNEL(TRUCK_CHANNEL_fuel, float, telemetryPtr->truck_f.fuel);
    REGISTER_CHANNEL(TRUCK_CHANNEL_fuel_average_consumption, float, telemetryPtr->truck_f.fuelAvgConsumption);

    REGISTER_CHANNEL(TRUCK_CHANNEL_input_steering, float, telemetryPtr->truck_f.userSteer);
    REGISTER_CHANNEL(TRUCK_CHANNEL_input_throttle, float, telemetryPtr->truck_f.userThrottle);
    REGISTER_CHANNEL(TRUCK_CHANNEL_input_brake, float, telemetryPtr->truck_f.userBrake);
    REGISTER_CHANNEL(TRUCK_CHANNEL_input_clutch, float, telemetryPtr->truck_f.userClutch);

    REGISTER_CHANNEL(TRUCK_CHANNEL_effective_steering, float, telemetryPtr->truck_f.gameSteer);
    REGISTER_CHANNEL(TRUCK_CHANNEL_effective_throttle, float, telemetryPtr->truck_f.gameThrottle);
    REGISTER_CHANNEL(TRUCK_CHANNEL_effective_brake, float, telemetryPtr->truck_f.gameBrake);
    REGISTER_CHANNEL(TRUCK_CHANNEL_effective_clutch, float, telemetryPtr->truck_f.gameClutch);

    // Auxiliary stuff:
    REGISTER_CHANNEL(TRUCK_CHANNEL_retarder_level, u32, telemetryPtr->truck_ui.retarderBrake);
    REGISTER_CHANNEL(TRUCK_CHANNEL_hshifter_slot, u32, telemetryPtr->truck_ui.shifterSlot);
    for (auto i = scs_u32_t(0); i < scs_u32_t(2); i++) {
        REGISTER_CHANNEL_INDEX(TRUCK_CHANNEL_hshifter_selector, bool, telemetryPtr->truck_b.shifterToggle[i], i);
    }
    // Booleans
    REGISTER_CHANNEL(TRUCK_CHANNEL_wipers, bool, telemetryPtr->truck_b.wipers);
    REGISTER_CHANNEL(TRUCK_CHANNEL_parking_brake, bool, telemetryPtr->truck_b.parkBrake);
    REGISTER_CHANNEL(TRUCK_CHANNEL_motor_brake, bool, telemetryPtr->truck_b.motorBrake);
    REGISTER_CHANNEL(TRUCK_CHANNEL_electric_enabled, bool, telemetryPtr->truck_b.electricEnabled);
    REGISTER_CHANNEL(TRUCK_CHANNEL_engine_enabled, bool, telemetryPtr->truck_b.engineEnabled);
    REGISTER_CHANNEL(TRUCK_CHANNEL_lblinker, bool, telemetryPtr->truck_b.blinkerLeftActive);
    REGISTER_CHANNEL(TRUCK_CHANNEL_rblinker, bool, telemetryPtr->truck_b.blinkerRightActive);
    REGISTER_CHANNEL(TRUCK_CHANNEL_light_lblinker, bool, telemetryPtr->truck_b.blinkerLeftOn);
    REGISTER_CHANNEL(TRUCK_CHANNEL_light_rblinker, bool, telemetryPtr->truck_b.blinkerRightOn);
    REGISTER_CHANNEL(TRUCK_CHANNEL_light_parking, bool, telemetryPtr->truck_b.lightsParking);
    REGISTER_CHANNEL(TRUCK_CHANNEL_light_low_beam, bool, telemetryPtr->truck_b.lightsBeamLow);
    REGISTER_CHANNEL(TRUCK_CHANNEL_light_high_beam, bool, telemetryPtr->truck_b.lightsBeamHigh);
    REGISTER_CHANNEL(TRUCK_CHANNEL_light_aux_front, u32, telemetryPtr->truck_ui.lightsAuxFront);
    REGISTER_CHANNEL(TRUCK_CHANNEL_light_aux_roof, u32, telemetryPtr->truck_ui.lightsAuxRoof);
    REGISTER_CHANNEL(TRUCK_CHANNEL_light_beacon, bool, telemetryPtr->truck_b.lightsBeacon);
    REGISTER_CHANNEL(TRUCK_CHANNEL_light_brake, bool, telemetryPtr->truck_b.lightsBrake);
    REGISTER_CHANNEL(TRUCK_CHANNEL_light_reverse, bool, telemetryPtr->truck_b.lightsReverse);
    REGISTER_CHANNEL(TRUCK_CHANNEL_battery_voltage_warning, bool, telemetryPtr->truck_b.batteryVoltageWarning);
    REGISTER_CHANNEL(TRUCK_CHANNEL_brake_air_pressure_warning, bool, telemetryPtr->truck_b.airPressureWarning);
    REGISTER_CHANNEL(TRUCK_CHANNEL_brake_air_pressure_emergency, bool, telemetryPtr->truck_b.airPressureEmergency);
    REGISTER_CHANNEL(TRUCK_CHANNEL_adblue_warning, bool, telemetryPtr->truck_b.adBlueWarning);
    REGISTER_CHANNEL(TRUCK_CHANNEL_oil_pressure_warning, bool, telemetryPtr->truck_b.oilPressureWarning);
    REGISTER_CHANNEL(TRUCK_CHANNEL_water_temperature_warning, bool, telemetryPtr->truck_b.waterTemperatureWarning);

    // Floats
    REGISTER_CHANNEL(TRUCK_CHANNEL_brake_air_pressure, float, telemetryPtr->truck_f.airPressure);
    REGISTER_CHANNEL(TRUCK_CHANNEL_brake_temperature, float, telemetryPtr->truck_f.brakeTemperature);
    REGISTER_CHANNEL(TRUCK_CHANNEL_fuel_warning, bool, telemetryPtr->truck_b.fuelWarning);
    REGISTER_CHANNEL(TRUCK_CHANNEL_adblue, float, telemetryPtr->truck_f.adBlue);

    //registerChannel(TRUCK_CHANNEL_adblue_average_consumption, float, telemetryPtr->tel_rev3.adblueConsumption); // seems not to work in ets2/ATS at 19/10 skd 1.9 and actual game versions
    REGISTER_CHANNEL(TRUCK_CHANNEL_oil_pressure, float, telemetryPtr->truck_f.oilPressure);
    REGISTER_CHANNEL(TRUCK_CHANNEL_oil_temperature, float, telemetryPtr->truck_f.oilTemperature);
    REGISTER_CHANNEL(TRUCK_CHANNEL_water_temperature, float, telemetryPtr->truck_f.waterTemperature);
    REGISTER_CHANNEL(TRUCK_CHANNEL_battery_voltage, float, telemetryPtr->truck_f.batteryVoltage);
    REGISTER_CHANNEL(TRUCK_CHANNEL_dashboard_backlight, float, telemetryPtr->truck_f.lightsDashboard);
    REGISTER_CHANNEL(TRUCK_CHANNEL_wear_engine, float, telemetryPtr->truck_f.wearEngine);
    REGISTER_CHANNEL(TRUCK_CHANNEL_wear_transmission, float, telemetryPtr->truck_f.wearTransmission);
    REGISTER_CHANNEL(TRUCK_CHANNEL_wear_cabin, float, telemetryPtr->truck_f.wearCabin);
    REGISTER_CHANNEL(TRUCK_CHANNEL_wear_chassis, float, telemetryPtr->truck_f.wearChassis);
    REGISTER_CHANNEL(TRUCK_CHANNEL_wear_wheels, float, telemetryPtr->truck_f.wearWheels);
    REGISTER_CHANNEL(TRUCK_CHANNEL_odometer, float, telemetryPtr->truck_f.truckOdometer);
    REGISTER_CHANNEL(TRUCK_CHANNEL_cruise_control, float, telemetryPtr->truck_f.cruiseControlSpeed);
    REGISTER_CHANNEL(TRUCK_CHANNEL_navigation_speed_limit, float, telemetryPtr->truck_f.speedLimit);
    REGISTER_CHANNEL(TRUCK_CHANNEL_navigation_distance, float, telemetryPtr->truck_f.routeDistance);
    REGISTER_CHANNEL(TRUCK_CHANNEL_navigation_time, float, telemetryPtr->truck_f.routeTime);
    REGISTER_CHANNEL(TRUCK_CHANNEL_fuel_range, float, telemetryPtr->truck_f.fuelRange);


    for (auto i = scs_u32_t(0); i < WHEEL_SIZE; i++) {
        REGISTER_CHANNEL_INDEX(TRUCK_CHANNEL_wheel_on_ground, bool, telemetryPtr->truck_b.truck_wheelOnGround[i], i);
        REGISTER_CHANNEL_INDEX(TRUCK_CHANNEL_wheel_substance, u32, telemetryPtr->truck_ui.truck_wheelSubstance[i], i);
        REGISTER_CHANNEL_INDEX(TRUCK_CHANNEL_wheel_velocity, float, telemetryPtr->truck_f.truckWheelVelocity[i], i);
        REGISTER_CHANNEL_INDEX(TRUCK_CHANNEL_wheel_steering, float, telemetryPtr->truck_f.truckWheelSteering[i], i);
        REGISTER_CHANNEL_INDEX(TRUCK_CHANNEL_wheel_rotation, float, telemetryPtr->truck_f.truckWheelRotation[i], i);
        REGISTER_CHANNEL_INDEX(TRUCK_CHANNEL_wheel_susp_deflection, float, telemetryPtr->truck_f.
                               truckWheelSuspensionDeflection[i], i);
        REGISTER_CHANNEL_INDEX(TRUCK_CHANNEL_wheel_lift, float, telemetryPtr->truck_f.truckWheelLift[i], i);
        REGISTER_CHANNEL_INDEX(TRUCK_CHANNEL_wheel_lift_offset, float, telemetryPtr->truck_f.truckWheelLiftOffset[i], i
        );
    }

    REGISTER_CHANNEL(TRUCK_CHANNEL_cabin_offset, fplacement, telemetryPtr->truck_fp.cabinOffsetX);
    REGISTER_CHANNEL(TRUCK_CHANNEL_cabin_angular_velocity, fvector, telemetryPtr->truck_fv.cabinAVX);
    REGISTER_CHANNEL(TRUCK_CHANNEL_cabin_angular_acceleration, fvector, telemetryPtr->truck_fv.cabinAAX);
    REGISTER_CHANNEL(CHANNEL_next_rest_stop, s32, telemetryPtr->common_i.restStop);
    REGISTER_CHANNEL(CHANNEL_local_scale, float, telemetryPtr->common_f.scale);
    REGISTER_CHANNEL(TRUCK_CHANNEL_head_offset, fplacement, telemetryPtr->truck_fp.headOffsetX);


    // backwards code for trailer
    if (check_max_version(13, 0)) {


        REGISTER_CHANNEL(TRAILER_CHANNEL_connected, bool, telemetryPtr->trailer.trailer[0].com_b.attached);


        REGISTER_CHANNEL(TRAILER_CHANNEL_world_placement, dplacement, telemetryPtr->trailer.trailer[0].com_dp.worldX);

        REGISTER_CHANNEL(TRAILER_CHANNEL_local_linear_velocity, fvector,
                         telemetryPtr->trailer.trailer[0].com_fv.linearVelocityX);
        REGISTER_CHANNEL(TRAILER_CHANNEL_local_angular_velocity, fvector,
                         telemetryPtr->trailer.trailer[0].com_fv.angularVelocityX);
        REGISTER_CHANNEL(TRAILER_CHANNEL_local_linear_acceleration, fvector,
                         telemetryPtr->trailer.trailer[0].com_fv.linearAccelerationX
        );
        REGISTER_CHANNEL(TRAILER_CHANNEL_local_angular_acceleration, fvector,
                         telemetryPtr->trailer.trailer[0].com_fv.angularAccelerationX
        );

        REGISTER_CHANNEL(TRAILER_CHANNEL_wear_chassis, float, telemetryPtr->trailer.trailer[0].com_f.wearChassis);
        for (auto i = scs_u32_t(0); i < WHEEL_SIZE; i++) {
            REGISTER_CHANNEL_INDEX(TRAILER_CHANNEL_wheel_susp_deflection, float,
                                   telemetryPtr->trailer.trailer[0].com_f.wheelSuspensionDeflection[i], i);
            REGISTER_CHANNEL_INDEX(TRAILER_CHANNEL_wheel_on_ground, bool,
                                   telemetryPtr->trailer.trailer[0].com_b.wheelOnGround[i], i
            );
            REGISTER_CHANNEL_INDEX(TRAILER_CHANNEL_wheel_substance, u32,
                                   telemetryPtr->trailer.trailer[0].com_ui.wheelSubstance[i], i
            );
            REGISTER_CHANNEL_INDEX(TRAILER_CHANNEL_wheel_velocity, float,
                                   telemetryPtr->trailer.trailer[0].com_f.wheelVelocity[i], i
            );
            REGISTER_CHANNEL_INDEX(TRAILER_CHANNEL_wheel_steering, float,
                                   telemetryPtr->trailer.trailer[0].com_f.wheelSteering[i], i
            );
            REGISTER_CHANNEL_INDEX(TRAILER_CHANNEL_wheel_rotation, float,
                                   telemetryPtr->trailer.trailer[0].com_f.wheelRotation[i], i
            );

        }
    }
    // new in 1.35 so ets2 1.14 and ats 1.01
    if (check_min_version(14, 1)) {
        // could be loaded don't know actually in the sdk not loaded in ets2 but in ats so may only need to add this and than it should work but need to test this for both
        REGISTER_CHANNEL(JOB_CHANNEL_cargo_damage, float, telemetryPtr->job_f.cargoDamage);

        for (auto i = 0; i < 10; i++) {
            REGISTER_CHANNEL_TRAILER(i, connected, bool, telemetryPtr->trailer.trailer[i].com_b.attached);
            REGISTER_CHANNEL_TRAILER(i, cargo.damage, float, telemetryPtr->trailer.trailer[i].com_f.cargoDamage);


            REGISTER_CHANNEL_TRAILER(i, world.placement, dplacement, telemetryPtr->trailer.trailer[i].com_dp.worldX);

            REGISTER_CHANNEL_TRAILER(i, velocity.linear, fvector, telemetryPtr->trailer.trailer[i].com_fv.linearVelocityX);
            REGISTER_CHANNEL_TRAILER(i, velocity.angular, fvector,
                                     telemetryPtr->trailer.trailer[i].com_fv.angularVelocityX);
            REGISTER_CHANNEL_TRAILER(i, acceleration.linear, fvector,
                                     telemetryPtr->trailer.trailer[i].com_fv.linearAccelerationX
            );
            REGISTER_CHANNEL_TRAILER(i, acceleration.angular, fvector,
                                     telemetryPtr->trailer.trailer[i].com_fv.angularAccelerationX
            );

            REGISTER_CHANNEL_TRAILER(i, wear.chassis, float, telemetryPtr->trailer.trailer[i].com_f.wearChassis);
            REGISTER_CHANNEL_TRAILER(i, wear.wheels, float, telemetryPtr->trailer.trailer[i].com_f.wearWheels);
            for (auto j = scs_u32_t(0); j < WHEEL_SIZE; j++) {
                REGISTER_CHANNEL_TRAILER_INDEX(i, wheel.suspension.deflection, float,
                                               telemetryPtr->trailer.trailer[i].com_f.wheelSuspensionDeflection[j], j);
                REGISTER_CHANNEL_TRAILER_INDEX(i, wheel.on_ground, bool,
                                               telemetryPtr->trailer.trailer[i].com_b.wheelOnGround[j], j
                );
                REGISTER_CHANNEL_TRAILER_INDEX(i, wheel.substance, u32,
                                               telemetryPtr->trailer.trailer[i].com_ui.wheelSubstance[j], j
                );
                REGISTER_CHANNEL_TRAILER_INDEX(i, wheel.angular_velocity, float,
                                               telemetryPtr->trailer.trailer[i].com_f.wheelVelocity[j], j
                );
                REGISTER_CHANNEL_TRAILER_INDEX(i, wheel.steering, float,
                                               telemetryPtr->trailer.trailer[i].com_f.wheelSteering[j], j
                );
                REGISTER_CHANNEL_TRAILER_INDEX(i, wheel.rotation, float,
                                               telemetryPtr->trailer.trailer[i].com_f.wheelRotation[j], j
                );
                REGISTER_CHANNEL_TRAILER_INDEX(i, wheel.lift, float, telemetryPtr->trailer.trailer[0].com_f.wheelLift[j],
                                               j);
                REGISTER_CHANNEL_TRAILER_INDEX(i, wheel.lift.offset, float,
                                               telemetryPtr->trailer.trailer[0].com_f.wheelLiftOffset[j], j);

            }
        }

    }

    // new in 1.41, ets2 1.17 and ats 1.04
    if (check_min_version(17, 4)) {
        REGISTER_CHANNEL(TRUCK_CHANNEL_hazard_warning, bool, telemetryPtr->truck_b.lightsHazard);
        REGISTER_CHANNEL(TRUCK_CHANNEL_differential_lock, bool, telemetryPtr->truck_b.differentialLock);
        REGISTER_CHANNEL(TRUCK_CHANNEL_lift_axle, bool, telemetryPtr->truck_b.liftAxle);
        REGISTER_CHANNEL(TRUCK_CHANNEL_lift_axle_indicator, bool, telemetryPtr->truck_b.liftAxleIndicator);
        REGISTER_CHANNEL(TRUCK_CHANNEL_trailer_lift_axle, bool, telemetryPtr->truck_b.trailerLiftAxle);
        REGISTER_CHANNEL(TRUCK_CHANNEL_trailer_lift_axle_indicator, bool, telemetryPtr->truck_b.trailerLiftAxleIndicator);

    }

    // Set the structure with defaults.

    timestamp = static_cast<scs_timestamp_t>(0);
    lastTimestamp = static_cast<scs_timestamp_t>(-1);

    return SCS_RESULT_ok;
}


/**
 * @brief Telemetry API de-initialization function.
 *
 * See scssdk_telemetry.h
 */
SCSAPI_VOID scs_telemetry_shutdown() {
#if LOGGING
	logger::flush();
#endif
    // Close MemoryMap
    telemetryPtr->sdkActive = false;
    telemetryPtr->scs_values.game = 0;
    telemetryPtr->scs_values.telemetry_plugin_revision = 0;
    telemetryPtr->scs_values.telemetry_version_game_major = 0;
    telemetryPtr->scs_values.telemetry_version_game_minor = 0;
    telemetryPtr->scs_values.version_major = 0;
    telemetryPtr->scs_values.version_minor = 0;

    telemetryPtr->time = 0;
    telemetryPtr->simulatedTime = 0;
    telemetryPtr->renderTime = 0;
    telemetryPtr->common_ui.time_abs = 0;
    telemetryPtr->common_f.scale = 0;

//    if (telem_mem != nullptr) {
//        telem_mem->Close();
//    }

// TODO close TCP server
}

// Telemetry api.

#ifdef WIN32
// ReSharper disable once CppInconsistentNaming
BOOL APIENTRY DllMain(
    HMODULE module,
    DWORD reason_for_call,
    LPVOID reseved
) {
    if (reason_for_call == DLL_PROCESS_DETACH) {

        scs_telemetry_shutdown();

    }
    return TRUE;
}
#endif