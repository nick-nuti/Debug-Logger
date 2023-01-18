

// https://stackoverflow.com/questions/15278343/c11-thread-safe-queue
template <class T>
class SafeQueue
{
    public:
        SafeQueue()
        : q_work()
        , m()
        , c()
        {}

        ~SafeQueue()
        {
            std::cout << "SafeQueue: Destructing queue class\n";
        }

        // Add an element to the queue.
        void enqueue(T t)
        {
            //std::cout << t;
            std::lock_guard<std::mutex> lock(m); // mutex lock
            //std::cout << "WORKER THREAD PUSH\n";
            //std::cout << "worker here!\n";
            q_work.push(t);  // push item to queue
            c.notify_one(); // notify next thread that it can start
        }

        // Get the "front"-element.
        // If the queue is empty, wait till a element is avaiable.
        bool dequeue()
        {
            std::unique_lock<std::mutex> lock(m);

            /*while(q_work.empty())
            {
                // release lock as long as the wait and reaquire it afterwards.
                c.wait(lock);
            }*/

            //recommendation from Joe 
            /*
            if(q_work.empty())
            {
                //so c++ lambdas require "this" capturing (implicit/automatic "[=]"/"[&]" or not "[this]") 
                //or capturing a reference to the current object
                //link to explanation: https://www.nextptr.com/tutorial/ta1430524603/capture-this-in-lambda-expression-timeline-of-change
                c.wait(lock, [&]{return !q_work.empty();});

            }
            */

            // new
            auto now = std::chrono::system_clock::now();

            if(c.wait_until(lock, now + std::chrono::milliseconds(1), [&]{return !q_work.empty();}))
            {
                q_print = q_work; // copy the worker queue into a temp queue so we can release the worker queue faster

                // efficient queue clear: https://stackoverflow.com/questions/709146/how-do-i-clear-the-stdqueue-efficiently
                std::queue<T>().swap(q_work);

                lock.unlock();
                c.notify_one();

                return true;
            }

            else
            {
                std::cout << "\nQueue Lock timeout\n";
                return false;
            }
        }

        // verbosity 1
        void dumpqueue()
        {
            while(!q_print.empty())
            {
                std::cout << q_print.front(); // 1 - print to console

                q_print.pop();
            }
            
            std::queue<T>().swap(q_print);
        }

        void clearqueues()
        {
            std::queue<T>().swap(q_work);
            std::queue<T>().swap(q_print);

            std::cout << "Clearing Queues\n";
        }

    private:
        std::queue<T> q_work;
        std::queue<T> q_print;
        
        mutable std::mutex m;
        std::condition_variable c;
};

class PrintThreadClass
{
    public:

        bool keepalive = true;
        bool runthread = true;

        PrintThreadClass(SafeQueue<std::string> &pq)
            //: thr(&PrintThreadClass::queuemonitor, this, &pq)
            //: thr(&SafeQueue<std::string>::dequeue, &pq) //how do I pass &pq class reference over to queuemonitor
             : thr(&PrintThreadClass::queuemonitor, this, std::ref(pq)) // this is best practice
        {}

        ~PrintThreadClass()
        {
            thr.join();
            std::cout << "PrintThreadClass: Destrucing logger thread class\n";
        }

        void queuemonitor(SafeQueue<std::string> &pq)
        {
            std::cout << "Starting logger thread\n";

            bool dump_printqueue;

            
            while(keepalive)
            {
                while(runthread)
                {
                    usleep(1);
                    dump_printqueue = pq.dequeue();
                    if(dump_printqueue) pq.dumpqueue();
                }
            }
        }

    private:
        std::thread thr;
        std::string buff;
};

// TO DO:
// 1. Write printer queue contents to console and/or file (depending on input arg...)
// 2. Need to think about boosting performance, too slow
// 3. Errors, exceptions with threads.... etc.
// 4. Logger characteristics like Error, Warning, Info....

// Ways this can be done:
// 1. print thread checks queue 1, if it has nothing in it then sleep for 1-2 seconds
// 2. print thread uses thread condition.wait() with mutexs