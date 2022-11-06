
#include <boost/fiber/fiber.hpp>
#include <boost/test/prg_exec_monitor.hpp>
#include <boost/test/execution_monitor.hpp>
#include <boost/test/utils/basic_cstring/io.hpp>
#include <iostream>

struct my_exception1
{
    explicit    my_exception1( int res_code ) : m_res_code( res_code ) {}

    int         m_res_code;
};

struct my_exception2
{
    explicit    my_exception2( int res_code ) : m_res_code( res_code ) {}

    int         m_res_code;
};

void translate_my_exception1( my_exception1 const& ex )
{
    std::cout << "Caught my_exception1(" << ex.m_res_code << ")"<< std::endl;
}

void translate_my_exception2( my_exception2 const& ex )
{
    std::cout << "Caught my_exception2(" << ex.m_res_code << ")"<< std::endl;
}

#if (defined(__clang__) && __clang_major__ >= 6) || (defined(__GNUC__) && __GNUC__ >= 8)
__attribute__((no_sanitize("null")))
#endif
int generate_sigfault()
{
    int* p = 0;

    return *p;
}

int main()
{
    // boost::fibers::fiber f([]() {
    //     try
    //     {
    //         throw 10;
    //     }
    //     catch(int i)
    //     {
    //         printf("bbbbb\n");
    //     }
        
    //     printf("aaaaaa\n");
    // });
    // f.join();
    
    // boost::execution_monitor ex_mon;

    // ex_mon.register_exception_translator<my_exception1>( &translate_my_exception1, "except1" );
    // ex_mon.register_exception_translator<my_exception2>( &translate_my_exception2, "except2" );
    
    // ex_mon.p_catch_system_errors.value = true;

    // try {
    //     ex_mon.execute( &generate_sigfault );
    //     printf("aaaaaaaaa\n");
    // }
    // catch ( boost::execution_exception const& ex ) {
    //     printf("bbbbbbbbbb\n");
    // }


    return 0;
}