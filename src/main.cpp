#include <thread>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <stdint.h>
#include <math.h>

#define BUILD_INFO "1.1"

unsigned long long timeSinceEpochMillisec() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void performStressTest(uint64_t loops) {for (uint64_t x = 0; x < loops; x++) {
    uint64_t result = pow(x, 2.0);
    sqrt(result + 1);
}}

void performStressTestFloat(uint64_t loops) {for (uint64_t x = 0; x < loops; x++) {
    double result = powf(x, 2.0);
    sqrtf(result + 1);
}}

unsigned long long getUserLoopTimes(void) {
    unsigned long long loops;
    std::string loopsString = "";

    std::cout << "\n";
    std::cout << "How many times would you like to run the test\n";
    std::cout << "Recomended: 10000\n";

    std::cin >> loopsString;

    try {loops = std::stoi(loopsString);}
    catch(const std::invalid_argument&) {loops = 0;}

    if (loops == 0) {
        std::cout << "Invalid loop interger, exiting\n";
        exit(1);
    }

    return loops;
}

unsigned char getUserSelectionForTest(void) {
    unsigned char selection;
    std::string selectionString = "";

    std::cout << "\n";
    std::cout << "[1] Interger math test\n";
    std::cout << "[2] Floating math test\n";
    std::cout << "[3] Both tests\n";

    std::cout << "\n";
    std::cout << "Which test would you like to perform\n";
    std::cout << "Recomended: 3\n";

    std::cin >> selectionString;
    
    try {selection = std::stoi(selectionString);}
    catch(const std::invalid_argument&) {selection = 0;}
    
    if (selection > 3 || selection == 0) {
        std::cout << "Invalid selection, exiting\n";
        exit(1);
    }

    return selection;
}

unsigned long long getThreads(void) {
    unsigned long long threads;
    std::string threadsString = "";

    std::cout << "\n";
    std::cout << "How many threads would you like the test to use\n";
    std::cout << "Minimum is 1 and max is " << std::thread::hardware_concurrency() << "\n";

    std::cin >> threadsString;

    try {threads = std::stoi(threadsString);}
    catch(const std::invalid_argument&) {threads = 0;}

    if (threads == 0 || threads > std::thread::hardware_concurrency()) {
        std::cout << "Invalid thread count, exiting\n";
        exit(1);
    }

    return threads;
}

void selectedTask(uint8_t selection, uint64_t loops) {
    switch (selection) {
    case 1:
        performStressTest(loops);
    break;
    case 2:
        performStressTestFloat(loops);
    break;
    case 3:
        performStressTest(loops);
        performStressTestFloat(loops);
    break;
    }
}

int main(void) {
    std::cout << "Cpu performance test starting\n";
    std::cout << "Version: " << BUILD_INFO << "\n";
    
    std::vector<std::thread> threadVector;
    unsigned long long threadCount = getThreads();
    uint8_t userTest = getUserSelectionForTest();
    uint64_t loopFor = getUserLoopTimes();
    
    unsigned long long start = timeSinceEpochMillisec();
    for (unsigned long long x = 0; x < threadCount; x++) {
        threadVector.push_back(std::thread(selectedTask, userTest, (loopFor * 50000) / threadCount));
    }

    for (unsigned long long x = 0; x < threadCount; x++) {
        threadVector[x].join();
    }
    
    unsigned long long end = timeSinceEpochMillisec();
    std::cout << "Cpu performance test took " << end - start << " ms\n";

    std::cout << "Press CTRL + C to exit" << std::endl;
    std::string dont_care;
    std::cin >> dont_care;
    
    return 0;
}