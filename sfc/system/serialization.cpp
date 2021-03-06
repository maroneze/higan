#ifdef SYSTEM_CPP

// This version is slower but since the size is now dynamic, we cannot
// use a statically defined size.
unsigned System::serialize_size() {
  serializer s;

  unsigned signature = 0, version = 0;
  char hash[64], profile[16], description[512];

  s.integer(signature);
  s.integer(version);
  s.array(hash);
  s.array(profile);
  s.array(description);

  serialize_all(s);
  return s.size();
}


serializer System::serialize() {
  serializer s(serialize_size());

  unsigned signature = 0x31545342, version = Info::SerializerVersion;
  char hash[64], description[512], profile[16];
  memcpy(&hash, (const char*)cartridge.sha256(), 64);
  memset(&description, 0, sizeof description);
  memset(&profile, 0, sizeof profile);
  strmcpy(profile, Emulator::Profile, sizeof profile);

  s.integer(signature);
  s.integer(version);
  s.array(hash);
  s.array(description);
  s.array(profile);

  serialize_all(s);
  return s;
}

bool System::unserialize(serializer& s) {
  unsigned signature, version;
  char hash[64], description[512], profile[16];

  s.integer(signature);
  s.integer(version);
  s.array(hash);
  s.array(description);
  s.array(profile);

  if(signature != 0x31545342) return false;
  if(version != Info::SerializerVersion) return false;
  if(strcmp(profile, Emulator::Profile)) return false;

  power();
  serialize_all(s);
  return true;
}

//========
//internal
//========

void System::serialize(serializer& s) {
  s.integer((unsigned&)region);
  s.integer((unsigned&)expansion);
}

void System::serialize_all(serializer& s) {
  cartridge.serialize(s);
  system.serialize(s);
  random.serialize(s);
  cpu.serialize(s);
  smp.serialize(s);
  ppu.serialize(s);
  dsp.serialize(s);

  if(cartridge.has_gb_slot()) icd2.serialize(s);
  if(cartridge.has_bs_cart()) bsxcartridge.serialize(s);
  if(cartridge.has_event()) event.serialize(s);
  if(cartridge.has_sa1()) sa1.serialize(s);
  if(cartridge.has_superfx()) superfx.serialize(s);
  if(cartridge.has_armdsp()) armdsp.serialize(s);
  if(cartridge.has_hitachidsp()) hitachidsp.serialize(s);
  if(cartridge.has_necdsp()) necdsp.serialize(s);
  if(cartridge.has_epsonrtc()) epsonrtc.serialize(s);
  if(cartridge.has_sharprtc()) sharprtc.serialize(s);
  if(cartridge.has_spc7110()) spc7110.serialize(s);
  if(cartridge.has_sdd1()) sdd1.serialize(s);
  if(cartridge.has_obc1()) obc1.serialize(s);
  if(cartridge.has_hsu1()) hsu1.serialize(s);
  if(cartridge.has_msu1()) msu1.serialize(s);
  if(cartridge.has_st_slots()) sufamiturboA.serialize(s), sufamiturboB.serialize(s);

  inputRecorder.serialize(s);
}

//perform dry-run state save:
//determines exactly how many bytes are needed to save state for this cartridge,
//as amount varies per game (eg different RAM sizes, special chips, etc.)
// --> Replaced by serialize_size(), which allows for dynamically-sized data
//     structures
/*
void System::serialize_init() {
  return;
  serializer s;

  unsigned signature = 0, version = 0;
  char hash[64], profile[16], description[512];

  s.integer(signature);
  s.integer(version);
  s.array(hash);
  s.array(profile);
  s.array(description);

  serialize_all(s);
  //serialize_size = s.size();
}
*/

#endif
