// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: LuaModule.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "LuaModule.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
// @@protoc_insertion_point(includes)

namespace main {

void protobuf_ShutdownFile_LuaModule_2eproto() {
  delete LuaCommonRequest::default_instance_;
  delete LuaCommonResponse::default_instance_;
  delete LuaTestRequest::default_instance_;
  delete LuaTestResponse::default_instance_;
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
void protobuf_AddDesc_LuaModule_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#else
void protobuf_AddDesc_LuaModule_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#endif
  LuaCommonRequest::default_instance_ = new LuaCommonRequest();
  LuaCommonResponse::default_instance_ = new LuaCommonResponse();
  LuaTestRequest::default_instance_ = new LuaTestRequest();
  LuaTestResponse::default_instance_ = new LuaTestResponse();
  LuaCommonRequest::default_instance_->InitAsDefaultInstance();
  LuaCommonResponse::default_instance_->InitAsDefaultInstance();
  LuaTestRequest::default_instance_->InitAsDefaultInstance();
  LuaTestResponse::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_LuaModule_2eproto);
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_LuaModule_2eproto_once_);
void protobuf_AddDesc_LuaModule_2eproto() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_LuaModule_2eproto_once_,
                 &protobuf_AddDesc_LuaModule_2eproto_impl);
}
#else
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_LuaModule_2eproto {
  StaticDescriptorInitializer_LuaModule_2eproto() {
    protobuf_AddDesc_LuaModule_2eproto();
  }
} static_descriptor_initializer_LuaModule_2eproto_;
#endif

// ===================================================================

#ifndef _MSC_VER
const int LuaCommonRequest::kCmdFieldNumber;
const int LuaCommonRequest::kRequestFieldNumber;
#endif  // !_MSC_VER

LuaCommonRequest::LuaCommonRequest()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:main.LuaCommonRequest)
}

void LuaCommonRequest::InitAsDefaultInstance() {
}

LuaCommonRequest::LuaCommonRequest(const LuaCommonRequest& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:main.LuaCommonRequest)
}

void LuaCommonRequest::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  cmd_ = 0;
  request_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

LuaCommonRequest::~LuaCommonRequest() {
  // @@protoc_insertion_point(destructor:main.LuaCommonRequest)
  SharedDtor();
}

void LuaCommonRequest::SharedDtor() {
  if (request_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete request_;
  }
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void LuaCommonRequest::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const LuaCommonRequest& LuaCommonRequest::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_LuaModule_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_LuaModule_2eproto();
#endif
  return *default_instance_;
}

LuaCommonRequest* LuaCommonRequest::default_instance_ = NULL;

LuaCommonRequest* LuaCommonRequest::New() const {
  return new LuaCommonRequest;
}

void LuaCommonRequest::Clear() {
  if (_has_bits_[0 / 32] & 3) {
    cmd_ = 0;
    if (has_request()) {
      if (request_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        request_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->clear();
}

bool LuaCommonRequest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  ::google::protobuf::io::StringOutputStream unknown_fields_string(
      mutable_unknown_fields());
  ::google::protobuf::io::CodedOutputStream unknown_fields_stream(
      &unknown_fields_string);
  // @@protoc_insertion_point(parse_start:main.LuaCommonRequest)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 cmd = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &cmd_)));
          set_has_cmd();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_request;
        break;
      }

      // optional bytes request = 2;
      case 2: {
        if (tag == 18) {
         parse_request:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_request()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(
            input, tag, &unknown_fields_stream));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:main.LuaCommonRequest)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:main.LuaCommonRequest)
  return false;
#undef DO_
}

void LuaCommonRequest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:main.LuaCommonRequest)
  // required int32 cmd = 1;
  if (has_cmd()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->cmd(), output);
  }

  // optional bytes request = 2;
  if (has_request()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytesMaybeAliased(
      2, this->request(), output);
  }

  output->WriteRaw(unknown_fields().data(),
                   unknown_fields().size());
  // @@protoc_insertion_point(serialize_end:main.LuaCommonRequest)
}

int LuaCommonRequest::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 cmd = 1;
    if (has_cmd()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->cmd());
    }

    // optional bytes request = 2;
    if (has_request()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->request());
    }

  }
  total_size += unknown_fields().size();

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void LuaCommonRequest::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const LuaCommonRequest*>(&from));
}

void LuaCommonRequest::MergeFrom(const LuaCommonRequest& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_cmd()) {
      set_cmd(from.cmd());
    }
    if (from.has_request()) {
      set_request(from.request());
    }
  }
  mutable_unknown_fields()->append(from.unknown_fields());
}

