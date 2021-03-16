#pragma warning disable 1570

namespace SCSSdkClient.Object {
    public partial class ScsTelemetry {
        /// <summary>
        ///     Special Events the plugin deliver like onJob and JobFinished
        /// </summary>
        public class SpecialEvents {
            /// <summary>
            ///     Event flag that displays if a driver is doing a job
            /// </summary>
            public bool OnJob { get; internal set; }

            /// <summary>
            ///     Event flag that displays (for some ticks) that a job was finished, doesn't matter how (cancelled, delivered, ...)
            ///     Deprecated for Version 1.35 of both games. Use JobCancelled and JobDelivered Event for version starting with 1.35+
            ///     But should still work
            /// </summary>
            public bool JobFinished { get; internal set; }

            /// <summary>
            /// Flag if the job was cancelled
            /// </summary>
            public bool JobCancelled { get; internal set; }
            /// <summary>
            /// Flag if the job was delivered
            /// </summary>
            public bool JobDelivered { get; internal set; }
            /// <summary>
            /// Flag if the player was fined
            /// </summary>
            public bool Fined { get; internal set; }
            /// <summary>
            /// Flag if the player went through a tollgate
            /// </summary>
            public bool Tollgate { get; internal set; }
            /// <summary>
            /// Flag if player used a ferry to transport
            /// </summary>
            public bool Ferry { get; internal set; }
            /// <summary>
            /// Flag if player used a train to transport
            /// </summary>
            public bool Train { get; internal set; }
            /// <summary>
            /// Flag if player is refueling
            /// </summary>
            public bool Refuel {get; internal set;}
            /// <summary>
            /// Flag if player paid for fuel
            /// </summary>
            public bool RefuelPaid {get; internal set;}
        }
    }
}