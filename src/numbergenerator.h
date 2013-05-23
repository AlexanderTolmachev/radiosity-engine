#pragma once

class NumberGenerator {
  public:
    static NumberGenerator& getInstance();
  
    unsigned int generateNextId();
    float generateRamdomNumberBetweenZeroAndOne() const;

  private:
    NumberGenerator();
    NumberGenerator(NumberGenerator const&);
    void operator=(NumberGenerator const&);

  private:
    unsigned int mCurrentId; 
};