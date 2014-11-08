InputRecorder inputRecorder;

// called by sfc/cpu/timing/timing.cpp
void InputRecorder::cpuAddClocks(unsigned clocks) {
  recInputs.cpuTimer += static_cast<unsigned long>(clocks);
}

// called by sfc/cpu/timing/timing.cpp
void InputRecorder::cpuReset() {
  recInputs.cpuTimer = 0;
  recInputs.status = IR_OFF;
}

void InputRecorder::resetRecords(unsigned long deltaTimer) {
  recInputs.rport.reset();
  auto& sys = system();
  InputData id;
  id.cpuTimer = recInputs.cpuTimer;
  id.deltaTimer = deltaTimer;
  id.deltaReplayTimer = 0;
  id.recordedCount = 0;
  id.replayedCount = 0;

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
  if (inputRecorder.recInputs.rport.size() == 0) {
    fprintf(stderr, "resetReplay: warning: recInputs not initialized - ignoring command\n");
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
    inputRecorder.recInputs.deltaReplayTimer = inputRecorder.recInputs.cpuTimer;
  } else {
    inputRecorder.recInputs.deltaReplayTimer = 0;
  }
  inputRecorder.recInputs.replayedCount = 0;
}

optional<int16_t> InputRecorder::interfaceReplay(unsigned port, unsigned device, unsigned input) {
  if (recInputs.status == IR_REPLAY_FULL || recInputs.status == IR_REPLAY_MACRO) {
    if (recInputs.replayedCount == recInputs.recordedCount) {
      recInputs.status = IR_OFF;
      return false;
    }
    RecInput& ri = recInputs.rport[port].rdevice[device].rinput[input];
    int16_t last_val;
    if (ri.pIndex >= ri.pulse.size()) {
      // no more recorded pulses for this input (but possibly for other inputs)
      return false;
    }
    if (recInputs.cpuTimer - recInputs.deltaReplayTimer >= ri.pulse[ri.pIndex].time) {
      // timer advanced past the current pulse for this input
      ri.curVal = ri.pulse[ri.pIndex].val;
      ri.pIndex++;
      recInputs.replayedCount++;
    }
    return {true, ri.curVal};
  } else {
    return false;
  }
}

void InputRecorder::replay() {
  recInputs.status = IR_REPLAY_FULL;  
  resetReplay(false);
}

void InputRecorder::replayMacro() {
  recInputs.status = IR_REPLAY_MACRO;
  resetReplay(true);
}

void InputRecorder::toggleRecord() {
  if (recInputs.status != IR_REC) {
    resetRecords(0L);
    recInputs.status = IR_REC;
  } else {
    recInputs.status = IR_OFF;
  }
}


void InputRecorder::interfaceRecord(unsigned port, unsigned device, unsigned input, int16_t v) {
  if (recInputs.status == IR_REC) {
    RecInput& ri = recInputs.rport[port].rdevice[device].rinput[input];
    if (ri.pulse.size() > 0 && ri.pulse.last().val == v) {
      // no change
      return;
    } else {
      // first input, or input has changed
      if (recInputs.cpuTimer < recInputs.deltaTimer) {
	fprintf(stderr, "ASSERT ERROR (delta: %lu)\n", recInputs.deltaTimer);
	exit(1);
      }
      Pulse p;
      p.time = recInputs.cpuTimer - recInputs.deltaTimer;
      p.val = v;
      ri.pulse.append(p);
      recInputs.recordedCount++;
    }
  }
}

void InputRecorder::toggleRecordMacro() {
  if (recInputs.status != IR_REC) {
    resetRecords(recInputs.cpuTimer);
    recInputs.status = IR_REC;
  } else {
    recInputs.status = IR_OFF;
  }
}

void InputRecorder::serialize(serializer& s) {
  s.integer(recInputs.status);
  s.integer(recInputs.cpuTimer);
  s.integer(recInputs.deltaTimer);
  s.integer(recInputs.deltaReplayTimer);
  s.integer(recInputs.recordedCount);
  s.integer(recInputs.replayedCount);

  if (s.mode() != serializer::Load) {
    unsigned n;
    n = recInputs.rport.size();
    s.integer(n);
    for (auto& rdevice : recInputs.rport) {
      n = rdevice.rdevice.size();
      s.integer(n);
      for (auto& rinput : rdevice.rdevice) {
	n = rinput.rinput.size();
	s.integer(n);
	for (auto& recinput : rinput.rinput) {
	  s.integer(recinput.pIndex);
	  s.integer(recinput.curVal);
	  n = recinput.pulse.size();
	  s.integer(n);
	  for (auto& pulse : recinput.pulse) {
	    s.integer(pulse.time);
	    s.integer(pulse.val);
	  }
	  }
      }
    }
  } else if (s.mode() == serializer::Load) {
    int ndevices;
    s.integer(ndevices);
    recInputs.rport.reset();
    while (ndevices > 0) {
      RDevice rdevice;
      int ninputs;
      s.integer(ninputs);
      while (ninputs > 0) {
	RInput rinput;
	int nrecinputs;
	s.integer(nrecinputs);
	while (nrecinputs > 0) {
	  RecInput ri;
	  s.integer(ri.pIndex);
	  s.integer(ri.curVal);
	  int npulses;
	  s.integer(npulses);
	  while (npulses > 0) {
	    Pulse p;
	    s.integer(p.time);
	    s.integer(p.val);
	    ri.pulse.append(p);
	    npulses--;
	  }
	  rinput.rinput.append(ri);
	  nrecinputs--;
	}
	rdevice.rdevice.append(rinput);
	ninputs--;
      }
      recInputs.rport.append(rdevice);
      ndevices--;
    }
  }
}

string statusToString(int status) {
  switch (status) {
  case IR_OFF: return "OFF";
  case IR_REPLAY_FULL: return "REPLAY";
  case IR_REPLAY_MACRO: return "REPLAY MACRO";
  case IR_REC: return "RECORD";
  default: return "INVALID";
  }
}

string statusCountersToString(InputRecorder::InputData& id) {
  switch (id.status) {
  case IR_OFF: 
  case IR_REC: 
    return {id.recordedCount, " recorded"};
  case IR_REPLAY_FULL:
  case IR_REPLAY_MACRO:
    return {id.replayedCount, "/", id.recordedCount, " replayed"};
  default: 
    return "INVALID";
  }
}

string InputRecorder::statusString() {
  return {"Input recorder: ", statusToString(recInputs.status), " (", statusCountersToString(recInputs), ")"};
}

string InputRecorder::debugStatusString() {
  return {"Input recorder: ", statusToString(recInputs.status), " - CPU: ", recInputs.cpuTimer, " - Δ: ", recInputs.deltaTimer, " - Δ Replay: ", recInputs.deltaReplayTimer, " - ", statusCountersToString(recInputs)};
}
