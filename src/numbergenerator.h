#pragma once

class NumberGenerator {
  public:
    static NumberGenerator& getInstance();
  
    unsigned int generateNextPatchId();
    unsigned int generateNextVertexId();
    float generateRamdomNumberBetweenZeroAndOne() const;

  private:
    NumberGenerator();
    NumberGenerator(NumberGenerator const&);
    void operator=(NumberGenerator const&);

  private:
    unsigned int mCurrentPatchId;
    unsigned int mCurrentVertexId; 
};