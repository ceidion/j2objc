// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Author: tball@google.com (Tom Ball)
//  Based on original Protocol Buffers design by
//  Sanjay Ghemawat, Jeff Dean, Kenton Varda, and others.

#ifndef GOOGLE_PROTOBUF_COMPILER_J2OBJC_FIELD_H__
#define GOOGLE_PROTOBUF_COMPILER_J2OBJC_FIELD_H__

#include <map>
#include <memory>
#include <set>
#include <string>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/descriptor.h>

namespace google {
namespace protobuf {
  namespace io {
    class Printer;             // printer.h
  }
}

namespace protobuf {
namespace compiler {
namespace j2objc {

class FieldGenerator {
 public:
  FieldGenerator(const FieldDescriptor *descriptor);
  virtual ~FieldGenerator();

  virtual void GenerateFieldHeader(io::Printer* printer) const;
  virtual void GenerateFieldBuilderHeader(io::Printer* printer) const = 0;

  virtual void GenerateMessageOrBuilderProtocol(io::Printer* printer) const = 0;

  virtual void GenerateDeclaration(io::Printer *printer) const = 0;
  virtual void GenerateFieldData(io::Printer *printer) const;

  virtual void CollectForwardDeclarations(set<string> &declarations) const;
  virtual void CollectMessageOrBuilderForwardDeclarations(
      set<string> &declarations) const;
  virtual void CollectSourceImports(set<string> &imports) const;

 protected:
  const FieldDescriptor* descriptor_;
  map<string, string> variables_;

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(FieldGenerator);
};

class SingleFieldGenerator : public FieldGenerator {
 public:
  SingleFieldGenerator(const FieldDescriptor *descriptor)
      : FieldGenerator(descriptor) {
  }

  virtual ~SingleFieldGenerator() { }

  virtual void GenerateFieldBuilderHeader(io::Printer* printer) const;

  virtual void GenerateMessageOrBuilderProtocol(io::Printer* printer) const;

  virtual void GenerateDeclaration(io::Printer* printer) const;

  virtual void CollectSourceImports(set<string> &imports) const;

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(SingleFieldGenerator);
};

class RepeatedFieldGenerator : public FieldGenerator {
 public:
  RepeatedFieldGenerator(const FieldDescriptor *descriptor)
      : FieldGenerator(descriptor) {
  }

  virtual ~RepeatedFieldGenerator() { }

  virtual void GenerateFieldBuilderHeader(io::Printer* printer) const;

  virtual void GenerateMessageOrBuilderProtocol(io::Printer* printer) const;

  virtual void GenerateDeclaration(io::Printer* printer) const;

  virtual void CollectForwardDeclarations(set<string> &declarations) const;
  virtual void CollectMessageOrBuilderForwardDeclarations(
      set<string> &declarations) const;

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(RepeatedFieldGenerator);
};

// Convenience class which constructs FieldGenerators for a Descriptor.
class FieldGeneratorMap {
 public:
  explicit FieldGeneratorMap(const Descriptor* descriptor);
  ~FieldGeneratorMap();

  const FieldGenerator& get(const FieldDescriptor* field) const;

 private:
  const Descriptor* descriptor_;
  scoped_array<scoped_ptr<FieldGenerator> > field_generators_;

  static FieldGenerator* MakeGenerator(const FieldDescriptor* field);

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(FieldGeneratorMap);
};

}  // namespace j2objc
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

#endif  // GOOGLE_PROTOBUF_COMPILER_J2OBJC_FIELD_H__