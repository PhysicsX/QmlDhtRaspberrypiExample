#include <QGuiApplication>
#include <QQmlApplicationEngine>


#include <QObject>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>
#include <QQuickItem>
#include <QQuickView>

#include <QTimer>

#include "wiringPi.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>


QObject *item = NULL;

QObject *itemH = NULL;
int value = 10;



#define MAXTIMINGS 85 // I do not know !!
static int DHTPIN = 7;
static int dht22_dat[5] = {0,0,0,0,0};

static uint8_t sizecvt(const int read)
{
  /* digitalRead() and friends from wiringpi are defined as returning a value
  < 256. However, they are returned as int() types. This is a safety function */

  if (read > 255 || read < 0)
  {
    printf("Invalid data from wiringPi library\n");
    exit(EXIT_FAILURE);
  }
  return (uint8_t)read;
}


int read_dht22_dat()
{
  uint8_t laststate = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;

  dht22_dat[0] = dht22_dat[1] = dht22_dat[2] = dht22_dat[3] = dht22_dat[4] = 0;

  // pull pin down for 18 milliseconds
  pinMode(DHTPIN, OUTPUT);
  digitalWrite(DHTPIN, HIGH);
  delay(10);
  digitalWrite(DHTPIN, LOW);
  delay(18);
  // then pull it up for 40 microseconds
  digitalWrite(DHTPIN, HIGH);
  delayMicroseconds(40);
  // prepare to read the pin
  pinMode(DHTPIN, INPUT);

  // detect change and read data
  for ( i=0; i< MAXTIMINGS; i++) {
    counter = 0;
    while (sizecvt(digitalRead(DHTPIN)) == laststate) {
      counter++;
      delayMicroseconds(2);
      if (counter == 255) {
        break;
      }
    }
    laststate = sizecvt(digitalRead(DHTPIN));

    if (counter == 255) break;

    // ignore first 3 transitions
    if ((i >= 4) && (i%2 == 0)) {
      // shove each bit into the storage bytesS
      dht22_dat[j/8] <<= 1;
      if (counter > 16)
        dht22_dat[j/8] |= 1;
      j++;
    }
  }

  // check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
  // print it out if data is good
  if ((j >= 40) &&
      (dht22_dat[4] == ((dht22_dat[0] + dht22_dat[1] + dht22_dat[2] + dht22_dat[3]) & 0xFF)) ) {
        float t, h;
        h = (float)dht22_dat[0] * 256 + (float)dht22_dat[1];
        h /= 10;
        t = (float)(dht22_dat[2] & 0x7F)* 256 + (float)dht22_dat[3];
        t /= 10.0;
        if ((dht22_dat[2] & 0x80) != 0)  t *= -1;


    printf("Humidity = %.2f %% Temperature = %.2f *C \n", h, t );
//    t = 21.0;
    item->setProperty("text",QString().setNum(t,'f',1));
    itemH->setProperty("text",QString().setNum(h,'f',1));

    return 1;
  }
  else
  {
    printf("Data not good, skip\n");
    return 0;
  }
}



int update()
{

    value = value + 1;
   // item->setProperty("text",value);

    return 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    wiringPiSetup();
    setuid(getuid());

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

  // QObject *itemC = NULL;
  // itemC = engine.rootObjects().at(0)->findChild<QQuickItem*>("temp1");
   //itemC->setProperty("text",QString::fromUtf8("°C"));


   item = engine.rootObjects().at(0)->findChild<QQuickItem*>("temp");
   itemH = engine.rootObjects().at(0)->findChild<QQuickItem*>("hum");



   QTimer timer;
   QObject::connect(&timer,&QTimer::timeout,read_dht22_dat);
   timer.start(2000);


    return app.exec();
}
