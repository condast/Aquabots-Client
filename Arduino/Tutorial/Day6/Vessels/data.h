#ifndef Data_h
#define Data_h

class Data {

  public: Data(void);
    void send( int request, String senderId, String senderName, String deviceId, String deviceName, String type, String sentence );
    void sendNMEA( String deviceId, String deviceName, String sentence );
};

#endif
