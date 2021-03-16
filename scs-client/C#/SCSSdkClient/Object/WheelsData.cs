namespace SCSSdkClient.Object
{
    /// <summary>
    ///     States of the Wheels
    /// </summary>
    public class WheelsData {
        /// <summary>
        ///     Substance below the wheel
        /// </summary>
        public uint[] Substance { get; internal set; }

        /// <summary>
        ///     Vertical displacement of the wheel from its axis in meters
        /// </summary>
        public float[] SuspDeflection { get; internal set; }

        /// <summary>
        ///     Angular velocity of the wheel in rotations per second.
        ///     Positive velocity corresponds to forward movement.
        /// </summary>
        public float[] Velocity { get; internal set; }

        /// <summary>
        ///     Steering rotation of the wheel in rotations.
        ///     Value is from &lt;0.25,0.25&gt; range in counterclockwise direction when looking from top
        ///         (e.g. 0.25 corresponds to left and -0.25 corresponds to right).
        ///     Set to zero for non-steered wheels.
        /// </summary>
        public float[] Steering { get; internal set; }

        /// <summary>
        ///     Rolling rotation of the wheel in rotations.
        ///     Value is from &lt;0.0,1.0) range in which value increase corresponds to forward movement
        /// </summary>
        public float[] Rotation { get; internal set; }

        /// <summary>
        ///     Is true if the wheel contacts the ground
        /// </summary>
        public bool[] OnGround { get; internal set; }

        /// <summary>
        ///     Lift of each tire
        /// </summary>
        public float[] Lift { get; internal set; }
        
        /// <summary>
        ///     Lift offset of each tire
        /// </summary>
        public float[] LiftOffset { get; internal set; }
    }
}