#include "Arduboy.h"
#include "Agent.h"

Arduboy arduboy;

const unsigned int POP_SIZE = 10;
Agent* agents[POP_SIZE];

const int X_START = 20;
const int Y_START = 10;
const int X_GOAL = WIDTH - 20;
const int Y_GOAL = HEIGHT - 10;

const int POP_CUTOFF = 5;
int iteration = 1;
int mutationRate = 10;
int curGene = 0;


void initPositions() {
  for(int i = 0; i < POP_SIZE; ++i) {
    agents[i]->xPos = X_START;
    agents[i]->yPos = Y_START;
  }
}

void handleInput() {
  if(arduboy.pressed(UP_BUTTON)) {
    mutationRate++;
  }

  if(arduboy.pressed(DOWN_BUTTON)) {
    mutationRate--;
  }

  if(arduboy.pressed(A_BUTTON)) {
    mutationRate = 10;
  }
  
  mutationRate = constrain(mutationRate, 0, 100);
}

void updateAgents() {
  for(int i = 0; i < POP_SIZE; ++i) {
    unsigned char geneBits = agents[i]->gene[curGene];
    agents[i]->xPos += geneBits & 0x1;
    agents[i]->xPos -= (geneBits >> 1) & 0x1;
    agents[i]->yPos += (geneBits >> 2) & 0x1;
    agents[i]->yPos -= (geneBits >> 3) & 0x1;
  }
  ++curGene;
}

void drawAgents() {
  for(int i = 0; i < POP_SIZE; ++i) {
    arduboy.drawRect(agents[i]->xPos, agents[i]->yPos, 2, 2, 1);
  }
}

void drawEndpoints() {
  arduboy.drawRect(X_START - 1, Y_START - 1, 3, 3, 1);
  arduboy.drawRect(X_GOAL - 1, Y_GOAL - 1, 3, 3, 1);
}

void drawText() {
  arduboy.setCursor(0, 48);
  arduboy.print("Iter:");
  arduboy.println(iteration);
  arduboy.print("Mut rate:");
  arduboy.print(mutationRate);
  arduboy.println("%");
  
}

int getFitness(Agent* agent) {
  return sqrt(pow(X_GOAL - agent->xPos, 2) + pow(Y_GOAL - agent->yPos, 2));
}

// slow, but fine for small amounts
void sort(Agent** a, int arrLen) {
  int curIndex;
  Agent* temp;
  for(int i = 0; i < arrLen; ++i) {
    curIndex = i;
    for(int j = i + 1; j < arrLen; ++j) {
      if(getFitness(a[j]) < getFitness(a[curIndex])) {
        curIndex = j;
      }
    }

    temp = a[i];
    a[i] = a[curIndex];
    a[curIndex] = temp;
  }
}

void crossover(Agent* a, Agent* b, Agent* c) {
  int crossPoint = random(Agent::GENE_LENGTH);

  for(int i = 0; i < crossPoint; ++i) {
    c->gene[i] = a->gene[i];
  }
  for(int i = crossPoint; i < Agent::GENE_LENGTH; ++i) {
    c->gene[i] = b->gene[i];
  }
}

void pointMutate(Agent* a) {
  for(int i = 0; i < Agent::GENE_LENGTH; ++i) {
    if(random(100) < mutationRate) {
      a->gene[i] = random(3) | (random(3) << 2);
    }
  }
}

void mutate() {
  sort(agents, POP_SIZE);

  for(int i = POP_SIZE / 2; i < POP_SIZE; ++i) {
    crossover(agents[random(POP_SIZE / 2)], agents[random(POP_SIZE / 2)], agents[i]);
  }

  for(int i = 0; i < POP_SIZE; ++i) {
    pointMutate(agents[i]);
  }
}

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(15);
  arduboy.display();
  for(int i = 0; i < POP_SIZE; i++) {
    agents[i] = new Agent();
  }
  initPositions();
}

void loop() {
  if(!arduboy.nextFrame()) {
    return;
  }

  if(curGene >= Agent::GENE_LENGTH) {
    mutate();
    initPositions();
    curGene = 0;
    iteration++;
  }

  arduboy.clear();
  handleInput();
  updateAgents();
  drawAgents();
  drawEndpoints();
  drawText();
  arduboy.display();
}
