using System;
using System.Threading;
using SCSSdkClient.Object;

//TODO: possible idea: check if ets is running and if not change update rate to infinity (why most of the user may not quit the application while ets is running)
namespace SCSSdkClient {
    /// <summary>
    /// Called when telemetry data arrives
    /// </summary>
    /// <param name="data">Telemetry data</param>
    /// <param name="newTimestamp">Flag if there is a new timestamp</param>
    public delegate void TelemetryData(ScsTelemetry data, bool newTimestamp);

    /// <summary>
    ///     Handle the SCSSdkTelemetry.
    ///     Currently IDisposable. Was implemented because of an error
    /// </summary>
    public class ScsSdkTelemetry : IDisposable {
        private const string _defaultSharedMemoryMap = "Local\\SCSTelemetry";
        private const int _defaultUpdateInterval = 100;
        private const int _defaultPausedUpdateInterval = 1000;

        private int updateInterval;

        // todo: enhancement:  some way to set this value 
        private const int _pausedUpdateInterval = _defaultPausedUpdateInterval;

        private Timer updateTimer;

        private ulong lastTime = 0xFFFFFFFFFFFFFFFF;


#if LOGGING
        public void Dispose() {
            _updateTimer?.Dispose();
            Log.SaveShutdown();
        }
#else
        /// <inheritdoc />
        public void Dispose() => updateTimer?.Dispose();

#endif


        private SharedMemory sharedMemory;

        private bool wasOnJob;
        //? private bool wasConnected; what was the use here?
        private bool cancelled;
        private bool delivered;
        private bool fined;
        private bool tollgate;
        private bool ferry;
        private bool train;
        private bool paused;
        private bool refuel;
        private bool refuelPayed;
        private bool wasPaused;

        /// <summary>
        ///     Set up SCS telemetry provider.
        ///     Connects to shared memory map, sets up timebase.
        /// </summary>
        public ScsSdkTelemetry() => Setup(_defaultSharedMemoryMap, _defaultUpdateInterval);

        /// <summary>
        ///     Set up SCS telemetry provider.
        ///     Connects to shared memory map, sets up timebase.
        /// </summary>
        /// <param name="map">Memory Map location</param>
        public ScsSdkTelemetry(string map) => Setup(map, _defaultUpdateInterval);

        /// <summary>
        ///     Set up SCS telemetry provider.
        ///     Connects to shared memory map, sets up timebase.
        /// </summary>
        /// <param name="interval">Timebase interval</param>
        public ScsSdkTelemetry(int interval) => Setup(_defaultSharedMemoryMap, interval);

        /// <summary>
        ///     Set up SCS telemetry provider.
        ///     Connects to shared memory map, sets up timebase.
        /// </summary>
        /// <param name="map">Memory Map location</param>
        /// <param name="interval">Timebase interval</param>
        public ScsSdkTelemetry(string map, int interval) => Setup(map, interval);

        /// <summary>
        /// Path of the Memory Mapped File
        /// </summary>
        public string Map { get; private set; }
        
        /// <summary>
        /// Current update interval 
        /// </summary>
        public int UpdateInterval => paused ? _pausedUpdateInterval : updateInterval;
        
        /// <summary>
        /// Error of the telemetry receiver, if any 
        /// </summary>
        public Exception Error { get; private set; }

        /// <summary>
        /// Raw data of the telemetry
        /// </summary>
        public event TelemetryData Data;

        /// <summary>
        /// Event when job has been started
        /// </summary>
        public event EventHandler JobStarted;
        /// <summary>
        /// Event when job has been cancelled
        /// </summary>
        public event EventHandler JobCancelled;
        /// <summary>
        /// Event when job has been delivered
        /// </summary>
        public event EventHandler JobDelivered;
        /// <summary>
        /// Event when player was fined
        /// </summary>
        public event EventHandler Fined;
        /// <summary>
        /// Event when player paid in the tollgate
        /// </summary>
        public event EventHandler Tollgate;
        /// <summary>
        /// Event when player travelled by ferry
        /// </summary>
        public event EventHandler Ferry;
        /// <summary>
        /// Event when player travelled by train
        /// </summary>
        public event EventHandler Train;
        /// <summary>
        /// Event when player started to refuel
        /// </summary>
        public event EventHandler RefuelStart;
        /// <summary>
        /// Event when player finished refueling
        /// </summary>
        public event EventHandler RefuelEnd;
        /// <summary>
        /// Event when player paid for the fuel
        /// </summary>
        public event EventHandler RefuelPaid;

        /// <summary>
        /// 'Stops' the update until the game is unpaused to save resources
        /// </summary>
        public void Pause() => updateTimer.Change(Timeout.Infinite, Timeout.Infinite);

        /// <summary>
        /// Lowers the delay between updates
        /// </summary>
        public void Resume() {
            var tsInterval = new TimeSpan(0, 0, 0, 0, UpdateInterval);
            updateTimer.Change(tsInterval, tsInterval);
        }
        
