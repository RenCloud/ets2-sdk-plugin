namespace SCSSdkClient {
    /// <summary>
    /// Market of the job
    /// </summary>
    public enum JobMarket {
        /// <summary>
        /// Unknown market
        /// </summary>
        NoValue,
        /// <summary>
        /// Job with own truck and provided trailer
        /// </summary>
        CargoMarket,
        /// <summary>
        /// Job with provided truck and trailer
        /// </summary>
        QuickJob,
        /// <summary>
        /// Job with own truck and trailer
        /// </summary>
        FreightMarket,
        /// <summary>
        /// Job with own truck and provided trailer
        /// </summary>
        ExternalContracts,
        /// <summary>
        /// Job with own truck and own trailer
        /// </summary>
        ExternalMarket
    }
}