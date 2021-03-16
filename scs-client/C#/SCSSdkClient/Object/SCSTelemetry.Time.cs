using System;

#pragma warning disable 1570

namespace SCSSdkClient.Object {
    public partial class ScsTelemetry {
        /// <summary>
        ///     Represent a time object with time in minutes and a DateTime object
        /// </summary>
        public class Time {
            /// <summary>
            /// Creates new <see cref="Time"/> object from the provided minute value
            /// </summary>
            /// <param name="i"></param>
            public Time(uint i) => Value = i;

            /// <summary>
            /// Creates new <see cref="Time"/> object
            /// </summary>
            public Time() { }

            /// <summary>
            ///     Represented in number of in-game minutes
            /// </summary>
            public uint Value { get; internal set; }

            /// <summary>
            ///     Represented in data of in-game minutes
            /// </summary>
            public DateTime Date => MinutesToDate(Value);

            /// <summary>
            /// Creates new <see cref="Time"/> object from the provided minute value
            /// </summary>
            /// <param name="i">Minutes</param>
            /// <returns><see cref="Time"/> object</returns>
            public static implicit operator Time(uint i) => new Time(i);

            /// <summary>
            /// Subtracts minutes from another <see cref="Time"/> object
            /// </summary>
            /// <param name="a">Left value</param>
            /// <param name="b">Right value</param>
            /// <returns>Value a - value b</returns>
            public static Time operator -(Time a, Time b) => new Time(a.Value - b.Value);
        }
    }
}