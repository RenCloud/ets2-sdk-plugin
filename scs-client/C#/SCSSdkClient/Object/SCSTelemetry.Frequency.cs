using System;

#pragma warning disable 1570

namespace SCSSdkClient.Object {
    public partial class ScsTelemetry {
        /// <summary>
        /// Like Time object represent minutes and DateTime object,
        /// but represent more a timespan positive or negative
        /// </summary>
        public class Frequency {
            private Frequency(int i) => Value = i;

            /// <summary>
            /// Creates new <see cref="Frequency"/> object
            /// </summary>
            public Frequency() { }

            /// <summary>
            ///     Represented in number of in-game minutes
            /// </summary>
            public int Value { get; }

            /// <summary>
            ///     Represented in data of in-game minutes
            /// </summary>
            public DateTime Date => MinutesToDate(Value);

            /// <summary>
            /// Creates new <see cref="Frequency"/> object from value
            /// </summary>
            /// <param name="i">Provided value</param>
            /// <returns><see cref="Frequency"/> object</returns>
            public static implicit operator Frequency(int i) => new Frequency(i);
        }
    }
}