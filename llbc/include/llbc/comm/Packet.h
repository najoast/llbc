/**
 * @file    Packet.h
 * @author  Longwei Lai<lailongwei@126.com>
 * @date    2013/06/16
 * @version 1.0
 *
 * @brief
 */
#ifndef __LLBC_COMM_PACKET_H__
#define __LLBC_COMM_PACKET_H__

#include "llbc/common/Common.h"
#include "llbc/core/Core.h"
#include "llbc/objbase/ObjBase.h"

/**
 * Pre-declare some classes.
 */
__LLBC_NS_BEGIN
class LLBC_ICoder;
class LLBC_Session;
class LLBC_PacketHeaderDesc;
__LLBC_NS_END

__LLBC_NS_BEGIN

/**
 * \brief The packet class encapsulation.
 */
class LLBC_EXPORT LLBC_Packet
{
public:
    LLBC_Packet();
    ~LLBC_Packet();

public:
    /**
     * Get packet length.
     * @return int - packet length.
     */
    int GetLength() const;

    /**
     * Get opcode.
     * @return int - opcode.
     */
    int GetOpcode() const;

    /**
     * Set opcode.
     * @param[in] opcode - opcode.
     */
    void SetOpcode(int opcode);

    /**
     * Get status code.
     * @return int - status code.
     */
    int GetStatus() const;
    /**
     * Set status code.
     * @param[in] status - status code.
     */
    void SetStatus(int status);

#if LLBC_CFG_COMM_ENABLE_STATUS_DESC
    /**
     * Get the status describe.
     * @return const LLBC_String & - the status describe.
     */
    const LLBC_String &GetStatusDesc() const;

    /**
     * Set the status describe.
     * @param[in] desc - the status describe.
     */
    void SetStatusDesc(const LLBC_String &desc);
#endif // LLBC_CFG_COMM_ENABLE_STATUS_DESC

    /**
     * Get service Id.
     * @return int - service Id.
     */
    int GetServiceId() const;
    /**
     * Set service Id.
     * @param[in] serviceId - service Id.
     */
    void SetServiceId(int serviceId);

    /**
     * Get packet flags.
     * @return int - the packet flags.
     */
    int GetFlags() const;
    /**
     * Set packet flags.
     * @param[in] flags - the packet falgs.
     */
    void SetFlags(int flags);

    /**
     * Check this packet already set specified flags or not.
     * @param[in] flags - the packet flags.
     * @return bool - return true if has specified flags, otherwise return return false.
     */
    bool HasFlags(int flags) const;

    /**
     * Add flags to packet.
     * @param[in] flags - the packet flags.
     */
    void AddFlags(int flags);

    /**
     * Remove flags from packet.
     * @param[in] flags - the packet flags.
     */
    void RemoveFlags(int flags);

public:
    /**
     * Get special header part value APIs.
     * @param[in] serialNo - the serialNo.
     * @return T - the value.
     */
    sint8 GetHeaderPartAsSInt8(int serialNo) const;
    uint8 GetHeaderPartAsUInt8(int serialNo) const;
    sint16 GetHeaderPartAsSInt16(int serialNo) const;
    uint16 GetHeaderPartAsUInt16(int serialNo) const;
    sint32 GetHeaderPartAsSInt32(int serialNo) const;
    uint32 GetHeaderPartAsUInt32(int serialNo) const;
    sint64 GetHeaderPartAsSInt64(int serialNo) const;
    uint64 GetHeaderPartAsUInt64(int serialNo) const;
    float GetHeaderPartAsFloat(int serialNo) const;
    double GetHeaderPartAsDouble(int serialNo) const;
    LLBC_String GetHeaderPartAsStr(int serialNo) const;
    
    const void *GetHeaderPartVal(int serialNo) const;
    size_t GetHeaderPartLen(int serialNo) const;

public:
    /**
     * Set special header part value APIs.
     * @param[in] serialNo - the serialNo.
     * @param[in] val      - the will set part value.
     * @return int - return 0 if success, otherwise return -1.
     */
    template <typename _Ty>
    int SetHeaderPartVal(int serialNo, const _Ty &val);
    int SetHeaderPartVal(int serialNo, const void *data, size_t len);

public:
    /**
     * Get session Id.
     * @return int - the session Id.
     */
    int GetSessionId() const;

    /**
     * Set session Id.
     * @param[in] sessionId - the session Id.
     */
    void SetSessionId(int sessionId);

    /**
     * Get local address.
     * @return const LLBC_SockAddr_IN & - the local address.
     */
    const LLBC_SockAddr_IN &GetLocalAddr() const;
    /**
     * Set local address.
     * @param[in] addr - the local address.
     */
    void SetLocalAddr(const LLBC_SockAddr_IN &addr);

