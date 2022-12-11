#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

class Integer{
private:
    int value;
    sem_t* semaph;
public:
    Integer(int val):value(val){
        this->semaph = new sem_t();
        sem_init(this->semaph, 0, 1);
    }

    int get_value(){
        return this->value;
    }
    void set_value(int val){
        this->value = val;
    }
    void inc(){
        sem_wait(this->semaph);
        this->value++;
        sem_post(this->semaph);
    }
    ~Integer() {
        sem_destroy(this->semaph);
        delete this->semaph;
    }
};

void* inc(void* arg){
    Integer* integ = (Integer*) arg;
    int inc_num = 10000;
    for (int i = 0; i < inc_num; i++){
       integ->inc();
    }
    return NULL;
}

int main () {
    int thread_num = 56;
    Integer* integ = new Integer(0);
    pthread_t* thrds = new pthread_t[thread_num];
    double time = 0.0;
    clock_t begin = clock();
    for (int i = 0; i < thread_num; i++){
        if (pthread_create(&thrds[i], NULL, inc, (void*)integ) != 0){
            return 1;
        }
    }

    for (int i = 0; i < thread_num; i++){
        if (pthread_join(thrds[i], NULL) != 0){
            return 2;
        }
    }
    clock_t end = clock();
    time += (double)(end - begin) / CLOCKS_PER_SEC;
    std::cout << integ->get_value() << "\n";

    delete [] thrds;
    delete integ;
    
    std::string file = "time.txt";
    std::string _time = "Semaphores time is "; 
    _time += std::to_string(time) + "\n";
    int fd = open(file.c_str(), O_RDWR | O_CREAT, 0644);
    write(fd, _time.c_str(), _time.size());
    close(fd);

    return 0;
}
                                                                                                                                                                                                                                                                                                                
                                                                                                                                                                                                                                                                                                                
                        
