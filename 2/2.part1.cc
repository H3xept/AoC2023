#include "stdio.h"
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

std::ifstream read(char *in) {
  std::ifstream input;
  input.open(in);
  return input;
}

struct BagConfig {
  uint64_t r;
  uint64_t g;
  uint64_t b;
};

struct Game {
  uint64_t id;
  std::vector<BagConfig> extractions;
};

BagConfig parseConfig(char *config) {
  std::string sconfig = std::string(config);
  unsigned long rpos = sconfig.find('r');
  unsigned long gpos = sconfig.find('g');
  unsigned long bpos = sconfig.find('b');
  return (struct BagConfig){
      .r = std::stoul(sconfig.substr(rpos + 1, gpos - 1)),
      .g = std::stoul(sconfig.substr(gpos + 1, bpos - 1)),
      .b = std::stoul(sconfig.substr(bpos + 1, sconfig.length())),
  };
}

// Parses *%d_:_
unsigned long parseId(std::string line, uint64_t *id) {
  unsigned long numPos = line.find(' ') + 1;
  unsigned long columnPos = line.find(':');
  *id = std::stoul(line.substr(numPos, columnPos - 1));
  return columnPos + 2;
}

// Parses %d_%c*,
unsigned long parseComponent(std::string line, char *color, uint64_t *n) {
  unsigned long colorPos = line.find(' ') + 1;
  *n = std::stoul(line.substr(0, colorPos - 1));
  *color = line.substr(colorPos, colorPos + 1).c_str()[0];
  unsigned long result = line.find_first_of(",;");
  return result == -1 ? result : result + 2;
}

// Parses one or more %d_%c* ending in , or ;
unsigned long parseExtraction(std::string line, BagConfig *config) {
  unsigned long next = 0;
  for (uint8_t i = 0; i < 3; i++) {
    char color;
    uint64_t n;
    std::string currentSubstr = line.substr(next);
    unsigned long nextOrHalt = parseComponent(currentSubstr, &color, &n);
    switch (color) {
    case 'r':
      config->r = n;
      break;
    case 'g':
      config->g = n;
      break;
    case 'b':
      config->b = n;
      break;
    }

    if (nextOrHalt == -1) {
      return nextOrHalt;
    }

    next += nextOrHalt;
    if (currentSubstr.find(';') < currentSubstr.find(',')) {
      break;
    }
  }
  return next;
}

std::vector<BagConfig> parseExtractions(std::string line) {
  unsigned long next = 0;
  std::vector<BagConfig> extractions = std::vector<BagConfig>();
  while (true) {
    BagConfig extraction = {0, 0, 0};
    unsigned long nextOrHalt = parseExtraction(line.substr(next), &extraction);
    extractions.push_back(extraction);
    if (nextOrHalt == -1)
      break;
    next += nextOrHalt;
  }
  return extractions;
}

Game parseGame(std::string gameLine) {
  unsigned long next = 0;
  std::string sgame = std::string(gameLine);
  uint64_t id;
  next += parseId(gameLine, &id);
  std::vector<BagConfig> extractions = parseExtractions(gameLine.substr(next));
  return Game{.id = id, .extractions = extractions};
}

bool isSetPossible(const BagConfig *config, const BagConfig *set) {
  return set->r <= config->r && set->g <= config->g && set->b <= config->b;
}

bool isGamePossible(BagConfig *config, std::vector<BagConfig> const &sets) {
  for (uint16_t i = 0; i < sets.size(); i++) {
    if (!isSetPossible(config, &(sets[i])))
      return false;
  }
  return true;
}

void ppgame(Game *game) {
  printf("#%llu: ", game->id);
  for (uint64_t i = 0; i < game->extractions.size() - 1; ++i)
    printf("\t%llu> r: %llu, g: %llu, b: %llu\n",
        i,
        game->extractions[i].r,
        game->extractions[i].g,
        game->extractions[i].b
    );
}

int main(int argc, char **argv) {
  std::ifstream in = read(argv[1]);
  BagConfig bagConfig = parseConfig(argv[2]);
  uint64_t id_sum = 0;
  while (in.peek() != EOF) {
    std::string line;
    std::getline(in, line);
    Game game = parseGame(line);
    ppgame(&game);
    id_sum += isGamePossible(&bagConfig, game.extractions) ? game.id : 0;
  }

  printf("Sum of all possible games: %llu\n", id_sum);
  return 0;
}
