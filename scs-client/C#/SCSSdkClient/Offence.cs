namespace SCSSdkClient {
    /// <summary>
    ///     Offence type
    /// </summary>
    public enum Offence {
        /// <summary>
        ///     No Value from the sdk
        /// </summary>
        NoValue,

        /// <summary>
        ///     Crash with another vehicle
        /// </summary>
        Crash,

        /// <summary>
        ///     driver did not sleep
        /// </summary>
        AvoidSleeping,

        /// <summary>
        ///     drive on the wrong side of the street
        /// </summary>
        WrongWay,

        /// <summary>
        ///     drives to fast at a camera
        /// </summary>
        SpeedingCamera,

        /// <summary>
        ///     drives without lights on
        /// </summary>
        NoLights,

        /// <summary>
        ///     ignores a red signal
        /// </summary>
        RedSignal,

        /// <summary>
        ///     ignore weighting
        /// </summary>
        AvoidWeighting,

        /// <summary>
        ///     drives to fast
        /// </summary>
        Speeding,

        /// <summary>
        ///     carries a trailer that is not allowed in this area
        /// </summary>
        IllegalTrailer,

        /// <summary>
        ///     avoid an inspection
        /// </summary>
        AvoidInspection,

        /// <summary>
        ///     illegal border crossing
        /// </summary>
        IllegalBorderCrossing,

        /// <summary>
        ///     Hard shoulder violation
        /// </summary>
        HardShoulderViolation,

        /// <summary>
        /// Damaged vehicle usage
        /// </summary>
        DamagedVehicleUsage,

        /// <summary>
        ///     some other generic fine
        /// </summary>
        Generic
    }
}