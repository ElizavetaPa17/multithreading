#include <stdio.h>

#ifdef CPP_VERSION

#include <thread>
#include <iostream>
#include <string>

void thread_func(std::string msg) {
    std::cout << std::this_thread::get_id() << ": " << msg;
}

int main(int argc, char *argv[]) {
    std::thread thread(thread_func, "Hello from multithreading world in C++!\n");

    for (int i = 0; i < 1e5; ++i) {
        i = i + 0;
        i = i - 0; 
    }

    std::cout << "Main thread" << std::this_thread::get_id() << "\n";

    thread.join();
    return 0;
}

#else
#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

void* thread_func(void *msg) {  
    printf("%d: %s", gettid(), (char*)msg);

    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t thread_id = 0;
    const char *msg = "Hello from multithreading world in C!\n";

    if (pthread_create(&thread_id, NULL, thread_func, (void*)msg) < 0) {

    }

    printf("Main funciton, id: %d\n", gettid());

    pthread_join(thread_id, NULL);
    return 0;
}

#endif