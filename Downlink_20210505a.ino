
/* sigfox-downlink.ino 		Thu may 13 2021 12:00:00 ramasubbu

Module: sigfox-downlink.ino

Function:
	Downlink performed in sigfox 

Version:
	VN.nna Thu may 13 2021 12:00:00 ramasubbu
      
Copyright notice:
	See LICENSE file accompanying this project
      
 Author:
 	ramasubbu, MCCI Corporation	May 2021
 	
*/ 
 
 #include "MCCI_Sigfox.h"
 
/****************************************************************************
| This is one of the first ways toi setup the library
| You can define your own function. As an exemple if you need to
| store the ids in the eeprom for an industrial production
| or if you setup them using a serial line then in eeprom
| you can rewrite these function ot make it working.
| They are callback function the library use to acces the informations
\****************************************************************************/

MCCI_Sigfox Sigfox( "01415DEE", "814890597F820C8A", "5B54ED1A889FC76FC575A3D201482E45", REGION_RC2,0x8080008,false); 

/*

Name:	setup()

Function:
	Arduino setup function.
	
Definition:
	void setup(
		void
		);
		
Description:
	This function is called by the Arduino framework after
	basic framework has been initialized.And print the information
	as we get from the library such as device ID, region, pac ,
	device key, transmission power.
	
Returns:
	No explicit result.
	
*/

void setup() 
	{
  	// put your setup code here, to run once:
  	pinMode(LED_BUILTIN, OUTPUT);
  	Serial.begin(115200);
  	Serial.println("GO !");
 
  	for (int i=0 ; i < 3 ; i++) 
  		{
		digitalWrite(LED_BUILTIN, HIGH);
		delay(200);
		digitalWrite(LED_BUILTIN, LOW);
		delay(200);
		}

  	// This function allows to ensure the library initialization has been
  	// correctly executed.
  	if ( Sigfox.isReady() ) 
  		{
    		// When using the simple initilization, you can associate in a second step a logger
    		// The library will print information only if the logs are enable in the config.h file.
    		Sigfox.setLogger(&Serial);

                // Now let's print some of the information we can get from the library
    		Serial.println("Sigfox library is ready");
    		Serial.print(" RC : "); Serial.println(Sigfox.getCurrentRC());
    		Serial.print(" DevId: 0x"); Serial.println(Sigfox.getDeviceId(),16);
    		Serial.print(" InitialPac: ");
    		uint8_t lpac[8];
    		Sigfox.getInitialPac(lpac);
    		for (int i = 0 ; i < 8 ; i++) 
    			{
      			Serial.print(lpac[i],16);
    			}
    			Serial.println();
    		Serial.print(" Last Reception Rssi: ");Serial.print(Sigfox.getLastRssi());Serial.println("dBm");
    		Serial.print(" Current Tx Power: ");Serial.print(Sigfox.getTxPower());Serial.println("dBm");
    		Serial.print(" Last Sequence number used: ");Serial.println(Sigfox.getLastSeqId());
    		Serial.print(" Sigfox version: ");Sigfox.printSigfoxVersion();
    
  		} 
  	else 
  		{
    		Serial.println("Failed to init Sigfox Library");
    		while(true);
  		}
    
	}


void loop() 
	{
  	uint8_t mess[12] = { 0,1,2,3,4,5,6,7,8,9,10,11 }; 
  	mcci_sigfox_response_e status;
        uint8_t* buffer;

	Serial.println("Fire a Sigfox 1 bit message");
  	if ( Sigfox.sendBit(true) == MCCSIG_SUCCESS ) 
  		{
    		Serial.println("... Success");
  		} 
  	else
  		{
    		Serial.println("... Failed");
  		}
	delay(1*60*1000);
  	Serial.println("Fire a Sigfox message 8 Bytes");
  	if ( Sigfox.sendFrame(mess,8) == MCCSIG_SUCCESS ) 
  		{
    		Serial.println("... Success");
  		} 
  	else 
  		{
    		Serial.println("... Failed");
 		}
	delay(19*60*1000);
 	
        status = Sigfox.sendFrameWithAck(mess,8, buffer);
 	if(status == SIGFOX_TXRX_NO_DOWNLINK)
 
        mcci_sigfox_response_e a;
	
	uint8_t* buffer;

        a = Sigfox.sendFrameWithAck(mess,8, buffer);
      
 		if(a == SIGFOX_TXRX_NO_DOWNLINK)
   		{
        	Serial.println(" SIGFOX_TXRX_NO_DOWNLINK");
      		}
   	else if( status == SIGFOX_TXRX_DOWLINK_RECEIVED)
   		{
        	Serial.println("SIGFOX_TXRX_DOWLINK_RECEIVED:");
   		}
   	else
   		{
       		Serial.println(" SIGFOX_TXRX_ERROR:");
		}
 	}
 
	
