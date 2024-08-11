#include <random>
#include <ctime>
#include <cstdio>
#include <cstdlib>

#define TRIALS 1000000000ul
#define ROLLS_PER_TRIAL 231
#define DIE 4

using uint = unsigned int;
using luint = unsigned long;

int main(int argc, char** argv) {
   std::srand(argc > 1 ? std::atoi(argv[1]) : 0);
   uint maxOnes = 0;
   std::time_t startTime;
   std::time_t endTime;

   uint currentOnes;
   startTime = std::time(nullptr);
   for (luint i = TRIALS; i; --i) {
      currentOnes = 0;
      for (uint i = ROLLS_PER_TRIAL; i; --i) {
         currentOnes += !(std::rand() % DIE);
      }
      maxOnes = currentOnes > maxOnes ? currentOnes : maxOnes;
   }
   endTime = std::time(nullptr);

   std::printf("Ran %lu trials in %lu seconds\nMost successful trial: %u successes\n", TRIALS, endTime-startTime, maxOnes);
}