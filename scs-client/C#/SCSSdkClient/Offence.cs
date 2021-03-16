namespace SCSSdkClient {
    /// <summary>
    ///     Offence type
    /// </summary>
    public enum Offence {
        ///About: Values
        ///
        /// NoValue                 - No Value from the sdk
        /// Crash                   - Crash with another vehicle
        /// Avoid_sleeping          - driver did not sleep
        /// Wrong_way               - drive on the wrong side of the street
        /// Speeding_camera         - drives to fast at a camera
        /// No_lights               - drives without lights on
        /// Red_signal              - ignores a red signal
        /// Avoid-Weighting         - ignore weighting
        /// Speeding                - drives to fast
        /// Illegal_trailer         - carries a trailer that is not allowed in this area
        /// Avoid_Inspection        - avoid inspection
        /// Illegal_Border_Crossing - illegal border crossing
        /// Hard_Shoulder_Violation - hard shoulder violation
        /// Damaged_Vehicle_Usage   - damaged vehicle usage
        /// Generic                 - some other generic fine



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