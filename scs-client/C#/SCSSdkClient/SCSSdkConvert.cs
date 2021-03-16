// 
// Ets2SdkClient
// Ets2SdkDataAlt.cs
// 22:51

using System;
using System.Text;
using SCSSdkClient.Object;

namespace SCSSdkClient {
    /// <summary>
    ///     Convert class
    /// </summary>
    public class ScsSdkConvert {
        private const int _stringSize = 64;
        private const int _wheelSize = 16;
        private const int _substances = 25;


        private readonly int[] offsetAreas =
            {0, 40, 500, 700, 1500, 1640, 2000, 2200, 2300, 4000, 4200, 4300, 4400, 6000};

        private byte[] data;
        private int offset;

        private int offsetArea;

        /// <summary>
        ///     Convert the Shared Memory Byte data structure in a C# object
        /// </summary>
        /// <param name="structureDataBytes">
        ///     byte array from the shared memory
        /// </param>
        /// <returns>
        ///     C# object with game data of the shared memory
        /// </returns>
        public ScsTelemetry Convert(byte[] structureDataBytes) {
            offsetArea = 0;
            SetOffset();


            data = structureDataBytes;
            var retData = new ScsTelemetry();

            #region FIRST ZONE 

            retData.SdkActive = GetBool();
            GetBoolArray(3);
            retData.Paused = GetBool();
            GetBoolArray(3);
            retData.Timestamp = GetULong();
            retData.SimulationTimestamp = GetULong();
            retData.RenderTimestamp = GetULong();



            NextOffsetArea();

            #endregion

            #region SECOND ZONE

            retData.DllVersion = GetUint();
            retData.GameVersion.Major = GetUint();
            retData.GameVersion.Minor = GetUint();
            retData.Game = GetUint().ToEnum<ScsGame>();
            retData.TelemetryVersion.Major = GetUint();
            retData.TelemetryVersion.Minor = GetUint();

            retData.CommonValues.GameTime = GetUint();
            retData.TruckValues.ConstantsValues.MotorValues.ForwardGearCount = GetUint();
            retData.TruckValues.ConstantsValues.MotorValues.ReverseGearCount = GetUint();
            retData.TruckValues.ConstantsValues.MotorValues.RetarderStepCount = GetUint();
            retData.TruckValues.ConstantsValues.WheelsValues.Count = GetUint();
            retData.TruckValues.ConstantsValues.MotorValues.SelectorCount = GetUint();
            retData.SetDeliveryTime(GetUint());
            retData.MaxTrailerCount = GetUint();
            retData.JobValues.CargoValues.UnitCount = GetUint();
            retData.JobValues.PlannedDistanceKm = GetUint();


            retData.TruckValues.CurrentValues.MotorValues.GearValues.HShifterSlot = GetUint();
            retData.TruckValues.CurrentValues.MotorValues.BrakeValues.RetarderLevel = GetUint();
            retData.TruckValues.CurrentValues.LightsValues.AuxFront = GetUint().ToEnum<AuxLevel>();
            retData.TruckValues.CurrentValues.LightsValues.AuxRoof = GetUint().ToEnum<AuxLevel>();
            retData.TruckValues.CurrentValues.WheelsValues.Substance = GetUintArray(_wheelSize);

            retData.TruckValues.ConstantsValues.MotorValues.SlotHandlePosition = GetUintArray(32);
            retData.TruckValues.ConstantsValues.MotorValues.SlotSelectors = GetUintArray(32);

            retData.GamePlay.JobDelivered.DeliveryTime = GetUint();
            var jobStartingTime = new ScsTelemetry.Time(GetUint());
            retData.GamePlay.JobCancelled.Started = jobStartingTime;
            retData.GamePlay.JobDelivered.Started = jobStartingTime;
            var jobFinishingTime = new ScsTelemetry.Time(GetUint());
            retData.GamePlay.JobCancelled.Finished = jobFinishingTime;
            retData.GamePlay.JobDelivered.Finished = jobFinishingTime;

            NextOffsetArea();

            #endregion

            #region THIRD ZONE

            retData.CommonValues.NextRestStop = GetInt();

            retData.TruckValues.CurrentValues.MotorValues.GearValues.Selected = GetInt();
            retData.TruckValues.CurrentValues.DashboardValues.GearDashboards = GetInt();
            retData.TruckValues.ConstantsValues.MotorValues.SlotGear = GetIntArray(32);

            retData.GamePlay.JobDelivered.EarnedXp = GetInt();

            NextOffsetArea();

            #endregion

            #region 4TH ZONE

            retData.CommonValues.Scale = GetFloat();

            retData.TruckValues.ConstantsValues.CapacityValues.Fuel = GetFloat();
            retData.TruckValues.ConstantsValues.WarningFactorValues.Fuel = GetFloat();
            retData.TruckValues.ConstantsValues.CapacityValues.AdBlue = GetFloat();
            retData.TruckValues.ConstantsValues.WarningFactorValues.AdBlue = GetFloat();
            retData.TruckValues.ConstantsValues.WarningFactorValues.AirPressure = GetFloat();
            retData.TruckValues.ConstantsValues.WarningFactorValues.AirPressureEmergency = GetFloat();
            retData.TruckValues.ConstantsValues.WarningFactorValues.OilPressure = GetFloat();
            retData.TruckValues.ConstantsValues.WarningFactorValues.WaterTemperature = GetFloat();
            retData.TruckValues.ConstantsValues.WarningFactorValues.BatteryVoltage = GetFloat();
            retData.TruckValues.ConstantsValues.MotorValues.EngineRpmMax = GetFloat();
            retData.TruckValues.ConstantsValues.MotorValues.DifferentialRation = GetFloat();
            retData.JobValues.CargoValues.Mass = GetFloat();
            retData.TruckValues.ConstantsValues.WheelsValues.Radius = GetFloatArray(_wheelSize);
            retData.TruckValues.ConstantsValues.MotorValues.GearRatiosForward = GetFloatArray(24);
            retData.TruckValues.ConstantsValues.MotorValues.GearRatiosReverse = GetFloatArray(8);
            retData.JobValues.CargoValues.UnitMass = GetFloat();

            retData.TruckValues.CurrentValues.DashboardValues.Speed.Value = GetFloat();
            retData.TruckValues.CurrentValues.DashboardValues.Rpm = GetFloat();
            retData.ControlValues.InputValues.Steering = GetFloat();
            retData.ControlValues.InputValues.Throttle = GetFloat();
            retData.ControlValues.InputValues.Brake = GetFloat();
            retData.ControlValues.InputValues.Clutch = GetFloat();
            retData.ControlValues.GameValues.Steering = GetFloat();
            retData.ControlValues.GameValues.Throttle = GetFloat();
            retData.ControlValues.GameValues.Brake = GetFloat();
            retData.ControlValues.GameValues.Clutch = GetFloat();
            retData.TruckValues.CurrentValues.DashboardValues.CruiseControlSpeed.Value = GetFloat();
            retData.TruckValues.CurrentValues.MotorValues.BrakeValues.AirPressure = GetFloat();
            retData.TruckValues.CurrentValues.MotorValues.BrakeValues.Temperature = GetFloat();
            retData.TruckValues.CurrentValues.DashboardValues.FuelValue.Amount = GetFloat();
            retData.TruckValues.CurrentValues.DashboardValues.FuelValue.AverageConsumption = GetFloat();
            retData.TruckValues.CurrentValues.DashboardValues.FuelValue.Range = GetFloat();
            retData.TruckValues.CurrentValues.DashboardValues.AdBlue = GetFloat();
            retData.TruckValues.CurrentValues.DashboardValues.OilPressure = GetFloat();
            retData.TruckValues.CurrentValues.DashboardValues.OilTemperature = GetFloat();
            retData.TruckValues.CurrentValues.DashboardValues.WaterTemperature = GetFloat();
            retData.TruckValues.CurrentValues.DashboardValues.BatteryVoltage = GetFloat();
            retData.TruckValues.CurrentValues.LightsValues.DashboardBacklight = GetFloat();
            retData.TruckValues.CurrentValues.DamageValues.Engine = GetFloat();
            retData.TruckValues.CurrentValues.DamageValues.Transmission = GetFloat();
            retData.TruckValues.CurrentValues.DamageValues.Cabin = GetFloat();
            retData.TruckValues.CurrentValues.DamageValues.Chassis = GetFloat();
            retData.TruckValues.CurrentValues.DamageValues.WheelsAvg = GetFloat();

       
            retData.TruckValues.CurrentValues.DashboardValues.Odometer = GetFloat();
            retData.NavigationValues.NavigationDistance = GetFloat();
            retData.NavigationValues.NavigationTime = GetFloat();
            retData.NavigationValues.SpeedLimit = GetFloat();
            retData.TruckValues.CurrentValues.WheelsValues.SuspDeflection = GetFloatArray(_wheelSize);
            retData.TruckValues.CurrentValues.WheelsValues.Velocity = GetFloatArray(_wheelSize);
            retData.TruckValues.CurrentValues.WheelsValues.Steering = GetFloatArray(_wheelSize);
            retData.TruckValues.CurrentValues.WheelsValues.Rotation = GetFloatArray(_wheelSize);
            retData.TruckValues.CurrentValues.WheelsValues.Lift = GetFloatArray(_wheelSize);
            retData.TruckValues.CurrentValues.WheelsValues.LiftOffset = GetFloatArray(_wheelSize);

            retData.GamePlay.JobDelivered.CargoDamage = GetFloat();
            retData.GamePlay.JobDelivered.DistanceKm = GetFloat();

            retData.GamePlay.RefuelEvent.Amount = GetFloat();

            retData.JobValues.CargoValues.CargoDamage = GetFloat();

            NextOffsetArea();

            #endregion

            #region 5Th ZONE

            retData.TruckValues.ConstantsValues.WheelsValues.Steerable = GetBoolArray(_wheelSize);
            retData.TruckValues.ConstantsValues.WheelsValues.Simulated = GetBoolArray(_wheelSize);
            retData.TruckValues.ConstantsValues.WheelsValues.Powered = GetBoolArray(_wheelSize);
            retData.TruckValues.ConstantsValues.WheelsValues.Liftable = GetBoolArray(_wheelSize);

            retData.JobValues.CargoLoaded = GetBool();
            retData.JobValues.SpecialJob = GetBool();

            retData.TruckValues.CurrentValues.MotorValues.BrakeValues.ParkingBrake = GetBool();
            retData.TruckValues.CurrentValues.MotorValues.BrakeValues.MotorBrake = GetBool();
            retData.TruckValues.CurrentValues.DashboardValues.WarningValues.AirPressure = GetBool();
            retData.TruckValues.CurrentValues.DashboardValues.WarningValues.AirPressureEmergency = GetBool();

            retData.TruckValues.CurrentValues.DashboardValues.WarningValues.FuelW = GetBool();
            retData.TruckValues.CurrentValues.DashboardValues.WarningValues.AdBlue = GetBool();
            retData.TruckValues.CurrentValues.DashboardValues.WarningValues.OilPressure = GetBool();
            retData.TruckValues.CurrentValues.DashboardValues.WarningValues.WaterTemperature = GetBool();
            retData.TruckValues.CurrentValues.DashboardValues.WarningValues.BatteryVoltage = GetBool();
            retData.TruckValues.CurrentValues.ElectricEnabled = GetBool();
            retData.TruckValues.CurrentValues.EngineEnabled = GetBool();
            retData.TruckValues.CurrentValues.DashboardValues.Wipers = GetBool();
            retData.TruckValues.CurrentValues.LightsValues.BlinkerLeftActive = GetBool();
            retData.TruckValues.CurrentValues.LightsValues.BlinkerRightActive = GetBool();
            retData.TruckValues.CurrentValues.LightsValues.BlinkerLeftOn = GetBool();
            retData.TruckValues.CurrentValues.LightsValues.BlinkerRightOn = GetBool();
            retData.TruckValues.CurrentValues.LightsValues.Parking = GetBool();
            retData.TruckValues.CurrentValues.LightsValues.BeamLow = GetBool();
            retData.TruckValues.CurrentValues.LightsValues.BeamHigh = GetBool();
            retData.TruckValues.CurrentValues.LightsValues.Beacon = GetBool();
            retData.TruckValues.CurrentValues.LightsValues.Brake = GetBool();
            retData.TruckValues.CurrentValues.LightsValues.Reverse = GetBool();
            retData.TruckValues.CurrentValues.DashboardValues.CruiseControl = GetBool();
            retData.TruckValues.CurrentValues.WheelsValues.OnGround = GetBoolArray(_wheelSize);
            retData.TruckValues.CurrentValues.MotorValues.GearValues.HShifterSelector = GetBoolArray(2);

            retData.GamePlay.JobDelivered.AutoParked = GetBool();
            retData.GamePlay.JobDelivered.AutoLoaded = GetBool();

            NextOffsetArea();

            #endregion

            #region 6TH ZONE

            retData.TruckValues.Positioning.Cabin = GetFVector();
            retData.TruckValues.Positioning.Head = GetFVector();
            retData.TruckValues.Positioning.Hook = GetFVector();
            var tempPos = new ScsTelemetry.FVector[_wheelSize];
            for (var j = 0; j < _wheelSize; j++) {
                tempPos[j] = new ScsTelemetry.FVector {X = GetFloat()};
            }

            for (var j = 0; j < _wheelSize; j++) {
                tempPos[j].Y = GetFloat();
            }

            for (var j = 0; j < _wheelSize; j++) {
                tempPos[j].Z = GetFloat();
            }

            retData.TruckValues.ConstantsValues.WheelsValues.PositionValues = tempPos;


            retData.TruckValues.CurrentValues.AccelerationValues.LinearVelocity = GetFVector();
            retData.TruckValues.CurrentValues.AccelerationValues.AngularVelocity = GetFVector();
            retData.TruckValues.CurrentValues.AccelerationValues.LinearAcceleration = GetFVector();
            retData.TruckValues.CurrentValues.AccelerationValues.AngularAcceleration = GetFVector();
            retData.TruckValues.CurrentValues.AccelerationValues.CabinAngularVelocity = GetFVector();
            retData.TruckValues.CurrentValues.AccelerationValues.CabinAngularAcceleration = GetFVector();

            NextOffsetArea();

            #endregion

            #region 7TH ZONE

            retData.TruckValues.Positioning.CabinOffset = GetFPlacement();
            retData.TruckValues.Positioning.HeadOffset = GetFPlacement();


            NextOffsetArea();

            #endregion

            #region 8TH ZONE 

            retData.SetTruckPosition(GetDPlacement());


            NextOffsetArea();

            #endregion

            #region 9TH ZONE

            retData.TruckValues.ConstantsValues.BrandId = GetString();
            retData.TruckValues.ConstantsValues.Brand = GetString();
            retData.TruckValues.ConstantsValues.Id = GetString();
            retData.TruckValues.ConstantsValues.Name = GetString();
            retData.JobValues.CargoValues.Id = GetString();
            retData.JobValues.CargoValues.Name = GetString();
            retData.JobValues.CityDestinationId = GetString();
            retData.JobValues.CityDestination = GetString();
            retData.JobValues.CompanyDestinationId = GetString();
            retData.JobValues.CompanyDestination = GetString();
            retData.JobValues.CitySourceId = GetString();
            retData.JobValues.CitySource = GetString();
            retData.JobValues.CompanySourceId = GetString();
            retData.JobValues.CompanySource = GetString();
            var tempShift = GetString(16);
            if (tempShift?.Length > 0) {
                retData.TruckValues.ConstantsValues.MotorValues.ShifterTypeValue = tempShift.ToEnum<ShifterType>();
            }

            retData.TruckValues.ConstantsValues.LicensePlate = GetString();
            retData.TruckValues.ConstantsValues.LicensePlateCountryId = GetString();
            retData.TruckValues.ConstantsValues.LicensePlateCountry = GetString();

            var tempJobMarket = GetString(32);
            if (tempJobMarket?.Length > 0) {
                retData.JobValues.Market = tempJobMarket.ToEnum<JobMarket>();
            }

            var tempfineOffence = GetString(32);
            if (tempfineOffence?.Length > 0) {
                retData.GamePlay.FinedEvent.Offence = tempfineOffence.ToEnum<Offence>();
            }

            retData.GamePlay.FerryEvent.SourceName = GetString();
            retData.GamePlay.FerryEvent.TargetName = GetString();
            retData.GamePlay.FerryEvent.SourceId = GetString();
            retData.GamePlay.FerryEvent.TargetId = GetString();
            retData.GamePlay.TrainEvent.SourceName = GetString();
            retData.GamePlay.TrainEvent.TargetName = GetString();
            retData.GamePlay.TrainEvent.SourceId = GetString();
            retData.GamePlay.TrainEvent.TargetId = GetString();


            NextOffsetArea();

            #endregion

            #region 10TH ZONE

            retData.JobValues.Income = GetULong();

            NextOffsetArea();

            #endregion

            #region 11TH ZONE

            retData.GamePlay.JobCancelled.Penalty = GetLong();
            retData.GamePlay.JobDelivered.Revenue = GetLong();
            retData.GamePlay.FinedEvent.Amount = GetLong();
            retData.GamePlay.TollgateEvent.PayAmount = GetLong();
            retData.GamePlay.FerryEvent.PayAmount = GetLong();
            retData.GamePlay.TrainEvent.PayAmount = GetLong();

            NextOffsetArea();

            #endregion

            #region 12TH ZONE

            retData.SpecialEventsValues.OnJob = GetBool();
            retData.SpecialEventsValues.JobFinished = GetBool();

            retData.SpecialEventsValues.JobCancelled = GetBool(); 
            retData.SpecialEventsValues.JobDelivered = GetBool();
            retData.SpecialEventsValues.Fined = GetBool();
            retData.SpecialEventsValues.Tollgate = GetBool();
            retData.SpecialEventsValues.Ferry = GetBool();
            retData.SpecialEventsValues.Train = GetBool();

            retData.SpecialEventsValues.Refuel = GetBool();
            retData.SpecialEventsValues.RefuelPaid = GetBool();

            NextOffsetArea();

            #endregion

            #region 13TH ZONE

            for (var i = 0; i < _substances; i++) {
                var tempSubstance = GetString();
                if (tempSubstance.Length != 0) {
                    retData.Substances.Add(new ScsTelemetry.Substance {Index = i, Value = tempSubstance});
                }
            }

            NextOffsetArea();

            #endregion

            #region 14TH ZONE

            retData.TrailerValues = GetTrailers();

            #endregion


            return retData;
        }

