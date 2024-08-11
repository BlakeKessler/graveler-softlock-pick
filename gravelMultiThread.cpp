#include <random>
#include <chrono>

#include <thread>
#include <future>
#include <functional>
#include <vector>

#include <cstdio>

#define TRIALS 1000000000ul
#define ROLLS_PER_TRIAL 231ul
#define DIE 4

using uint = unsigned int;
using luint = unsigned long;
using RNG_t = std::mt19937;

//get current time in milliseconds
luint time() {
   return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

//!simple container to store information about a set of trials
struct TrialResult {
   luint maxOnes;
   luint trialID;
   luint startTime;
   luint endTime;
   luint timePerSecond;

   TrialResult() {}
   TrialResult(luint id):
      maxOnes(0),
      trialID(id),
      startTime(time()),
      endTime(),
      timePerSecond(std::milli::den) {

   }
   auto stopTimer() {
      endTime = time();
      return endTime;
   }

};

//!run a set of trials simulating a Pikasprey Graveler soft-lock break attempt
//!returns an object containing information about the trials
TrialResult runTrials(const luint trials, const luint rollsPerTrial, const uint uniqueID) {
   //!set up RNG
   //!seeded with the trial's unique ID
      //!added in a call to std::rand() because it is a global RNG function
      //!which means that providing non-unique IDs should still create distinct seeds
         //!not perfect because std::rand() is not thread-safe, but I'm going to provide unique IDs so it doesn't really matter
      //!multiplying parameters used to calculate the seed by distinct prime numbers effectively prevents collisions
   RNG_t gen((uniqueID * 541) + (std::rand() * 547));
   uint trialSuccesses;

   //!set up results object (starts the clock)
   TrialResult results(uniqueID);
   
   //run all trials
   for (luint i = trials; i; --i) {
      //run trial
      trialSuccesses = 0;
      for (uint j = rollsPerTrial; j; --j) {
         trialSuccesses += !(gen() % DIE); //true == 1, !a == 0 if and only if a != 0
      }
      //update maxOnes
      results.maxOnes = (trialSuccesses > results.maxOnes) ? trialSuccesses : results.maxOnes;
   }
   results.stopTimer();
   std::printf("Thread %u complete\n", uniqueID+1);
      std::fflush(stdout);
   return results;
}

//!main function
//!sets up threads
   //!has each one run its share of the trials
   //!consolidates the results
   //!times the whole process
int main() {
   //!reserve space to store threads and results
   std::printf("Allocating memory\n\n");
   std::fflush(stdout);
   TrialResult finalResults(-1);
   std::vector<std::thread> threads(std::thread::hardware_concurrency());
   std::vector<std::future<TrialResult>> resultsArr(threads.size());
   std::vector<TrialResult> trialResults(threads.size());
   //!dispatch threads
   for (uint i = 0; i < threads.size(); ++i) {
      std::printf("Dispatching thread %u: ", i+1);
      std::fflush(stdout);
      auto temp = std::packaged_task<TrialResult()>(std::bind(runTrials,TRIALS / threads.size(),ROLLS_PER_TRIAL,i));
      resultsArr[i] = temp.get_future();
      threads[i] = std::thread(std::move(temp));
      std::printf("✓\n");
      std::fflush(stdout);
   }
   //!get results
   std::printf("\n");
   for (uint i = 0; i < threads.size(); ++i) {
      threads[i].join();
      resultsArr[i].wait();
      trialResults[i] = resultsArr[i].get();
   }

   //!combine results from each thread
   std::printf("\nDone simulating. Processing results.\n\n");
   std::fflush(stdout);
   finalResults.maxOnes = 0;
   for (auto& i : trialResults) {
      finalResults.maxOnes = (i.maxOnes > finalResults.maxOnes) ? i.maxOnes : finalResults.maxOnes;
   }
   finalResults.stopTimer();
   //!print results
   std::printf(
      "RESULTS:\n - Most successful trial: \033[4m%lu\033[24m successes\n - Total simulation time: \033[4m%.3f\033[24m seconds \n",
      finalResults.maxOnes,
      double(finalResults.endTime - finalResults.startTime) / finalResults.timePerSecond
   );
}