    /**
     * Get peer address.
     * @return const LLBC_SockAddr_IN & - the peer address.
     */
    const LLBC_SockAddr_IN &GetPeerAddr() const;
    
    /**
     * Set peer address.
     * @param[in] addr - the peer address.
     */
    void SetPeerAddr(const LLBC_SockAddr_IN &addr);

    /**
     * Set packet header.
     * @param[in] svcId     - service Id.
     * @param[in] sessionId - session Id.
     * @param[in] opcode    - the opcode.
     * @param[in] status    - the status code.
     */
    void SetHeader(int sessionId, int opcode, int status);
    void SetHeader(int svcId, int sessionId, int opcode, int status);

    /**
     * Set packet header.
     * @param[in] packet - packet.
     * @param[in] opcode - the opcode.
     * @param[in] status - the status code.
     */
    void SetHeader(const LLBC_Packet &packet, int opcode, int status);

public:
    /**
     * Raw data type read methods.
     * @param[out] val - storage location for data.
     * @return int - return 0 if success, otherwise return -1.
     */
    int Read(bool &val);
    int Read(sint8 &val);
    int Read(uint8 &val);
    int Read(sint16 &val);
    int Read(uint16 &val);
    int Read(sint32 &val);
    int Read(uint32 &val);
    int Read(long &val);
    int Read(ulong &val);
    int Read(sint64 &val);
    int Read(uint64 &val);
    int Read(float &val);
    int Read(double &val);

    /**
     * Read data from packet.
     * @param[out] buf - storage location for data.
     * @param[in]  len - buffer len.
     * @return int - return 0 if success, otherwise return -1.
     */
    int Read(void *buf, size_t len);

    /**
     * STL container adapt read functions.
     * @param[out] val - container.
     * @return int - return 0 if success, otherwise return -1.
     */
    template <typename _Ty>
    int Read(std::vector<_Ty> &val);
    template <typename _Ty>
    int Read(std::list<_Ty> &val);
    template <typename _Ty>
    int Read(std::deque<_Ty> &val);
    template <typename _Kty>
    int Read(std::set<_Kty> &val);
    template <typename _Kty, typename _Ty>
    int Read(std::map<_Kty, _Ty> &val);

    /**
     * Read object from packet.
     * @param[out] val - the object reference.
     * @return int - return 0 if success, otherwise return -1.
     */
    template <typename _Ty>
    int Read(_Ty &val);

    /**
     * Raw data type write methods.
     * @param[in] val - will write to packet's object.
     * @return int - return 0 if success, otherwise retrun -1.
     */
    int Write(bool val);
    int Write(sint8 val);
    int Write(uint8 val);
    int Write(sint16 val);
    int Write(uint16 val);
    int Write(sint32 val);
    int Write(uint32 val);
    int Write(long val);
    int Write(ulong val);
    int Write(sint64 val);
    int Write(uint64 val);
    int Write(float val);
    int Write(double val);
    int Write(const char *val);

    /**
     * Write data to packet.
     * @param[in] buf - buffer to write from.
     * @param[in] len - number of bytes to write.
     * @return int - return 0 if success, otherwise return -1.
     */
    int Write(const void *buf, size_t len);

    /**
     * Write stream type data to packet.
     * @param[in] stream - stream.
     * @return int - return 0 if success, otherwise return -1.
     */
    int Write(const LLBC_Stream &stream);

    /**
     * STL container adapt write functions.
     * @param[in] val - container.
     * @return int - return 0 if success, otherwise return -1.
     */
    template <typename _Ty>
    int Write(const std::vector<_Ty> &val);
    template <typename _Ty>
    int Write(const std::list<_Ty> &val);
    template <typename _Ty>
    int Write(const std::deque<_Ty> &val);
    template <typename _Kty>
    int Write(const std::set<_Kty> &val);
    template <typename _Kty, typename _Ty>
    int Write(const std::map<_Kty, _Ty> &val);

    /**
     * Write object to packet.
     * @param[in] obj - the object.
     * @return int - return 0 if success, otherwise return -1.
     */
    template <typename _Ty>
    int Write(const _Ty &obj);

    /**
     * Write header(unsafe method).
     * @param[in] buf - buffer.
     * @return int - return 0 if success, otherwise return -1.
     */
     int WriteHeader(const void *buf);

    /**
     * Get payload data.
     * @return void * - payload data pointer.
     */
    void *GetPayload() const;

    /**
     * Get payload length.
     * @return size_t - payload length.
     */
    size_t GetPayloadLength() const;

public:
    /**
     * Get encoder.
     * @return LLBC_ICoder * - encoder.
     */
    LLBC_ICoder *GetEncoder() const;

    /**
     * Set encoder.
     * @param[in] encoder - encoder.
     */
    void SetEncoder(LLBC_ICoder *encoder);

