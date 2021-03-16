using System;
using System.IO.MemoryMappedFiles;
using SCSSdkClient.Object;

namespace SCSSdkClient {
    /// <summary>
    ///     Manage the shared memory
    /// </summary>
    public class SharedMemory {
        /// <summary>
        ///     size of the shared memory in bytes
        /// </summary>
        private const uint _defaultMapSize = 32 * 1024;

        /// <summary>
        ///     holds the byte to object convert class
        /// </summary>
        private readonly ScsSdkConvert sdkconvert = new ScsSdkConvert();

        /// <summary>
        ///     memory mapped file
        /// </summary>
        private MemoryMappedFile memoryMappedHandle;

        /// <summary>
        ///     memory mapped view accessor
        /// </summary>
        private MemoryMappedViewAccessor memoryMappedView;

        /// <summary>
        ///     Could we create a memory view on the memory map
        /// </summary>
        public bool Hooked { get; private set; }

        /// <summary>
        ///     if we can't create a memory view hold the exception
        /// </summary>
        public Exception HookException { get; private set; }

        /// <summary>
        ///     raw byte data of the memory map
        /// </summary>
        public byte[] RawData { get; private set; }

        /// <summary>
        ///     create/connect to a shared memory file
        /// </summary>
        /// <param name="map">Map location string</param>
        public void Connect(string map) => Connect(map, _defaultMapSize);

        /// <summary>
        ///     create/connect to a shared memory file
        /// </summary>
        /// <param name="map">map location string</param>
        /// <param name="mapSize">size of the map</param>
        public void Connect(string map, uint mapSize) {
            if (Hooked) {
                Disconnect();
            }

            // Reset any errors
            HookException = null;

            try {
                RawData = new byte[mapSize];

                // Open the map and create a "memory view" at the begin (byte 0)
                memoryMappedHandle = MemoryMappedFile.CreateOrOpen(map, mapSize, MemoryMappedFileAccess.ReadWrite);
                memoryMappedView = memoryMappedHandle.CreateViewAccessor(0, mapSize);

                // Mark as a success.
                Hooked = true;
            } catch (Exception e) {
                // We were unable to hook onto the map.
                Hooked = false;
                HookException = e;
            }
        }

        /// <summary>
        ///     close the memory view and handle
        /// </summary>
        public void Disconnect() {
            Hooked = false;

            memoryMappedView.Dispose();
            memoryMappedHandle.Dispose();
        }

        /// <summary>
        ///     read data from memory and update object
        /// </summary>
        /// <returns></returns>
        public ScsTelemetry UpdateData() {
            Update();

            // Convert the data to our object.
            return ToObject(RawData);
        }

        /// <summary>
        ///     reread data from memory view
        /// </summary>
        public void Update() {
            if (!Hooked || memoryMappedView == null) {
                return;
            }

            // Re-read data from the view.
            memoryMappedView.ReadArray(0, RawData, 0, RawData.Length);
        }

        /// <summary>
        ///     Cast a set of bytes to a managed C# object.
        /// </summary>
        /// <param name="structureDataBytes">Bytes array</param>
        /// <returns>Managed object from given bytes</returns>
        protected ScsTelemetry ToObject(byte[] structureDataBytes) => sdkconvert.Convert(structureDataBytes);
    }
}