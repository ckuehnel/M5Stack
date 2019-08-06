// Connection to Thingspeak Server via GPRS

void getResponse() 
{
  delay(midZ);
  while (Serial2.available()) Serial.write(Serial2.read());
}

void sendUpdate()
{
  if (DEBUG) Serial.println("Prepare Connection to Thingspeak...");
  Serial2.println("at+csq");
  delay(minZ);
  getResponse();
  Serial2.println("at+cipshut");
  delay(maxZ);
  getResponse();
  Serial2.println("at+cstt=\""+ APN +"\",\"\",\"\"");
  getResponse();
  Serial2.println("at+ciicr");//start wireless connection cellular network
  getResponse();
  Serial2.println("at+cifsr");//enquire regarding the IP address allocated
  getResponse();
  Serial2.println("at+cipstart=\"TCP\",\""+ URL +"\",\"80\"");  //connect to the ThingSpeak update URL (https://api.thingspeak.com)
  getResponse();
  Serial2.println("at+cipsend=80");//declare the number of bytes (characters) I want to send
  getResponse();
  Serial2.print("GET https://");    //this is a constant beginning for the GET command and is as provided by ThingSpeak
  Serial2.print(URL);
  Serial2.print("/update?");
  Serial2.print(APIKEY); //the channel API key provided by ThingSpeak
  getResponse();
  Serial2.print("&field1=");
  getResponse();
  Serial2.println(String(temp));//the value to post to field 
  getResponse();
  Serial2.println("at+cipack");//ask for acknowledge details
  getResponse();
  Serial2.println("at+cipclose");//close the IP connection
  getResponse();
}
