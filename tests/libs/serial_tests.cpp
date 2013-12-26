
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/MemoryLeakDetectorNewMacros.h>
#include "serial_comm.h"

class TestComm : public SerialComm
{
public:
  SerialMessage *last_message;
  
  TestComm(HardwareSerial *s) : SerialComm(s)
  {
    this->last_message = NULL;
  }
  
  void msgReceived(SerialMessage *msg)
  {
    last_message = msg;
  }
  
};

TEST_GROUP(Core)
{
  HardwareSerial s;
  TestComm *sc;
  
  void setup()
  {
    sc = new TestComm(&s);
  }
  
  void teardown()
  {
    delete sc;
  }
};

// read
TEST(Core, CallCallback)
{
  // fill buffer
  char msg[] = "SET 1 OFF\x04";
  s.write((uint8_t *)msg, strlen(msg) + 1);
  
  // and act as if we received it
  sc->dataAvailable();
  
  CHECK(sc->last_message != NULL);
  
  STRCMP_EQUAL("SET", sc->last_message->getCmd());
  LONGS_EQUAL(2, sc->last_message->argsCount());
  STRCMP_EQUAL("1", sc->last_message->getArg(0));
  STRCMP_EQUAL("OFF", sc->last_message->getArg(1));
  
  delete sc->last_message;
}


// write
TEST(Core, ShouldAddStringArgument)
{
  SerialMessage msg("SEND");
  
  msg.addArgument("arg1");
    
  LONGS_EQUAL(1, msg.argsCount());
  STRCMP_EQUAL("arg1", msg.getArg(0));
}

TEST(Core, ShouldAddUInt16Argument)
{
  SerialMessage msg("SEND");
  
  msg.addArgument(17493);
    
  LONGS_EQUAL(1, msg.argsCount());
  STRCMP_EQUAL("4455", msg.getArg(0));
}

TEST(Core, ShouldAddCharAndUInt16Argument)
{
  SerialMessage msg("SEND");
  
  msg.addArgument('t', 56);
    
  LONGS_EQUAL(1, msg.argsCount());
  STRCMP_EQUAL("t:38", msg.getArg(0));
}


TEST(Core, NoArgs)
{
  SerialMessage msg("WTF");
  
  sc->sendMsg(&msg);
  
  STRCMP_EQUAL("WTF\x04", s.getBuffer());
}


TEST(Core, OneArgs)
{
  const char *buffer;
  
  SerialMessage msg("STATE_UPDATE");
  
  msg.addArgument("42");
  
  sc->sendMsg(&msg);
  
  
  buffer = s.getBuffer();
  // BYTES_EQUAL('2', buffer[7]);
  // BYTES_EQUAL(0x04, buffer[8]);
  
  STRCMP_EQUAL("STATE_UPDATE 42\x04", buffer);
}

TEST(Core, TwoArgs)
{
  SerialMessage msg("STATE");
  
  msg.addArgument(42);
  msg.addArgument("toto");
  
  sc->sendMsg(&msg);
  
  STRCMP_EQUAL("STATE 2a toto\x04", s.getBuffer());
}




int main(int ac, char** av)
{
   return CommandLineTestRunner::RunAllTests(ac, av);
}
