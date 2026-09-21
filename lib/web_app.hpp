#ifndef WEB_APP_H_
#define WEB_APP_H_

#include <chrono>
#include "WebServer.h"
#include "json.hpp"
using json = nlohmann::json;

class SimpleJSONSession : public WebServerBase::Session {
public:
	virtual void receiveJSON(const json& val) {}
	
	virtual void sendJSON(json& val) {
		sendMessage(val.dump());
	}

	void receiveMessage(const std::string& msg) {
        json val = json::parse(msg);
        receiveJSON(val);
	}
};

template<typename SHARED_STATE>
class WebApp : public SimpleJSONSession {
public:
    WebApp(SHARED_STATE& state) : state(state), start(std::chrono::system_clock::now()), time(0.0) {}
    ~WebApp() {}
    
    void receiveJSON(const json& data) {
        //std::cout << data << std::endl;
        if (!data.contains("cmd")) {
            return;
        }
        std::string cmd = data["cmd"].get<std::string>();
        json returnValue;
        returnValue["id"] = data["id"].get<int>();
        ReceiveCommand(cmd, data, returnValue);
        sendJSON(returnValue);   
    }

    void SendCommand(const std::string& command, json& data) {
        data["cmd"] = command;
        sendJSON(data);
    }

    void ReceiveCommand(const std::string& cmd, const json& data, json& returnValue) {
        //std::cout << cmd << std::endl;
        if (cmd == "update") {
            returnValue["n"] = data["n"].get<int>();
            std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
            std::chrono::duration<double> diff = end - start;
            double delta = diff.count() - time;
            time += delta;

            //double simSpeed = data["simSpeed"].get<double>();
            //delta *= simSpeed;

            if (delta > 0.1) {
                for (float f = 0.0; f < delta; f+=0.01) {
                    Update(0.01, returnValue);
                }
            }
            else {
                //std::cout << "other" << delta << std::endl;
                Update(delta, returnValue);
            }
        }
        else if (cmd == "keyup") {
            KeyUp(data["key"].get<std::string>(), data["keyCode"].get<double>());
        }
        else if (cmd == "keydown") {
            KeyDown(data["key"].get<std::string>(), data["keyCode"].get<double>());
        }
        else if (cmd == "mousemove") {
            MouseMove(data["x"].get<double>(), data["y"].get<double>());
        }
        else if (cmd == "setup") {
            Setup(returnValue);
        }
    }

    virtual void Setup(json& returnValue) {}
    virtual void Update(double dt, json& returnValue) {}
    virtual void KeyUp(const std::string& key, int keyCode) {}
    virtual void KeyDown(const std::string& key, int keyCode) {}
    virtual void MouseMove(float x, float y) {}
    float getWidth() const { return width; }
    float getHeight() const { return height; }

private:
    std::chrono::time_point<std::chrono::system_clock> start;
    double time;

protected:
    SHARED_STATE state;
    double width;
    double height;
};


#endif
