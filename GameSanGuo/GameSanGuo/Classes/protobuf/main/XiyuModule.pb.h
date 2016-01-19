// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: XiyuModule.proto

#ifndef PROTOBUF_XiyuModule_2eproto__INCLUDED
#define PROTOBUF_XiyuModule_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include "ItemModule.pb.h"
// @@protoc_insertion_point(includes)

namespace main {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_XiyuModule_2eproto();
void protobuf_AssignDesc_XiyuModule_2eproto();
void protobuf_ShutdownFile_XiyuModule_2eproto();

class XiyuInfoRequest;
class XiyuInfoResponse;
class XiyuBuyRequest;
class XiyuBuyResponse;

// ===================================================================

class XiyuInfoRequest : public ::google::protobuf::MessageLite {
 public:
  XiyuInfoRequest();
  virtual ~XiyuInfoRequest();

  XiyuInfoRequest(const XiyuInfoRequest& from);

  inline XiyuInfoRequest& operator=(const XiyuInfoRequest& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::std::string& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::std::string* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const XiyuInfoRequest& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const XiyuInfoRequest* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(XiyuInfoRequest* other);

  // implements Message ----------------------------------------------

  XiyuInfoRequest* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const XiyuInfoRequest& from);
  void MergeFrom(const XiyuInfoRequest& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  void DiscardUnknownFields();
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // @@protoc_insertion_point(class_scope:main.XiyuInfoRequest)
 private:

  ::std::string _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_XiyuModule_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_XiyuModule_2eproto();
  #endif
  friend void protobuf_AssignDesc_XiyuModule_2eproto();
  friend void protobuf_ShutdownFile_XiyuModule_2eproto();

  void InitAsDefaultInstance();
  static XiyuInfoRequest* default_instance_;
};
// -------------------------------------------------------------------

class XiyuInfoResponse : public ::google::protobuf::MessageLite {
 public:
  XiyuInfoResponse();
  virtual ~XiyuInfoResponse();

  XiyuInfoResponse(const XiyuInfoResponse& from);

  inline XiyuInfoResponse& operator=(const XiyuInfoResponse& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::std::string& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::std::string* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const XiyuInfoResponse& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const XiyuInfoResponse* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(XiyuInfoResponse* other);

  // implements Message ----------------------------------------------

  XiyuInfoResponse* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const XiyuInfoResponse& from);
  void MergeFrom(const XiyuInfoResponse& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  void DiscardUnknownFields();
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 state = 1;
  inline bool has_state() const;
  inline void clear_state();
  static const int kStateFieldNumber = 1;
  inline ::google::protobuf::int32 state() const;
  inline void set_state(::google::protobuf::int32 value);

  // optional int32 time = 2;
  inline bool has_time() const;
  inline void clear_time();
  static const int kTimeFieldNumber = 2;
  inline ::google::protobuf::int32 time() const;
  inline void set_time(::google::protobuf::int32 value);

  // repeated .main.GoodsDataProto goods = 3;
  inline int goods_size() const;
  inline void clear_goods();
  static const int kGoodsFieldNumber = 3;
  inline const ::main::GoodsDataProto& goods(int index) const;
  inline ::main::GoodsDataProto* mutable_goods(int index);
  inline ::main::GoodsDataProto* add_goods();
  inline const ::google::protobuf::RepeatedPtrField< ::main::GoodsDataProto >&
      goods() const;
  inline ::google::protobuf::RepeatedPtrField< ::main::GoodsDataProto >*
      mutable_goods();

  // @@protoc_insertion_point(class_scope:main.XiyuInfoResponse)
 private:
  inline void set_has_state();
  inline void clear_has_state();
  inline void set_has_time();
  inline void clear_has_time();

  ::std::string _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int32 state_;
  ::google::protobuf::int32 time_;
  ::google::protobuf::RepeatedPtrField< ::main::GoodsDataProto > goods_;
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_XiyuModule_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_XiyuModule_2eproto();
  #endif
  friend void protobuf_AssignDesc_XiyuModule_2eproto();
  friend void protobuf_ShutdownFile_XiyuModule_2eproto();

  void InitAsDefaultInstance();
  static XiyuInfoResponse* default_instance_;
};
// -------------------------------------------------------------------

class XiyuBuyRequest : public ::google::protobuf::MessageLite {
 public:
  XiyuBuyRequest();
  virtual ~XiyuBuyRequest();

  XiyuBuyRequest(const XiyuBuyRequest& from);

