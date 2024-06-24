#include <stdio.h>

#define ATTEMPT_COUNT 10

#ifdef CPP_VERSION

#include <thread>
#include <iostream>
#include <string>

void thread_func(std::string msg) {
    std::cout << std::this_thread::get_id() << ": " << msg;

    for (int i = 0; i < ATTEMPT_COUNT; ++i) {
        std::cout << "Support thread msg\n";
    }
}

int main(int argc, char *argv[]) {
    std::thread thread(thread_func, "Hello from multithreading world in C++!\n");
    thread.join();

    std::cout << "Main thread" << std::this_thread::get_id() << "\n";
    for (int i = 0; i < ATTEMPT_COUNT; ++i) {
        std::cout << "Main thread msg\n";
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
    printf("%d: %s\n", gettid(), (char*)msg);
    for (int i = 0; i < ATTEMPT_COUNT; ++i) {
        printf("Support thread msg\n");
    }

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

    pthread_join(thread_id, NULL);

    printf("Main funciton, id: %d\n", gettid());
    for (int i = 0; i < ATTEMPT_COUNT; ++i) {
        printf("Main thread msg\n");
    }

    return 0;
}

#endif