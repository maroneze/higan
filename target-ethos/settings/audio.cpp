AudioSettings* audioSettings = nullptr;

AudioSlider::AudioSlider() {
  append(name, {75, 0});
  append(value, {75, 0});
  append(slider, {~0, 0});
}

AudioSettings::AudioSettings() {
  frequencyLabel.setText("Frequency:");
  frequency.append("32000hz");
  frequency.append("44100hz");
  frequency.append("48000hz");
  frequency.append("96000hz");
  latencyLabel.setText("Latency:");
  latency.append("20ms");
  latency.append("40ms");
  latency.append("60ms");
  latency.append("80ms");
  latency.append("100ms");
  resamplerLabel.setText("Resampler:");
  resampler.append("Linear");
  resampler.append("Hermite");
  resampler.append("Sinc");
  volume.name.setText("Volume:");
  volume.slider.setLength(201);

  append(controlLayout, {~0, 0}, 5);
    controlLayout.append(frequencyLabel, {0, 0}, 5);
    controlLayout.append(frequency, {~0, 0}, 5);
    controlLayout.append(latencyLabel, {0, 0}, 5);
    controlLayout.append(latency, {~0, 0}, 5);
    controlLayout.append(resamplerLabel, {0, 0}, 5);
    controlLayout.append(resampler, {~0, 0});
  append(volume, {~0, 0});

  switch(config->audio.frequency) { default:
  case 32000: frequency.setSelection(0); break;
  case 44100: frequency.setSelection(1); break;
  case 48000: frequency.setSelection(2); break;
  case 96000: frequency.setSelection(3); break;
  }
  switch(config->audio.latency) { default:
  case  20: latency.setSelection(0); break;
  case  40: latency.setSelection(1); break;
  case  60: latency.setSelection(2); break;
  case  80: latency.setSelection(3); break;
  case 100: latency.setSelection(4); break;
  }
  resampler.setSelection(config->audio.resampler);
  volume.slider.setPosition(config->audio.volume);

  frequency.onChange = latency.onChange = resampler.onChange = volume.slider.onChange =
  {&AudioSettings::synchronize, this};

  synchronize();
}

void AudioSettings::synchronize() {
  switch(frequency.selection()) {
  case 0: config->audio.frequency = 32000; break;
  case 1: config->audio.frequency = 44100; break;
  case 2: config->audio.frequency = 48000; break;
  case 3: config->audio.frequency = 96000; break;
  }
  switch(latency.selection()) {
  case 0: config->audio.latency =  20; break;
  case 1: config->audio.latency =  40; break;
  case 2: config->audio.latency =  60; break;
  case 3: config->audio.latency =  80; break;
  case 4: config->audio.latency = 100; break;
  }
  config->audio.resampler = resampler.selection();
  config->audio.volume = volume.slider.position();

  volume.value.setText({config->audio.volume, "%"});

  utility->synchronizeRuby();
}
