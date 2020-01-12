#include <WiFi.h>
#include <HTTPClient.h>
#include "asyncHTTPrequest.h"

const char* ssid = "phloenlom_2.4GHz";
const char* password =  "248248248";
String payload_pattern = "{\"username\":\"smazcontrol\",\"password\":\"palace66\",\"title\":\"ทดสอบภาษาไทย\",\"link\":\"www.test.com\",\"description\":\"กุ้งกั้ง\",\"pubdate\":\"2020-01-01\"}";

int break_state = 0;
String url1;
asyncHTTPrequest request[85];
String response;

void requestCB(void* optParm, asyncHTTPrequest* request, int readyState){
    response = request->responseText();
    Serial.println(response);
    Serial.println("out");
    request->setDebug(false);
    break_state = 1;
}

void scan(int start, int quan) {
  int time;
  int j=0;
  for (int i=start; i<start+quan; i++) {
    request[j].onData(requestCB);
    request[j].setTimeout(2);
    time = millis();
    String url = "http://192.168.1."+String(i)+"/scan";
    url1 = "192.168.1."+String(i);
    request[j].open("GET", url.c_str());
    request[j].send();
    Serial.print(url);
    while (time+500>millis()) {
      Serial.print(".");
      delay(100);
    }
    if (break_state == 1) {
      break;
    }
    Serial.println("/");
    j+=1;
  }
  if (break_state != 1) {
    delay(1000);
  }  
}

void setup() {
 
  Serial.begin(115200);
  delay(4000);   //Delay needed before calling the WiFi.begin
 
  WiFi.begin(ssid, password); 
 
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");

  Serial.println(" ");
  scan(2,81);
  if (break_state!=1) {
    scan(83,81);
  }
  if (break_state!=1) {
    scan(164,81);
  }

 
  delay(1000);
 
}

void loop() {
  Serial.println(" ");
  Serial.println("IP_SCAN");
  Serial.println("==============");
  if (break_state==1) {
    Serial.println(response);
  
   
    //float temperature = 23.456;
    //float humidity = 80.21;
    String payload = payload_pattern;
    //payload.replace("$temperature$",String(temperature));
    //payload.replace("$humidity$",String(humidity));
    //payload.replace("$counter$",String(counter));
 
  
    HTTPClient http;   
 
    http.begin("http://"+response+"/postplain/");  
    int httpResponseCode = http.POST(payload); 
 
    if(httpResponseCode>0){
      String response = http.getString(); 
      Serial.println(httpResponseCode);
      Serial.println(response);
    }else{
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();    
  } else {
    Serial.println("IP_SCAN Not found");
  }
  
  while (true) {
    delay(100);
  }
}
