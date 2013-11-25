
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/MemoryLeakDetectorNewMacros.h>
#include "serial_comm.h"

TEST_GROUP(Core)
{
  HardwareSerial s;
  SerialComm *sc;
  
  void setup()
  {
    sc = new SerialComm(&s);
  }
  
  void teardown()
  {
    delete sc;
  }
};

TEST(Core, ShouldAddArgument)
{
  SerialMessage msg("SEND");
  
  msg.addArgument("arg1");
    
  LONGS_EQUAL(1, msg.argsCount());
  STRCMP_EQUAL("arg1", msg.getArg(0));
}

TEST(Core, NoArgs)
{
  SerialMessage msg("WTF");
  
  sc->sendMsg(&msg);
  
  STRCMP_EQUAL("WTF", s.getBuffer());
}


TEST(Core, OneArgs)
{
  SerialMessage msg("STATE");
  
  msg.addArgument("42");
  
  sc->sendMsg(&msg);
  
  STRCMP_EQUAL("STATE 42", s.getBuffer());
}

TEST(Core, TwoArgs)
{
  SerialMessage msg("STATE");
  
  msg.addArgument("42");
  msg.addArgument("toto");
  
  sc->sendMsg(&msg);
  
  STRCMP_EQUAL("STATE 42 toto", s.getBuffer());
}




int main(int ac, char** av)
{
   return CommandLineTestRunner::RunAllTests(ac, av);
}
