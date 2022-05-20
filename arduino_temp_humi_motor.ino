#include <SoftwareSerial.h>
#include "DHT.h"
#define LED_PIN   4  // LED핀 번호
#define BTtx      2  // 블루투스 tx핀이 연결된 아두이노 핀 번호
#define BTrx      3  // 블루투스 rx핀이 연결된 아두이노 핀 번호
#define DHTPIN      5
#define DHTTYPE     DHT11
#define BTtx        7 
#define BTrx        8

SoftwareSerial BT(BTtx, BTrx); // 소프트웨어 시리얼 객체 

char data = 0;           // 모바일 앱으로 부터 입력받은 값 저장할 변수
DHT dht(DHTPIN, DHTTYPE);
unsigned long previousTime = 0;

void setup() {
    BT.begin(9600);   // 소프트웨어 시리얼 통신 준비
    Serial.begin(9600); // 하드웨어 시리얼 통신 준비
    pinMode(LED_PIN, OUTPUT); // 4번핀 출력모드
    BT.begin(9600); 
    dht.begin();
}

void loop() {
    unsigned long currentTime = millis();
    if((unsigned long)(currentTime - previousTime) >= 2000) {
        previousTime = currentTime;
        byte t = dht.readTemperature();// Celsius
        byte h = dht.readHumidity();
        if (!(isnan(h) || isnan(t))) {//return 1 if not a num
            BT.print(t);
            BT.print(",");
            BT.println(h);           
        }
    }
    
    if(BT.available() > 0) { // 블루투스 통신으로 입력된 데이터가 있으면
        data = BT.read();   // 입력된 데이터를 읽어서 변수에 저장하기
    }
    if(data == '0') { // data == '0' 이면
        digitalWrite(LED_PIN, LOW); // LED 끄기
        Serial.println("LED OFF");  // 시리얼 모니터에 글자출력용
    }
    else if(data == '1') { // data == '1' 이면
        digitalWrite(LED_PIN, HIGH); // LED 켜기
        Serial.println("LED ON"); // 시리얼 모니터에 글자출력용
    }
    data = 0; // data 초기화
}
