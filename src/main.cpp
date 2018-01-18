#define GUARD_LVL 2

#include "Espionage.hpp"

using namespace NQueue;

int main(int argc, char *argv[])
{
	Queue<int, 4> q;
	q.push(4);
	q.push(5);
	q.push(6);
	q.pop();
	q.push(7);
	q.push(8);
	q.dump();

	system("pause");
	return 0;
}