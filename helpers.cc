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

char buffer[] = { 0, 1, 2, 3, 4, 5, 6, 7 };

namespace helpers {
  using v8::FunctionCallbackInfo;
  using v8::Number;
  using v8::Isolate;
  using v8::Local;
  using v8::Object;
  using v8::Value;

  void get_buffer_address(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = args.GetIsolate();
    Local<Number> handle = Number::New(isolate, (uint64_t) buffer);
    args.GetReturnValue().Set(handle);
  }

  void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "getBufferAddress", get_buffer_address);
  }

  NODE_MODULE(helpers, init)
};
