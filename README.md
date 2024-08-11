# README

It is definitely possible to do better than the attached program.  

* This is a perfect problem for a GPU - GPU's have more threads than CPU's, and are optimized for simple mathematical tasks with trivial control flow. I might make a version that runs on a GPU, but I'd need to do some research first.  
  * By my extremely crude and underinformed estimation, my GPU could do all of the calculations in less than a second.  
* There are probably appropriate faster random number generation algorithms than the Mersenne Twister, but Mersenne Twisters are still very fast and the C++ standard library implementation of them is good, so it'll do.  
* The print statements are technically wasteful, but I like having them and removing them speeds the program up by less than a second, so I kept them.  
* The whole program could technically be reduced to just printing the results using compile-time evaluation, but that wouldn't be in the spirit of the challenge unless you count compilation time as well as execution time. If the time doesn't include actually doing the randomization and analysis, it doesn't count in my books.  

I compiled the program using clang++ with O3 optimization. The attached log files are the results of running the multithreaded version of the program on my personal computer in the Recovery Mode root terminal. That prevents the computer from spending resources on graphics, which are obviously unnecessary for a task like this. When I told the compiler to give me a thread for each CPU core, I meant it.  