void LuaCommonRequest::CopyFrom(const LuaCommonRequest& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool LuaCommonRequest::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void LuaCommonRequest::Swap(LuaCommonRequest* other) {
  if (other != this) {
    std::swap(cmd_, other->cmd_);
    std::swap(request_, other->request_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.swap(other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string LuaCommonRequest::GetTypeName() const {
  return "main.LuaCommonRequest";
}


// ===================================================================

#ifndef _MSC_VER
const int LuaCommonResponse::kCmdFieldNumber;
const int LuaCommonResponse::kResponseFieldNumber;
#endif  // !_MSC_VER

LuaCommonResponse::LuaCommonResponse()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:main.LuaCommonResponse)
}

void LuaCommonResponse::InitAsDefaultInstance() {
}

LuaCommonResponse::LuaCommonResponse(const LuaCommonResponse& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:main.LuaCommonResponse)
}

void LuaCommonResponse::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  cmd_ = 0;
  response_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

LuaCommonResponse::~LuaCommonResponse() {
  // @@protoc_insertion_point(destructor:main.LuaCommonResponse)
  SharedDtor();
}

void LuaCommonResponse::SharedDtor() {
  if (response_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete response_;
  }
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void LuaCommonResponse::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const LuaCommonResponse& LuaCommonResponse::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_LuaModule_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_LuaModule_2eproto();
#endif
  return *default_instance_;
}

LuaCommonResponse* LuaCommonResponse::default_instance_ = NULL;

LuaCommonResponse* LuaCommonResponse::New() const {
  return new LuaCommonResponse;
}

void LuaCommonResponse::Clear() {
  if (_has_bits_[0 / 32] & 3) {
    cmd_ = 0;
    if (has_response()) {
      if (response_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        response_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->clear();
}

bool LuaCommonResponse::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  ::google::protobuf::io::StringOutputStream unknown_fields_string(
      mutable_unknown_fields());
  ::google::protobuf::io::CodedOutputStream unknown_fields_stream(
      &unknown_fields_string);
  // @@protoc_insertion_point(parse_start:main.LuaCommonResponse)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 cmd = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &cmd_)));
          set_has_cmd();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_response;
        break;
      }

      // optional bytes response = 2;
      case 2: {
        if (tag == 18) {
         parse_response:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_response()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(
            input, tag, &unknown_fields_stream));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:main.LuaCommonResponse)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:main.LuaCommonResponse)
  return false;
#undef DO_
}

void LuaCommonResponse::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:main.LuaCommonResponse)
  // required int32 cmd = 1;
  if (has_cmd()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->cmd(), output);
  }

  // optional bytes response = 2;
  if (has_response()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytesMaybeAliased(
      2, this->response(), output);
  }

  output->WriteRaw(unknown_fields().data(),
                   unknown_fields().size());
  // @@protoc_insertion_point(serialize_end:main.LuaCommonResponse)
}

int LuaCommonResponse::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 cmd = 1;
    if (has_cmd()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->cmd());
    }

    // optional bytes response = 2;
    if (has_response()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->response());
    }

  }
  total_size += unknown_fields().size();

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void LuaCommonResponse::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const LuaCommonResponse*>(&from));
}

void LuaCommonResponse::MergeFrom(const LuaCommonResponse& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_cmd()) {
      set_cmd(from.cmd());
    }
    if (from.has_response()) {
      set_response(from.response());
    }
  }
  mutable_unknown_fields()->append(from.unknown_fields());
}

void LuaCommonResponse::CopyFrom(const LuaCommonResponse& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool LuaCommonResponse::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void LuaCommonResponse::Swap(LuaCommonResponse* other) {
  if (other != this) {
    std::swap(cmd_, other->cmd_);
    std::swap(response_, other->response_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.swap(other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string LuaCommonResponse::GetTypeName() const {
  return "main.LuaCommonResponse";
}


// ===================================================================

#ifndef _MSC_VER
const int LuaTestRequest::kCmdIdFieldNumber;
const int LuaTestRequest::kInfoFieldNumber;
#endif  // !_MSC_VER

LuaTestRequest::LuaTestRequest()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:main.LuaTestRequest)
}

void LuaTestRequest::InitAsDefaultInstance() {
}

LuaTestRequest::LuaTestRequest(const LuaTestRequest& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:main.LuaTestRequest)
}

void LuaTestRequest::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  cmdid_ = 0;
  info_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

LuaTestRequest::~LuaTestRequest() {
  // @@protoc_insertion_point(destructor:main.LuaTestRequest)
  SharedDtor();
}

void LuaTestRequest::SharedDtor() {
  if (info_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete info_;
  }
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void LuaTestRequest::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const LuaTestRequest& LuaTestRequest::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_LuaModule_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_LuaModule_2eproto();
#endif
  return *default_instance_;
}

LuaTestRequest* LuaTestRequest::default_instance_ = NULL;

LuaTestRequest* LuaTestRequest::New() const {
  return new LuaTestRequest;
}

void LuaTestRequest::Clear() {
  if (_has_bits_[0 / 32] & 3) {
    cmdid_ = 0;
    if (has_info()) {
      if (info_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        info_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->clear();
}

bool LuaTestRequest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  ::google::protobuf::io::StringOutputStream unknown_fields_string(
      mutable_unknown_fields());
  ::google::protobuf::io::CodedOutputStream unknown_fields_stream(
      &unknown_fields_string);
  // @@protoc_insertion_point(parse_start:main.LuaTestRequest)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int32 cmdId = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &cmdid_)));
          set_has_cmdid();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_info;
        break;
      }

      // optional string info = 2;
      case 2: {
        if (tag == 18) {
         parse_info:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_info()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(
            input, tag, &unknown_fields_stream));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:main.LuaTestRequest)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:main.LuaTestRequest)
  return false;
#undef DO_
}

void LuaTestRequest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:main.LuaTestRequest)
  // optional int32 cmdId = 1;
  if (has_cmdid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->cmdid(), output);
  }

  // optional string info = 2;
  if (has_info()) {
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      2, this->info(), output);
  }

  output->WriteRaw(unknown_fields().data(),
                   unknown_fields().size());
  // @@protoc_insertion_point(serialize_end:main.LuaTestRequest)
}

