#include <thread>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

unsigned char selection;
unsigned long long loops;
unsigned long long threadCount;

volatile void stdMathTest(unsigned long long loopTimes) {
    unsigned long long x;
    unsigned long long y;
    unsigned long long z;

    for (unsigned long currentLoop = 0; currentLoop < loopTimes * 10000; currentLoop++) {
        x = 1;
        y = 2;
        z = 0;
        
        z = x + y;
        z = y - x;
        z = x * y;
        z = y / x;
        z >> 1;
        z << 1;
    }
}

volatile void fpuMathTest(unsigned long long loopTimes) {
    double x;
    double y;
    double z;

    for (unsigned long currentLoop = 0; currentLoop < loopTimes * 10000; currentLoop++) {
        x = 1;
        y = 2;
        z = 0;
        
        z = x + y;
        z = y - x;
        z = x * y;
        z = y / x;
    }
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

unsigned long long getUserLoopTimes(void) {
    unsigned long long loops;
    std::string loopsString = "";

    std::cout << "\n";
    std::cout << "How many times would you like to run the test\n";
    std::cout << "Recomended: 1000\n";

    std::cin >> loopsString;

    try {loops = std::stoi(loopsString);}
    catch(const std::invalid_argument&) {loops = 0;}

    if (loops == 0) {
        std::cout << "Invalid loop interger, exiting\n";
        exit(1);
    }

    return loops;
}

unsigned long long getUserThreads(void) {
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

void threadDecider(void) {
    switch (selection) {
    case 1:
        stdMathTest(loops);
    break;

    case 2:
        fpuMathTest(loops);
    break;

    case 3:
        stdMathTest(loops);
        fpuMathTest(loops);
    break;
    }
}

unsigned long long timeSinceEpochMillisec() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int main(void) {
    std::cout << "Cpu performance test starting\n";
    std::cout << "Version ID: " << BUILD_PLATFORM << "\n";

    selection = getUserSelectionForTest();
    loops = getUserLoopTimes();
    threadCount = getUserThreads();

    std::vector<std::thread> threadVector;

    loops = loops / threadCount; //does skip some loops if loops is not perfectly divisble by thread count

    unsigned long long start = timeSinceEpochMillisec();

    for (unsigned long long x = 0; x < threadCount; x++) {
        threadVector.push_back(std::thread(threadDecider));
    }

    for (unsigned long long x = 0; x < threadCount; x++) {
        threadVector[x].join();
    }
    
    unsigned long long end = timeSinceEpochMillisec();
    std::cout << "Cpu performance test took " << end - start << " ms\n";
}