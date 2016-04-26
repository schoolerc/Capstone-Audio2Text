#include "prefix.hpp"
#include "TwitchStreamChunk.hpp"
#include "Utilities.hpp"



void TwitchStreamChunk::download() {
  _HttpClientType::request request(_uri);
  auto response = getHttpClient().get(request);

  if (response.status() != 200) {
	throw std::runtime_error("Failed to download video file");
  }

  std::tie(_hypothesis, _confidence) = process(response.body());

  int i = 0;

}

std::tuple<std::string, double> TwitchStreamChunk::process(std::string body){
  int pos = _uri.find_last_of('/');
  std::string fileName = _uri.substr(pos + 1);
  std::ofstream file(fileName);
  file << body;
  file.flush();
  file.close();

  std::stringstream cmd;
  std::string audioFile = boost::filesystem::unique_path().native();
  audioFile.append(".wav");
  cmd << "ffmpeg -i " << fileName << " -vn -ac 1 " << audioFile << " > /dev/null 2>&1";

  system(cmd.str().c_str());

  FILE *aFile = fopen(audioFile.c_str(), "r");
  ps_decode_raw(getDecoder(), aFile, -1);
  fclose(aFile);

  auto logarithm = ps_get_logmath(getDecoder());

  int confidence = 1;
  const char * result = ps_get_hyp(getDecoder(), &confidence);
  double tmp = logmath_exp(logarithm, confidence);

  std::remove(fileName.c_str());
  std::remove(audioFile.c_str());
  return std::make_tuple(result == nullptr ? "" : std::string(result), tmp);
}