        private bool GetBool() {
            var temp = data[offset];
            offset++;
            return temp > 0;
        }

        private uint GetUint() {
            while (offset % 4 != 0) {
                offset++;
            }

            var temp = (uint) ((data[offset + 3] << 24) |
                               (data[offset + 2] << 16) |
                               (data[offset + 1] << 8) |
                               data[offset]);
            offset += 4;
            return temp;
        }

        private float GetFloat() {
            while (offset % 4 != 0) {
                offset++;
            }

            var temp = new[] {data[offset], data[offset + 1], data[offset + 2], data[offset + 3]};
            offset += 4;
            return BitConverter.ToSingle(temp, 0);
        }

        private double GetDouble() {
            while (offset % 4 != 0) {
                offset++;
            }

            var temp = new[] {
                                 data[offset], data[offset + 1], data[offset + 2], data[offset + 3],
                                 data[offset + 4], data[offset + 5], data[offset + 6], data[offset + 7]
                             };
            offset += 8;
            return BitConverter.ToDouble(temp, 0);
        }

        private int GetInt() {
            while (offset % 4 != 0) {
                offset++;
            }

            var temp = (data[offset + 3] << 24) |
                       (data[offset + 2] << 16) |
                       (data[offset + 1] << 8) |
                       data[offset];
            offset += 4;
            return temp;
        }

