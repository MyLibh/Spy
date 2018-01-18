#include <mutex>
#include <iostream>
#include <thread>

int main(int argc, char *argv[])
{
	std::thread g([]() { std::cout << "x"; });
	 
	for (;;)
	{
		std::cout << "Thread1: " << __threadid() << std::endl;
		g.join();
	}

	return 0;
}