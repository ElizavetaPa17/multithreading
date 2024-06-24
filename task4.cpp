#include <stdio.h>

#define ATTEMPT_COUNT 10

#ifdef CPP_VERSION

#include <thread>
#include <iostream>
#include <string>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>

std::mutex mutex;
std::condition_variable cond_var;

std::atomic<bool> canceled{false};
std::atomic<int>  index{0};

using namespace std::chrono_literals;

void thread_func(std::string msg) {
    std::cout << "Support thread " << std::this_thread::get_id() << ": " << msg;

    std::unique_lock<std::mutex> uniq_lock(mutex);
    while (!canceled.load()) {
        std::cout << "wait\n" << index.load();
        index.fetch_add(1);

        cond_var.wait(uniq_lock);
    }

    std::cout << "End msg\n";
}

int main(int argc, char *argv[]) {
    std::thread thread(thread_func, "Hello from multithreading world in C++!\n");

    std::cout << "Main thread" << std::this_thread::get_id() << "\n";

    std::this_thread::sleep_for(2000ms);
    canceled.store(true);  
    cond_var.notify_one();

    thread.join();
    return 0;
}

#else
#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>

void* thread_func(void *msg) {  
    printf("Main thread %d: %s\n", gettid(), (char*)msg);

    sleep(5);

    printf("End msg\n");
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t thread_id = 0;
    pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);

    const char *msg = "Hello from multithreading world in C!\n";

    if (pthread_create(&thread_id, &thread_attr, thread_func, (void*)msg) < 0) {
        fprintf(stderr, "failed to create thread\n");
        exit(EXIT_FAILURE);
    }

    printf("Main thread: %d\n", gettid());
    sleep(2);

    pthread_cancel(thread_id);

    pthread_join(thread_id, NULL);

    printf("Main thread end\n");
    return 0;
}

#endif