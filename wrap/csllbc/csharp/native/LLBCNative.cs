/**
 * @file    NativeLLBC.cs
* @author   Longwei Lai<lailongwei@126.com>
* @date     2016/01/25
* @version  1.0
*
* @brief
*/

using System.Runtime.InteropServices;
namespace llbc
{
    /// <summary>
    /// The all llbc native methods declare.
    /// </summary>
    internal partial class LLBCNative
    {
        #region Startup/Cleanup llbc library
        /// <summary>
        /// Startup the llbc library.
        /// </summary>
        /// <returns></returns>
        [DllImport(NativeLibName, CallingConvention = CallingConvention.Cdecl)]
        public extern static int csllbc_Startup();

        /// <summary>
        /// Cleanup the llbc library.
        /// </summary>
        /// <returns></returns>
        [DllImport(NativeLibName, CallingConvention = CallingConvention.Cdecl)]
        public extern static int csllbc_Cleanup();
        #endregion

        # region Library some constants
        /// <summary>
        /// All llbc native library functions execute success return code.
        /// </summary>
        public const int LLBC_OK = 0;

        /// <summary>
        /// All llbc native library functions execute failed return code.
        /// </summary>
        public const int LLBC_FAILED = -1;
        #endregion

        #region llbc core library name & native library name define
#if !DEBUG
        /// <summary>
        /// The llbc core library name define.
        /// </summary>
        public const string CoreLibName = "libllbc.dll";

        /// <summary>
        /// The llbc c# native library name define.
        /// </summary>
        public const string NativeLibName = "libcsllbc_native.dll";
#else // Debug
        /// <summary>
        /// The llbc core library name define.
        /// </summary>
        public const string CoreLibName = "libllbc_debug.dll";

        /// <summary>
        /// The llbc c# native library name define.
        /// </summary>
        public const string NativeLibName = "libcsllbc_native_debug.dll";
#endif
        #endregion
    }
}
