#include <faasm/counter.h>
#include <faasm/faasm.h>
#include <faasm/input.h>
#include <faasm/random.h>

#include <stdio.h>
#include <vector>

#define N_GUESSES 1000000
#define COUNT_KEY "count"

int piStep()
{
    int chunkSize = faasm::getTypedInput<int>(0);
    if (chunkSize == 0) {
        printf("Didn't get told a chunk size\n");
        return 1;
    }

    int count = 0;
    for (int i = 0; i < chunkSize; i++) {
        // Two random points
        double x = faasm::randomFloat();
        double y = faasm::randomFloat();

        // Check if within unit circle
        if (x * x + y * y <= 1.0) {
            count = count + 1;
        }
    }

    faasm::incrementCounter(COUNT_KEY, count, false);

    return 0;
}

/**
 * Parallel estimation of pi
 */
int main(int argc, char* argv[])
{
    int nWorkers = faasm::getTypedInput<int>(4);

    // Write chunk size to state
    int chunkSize = N_GUESSES / nWorkers;

    // Set count to zero
    faasm::initCounter(COUNT_KEY);

    // Dispatch chained calls in a loop
    std::vector<unsigned int> callIds;
    for (int i = 0; i < nWorkers; i++) {
        auto inputData = BYTES(&chunkSize);
        unsigned int callId = faasmChain(piStep, inputData, sizeof(int));
        callIds.push_back(callId);
    }

    // Wait for calls to finish
    for (unsigned int callId : callIds) {
        faasmAwaitCall(callId);
    }

    int finalCount = faasm::getCounter(COUNT_KEY);
    float piEstimate = 4 * ((float)finalCount / (N_GUESSES));

    std::string output = "Pi estimate: " + std::to_string(piEstimate) + "\n";
    printf("%s", output.c_str());
    faasm::setStringOutput(output.c_str());

    return 0;
}
