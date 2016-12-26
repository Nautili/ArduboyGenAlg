#include "Agent.h"
#include <Arduino.h>

Agent::Agent() {
  xPos = 0;
  yPos = 0;
  initGene();
}

void Agent::initGene() {
  for(int i = 0; i < GENE_LENGTH; ++i) {
    gene[i] = random(3) | (random(3) << 2);
  }
}

