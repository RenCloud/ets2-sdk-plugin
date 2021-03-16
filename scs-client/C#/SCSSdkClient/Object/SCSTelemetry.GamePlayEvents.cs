#pragma warning disable 1570

namespace SCSSdkClient.Object {
    public partial class ScsTelemetry {
        /// <summary>
        ///     Gameplay Events that can be fired from the sdk (starting for game version 1.35+)
        ///     first implementation is that the values will be set, but never deleted
        ///     another change listener for bool values that will be changed will say if there are new values
        /// </summary>
        /// <note>
        ///     Refuel event is currently not provided by the sdk. It is provided through the fuel value.
        /// </note>
        public class GamePlayEvents
        {
            /// <summary>
            /// Property holding information about ferry transport
            /// </summary>
            public Transport FerryEvent { get; } = new Transport();
            /// <summary>
            /// Property holding information about player getting fined
            /// </summary>
            public Fined FinedEvent { get; } = new Fined();
            /// <summary>
            /// Property holding information about job cancellation
            /// </summary>
            public Cancelled JobCancelled { get; } = new Cancelled();
            /// <summary>
            /// Property holding information about job delivery
            /// </summary>
            public Delivered JobDelivered { get; } = new Delivered();
            /// <summary>
            /// Property holding information about passing a tollgate
            /// </summary>
            public Tollgate TollgateEvent { get; } = new Tollgate();
            /// <summary>
            /// Property holding information about train transport
            /// </summary>
            public Transport TrainEvent { get; } = new Transport();
            /// <summary>
            /// Property holding information about player refueling
            /// </summary>
            public Refuel RefuelEvent { get; } = new Refuel();

            
            /// <summary>
            /// Contains information about job cancellation
            /// </summary>
            public class Cancelled {
                /// <summary>
                /// Financial penalty
                /// </summary>
                public long Penalty{ get; internal set; }
                /// <summary>
                /// Time when job was cancelled
                /// </summary>
                public Time Finished{ get; internal set; }
                /// <summary>
                /// Time when job was started
                /// </summary>
                public Time Started{ get; internal set; }
            }

            /// <summary>
            /// Contains information about job delivery
            /// </summary>
            public class Delivered {
                /// <summary>
                /// Flag if job was auto loaded
                /// </summary>
                public bool AutoLoaded{ get; internal set; }
                /// <summary>
                /// Flag if job was auto parked
                /// </summary>
                public bool AutoParked{ get; internal set; }
                /// <summary>
                /// Amount of damage on the cargo
                /// </summary>
                public float CargoDamage{ get; internal set; }  // Typo fixed thanks to Patrick-van-Halm https://github.com/RenCloud/scs-sdk-plugin/pull/32
                /// <summary>
                /// Time when job was delivered
                /// </summary>
                public Time DeliveryTime{ get; internal set; }  // Theoretically more a `Frequency`, because it is a timespan and not a moment, but atm i won't change it to frequency also because it is a UINT from SDK and only positive.
                /// <summary>
                /// Distance on the job
                /// </summary>
                public float DistanceKm{ get; internal set; }
                /// <summary>
                /// XP earned on job delivery
                /// </summary>
                public int EarnedXp{ get; internal set; }
                /// <summary>
                /// Money earned on job delivery
                /// </summary>
                public long Revenue{ get; internal set; }
                /// <summary>
                /// Time when job was finished
                /// </summary>
                public Time Finished{ get; internal set; }
                /// <summary>
                /// Time when job was started
                /// </summary>
                public Time Started{ get; internal set; }
                /// <summary>
                /// Alternate value for <see cref="Started"/>
                /// </summary>
                public Time StartedBackup => Finished - DeliveryTime;
            }

            /// <summary>
            /// Contains information about player being fined
            /// </summary>
            public class Fined {
                /// <summary>
                /// Amount of money fined
                /// </summary>
                public long Amount{ get; internal set; }
                /// <summary>
                /// Type of offence
                /// </summary>
                public Offence Offence{ get; internal set; }
            }

            /// <summary>
            /// Contains information about tollgate
            /// </summary>
            public class Tollgate {
                /// <summary>
                /// How much money player paid on the tollgate
                /// </summary>
                public long PayAmount{ get; internal set; }
            }

            /// <summary>
            /// Contains information about player being transported
            /// </summary>
            public class Transport {
                /// <summary>
                /// How much money the player paid to get transported
                /// </summary>
                public long PayAmount{ get; internal set; }
                /// <summary>
                /// ID of the departure place
                /// </summary>
                public string SourceId{ get; internal set; }
                /// <summary>
                /// Localized name of the departure place
                /// </summary>
                public string SourceName{ get; internal set; }
                /// <summary>
                /// ID of the arrival place
                /// </summary>
                public string TargetId{ get; internal set; }
                /// <summary>
                /// Localized name of the departure place
                /// </summary>
                public string TargetName{ get; internal set; }
            }

            /// <summary>
            /// Contains information about player refueling
            /// </summary>
            public class Refuel {
                /// <summary>
                /// Amount of the fuel bought
                /// </summary>
                public float Amount{ get; internal set; }
            }
        }
    }
}