        private byte[] GetSubArray(int length) {
            var ret = new byte[length];
            for (var i = 0; i < length; i++) {
                ret[i] = data[offset + i];
            }

            offset += length;
            return ret;
        }


        private void NextOffsetArea() {
            offsetArea++;
            SetOffset();
        }

        private void SetOffset() {
            // Debug Fix?
            if (offsetArea >= offsetAreas.Length) {
                return;
            }

            offset = offsetAreas[offsetArea];
        }

        private void SetOffset(int off) => offset += off;

        private string GetString(int length = _stringSize) {
            var area = GetSubArray(length);
            return Encoding.UTF8.GetString(area).Replace('\0', ' ').Trim();
        }

        private uint[] GetUintArray(int length) {
            var res = new uint[length];
            for (var i = 0; i < length; i++) {
                res[i] = GetUint();
            }

            return res;
        }

        private int[] GetIntArray(int length) {
            var res = new int[length];
            for (var i = 0; i < length; i++) {
                res[i] = GetInt();
            }

            return res;
        }

        private float[] GetFloatArray(int length) {
            var res = new float[length];
            for (var i = 0; i < length; i++) {
                res[i] = GetFloat();
            }

            return res;
        }

        private bool[] GetBoolArray(int length) {
            var res = new bool[length];
            for (var i = 0; i < length; i++) {
                res[i] = GetBool();
            }

            return res;
        }

