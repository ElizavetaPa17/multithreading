#include <stdio.h>

#define ATTEMPT_COUNT 100
#define THREADS_COUNT 5

#ifdef CPP_VERSION

#include <thread>
#include <iostream>
#include <string>
#include <vector>

void thread_func(std::string msg) {
    for (int i = 0; i < ATTEMPT_COUNT; ++i) {
        std::cout << std::this_thread::get_id() << ": " << msg;
    }
}

int main(int argc, char *argv[]) {
    std::vector<std::thread> vec_thread(THREADS_COUNT);

    std::vector<std::string> msgs = {
        "Hello, multithreading world in C++!\n",
        "I'm glad to see you!\n",
        "I hope we will become friends!\n",
        "You are so cute!\n",
        "We must be friends!\n"
    };

    for (int i = 0; i < THREADS_COUNT; ++i) {
        vec_thread[i] = std::thread(thread_func, msgs[i]);
    }

    for (int i = 0; i < ATTEMPT_COUNT; ++i) {
        std::cout << "Main thread" << std::this_thread::get_id() << "\n";
    }

    for (int i = 0; i < THREADS_COUNT; ++i) {
        vec_thread[i].join();
    }

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
    for (int i = 0; i < ATTEMPT_COUNT; ++i) {
        printf("%d: %s\n", gettid(), (char*)msg);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t thread_id[THREADS_COUNT];
    pthread_attr_t thread_attr[THREADS_COUNT];
    const char* msgs[THREADS_COUNT] = {
        "Hello, multithreading world in C!\n",
        "I'm glad to see you!\n",
        "I hope we will become friends!\n",
        "You are so cute!\n",
        "We must be friends!\n"
    };

    for (int i = 0; i < THREADS_COUNT; ++i) {
        printf("TEST: %s", msgs[i]);
        if (pthread_create(thread_id+i, thread_attr+i, thread_func, (void*)(msgs[i])) < 0) {
            fprintf(stderr, "failed to create thread\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < ATTEMPT_COUNT; ++i) {
        printf("Main funciton, id: %d\n", gettid());
    }

    for (int i = 0; i < THREADS_COUNT; ++i) {
        pthread_join(thread_id[i], NULL);
    }
    return 0;
}

#endif