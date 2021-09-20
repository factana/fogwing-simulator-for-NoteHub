
/****************************************************************************************
   Title     : Send sensors data to Fogwing platform with Notecard
   updated on: 29-SEP-2021
****************************************************************************************/

#include <Notecard.h>

//configure Serial interface as serialDebug
#define serialDebug Serial

//add a definition for your ProductUID using the value you specified when creating your Notehub project.
#define productUID "com.factana.ajaykumar.kanojiya:notecardtest"

//data interval(mS)
#define TIME_FREQ 60000

//declare a global object to represent the Notecard.
Notecard notecard;

//initialize variabls
static uint32_t tNow, tLast = 0;
float temp, humi, pressure = 0;


void setup()
{
  delay(2500);

  //initialize the serialDebug object and tell the Notecard library to use this serial object for sending debug output.
  serialDebug.begin(115200);

  //initialize an instance of the Notecard class and initialize an I2C connection to the Notecard using the notecard.begin() function
  notecard.begin();

  //link the debug output to serialDebug
  notecard.setDebugOutputStream(serialDebug);

  //configure the Notecard using the hub.set request
  J *req = notecard.newRequest("hub.set");
  //associate this Notecard with the ProductUID of your project mode
  JAddStringToObject(req, "product", productUID);

  /*set the Notecard to operate in continuous, which indicates that the device should
    immediately make a connection to Notehub and keep it active.*/
  JAddStringToObject(req, "mode", "continuous");

  //fires the request off to the Notecard with the sendRequest() function
  notecard.sendRequest(req);
}

void loop()
{
  //read the current time after starting running the cotroller in ms.
  tNow = millis();

  if ((tNow - tLast) > TIME_FREQ) {

    //generating random data for simulation, you can replace it with your sensor data.
    temp = random(250, 300) / 10.0;
    humi = random(500, 600) / 10.0;
    pressure = random(1000, 1010);

    //construct a new JSON request to the note.add API
    J *req = notecard.newRequest("note.add");
    if (req != NULL) {

      //includes a new Notefile name (sensors.qo)
      JAddStringToObject(req, "file", "sensors.qo");

      //sets the start field to true to instruct the Notecard to sync to Notehub immediately
      JAddBoolToObject(req, "sync", true);
      J *body = JCreateObject();
      if (body != NULL) {

        //sets the body to the sensor temperature, humidity and pressure
        JAddNumberToObject(body, "temperature", temp);
        JAddNumberToObject(body, "humidity", humi);
        JAddNumberToObject(body, "pressure", pressure);
        JAddItemToObject(req, "body", body);
      }

      //fires the request off to the Notecard with the sendRequest() function
      notecard.sendRequest(req);
    }

    tLast = tNow;
  }

}
