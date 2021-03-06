void InputManager::appendHotkeys() {
  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Toggle Fullscreen Mode";
    hotkey->mapping = "1/Button/F11";

    hotkey->press = [] {
      utility->toggleFullScreen();
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Toggle Mouse Capture";
    hotkey->mapping = "1/Button/F12";

    hotkey->press = [] {
      input.acquired() ? input.unacquire() : input.acquire();
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name = "Show Library";
    hotkey->mapping = "1/Button/L";

    hotkey->press = [] {
      libraryManager->show();
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name = "Pause Emulation";
    hotkey->mapping = "1/Button/P";

    hotkey->press = [] {
      program->pause = !program->pause;
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Fast Forward";
    hotkey->mapping = "1/Button/Tilde";

    hotkey->press = [] {
      video.set(Video::Synchronize, false);
      audio.set(Audio::Synchronize, false);
    };

    hotkey->release = [] {
      video.set(Video::Synchronize, ::config->video.synchronize);
      audio.set(Audio::Synchronize, ::config->audio.synchronize);
    };
  }
  
  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Play Speed 25Percent";
    hotkey->mapping = "1/Button/F1";

    hotkey->press = [] {
      ::config->audio.multiplier = 0.25;
      audio.set(Audio::Frequency, (unsigned)(::config->audio.frequency*::config->audio.multiplier));
    };
  }
  
  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Play Speed 50Percent";
    hotkey->mapping = "1/Button/F2";

    hotkey->press = [] {
      ::config->audio.multiplier = 0.5;
      audio.set(Audio::Frequency, (unsigned)(::config->audio.frequency*::config->audio.multiplier));
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Play Speed 75Percent";
    hotkey->mapping = "None";

    hotkey->press = [] {
      ::config->audio.multiplier = 0.75;
      audio.set(Audio::Frequency, (unsigned)(::config->audio.frequency*::config->audio.multiplier));
    };
  }
  
  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Play Speed 100Percent";
    hotkey->mapping = "1/Button/F3";

    hotkey->press = [] {
      ::config->audio.multiplier = 1.0;
      audio.set(Audio::Frequency, (unsigned)(::config->audio.frequency*::config->audio.multiplier));
    };
  }
  
  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Play Speed 150Percent";
    hotkey->mapping = "None";

    hotkey->press = [] {
      ::config->audio.multiplier = 1.5;
      audio.set(Audio::Frequency, (unsigned)(::config->audio.frequency*::config->audio.multiplier));
    };
  }
  
  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Play Speed 200Percent";
    hotkey->mapping = "1/Button/F4";

    hotkey->press = [] {
      ::config->audio.multiplier = 2.0;
      audio.set(Audio::Frequency, (unsigned)(::config->audio.frequency*::config->audio.multiplier));
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Play Speed 3Percent";
    hotkey->mapping = "1/Button/U";

    hotkey->press = [] {
      ::config->audio.multiplier = 0.03125;
      audio.set(Audio::Frequency, (unsigned)(::config->audio.frequency*::config->audio.multiplier));
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Play Speed 6Percent";
    hotkey->mapping = "1/Button/I";

    hotkey->press = [] {
      ::config->audio.multiplier = 0.0625;
      audio.set(Audio::Frequency, (unsigned)(::config->audio.frequency*::config->audio.multiplier));
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Play Speed 12Percent";
    hotkey->mapping = "1/Button/O";

    hotkey->press = [] {
      ::config->audio.multiplier = 0.125;
      audio.set(Audio::Frequency, (unsigned)(::config->audio.frequency*::config->audio.multiplier));
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Toggle Record Input";
    hotkey->mapping = "1/Button/R";

    hotkey->press = [] {
      inputRecorder.toggleRecord();
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Replay Input Full";
    hotkey->mapping = "1/Button/Y";

    hotkey->press = [] {
      inputRecorder.replay();
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Toggle Record Macro";
    hotkey->mapping = "1/Button/M";

    hotkey->press = [] {
      inputRecorder.toggleRecordMacro();
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Replay Macro";
    hotkey->mapping = "1/Button/K";

    hotkey->press = [] {
      inputRecorder.replayMacro();
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Power Cycle";
    hotkey->mapping = "None";

    hotkey->press = [] {
      utility->power();
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Soft Reset";
    hotkey->mapping = "None";

    hotkey->press = [] {
      utility->reset();
    };
  }

  static unsigned activeSlot = 1;

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Save State";
    hotkey->mapping = "1/Button/F5";

    hotkey->press = [&] {
      utility->saveState(activeSlot);
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Load State";
    hotkey->mapping = "1/Button/F7";

    hotkey->press = [&] {
      utility->loadState(activeSlot);
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Decrement Slot";
    hotkey->mapping = "1/Button/F6";

    hotkey->press = [&] {
      if(--activeSlot == 0) activeSlot = 5;
      utility->showMessage({"Selected slot ", activeSlot});
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Increment Slot";
    hotkey->mapping = "1/Button/F8";

    hotkey->press = [&] {
      if(++activeSlot == 6) activeSlot = 1;
      utility->showMessage({"Selected slot ", activeSlot});
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Close Emulator";
    hotkey->mapping = "None";

    hotkey->press = [] {
      Application::quit();
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Toggle Tracer";
    hotkey->mapping = "None";

    hotkey->press = [&] {
      utility->tracerToggle();
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Export Memory";
    hotkey->mapping = "None";

    hotkey->press = [&] {
      if(program->active == nullptr) return;
      system().exportMemory();
      utility->showMessage("Memory exported");
    };
  }

  Configuration::Node node;
  for(auto& hotkey : hotkeyMap) {
    node.append(hotkey->mapping, string{hotkey->name}.replace(" ", ""));
  }
  config.append(node, "Hotkey");
}

void InputManager::pollHotkeys() {
  for(auto& hotkey : hotkeyMap) {
    bool state = hotkey->poll();
    if(hotkey->state == 0 && state == 1) if(hotkey->press) hotkey->press();
    if(hotkey->state == 1 && state == 0) if(hotkey->release) hotkey->release();
    hotkey->state = state;
  }
}