  inline XiyuBuyRequest& operator=(const XiyuBuyRequest& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::std::string& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::std::string* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const XiyuBuyRequest& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const XiyuBuyRequest* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(XiyuBuyRequest* other);

  // implements Message ----------------------------------------------

  XiyuBuyRequest* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const XiyuBuyRequest& from);
  void MergeFrom(const XiyuBuyRequest& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  void DiscardUnknownFields();
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 index = 1;
  inline bool has_index() const;
  inline void clear_index();
  static const int kIndexFieldNumber = 1;
  inline ::google::protobuf::int32 index() const;
  inline void set_index(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:main.XiyuBuyRequest)
 private:
  inline void set_has_index();
  inline void clear_has_index();

  ::std::string _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int32 index_;
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_XiyuModule_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_XiyuModule_2eproto();
  #endif
  friend void protobuf_AssignDesc_XiyuModule_2eproto();
  friend void protobuf_ShutdownFile_XiyuModule_2eproto();

  void InitAsDefaultInstance();
  static XiyuBuyRequest* default_instance_;
};
// -------------------------------------------------------------------

class XiyuBuyResponse : public ::google::protobuf::MessageLite {
 public:
  XiyuBuyResponse();
  virtual ~XiyuBuyResponse();

  XiyuBuyResponse(const XiyuBuyResponse& from);

