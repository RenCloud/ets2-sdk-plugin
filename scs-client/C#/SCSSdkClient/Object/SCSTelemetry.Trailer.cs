#pragma warning disable 1570

namespace SCSSdkClient.Object {
    public partial class ScsTelemetry {
        /// <summary>
        ///     Trailer Values
        /// </summary>
        public class Trailer {
            /// <summary>
            ///     Initialise a trailer object
            /// </summary>
            public Trailer() {
                WheelValues = new WheelsData();
                AccelerationValues = new Acceleration();
                WheelsConstant = new WheelsConstants();
                Hook = new FVector();
                DamageValues = new Damage();
            }

            /// <summary>
            ///     Is the trailer connected to the truck
            /// </summary>
            public bool Attached { get; internal set; }

            /// <summary>
            ///     Default position of the hook in vehicle space
            /// </summary>
            public FVector Hook { get; internal set; }

            /// <summary>
            /// Information about damages
            /// </summary>
            public Damage DamageValues { get; internal set; }

            /// <summary>
            ///     Current wheel values of the trailer, like rotation , substance, ...
            ///     <seealso cref="WheelsData" /> for more information
            /// </summary>
            public WheelsData WheelValues { get; internal set; }

            /// <summary>
            ///     Constant wheel values like Count, Radius,  ...
            ///     <seealso cref="WheelsConstants" /> for more information
            /// </summary>
            public WheelsConstants WheelsConstant { get; internal set; }

            /// <summary>
            ///     Acceleration values of the trailer
            ///     <seealso cref="Acceleration" /> for more informatiton
            /// </summary>
            public Acceleration AccelerationValues { get; internal set; }

            /// <summary>
            ///     Position of the trailer
            /// </summary>
            public DPlacement Position { get; internal set; }

            /// <summary>
            ///     Id for internal use by code.
            ///     Limited to C-identifier characters and dots
            /// </summary>
            public string Id { get; internal set; }

            /// <summary>
            ///     Name of cargo accessory for internal use by code.
            ///     Limited to C-identifier characters and dots.
            /// </summary>
            public string CargoAccessoryId { get; internal set; }

            /// <summary>
            /// Type of the body
            /// </summary>
            public string BodyType { get; internal set; }
            /// <summary>
            /// ID of the trailer brand
            /// </summary>
            public string BrandId { get; internal set; }
            /// <summary>
            /// Localized brand of the trailer
            /// </summary>
            public string Brand { get; internal set; }
            /// <summary>
            /// Name of the trailer
            /// </summary>
            public string Name { get; internal set; }
            /// <summary>
            /// Chain type of the trailer (single, double, etc)
            /// </summary>
            public string ChainType { get; internal set; }
            /// <summary>
            /// License plate on the trailer
            /// </summary>
            public string LicensePlate { get; internal set; }
            /// <summary>
            /// Localized country of the license plate
            /// </summary>
            public string LicensePlateCountry { get; internal set; }
            /// <summary>
            /// ID of the license plate
            /// </summary>
            public string LicensePlateCountryId { get; internal set; }


            /// <summary>
            /// Holds information about the damages
            /// </summary>
            public class Damage {
                /// <summary>
                /// Damage of the cargo
                /// </summary>
                public float Cargo { get; internal set; }
                /// <summary>
                /// Damage of the wheels
                /// </summary>
                public float Wheels { get; internal set; }
                /// <summary>
                /// Damage of the chassis
                /// </summary>
                public float Chassis { get; internal set; }
            }

            /// <summary>
            /// Holds acceleration information
            /// </summary>
            public class Acceleration {
                /// <summary>
                ///     Represents vehicle space linear velocity of the trailer measured in m/s
                /// </summary>
                public FVector LinearVelocity { get; internal set; }

                /// <summary>
                ///     Represents vehicle space angular velocity of the trailer measured in rotation/s
                /// </summary>
                public FVector AngularVelocity { get; internal set; }

                /// <summary>
                ///     Represents vehicle space linear acceleration of the trailer measured in m/s^2
                /// </summary>
                public FVector LinearAcceleration { get; internal set; }

                /// <summary>
                ///     Represents vehicle space angular acceleration of the trailer measured in rotations/s^2
                /// </summary>
                public FVector AngularAcceleration { get; internal set; }
            }
        }
    }
}