        private ScsTelemetry.FVector GetFVector() => new ScsTelemetry.FVector
                                                     {X = GetFloat(), Y = GetFloat(), Z = GetFloat()};

        private ScsTelemetry.FVector[] GetFVectorArray(int length) {
            var tempPos = new ScsTelemetry.FVector[length];
            for (var j = 0; j < length; j++) {
                tempPos[j] = new ScsTelemetry.FVector {X = GetFloat()};
            }

            for (var j = 0; j < length; j++) {
                tempPos[j].Y = GetFloat();
            }

            for (var j = 0; j < length; j++) {
                tempPos[j].Z = GetFloat();
            }

            return tempPos;
        }

        private ScsTelemetry.DVector GetDVector() => new ScsTelemetry.DVector
                                                     {X = GetDouble(), Y = GetDouble(), Z = GetDouble()};

        private ScsTelemetry.Euler GetEuler() => new ScsTelemetry.Euler
                                                 {Heading = GetFloat(), Pitch = GetFloat(), Roll = GetFloat()};

        private ScsTelemetry.Euler GetDEuler() =>
            new ScsTelemetry.Euler
            {Heading = (float) GetDouble(), Pitch = (float) GetDouble(), Roll = (float) GetDouble()};

        private ScsTelemetry.FPlacement GetFPlacement() => new ScsTelemetry.FPlacement
                                                           {Position = GetFVector(), Orientation = GetEuler()};