  inline XiyuBuyResponse& operator=(const XiyuBuyResponse& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::std::string& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::std::string* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const XiyuBuyResponse& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const XiyuBuyResponse* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(XiyuBuyResponse* other);

  // implements Message ----------------------------------------------

  XiyuBuyResponse* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const XiyuBuyResponse& from);
  void MergeFrom(const XiyuBuyResponse& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  void DiscardUnknownFields();
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 state = 1;
  inline bool has_state() const;
  inline void clear_state();
  static const int kStateFieldNumber = 1;
  inline ::google::protobuf::int32 state() const;
  inline void set_state(::google::protobuf::int32 value);

  // optional string hint = 2;
  inline bool has_hint() const;
  inline void clear_hint();
  static const int kHintFieldNumber = 2;
  inline const ::std::string& hint() const;
  inline void set_hint(const ::std::string& value);
  inline void set_hint(const char* value);
  inline void set_hint(const char* value, size_t size);
  inline ::std::string* mutable_hint();
  inline ::std::string* release_hint();
  inline void set_allocated_hint(::std::string* hint);

  // @@protoc_insertion_point(class_scope:main.XiyuBuyResponse)
 private:
  inline void set_has_state();
  inline void clear_has_state();
  inline void set_has_hint();
  inline void clear_has_hint();

  ::std::string _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* hint_;
  ::google::protobuf::int32 state_;
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_XiyuModule_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_XiyuModule_2eproto();
  #endif
  friend void protobuf_AssignDesc_XiyuModule_2eproto();
  friend void protobuf_ShutdownFile_XiyuModule_2eproto();

  void InitAsDefaultInstance();
  static XiyuBuyResponse* default_instance_;
};
// ===================================================================


// ===================================================================

// XiyuInfoRequest

// -------------------------------------------------------------------

// XiyuInfoResponse

// required int32 state = 1;
inline bool XiyuInfoResponse::has_state() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void XiyuInfoResponse::set_has_state() {
  _has_bits_[0] |= 0x00000001u;
}
inline void XiyuInfoResponse::clear_has_state() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void XiyuInfoResponse::clear_state() {
  state_ = 0;
  clear_has_state();
}
inline ::google::protobuf::int32 XiyuInfoResponse::state() const {
  // @@protoc_insertion_point(field_get:main.XiyuInfoResponse.state)
  return state_;
}
inline void XiyuInfoResponse::set_state(::google::protobuf::int32 value) {
  set_has_state();
  state_ = value;
  // @@protoc_insertion_point(field_set:main.XiyuInfoResponse.state)
}

// optional int32 time = 2;
inline bool XiyuInfoResponse::has_time() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void XiyuInfoResponse::set_has_time() {
  _has_bits_[0] |= 0x00000002u;
}
inline void XiyuInfoResponse::clear_has_time() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void XiyuInfoResponse::clear_time() {
  time_ = 0;
  clear_has_time();
}
inline ::google::protobuf::int32 XiyuInfoResponse::time() const {
  // @@protoc_insertion_point(field_get:main.XiyuInfoResponse.time)
  return time_;
}
inline void XiyuInfoResponse::set_time(::google::protobuf::int32 value) {
  set_has_time();
  time_ = value;
  // @@protoc_insertion_point(field_set:main.XiyuInfoResponse.time)
}

// repeated .main.GoodsDataProto goods = 3;
inline int XiyuInfoResponse::goods_size() const {
  return goods_.size();
}
inline void XiyuInfoResponse::clear_goods() {
  goods_.Clear();
}
inline const ::main::GoodsDataProto& XiyuInfoResponse::goods(int index) const {
  // @@protoc_insertion_point(field_get:main.XiyuInfoResponse.goods)
  return goods_.Get(index);
}
inline ::main::GoodsDataProto* XiyuInfoResponse::mutable_goods(int index) {
  // @@protoc_insertion_point(field_mutable:main.XiyuInfoResponse.goods)
  return goods_.Mutable(index);
}
inline ::main::GoodsDataProto* XiyuInfoResponse::add_goods() {
  // @@protoc_insertion_point(field_add:main.XiyuInfoResponse.goods)
  return goods_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::main::GoodsDataProto >&
XiyuInfoResponse::goods() const {
  // @@protoc_insertion_point(field_list:main.XiyuInfoResponse.goods)
  return goods_;
}
inline ::google::protobuf::RepeatedPtrField< ::main::GoodsDataProto >*
XiyuInfoResponse::mutable_goods() {
  // @@protoc_insertion_point(field_mutable_list:main.XiyuInfoResponse.goods)
  return &goods_;
}

// -------------------------------------------------------------------

// XiyuBuyRequest

// required int32 index = 1;
inline bool XiyuBuyRequest::has_index() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void XiyuBuyRequest::set_has_index() {
  _has_bits_[0] |= 0x00000001u;
}
inline void XiyuBuyRequest::clear_has_index() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void XiyuBuyRequest::clear_index() {
  index_ = 0;
  clear_has_index();
}
inline ::google::protobuf::int32 XiyuBuyRequest::index() const {
  // @@protoc_insertion_point(field_get:main.XiyuBuyRequest.index)
  return index_;
}
inline void XiyuBuyRequest::set_index(::google::protobuf::int32 value) {
  set_has_index();
  index_ = value;
  // @@protoc_insertion_point(field_set:main.XiyuBuyRequest.index)
}

// -------------------------------------------------------------------

// XiyuBuyResponse

// optional int32 state = 1;
inline bool XiyuBuyResponse::has_state() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void XiyuBuyResponse::set_has_state() {
  _has_bits_[0] |= 0x00000001u;
}
inline void XiyuBuyResponse::clear_has_state() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void XiyuBuyResponse::clear_state() {
  state_ = 0;
  clear_has_state();
}
inline ::google::protobuf::int32 XiyuBuyResponse::state() const {
  // @@protoc_insertion_point(field_get:main.XiyuBuyResponse.state)
  return state_;
}
inline void XiyuBuyResponse::set_state(::google::protobuf::int32 value) {
  set_has_state();
  state_ = value;
  // @@protoc_insertion_point(field_set:main.XiyuBuyResponse.state)
}

// optional string hint = 2;
inline bool XiyuBuyResponse::has_hint() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void XiyuBuyResponse::set_has_hint() {
  _has_bits_[0] |= 0x00000002u;
}
inline void XiyuBuyResponse::clear_has_hint() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void XiyuBuyResponse::clear_hint() {
  if (hint_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    hint_->clear();
  }
  clear_has_hint();
}
inline const ::std::string& XiyuBuyResponse::hint() const {
  // @@protoc_insertion_point(field_get:main.XiyuBuyResponse.hint)
  return *hint_;
}
inline void XiyuBuyResponse::set_hint(const ::std::string& value) {
  set_has_hint();
  if (hint_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    hint_ = new ::std::string;
  }
  hint_->assign(value);
  // @@protoc_insertion_point(field_set:main.XiyuBuyResponse.hint)
}
inline void XiyuBuyResponse::set_hint(const char* value) {
  set_has_hint();
  if (hint_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    hint_ = new ::std::string;
  }
  hint_->assign(value);
  // @@protoc_insertion_point(field_set_char:main.XiyuBuyResponse.hint)
}
inline void XiyuBuyResponse::set_hint(const char* value, size_t size) {
  set_has_hint();
  if (hint_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    hint_ = new ::std::string;
  }
  hint_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:main.XiyuBuyResponse.hint)
}
inline ::std::string* XiyuBuyResponse::mutable_hint() {
  set_has_hint();
  if (hint_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    hint_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:main.XiyuBuyResponse.hint)
  return hint_;
}
inline ::std::string* XiyuBuyResponse::release_hint() {
  clear_has_hint();
  if (hint_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = hint_;
    hint_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void XiyuBuyResponse::set_allocated_hint(::std::string* hint) {
  if (hint_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete hint_;
  }
  if (hint) {
    set_has_hint();
    hint_ = hint;
  } else {
    clear_has_hint();
    hint_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:main.XiyuBuyResponse.hint)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace main

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_XiyuModule_2eproto__INCLUDED
