
#include <boost/fiber/fiber.hpp>

int main()
{
    boost::fibers::fiber f([]() {
        try
        {
            throw 10;
        }
        catch(int i)
        {
            printf("bbbbb\n");
        }
        
        printf("aaaaaa\n");
    });
    f.join();

    return 0;
}

