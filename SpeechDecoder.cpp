//
// Created by chaz on 2/24/16.
//
#include "prefix.hpp"
#include "SpeechDecoder.hpp"
#include "AudioChunk.h"

SpeechDecoder::SpeechDecoder(const std::string &modelDir)
{
  _options = cmd_ln_init(nullptr, ps_args(), true,
                         "-hmm", (modelDir + "/en-us/en-us").c_str(),
                         "-lm", (modelDir + "/en-us/en-us.lm.bin").c_str(),
                         "-dict", (modelDir + "/en-us/cmudict-en-us.dict").c_str(),
                         nullptr);

  _decoder = ps_init(_options);
}

SpeechDecoder::~SpeechDecoder()
{
  if (_decoder != nullptr)
  {
    ps_free(_decoder);
  }
}

void SpeechDecoder::setLogFile(const std::string &file)
{
  _options = cmd_ln_init(_options, ps_args(), true,
                         "-logfn", file.c_str(), nullptr);
}

void SpeechDecoder::setAudioSampleRate(const unsigned int &rate)
{
  int nftt_factor = std::ceil(rate/16000.0);
  int nftt = nftt_factor*512;

  cmd_ln_t* updatedOptions = cmd_ln_init(_options, ps_args(), true,
  "-samprate", std::to_string(rate).c_str(),
  "-nftt", std::to_string(nftt).c_str(),
                                        nullptr);

  ps_reinit(_decoder, updatedOptions);

  _options = updatedOptions;
}

std::tuple<std::string, double> SpeechDecoder::processChunk(const AudioChunk & chunk)
{
  ps_start_utt(_decoder);

  ps_process_raw(_decoder, (const int16*)chunk.buffer(), 1024, 0, 1);

  ps_end_utt(_decoder);

  auto logmath = ps_get_logmath(_decoder);
  int confidence;

  return std::make_tuple(ps_get_hyp(_decoder, &confidence), logmath_exp(logmath, confidence));
}