    /**
     * Get decoder.
     * @return LLBC_ICoder * - decoder.
     */
    LLBC_ICoder *GetDecoder() const;

    /**
     * Set decoder.
     * @param[in] decoder - decoder.
     */
    void SetDecoder(LLBC_ICoder *decoder);

    /**
     * Get the pre-handle result in the packet.
     * @return void * - the packet pre-handle result.
     */
    void *GetPreHandleResult() const;

    /**
     * Set the pre-handle result into the packet.
     * @param[in] result      - the packet pre-handle result.
     * @param[in] obj         - the clear method target, default is NULL.
     * @param[in] clearMethod - the clear method, default is NULL.
     */
    template <typename Obj>
    void SetPreHandleResult(void *result, Obj *obj = NULL, void (Obj::*clearMethod)(void *) = NULL);

public:
    /**
     * Giveup the message block.
     * @return LLBC_MessageBlock * - message block.
     */
    LLBC_MessageBlock *GiveUp();

public:
    /**
     * stream output operations.
     */
    template <typename _Ty>
    LLBC_Packet &operator <<(const _Ty &val);

    /**
     * Stream input operations.
     */
    template <typename _Ty>
    LLBC_Packet &operator >>(_Ty &val);

public:
    /**
     * Encode packet data.
     */
    bool Encode();

    /**
     * Decode packet data.
     */
    bool Decode();

    /**
     * Get packet codec error.
     * @return const LLBC_String & - the packet codec error.
     */
    const LLBC_String &GetCodecError() const;

    /**
     * Set packet codec error.
     * @param[in] codecErr - the codec error.
     */
    void SetCodecError(const LLBC_String &codecErr);

private:
    /**
     * Raw get header part value from packet.
     * @param[in] serialNo - the serial number.
     * @param[out] val     - the output value.
     */
    template <typename _RawTy>
    void RawGetFloatTypeHeaderPartVal(int serialNo, _RawTy &val) const;
    template <typename _RawTy>
    void RawGetNonFloatTypeHeaderPartVal(int serialNo, _RawTy &val) const;
    /**
     * Raw get header part value from packet.
     * @param[in] buf    - part begin buffer.
     * @param[in] bufLen - part buffer length.
     * @param[out] val   - the output value.
     */
    template <typename _RawTy>
    void RawGetFloatTypeHeaderPartVal(const char *buf, size_t bufLen, _RawTy &val) const;
    template <typename _RawTy>
    void RawGetNonFloatTypeHeaderPartVal(const char *buf, size_t bufLen, _RawTy &val) const;

    /**
     * Raw set header part value to packet.
     * @param[in] serialNo - the serial number.
     * @param[in] val      - will set raw value.
     * @return int - return 0 if success, otherwise return -1.
     */
    template <typename _RawTy>
    int RawSetFloatTypeHeaderPartVal(int serialNo, const _RawTy &val);
    template <typename _RawTy>
    int RawSetNonFloatTypeHeaderPartVal(int serialNo, const _RawTy &val);

    /**
     * Raw set header part value to packet.
     * @param[in] buf    - part buffer begin.
     * @param[in] bufLen - part buffer length.
     * @param[in] val    - will set raw value.
     */
    template <typename _RawTy>
    void RawSetFloatTypeHeaderPartVal(char *buf, size_t bufLen, const _RawTy &val);
    template <typename _RawTy>
    void RawSetNonFloatTypeHeaderPartVal(char *buf, size_t bufLen, const _RawTy &val);

    /**
     * Read raw type data from packet.
     * @param[out] val - the value reference.
     * @return int - return 0 if success, otherwise return -1.
     */
    template <typename _RawTy>
    int ReadRawType(_RawTy &val);

    /**
     * Write raw type data to packet.
     * @param[in] val - value.
     * @return int - return 0 if success, otherwise return -1.
     */
    template <typename _RawTy>
    int WriteRawType(_RawTy val);

    /**
     * Cleanup the pre-handle result data.
     */
    void CleanupPreHandleResult();

private:
    const LLBC_PacketHeaderDesc *_headerDesc;
    const size_t _lenSize;
    const size_t _lenOffset;

private:
    int _sessionId;
    LLBC_SockAddr_IN _localAddr;
    LLBC_SockAddr_IN _peerAddr;

    LLBC_ICoder *_encoder;
    LLBC_ICoder *_decoder;
#if LLBC_CFG_COMM_ENABLE_STATUS_DESC
    LLBC_String *_statusDesc;
#endif // LLBC_CFG_COMM_ENABLE_STATUS_DESC

    void *_preHandleResult;
    LLBC_IDelegate1<void *> *_resultClearDeleg;

    LLBC_MessageBlock *_block;
    LLBC_String *_codecError;
};

__LLBC_NS_END

#include "llbc/comm/PacketImpl.h"

#endif // !__LLBC_COMM_PACKET_H__
