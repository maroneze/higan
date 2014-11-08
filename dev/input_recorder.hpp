// Recorder status
#define IR_OFF 0
#define IR_REPLAY_FULL 1
#define IR_REPLAY_MACRO 2
#define IR_REC 3

struct InputRecorder {

  struct Pulse {
    unsigned long time;
    int16_t val;
  };

  struct RecInput {
    vector<Pulse> pulse;
    int pIndex;
    int16_t curVal;
  };

  struct RInput {
    vector<RecInput> rinput;
  };

  struct RDevice {
    vector<RInput> rdevice;
  };

  struct InputData {
  // A typedef for 'status' could be useful,
  // but for serialization an int is simpler
    unsigned status;
    unsigned long cpuTimer;
    unsigned long deltaTimer;
    unsigned long deltaReplayTimer;
    unsigned recordedCount;
    unsigned replayedCount;
    vector<RDevice> rport;
  };
  
  InputData recInputs;

  // sfc/cpu/timing/timing.cpp
  void cpuAddClocks(unsigned clocks);
  void cpuReset();

  // target-ethos/interface/interface.cpp
  optional<int16_t> interfaceReplay(unsigned port, unsigned device, unsigned input);
  void interfaceRecord(unsigned port, unsigned device, unsigned input, int16_t v);
  
  // target-ethos/input/hotkeys.cpp
  void toggleRecord();
  void toggleRecordMacro();
  void replay();
  void replayMacro();

  void serialize(serializer& s);
  string statusString();
  string debugStatusString();

private:
  void resetRecords(unsigned long recordTimerStart);

};

extern InputRecorder inputRecorder;
