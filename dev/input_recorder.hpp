typedef enum { IR_REC, IR_REPLAY_FULL, IR_REPLAY_MACRO, IR_OFF } status_t;

// TODO: see how to make it serializable (sfc does not know Configuration::Document)

struct InputRecorder {
  status_t status;
  long unsigned cpuTimer;

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

private:
  void resetRecords(unsigned long recordTimerStart);

};

extern InputRecorder inputRecorder;
