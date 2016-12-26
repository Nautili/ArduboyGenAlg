#ifndef AGENT_H
#define AGENT_H

class Agent {
  public:
    Agent();
    int xPos;
    int yPos;

    static const int GENE_LENGTH = 100;
    unsigned char gene[GENE_LENGTH];
    void initGene();
};

#endif