        private ScsTelemetry.DPlacement GetDPlacement() => new ScsTelemetry.DPlacement
                                                           {Position = GetDVector(), Orientation = GetDEuler()};

        private long GetLong() {
            var temp = new[] {
                                 data[offset], data[offset + 1], data[offset + 2], data[offset + 3],
                                 data[offset + 4], data[offset + 5], data[offset + 6], data[offset + 7]
                             };
            offset += 8;
            return BitConverter.ToInt64(temp, 0);
        }

        private ulong GetULong() {
            var temp = new[] {
                                 data[offset], data[offset + 1], data[offset + 2], data[offset + 3],
                                 data[offset + 4], data[offset + 5], data[offset + 6], data[offset + 7]
                             };
            offset += 8;
            return BitConverter.ToUInt64(temp, 0);
        }

        private ScsTelemetry.Trailer[] GetTrailers() {
            var trailer = new ScsTelemetry.Trailer[10];
            //TODO : only 1 for old game versions
            for (var i = 0; i < 10; i++) {
                trailer[i] = GetTrailer();
            }

            return trailer;
        }

        private ScsTelemetry.Trailer GetTrailer() {
            var trailer = new ScsTelemetry.Trailer();

            #region bool Region

            trailer.WheelsConstant.Steerable = GetBoolArray(_wheelSize);
            trailer.WheelsConstant.Simulated = GetBoolArray(_wheelSize);
            trailer.WheelsConstant.Powered = GetBoolArray(_wheelSize);
            trailer.WheelsConstant.Liftable = GetBoolArray(_wheelSize);
            trailer.WheelValues.OnGround = GetBoolArray(_wheelSize);
            trailer.Attached = GetBool();
            SetOffset(3);

            #endregion First Zone 0 - 83

            #region uint Region

            trailer.WheelValues.Substance = GetUintArray(_wheelSize);
            trailer.WheelsConstant.Count = GetUint();

            #endregion Second Zone 84 - 151

            #region float Region

            trailer.DamageValues.Cargo = GetFloat();
            trailer.DamageValues.Chassis = GetFloat();
            trailer.DamageValues.Wheels = GetFloat();
            trailer.WheelValues.SuspDeflection = GetFloatArray(_wheelSize);
            trailer.WheelValues.Velocity = GetFloatArray(_wheelSize);
            trailer.WheelValues.Steering = GetFloatArray(_wheelSize);
            trailer.WheelValues.Rotation = GetFloatArray(_wheelSize);
            trailer.WheelValues.Lift = GetFloatArray(_wheelSize);
            trailer.WheelValues.LiftOffset = GetFloatArray(_wheelSize);

            trailer.WheelsConstant.Radius = GetFloatArray(_wheelSize);

            #endregion Third Zone 152 - 611

            #region floatvector Region

            trailer.AccelerationValues.LinearVelocity = GetFVector();
            trailer.AccelerationValues.AngularVelocity = GetFVector();
            trailer.AccelerationValues.LinearAcceleration = GetFVector();
            trailer.AccelerationValues.AngularAcceleration = GetFVector();

            trailer.Hook = GetFVector();

            trailer.WheelsConstant.PositionValues = GetFVectorArray(_wheelSize);

            #endregion 4Th Zone 612 - 863

            #region double placement Region

            trailer.Position = GetDPlacement();

            #endregion 5Th 864 - 911

            #region string Region

            trailer.Id = GetString();
            trailer.CargoAccessoryId = GetString();
            trailer.BodyType = GetString();
            trailer.BrandId = GetString();
            trailer.Brand = GetString();
            trailer.Name = GetString();
            trailer.ChainType = GetString();
            trailer.LicensePlate = GetString();
            trailer.LicensePlateCountry = GetString();
            trailer.LicensePlateCountryId = GetString();

            #endregion 6th Zone 912 - 1551

            return trailer;
        }
    }
}