#include <node.h>
#include <v8.h>
#include <uv.h>
#include <string>
#include <iostream>
#include <node_object_wrap.h>
#if defined _WIN32
#include <windows.h>
#else

#include <unistd.h>

#endif
#include "bot.h"
#include "bot.c"
#include "irc.c"
#include "socket.c"
static v8::Persistent<v8::Function> r_call;

static  uv_async_t* async = (uv_async_t*)malloc(sizeof(uv_async_t));

using namespace v8;
struct async_req {
    uv_work_t req;
    std::string server;
    std::string channel;
    std::string port;
    std::string nick;
    std::string output;
    v8::Isolate *isolate;
    v8::Persistent <v8::Function> callback;
    node::async_context context;
};
struct payload{
     const char* nick = "test";
     const char* msg = "test";
};

void doactualstuff(uv_async_t* handle) {
     payload* p = (payload*)(handle->data);
   Isolate* isolate = Isolate::GetCurrent();
   HandleScope scope(isolate);

        if (isolate) {
           Local<v8::Function> func = Local<Function>::New(isolate, r_call);
         if (!func.IsEmpty()) {
          v8::Local<v8::Value> argv[2];

          argv[1] = v8::String::NewFromUtf8(isolate, p->msg);
          argv[0] = v8::String::NewFromUtf8(isolate, p->nick);

 	func->Call(v8::Null(isolate), 2, argv);


     }
     }
    delete p->msg;
    delete p->nick;
    delete p;

}

void binding_sendStuff(char* nick, char* msg){
    payload* p = (payload*)malloc(sizeof(payload));
    p->nick = nick;
    p->msg = msg;
    async->data = (void*)p;
    uv_async_send(async);
}
void DoAsync(uv_work_t *r) {

    async_req *req = reinterpret_cast<async_req *>(r->data);
    bot_start(req->server.c_str(),req->channel.c_str(),req->port.c_str(),req->nick.c_str());
}

void AfterAsync(uv_work_t *req, int ignored) {


}
void GetAsync(const v8::FunctionCallbackInfo <v8::Value> &args) {
    Isolate* isolate = Isolate::GetCurrent();
    async_req *req = new async_req;

    req->req.data = req;
    String::Utf8Value server(isolate, args[0]->ToString());
    const std::string serverString = std::string(*server);
    String::Utf8Value channel(isolate, args[1]->ToString());
    const std::string channelString = std::string(*channel);
    String::Utf8Value port(isolate, args[2]->ToString());
    const std::string portString = std::string(*port);  
    String::Utf8Value nick(isolate, args[3]->ToString());
    const std::string nickString = std::string(*nick);
    req->server = serverString;
    req->channel = channelString;
    req->port = portString;
    req->nick = nickString;
    req->isolate = isolate;
    req->context = node::EmitAsyncInit(isolate, v8::Object::New(isolate), "test");



    if (args[4]->IsFunction()) {
        v8::Local<v8::Function> func = v8::Local<v8::Function>::Cast(args[4]);
        v8::Function * ptr = *func;
        r_call.Reset(isolate, func);
    }
    else {
        r_call.Reset();
    }
     uv_async_init(uv_default_loop(), async, doactualstuff);
     uv_queue_work(node::GetCurrentEventLoop(isolate),
                          &req->req,
                          DoAsync,
                          (uv_after_work_cb) AfterAsync);
}


static void binding_send(const v8::FunctionCallbackInfo <v8::Value> &info) {
    v8::Isolate *isolate = info.GetIsolate();
    String::Utf8Value keyUtf8(isolate, info[0]->ToString());
    const std::string msg = std::string(*keyUtf8);
    bot_send((char*)msg.c_str());
}
void init(Handle <Object> exports) {

    NODE_SET_METHOD(exports,"send",binding_send);
    NODE_SET_METHOD(exports, "start", GetAsync);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, init
)
