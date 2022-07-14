#ifndef Config_h
#define Config_h

#define CONFIG_START F("{'name':'root', 'id':'org.satr.arnac.costa', 'type':'vessel'")
#define ARRAY_START F("[")
#define ARRAY_END F("]")
#define CONFIG_END F("}")

class Config {
  private:
    bool sendConfig( String config );
  
  public: Config(void);
    void send();
};

#endif
