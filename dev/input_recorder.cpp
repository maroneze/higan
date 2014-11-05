InputRecorder inputRecorder;

// called by sfc/cpu/timing/timing.cpp
void InputRecorder::cpuAddClocks(unsigned clocks) {
  cpuTimer += clocks;
}

// called by sfc/cpu/timing/timing.cpp
void InputRecorder::cpuReset() {
  printf("cpuReset\n");
  cpuTimer = 0;
  status = IR_OFF;
}

void InputRecorder::resetRecords(unsigned long deltaTimer) {
  recInputs.rport.reset();
  auto& sys = system();
  InputData id;
  id.deltaTimer = deltaTimer;
  id.recordedCount = 0;

  for (auto& port : sys.port) {
    RDevice rdev;
    for (auto& device : port.device) {
      RInput rinp;
      for (auto& input : device.input) {
	RecInput ri;
	ri.pulse = vector<Pulse>();
	ri.pIndex = 0;
	ri.curVal = 0;
	rinp.rinput.append(ri);
      }
      rdev.rdevice.append(rinp);
    }
    id.rport.append(rdev);
  }
  recInputs = id;
}

// relative = true for macro, false for entire input
void resetReplay(bool relative) {
  printf("resetIndices\n");
  if (inputRecorder.recInputs.rport.size() == 0) {
    printf("resetReplay: warning: recInputs not initialized - ignoring command\n");
    return;
  }
  for (auto& rdevice : inputRecorder.recInputs.rport) {
    for (auto& rinput : rdevice.rdevice) {
      for (auto& ri : rinput.rinput) {
	ri.pIndex = 0;
      }
    }
  }
  if (relative) {
    inputRecorder.recInputs.deltaReplayTimer = inputRecorder.cpuTimer;
  } else {
    inputRecorder.recInputs.deltaReplayTimer = 0;
  }
  inputRecorder.recInputs.replayedCount = 0;
}

optional<int16_t> InputRecorder::interfaceReplay(unsigned port, unsigned device, unsigned input) {
  if (status == IR_REPLAY_FULL || status == IR_REPLAY_MACRO) {
    if (recInputs.replayedCount == recInputs.recordedCount) {
      printf("end replay (replayed %u inputs)\n", recInputs.replayedCount);
      status = IR_OFF;
      return false;
    }
    RecInput& ri = recInputs.rport[port].rdevice[device].rinput[input];
    int16_t last_val;
    if (ri.pIndex >= ri.pulse.size()) {
      // no more recorded pulses for this input (but possibly for other inputs)
      return false;
    }
    if (cpuTimer - recInputs.deltaReplayTimer >= ri.pulse[ri.pIndex].time) {
      // timer advanced past the current pulse for this input
      ri.curVal = ri.pulse[ri.pIndex].val;
      ri.pIndex++;
      recInputs.replayedCount++;
      printf("replaying new input\n");
    }
    return {true, ri.curVal};
  } else {
    return false;
  }
}

void InputRecorder::replay() {
  printf("replay\n");
  status = IR_REPLAY_FULL;  
  resetReplay(false);
}

void InputRecorder::replayMacro() {
  printf("replayMacro\n");
  status = IR_REPLAY_MACRO;
  resetReplay(true);
}

void InputRecorder::toggleRecord() {
  if (status != IR_REC) {
    printf("start recording\n");
    resetRecords(0);
    status = IR_REC;
  } else {
    printf("end recording (%d inputs)\n", recInputs.recordedCount);
    status = IR_OFF;
  }
}


void InputRecorder::interfaceRecord(unsigned port, unsigned device, unsigned input, int16_t v) {
  if (status == IR_REC) {
    RecInput& ri = recInputs.rport[port].rdevice[device].rinput[input];
    if (ri.pulse.size() > 0 && ri.pulse.last().val == v) {
      // no change
      return;
    } else {
      // first input, or input has changed
      printf("%lu: recording (%u, %u, %u, %d)\n", cpuTimer, port, device, input, v);
      Pulse p;
      p.time = cpuTimer - recInputs.deltaTimer;
      p.val = v;
      ri.pulse.append(p);
      recInputs.recordedCount++;
    }
  }
}

void InputRecorder::toggleRecordMacro() {
  if (status != IR_REC) {
    printf("start recording macro (%lu)\n", cpuTimer);
    resetRecords(cpuTimer);
    status = IR_REC;
  } else {
    printf("end recording macro (%d inputs)\n", recInputs.recordedCount);
    status = IR_OFF;
  }
  //TODO
}