        private void Setup(string map, int interval) {
#if LOGGING
            AppDomain.CurrentDomain.UnhandledException += CurrentDomain_UnhandledException;
            Log.Write("Start of the Telemetry");
            Log.Write("Set up everything and start the updatetimer");
#endif

            Map = map;
            updateInterval = interval;

            sharedMemory = new SharedMemory();
            sharedMemory.Connect(map);

            if (!sharedMemory.Hooked) {
                Error = sharedMemory.HookException;
                return;
            }

            var tsInterval = new TimeSpan(0, 0, 0, 0, interval);

            updateTimer = new Timer(_updateTimer_Elapsed, null, tsInterval.Add(tsInterval), tsInterval);
#if LOGGING
            Log.Write("Every thing is set up correctly and the timer was started");
#endif
        }

        private void _updateTimer_Elapsed(object sender) {
            var scsTelemetry = sharedMemory.UpdateData();
            // check if sdk is NOT running
            if (!scsTelemetry.SdkActive && !paused) {
                // if so don't check so often the data 
                var tsInterval = new TimeSpan(0, 0, 0, 0, _defaultPausedUpdateInterval);
                updateTimer.Change(tsInterval.Add(tsInterval), tsInterval);
                paused = true;
                // if sdk not active we don't need to do something
                return;
            }

            if (paused && scsTelemetry.SdkActive) {
                // ok sdk is active now
                paused = false;
                Resume(); // going back to normal update rate
            }

            var time = scsTelemetry.Timestamp;
            var updated = false;
            if (time != lastTime || wasPaused != scsTelemetry.Paused) {
                // time changed or game state change -> update data
                Data?.Invoke(scsTelemetry, true);
                wasPaused = scsTelemetry.Paused;
                lastTime = time;
                updated = true;
            }

            //TODO: make it nicer thats a lot of code for such less work
            // Job start event
            if (wasOnJob != scsTelemetry.SpecialEventsValues.OnJob) {
                wasOnJob = scsTelemetry.SpecialEventsValues.OnJob;
                if (wasOnJob) {
                    if (!updated) {
                        Data?.Invoke(scsTelemetry, true);
                        updated = true;
                    }

                    JobStarted?.Invoke(this, new EventArgs());
                }
            }

            if (cancelled != scsTelemetry.SpecialEventsValues.JobCancelled) {
                cancelled = scsTelemetry.SpecialEventsValues.JobCancelled;
                if (cancelled) {
                    if (!updated) {
                        Data?.Invoke(scsTelemetry, true);
                        updated = true;
                    }

                    JobCancelled?.Invoke(this, new EventArgs());
                }
            }

            if (delivered != scsTelemetry.SpecialEventsValues.JobDelivered) {
                delivered = scsTelemetry.SpecialEventsValues.JobDelivered;
                if (delivered) {
                    if (!updated) {
                        Data?.Invoke(scsTelemetry, true);
                        updated = true;
                    }

                    JobDelivered?.Invoke(this, new EventArgs());
                }
            }

            if (fined != scsTelemetry.SpecialEventsValues.Fined) {
                fined = scsTelemetry.SpecialEventsValues.Fined;
                if (fined) {
                    Fined?.Invoke(this, new EventArgs());
                }
            }

            if (tollgate != scsTelemetry.SpecialEventsValues.Tollgate) {
                tollgate = scsTelemetry.SpecialEventsValues.Tollgate;
                if (tollgate) {
                    Tollgate?.Invoke(this, new EventArgs());
                }
            }

            if (ferry != scsTelemetry.SpecialEventsValues.Ferry) {
                ferry = scsTelemetry.SpecialEventsValues.Ferry;
                if (ferry) {
                    if (!updated) {
                        Data?.Invoke(scsTelemetry, true);
                        updated = true;
                    }

                    Ferry?.Invoke(this, new EventArgs());
                }
            }

            if (train != scsTelemetry.SpecialEventsValues.Train) {
                train = scsTelemetry.SpecialEventsValues.Train;
                if (train) {
                    if (!updated) {
                        Data?.Invoke(scsTelemetry, true);
                        updated = true;
                    }

                    Train?.Invoke(this, new EventArgs());
                }
            }

            if (refuel != scsTelemetry.SpecialEventsValues.Refuel) {
                refuel = scsTelemetry.SpecialEventsValues.Refuel;
                if (scsTelemetry.SpecialEventsValues.Refuel) {
                    RefuelStart?.Invoke(this, new EventArgs());
                } else {
                    RefuelEnd?.Invoke(this, new EventArgs());
                }
            }

            if (refuelPayed != scsTelemetry.SpecialEventsValues.RefuelPaid) {
                refuelPayed = scsTelemetry.SpecialEventsValues.RefuelPaid;
                if (scsTelemetry.SpecialEventsValues.RefuelPaid) {
                    RefuelPaid?.Invoke(this, new EventArgs());
                }
            }

            // currently the design is that the event is called, doesn't matter if data changed
            // also the old demo didn't used the flag and expected to be refreshed each call
            // so without making a big change also call the event without update with false flag
            if (!updated) {
                Data?.Invoke(scsTelemetry, false);
            }
        }
#if LOGGING
        private static void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e) {
            try {
                var ex = (Exception) e.ExceptionObject;
                Console.WriteLine("Fatal unhandled Exception... pls create an issue and add the log file on github");
                Log.Write(ex.ToString());
                Log.SaveShutdown();
            } catch (Exception ex) {
                try {
                    Console.WriteLine("Can't write log / or close it after an unhandled Exception: "+ex);
                } finally {
                    Environment.Exit(142);
                }
            }
        }
#endif
    }
}