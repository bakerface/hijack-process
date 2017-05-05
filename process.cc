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
#include <mach/mach.h>
#include <mach/mach_init.h>
#include <mach/mach_vm.h>

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
      task_t task;
      int pid = args[0]->IntegerValue();

      if (task_for_pid(mach_task_self(), pid, &task)) {
        isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, "Unable to open process")));
      }
      else {
        Local<Integer> handle = Integer::New(isolate, (int64_t) task);
        args.GetReturnValue().Set(handle);
      }
    }
  }

  void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "open", process_open);
  }

  NODE_MODULE(surgeon, init)
};
