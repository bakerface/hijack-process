/**
 * Copyright (c) 2017 Chris Baker <mail.chris.baker@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <node.h>

#ifdef __APPLE__
#include "apple.h"
#endif

namespace process {
  using v8::Array;
  using v8::Exception;
  using v8::FunctionCallbackInfo;
  using v8::Handle;
  using v8::Integer;
  using v8::Isolate;
  using v8::Local;
  using v8::Object;
  using v8::String;
  using v8::Value;

  void process_open(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = args.GetIsolate();

    if (args.Length() < 1) {
      isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Expected process id as argument")));
    }
    else if (!args[0]->IsNumber()) {
      isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Expected process id to be a number")));
    }
    else {
      platform_handle_t handle;

      if (platform_process_open(args[0]->IntegerValue(), &handle) != PLATFORM_SUCCESS) {
        isolate->ThrowException(Exception::Error(
          String::NewFromUtf8(isolate, "Unable to open process")));
      }
      else {
        Local<Integer> result = Integer::New(isolate, handle);
        args.GetReturnValue().Set(result);
      }
    }
  }

  void process_read(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = args.GetIsolate();

    if (args.Length() < 3) {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate,
        "Expected handle, address, and size as arguments")));
    }
    else if (!args[0]->IsNumber()) {
      isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Expected process handle to be a number")));
    }
    else if (!args[1]->IsNumber()) {
      isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Expected address to be a number")));
    }
    else if (!args[2]->IsNumber()) {
      isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Expected size to be a number")));
    }
    else {
      platform_handle_t handle = args[0]->IntegerValue();
      platform_address_t address = args[1]->IntegerValue();
      platform_size_t size = args[2]->IntegerValue();
      platform_size_t count = size;
      uint8_t *buffer = (uint8_t *) malloc(size);

      if (platform_process_read(handle, address, size, (platform_ptr_t) buffer, &count) != KERN_SUCCESS) {
        free(buffer);

        isolate->ThrowException(Exception::Error(
          String::NewFromUtf8(isolate, "Unable to read process")));
      }
      else {
        Local<Array> result = Array::New(isolate, count);

        for (platform_size_t i = 0; i < count; i++) {
          result->Set(i, Integer::NewFromUnsigned(isolate, buffer[i] & 0xff));
        }

        args.GetReturnValue().Set(result);
      }
    }
  }

  void process_write(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = args.GetIsolate();

    if (args.Length() < 3) {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate,
        "Expected handle, address, and bytes as arguments")));
    }
    else if (!args[0]->IsNumber()) {
      isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Expected process handle to be a number")));
    }
    else if (!args[1]->IsNumber()) {
      isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Expected address to be a number")));
    }
    else if (!args[2]->IsArray()) {
      isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Expected bytes to be an array")));
    }
    else {
      platform_handle_t handle = args[0]->IntegerValue();
      platform_address_t address = args[1]->IntegerValue();
      Handle<Array> array = Handle<Array>::Cast(args[2]);
      platform_size_t length = array->Length();
      uint8_t *data = (uint8_t *) malloc(length);

      for (platform_size_t i = 0; i < length; i++) {
        data[i] = (uint8_t) array->Get(i)->IntegerValue();
      }

      if (platform_process_write(handle, address, (platform_ptr_t) data, length) != KERN_SUCCESS) {
        free(data);

        isolate->ThrowException(Exception::Error(
          String::NewFromUtf8(isolate, "Unable to write process")));
      }
      else {
        free(data);
      }
    }
  }

  void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "open", process_open);
    NODE_SET_METHOD(exports, "read", process_read);
    NODE_SET_METHOD(exports, "write", process_write);
  }

  NODE_MODULE(process, init)
};
