#pragma warning disable 1570

namespace SCSSdkClient.Object {
    public partial class ScsTelemetry {
        /// <summary>
        ///     Contains a in-game speed value (m/s), Kph (Km/h) and Mph of a speed value
        /// </summary>
        public class Movement {
            private Movement(float f) => Value = f;

            /// <summary>
            /// Creates new <see cref="Movement"/> object
            /// </summary>
            public Movement() { }

            /// <summary>
            ///     In-game speed value in m/s
            /// </summary>
            public float Value { get; internal set; }

            /// <summary>
            ///     In-game speed value in Kph (Km/h)
            /// </summary>
            public float Kph => Value * 3.6f;

            /// <summary>
            ///     In-game speed value in Mph
            /// </summary>
            public float Mph => Value * 2.25f;

            /// <summary>
            /// Creates new <see cref="Movement"/> object from the provided meters/second value
            /// </summary>
            /// <param name="f">Speed in m/s</param>
            /// <returns><see cref="Movement"/> object</returns>
            public static implicit operator Movement(float f) => new Movement(f);
        }
    }
}