int LuaTestRequest::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int32 cmdId = 1;
    if (has_cmdid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->cmdid());
    }

    // optional string info = 2;
    if (has_info()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->info());
    }

  }
  total_size += unknown_fields().size();

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void LuaTestRequest::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const LuaTestRequest*>(&from));
}

void LuaTestRequest::MergeFrom(const LuaTestRequest& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_cmdid()) {
      set_cmdid(from.cmdid());
    }
    if (from.has_info()) {
      set_info(from.info());
    }
  }
  mutable_unknown_fields()->append(from.unknown_fields());
}

void LuaTestRequest::CopyFrom(const LuaTestRequest& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool LuaTestRequest::IsInitialized() const {

  return true;
}

void LuaTestRequest::Swap(LuaTestRequest* other) {
  if (other != this) {
    std::swap(cmdid_, other->cmdid_);
    std::swap(info_, other->info_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.swap(other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string LuaTestRequest::GetTypeName() const {
  return "main.LuaTestRequest";
}


// ===================================================================

#ifndef _MSC_VER
const int LuaTestResponse::kCmdIdFieldNumber;
const int LuaTestResponse::kInfoFieldNumber;
#endif  // !_MSC_VER

LuaTestResponse::LuaTestResponse()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:main.LuaTestResponse)
}

void LuaTestResponse::InitAsDefaultInstance() {
}

LuaTestResponse::LuaTestResponse(const LuaTestResponse& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:main.LuaTestResponse)
}

void LuaTestResponse::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  cmdid_ = 0;
  info_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

LuaTestResponse::~LuaTestResponse() {
  // @@protoc_insertion_point(destructor:main.LuaTestResponse)
  SharedDtor();
}

void LuaTestResponse::SharedDtor() {
  if (info_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete info_;
  }
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void LuaTestResponse::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const LuaTestResponse& LuaTestResponse::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_LuaModule_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_LuaModule_2eproto();
#endif
  return *default_instance_;
}

LuaTestResponse* LuaTestResponse::default_instance_ = NULL;

LuaTestResponse* LuaTestResponse::New() const {
  return new LuaTestResponse;
}

void LuaTestResponse::Clear() {
  if (_has_bits_[0 / 32] & 3) {
    cmdid_ = 0;
    if (has_info()) {
      if (info_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        info_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->clear();
}

bool LuaTestResponse::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  ::google::protobuf::io::StringOutputStream unknown_fields_string(
      mutable_unknown_fields());
  ::google::protobuf::io::CodedOutputStream unknown_fields_stream(
      &unknown_fields_string);
  // @@protoc_insertion_point(parse_start:main.LuaTestResponse)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int32 cmdId = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &cmdid_)));
          set_has_cmdid();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_info;
        break;
      }

      // optional string info = 2;
      case 2: {
        if (tag == 18) {
         parse_info:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_info()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(
            input, tag, &unknown_fields_stream));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:main.LuaTestResponse)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:main.LuaTestResponse)
  return false;
#undef DO_
}

void LuaTestResponse::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:main.LuaTestResponse)
  // optional int32 cmdId = 1;
  if (has_cmdid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->cmdid(), output);
  }

  // optional string info = 2;
  if (has_info()) {
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      2, this->info(), output);
  }

  output->WriteRaw(unknown_fields().data(),
                   unknown_fields().size());
  // @@protoc_insertion_point(serialize_end:main.LuaTestResponse)
}

int LuaTestResponse::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int32 cmdId = 1;
    if (has_cmdid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->cmdid());
    }

    // optional string info = 2;
    if (has_info()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->info());
    }

  }
  total_size += unknown_fields().size();

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void LuaTestResponse::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const LuaTestResponse*>(&from));
}

void LuaTestResponse::MergeFrom(const LuaTestResponse& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_cmdid()) {
      set_cmdid(from.cmdid());
    }
    if (from.has_info()) {
      set_info(from.info());
    }
  }
  mutable_unknown_fields()->append(from.unknown_fields());
}

void LuaTestResponse::CopyFrom(const LuaTestResponse& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool LuaTestResponse::IsInitialized() const {

  return true;
}

void LuaTestResponse::Swap(LuaTestResponse* other) {
  if (other != this) {
    std::swap(cmdid_, other->cmdid_);
    std::swap(info_, other->info_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.swap(other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string LuaTestResponse::GetTypeName() const {
  return "main.LuaTestResponse";
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace main

// @@protoc_insertion_point(global_scope)
